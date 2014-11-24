///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include "SceneRenderer.h"

#include <algorithm>

#include "Hect/Logic/Scene.h"
#include "Hect/Logic/Components/BoundingBox.h"
#include "Hect/Logic/Components/Camera.h"
#include "Hect/Logic/Components/DirectionalLight.h"
#include "Hect/Logic/Components/LightProbe.h"
#include "Hect/Logic/Components/Model.h"
#include "Hect/Logic/Components/SkyBox.h"
#include "Hect/Logic/Components/Transform.h"
#include "Hect/Logic/Systems/CameraSystem.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

SceneRenderer::SceneRenderer(Renderer& renderer, AssetCache& assetCache) :
    _renderer(renderer),
    _buffersInitialized(false)
{
    _compositorShader = assetCache.getHandle<Shader>("Hect/Compositor.shader");
    _environmentShader = assetCache.getHandle<Shader>("Hect/Environment.shader");
    _directionalLightShader = assetCache.getHandle<Shader>("Hect/DirectionalLight.shader");
    _skyBoxShader = assetCache.getHandle<Shader>("Hect/SkyBox.shader");

    _screenMesh = assetCache.getHandle<Mesh>("Hect/Screen.mesh");
    _skyBoxMesh = assetCache.getHandle<Mesh>("Hect/SkyBox.mesh");
}

void SceneRenderer::renderScene(Scene& scene, RenderTarget& target)
{
    CameraSystem& cameraSystem = scene.system<CameraSystem>();
    Component<Camera>::Iterator camera = cameraSystem.activeCamera();
    if (camera)
    {
        // Update the camera's aspect ratio if needed
        if (camera->aspectRatio != target.aspectRatio())
        {
            camera->aspectRatio = target.aspectRatio();
            cameraSystem.updateCamera(*camera);
        }

        // Initialize buffers if needed
        if (!_buffersInitialized)
        {
            initializeBuffers(target.width(), target.height());
        }

        // Geometry buffer rendering
        {
            _renderer.beginFrame();
            _renderer.bindTarget(_geometryFrameBuffer);
            _renderer.clear();

            // Render each entity in hierarchical order
            for (Entity& entity : scene.entities())
            {
                if (!entity.parent())
                {
                    render(*camera, target, entity);
                }
            }

            _renderer.endFrame();
        }

        // Accumulation buffer rendering
        {
            _renderer.beginFrame();
            _renderer.bindTarget(_accumulationFrameBuffer);
            _renderer.clear();

            RenderState state;
            state.enable(RenderStateFlag_Blend);
            state.disable(RenderStateFlag_DepthTest);
            state.disable(RenderStateFlag_DepthWrite);
            _renderer.bindState(state);

            // Get the first light probe
            auto lightProbe = scene.components<LightProbe>().begin();
            if (!lightProbe)
            {
                throw Error("No light probe in scene");
            }

            unsigned int index = 0;
            for (FrameBuffer::TextureAttachment& attachment : _geometryFrameBuffer.textureAttachments())
            {
                _renderer.bindTexture(*attachment.texture, index++);
            }
            _renderer.bindTexture(*lightProbe->texture, index++);
            _renderer.bindMesh(*_screenMesh);

            Transform identity;

            // Render environment light
            {
                _renderer.bindShader(*_environmentShader);
                setBoundShaderParameters(*_environmentShader, *camera, target, identity);

                _renderer.draw();
            }

            // Render directional lights
            {
                _renderer.bindShader(*_directionalLightShader);
                setBoundShaderParameters(*_directionalLightShader, *camera, target, identity);

                // Get the parameters required for directional lights
                const ShaderParameter& colorShaderParameter = _directionalLightShader->parameterWithName("lightColor");
                const ShaderParameter& directionShaderParameter = _directionalLightShader->parameterWithName("lightDirection");

                // Render each directional light in the scene
                for (const DirectionalLight& light : scene.components<DirectionalLight>())
                {
                    _renderer.bindShaderParameter(colorShaderParameter, light.color);
                    _renderer.bindShaderParameter(directionShaderParameter, light.direction);
                    _renderer.draw();
                }
            }

            // Render point lights
            {
            }

            _renderer.endFrame();
        }

        // Compositor rendering
        {
            _renderer.beginFrame();
            _renderer.bindTarget(target);
            _renderer.clear();

            // Pre composite
            {
                // Render the sky box if there is one
                auto skyBox = scene.components<SkyBox>().begin();
                if (skyBox)
                {
                    RenderState state;
                    state.disable(RenderStateFlag_CullFace);
                    state.disable(RenderStateFlag_DepthTest);
                    _renderer.bindState(state);

                    // Construct a transform at the camera's position
                    Transform transform;
                    transform.globalPosition = camera->position;

                    _renderer.bindTexture(*skyBox->texture, 0);

                    _renderer.bindShader(*_skyBoxShader);
                    setBoundShaderParameters(*_skyBoxShader, *camera, target, transform);

                    // Bind and draw the skybox
                    _renderer.bindMesh(*_skyBoxMesh);
                    _renderer.draw();
                }
            }

            RenderState state;
            state.disable(RenderStateFlag_DepthTest);
            _renderer.bindState(state);

            Transform transform;

            _renderer.bindShader(*_compositorShader);
            setBoundShaderParameters(*_compositorShader, *camera, target, transform);

            _renderer.bindTexture(*_accumulationFrameBuffer.textureAttachments().begin()->texture, 0);

            // Bind and draw the composited image
            _renderer.bindMesh(*_screenMesh);
            _renderer.draw();

            _renderer.endFrame();
        }
    }
}

void SceneRenderer::initializeBuffers(unsigned width, unsigned height)
{
    _buffersInitialized = true;

    TextureFilter nearest = TextureFilter_Nearest;

    // Depth buffer
    _depthBuffer = RenderBuffer(RenderBufferFormat_DepthComponent, width, height);

    // Diffuse buffer: Red Green Blue
    _diffuseBuffer = Texture("DiffuseBuffer", width, height, PixelType_Float16, PixelFormat_Rgb, nearest, nearest, false, false);

    // Material buffer: Roughness Metallic ?
    _materialBuffer = Texture("MaterialBuffer", width, height, PixelType_Float16, PixelFormat_Rgb, nearest, nearest, false, false);

    // World position buffer: X Y Z
    _positionlBuffer = Texture("PositionBuffer", width, height, PixelType_Float32, PixelFormat_Rgb, nearest, nearest, false, false);

    // World normal buffer: X Y Z Depth
    _normalBuffer = Texture("NormalBuffer", width, height, PixelType_Float16, PixelFormat_Rgba, nearest, nearest, false, false);

    // Light accumulation buffer
    _accumulationBuffer = Texture("AccumulationBuffer", width, height, PixelType_Float16, PixelFormat_Rgba, nearest, nearest, false, false);

    // Geometry frame buffer
    _geometryFrameBuffer = FrameBuffer(width, height);
    _geometryFrameBuffer.attachTexture(FrameBufferSlot_Color0, _diffuseBuffer);
    _geometryFrameBuffer.attachTexture(FrameBufferSlot_Color1, _materialBuffer);
    _geometryFrameBuffer.attachTexture(FrameBufferSlot_Color2, _positionlBuffer);
    _geometryFrameBuffer.attachTexture(FrameBufferSlot_Color3, _normalBuffer);
    _geometryFrameBuffer.attachRenderBuffer(FrameBufferSlot_Depth, _depthBuffer);

    // Light accumulation frame buffer
    _accumulationFrameBuffer = FrameBuffer(width, height);
    _accumulationFrameBuffer.attachTexture(FrameBufferSlot_Color0, _accumulationBuffer);
    _accumulationFrameBuffer.attachRenderBuffer(FrameBufferSlot_Depth, _depthBuffer);

    _renderer.uploadFrameBuffer(_geometryFrameBuffer);
    _renderer.uploadFrameBuffer(_accumulationFrameBuffer);
}

Technique& SceneRenderer::selectTechnique(Material& material) const
{
    auto techniques = material.techniques();
    if (techniques.empty())
    {
        throw Error("The material has no techniques");
    }
    else
    {
        return techniques[0];
    }
}

void SceneRenderer::render(Camera& camera, RenderTarget& target, Entity& entity, bool frustumTest)
{
    // If the entity has a model component
    auto model = entity.component<Model>();
    if (model)
    {
        // If the entity has a transform component
        auto transform = entity.component<Transform>();
        if (transform)
        {
            // By default, assume that the entity is visible
            bool visible = true;

            // If we need to test this entity against the frustum
            if (frustumTest)
            {
                // If the entity has a bounding box
                auto boundingBox = entity.component<BoundingBox>();
                if (boundingBox)
                {
                    // Test the bounding box against the frustum
                    FrustumTestResult result = camera.frustum.testAxisAlignedBox(boundingBox->axisAlignedBox);
                    if (result == FrustumTestResult_Inside)
                    {
                        // No need to test any children
                        frustumTest = false;
                    }
                    else if (result == FrustumTestResult_Intersect)
                    {
                        // Need to test children
                        frustumTest = true;
                    }
                    else
                    {
                        // The entity is outside of the frustum
                        visible = false;
                    }
                }
            }

            // If the entity is visible
            if (visible)
            {
                // Render the model
                for (const ModelSurface& surface : model->surfaces)
                {
                    Mesh& mesh = *surface.mesh;
                    Material& material = *surface.material;

                    renderMesh(camera, target, material, mesh, *transform);
                }

                // Render all children
                for (Entity& child : entity.children())
                {
                    render(camera, target, child, frustumTest);
                }
            }
        }
    }
}

void SceneRenderer::renderMesh(const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform)
{
    // Render the mesh for each pass
    for (Pass& pass : selectTechnique(material).passes())
    {
        renderMeshPass(camera, target, pass, mesh, transform);
    }
}

void SceneRenderer::renderMeshPass(const Camera& camera, const RenderTarget& target, Pass& pass, Mesh& mesh, const Transform& transform)
{
    // Prepare the pass
    pass.prepare(_renderer);

    // Set parameters with bindings
    Shader& shader = *pass.shader();
    setBoundShaderParameters(shader, camera, target, transform);

    // Bind and draw the mesh
    _renderer.bindMesh(mesh);
    _renderer.draw();
}

void SceneRenderer::setBoundShaderParameters(Shader& shader, const Camera& camera, const RenderTarget& target, const Transform& transform)
{
    // Buid the model matrix
    Matrix4 model;

    // Translate the matrix to the global position
    if (transform.globalPosition != Vector3::zero())
    {
        model.translate(transform.globalPosition);
    }

    // Scale the matrix by the global scale
    if (transform.globalScale != Vector3::zero())
    {
        model.scale(transform.globalScale);
    }

    // Rotate the matrix by the global rotation
    if (transform.globalRotation != Quaternion())
    {
        model.rotate(transform.globalRotation);
    }

    for (const ShaderParameter& parameter : shader.parameters())
    {
        if (parameter.hasBinding())
        {
            ShaderParameterBinding binding = parameter.binding();

            switch (binding)
            {
            case ShaderParameterBinding_None:
                break;
            case ShaderParameterBinding_RenderTargetSize:
                _renderer.bindShaderParameter(parameter, Vector2(static_cast<Real>(target.width()), static_cast<Real>(target.height())));
                break;
            case ShaderParameterBinding_CameraPosition:
                _renderer.bindShaderParameter(parameter, camera.position);
                break;
            case ShaderParameterBinding_CameraFront:
                _renderer.bindShaderParameter(parameter, camera.front);
                break;
            case ShaderParameterBinding_CameraUp:
                _renderer.bindShaderParameter(parameter, camera.up);
                break;
            case ShaderParameterBinding_CameraExposure:
                _renderer.bindShaderParameter(parameter, camera.exposure);
                break;
            case ShaderParameterBinding_CameraOneOverGamma:
                _renderer.bindShaderParameter(parameter, Real(1) / camera.gamma);
                break;
            case ShaderParameterBinding_ViewMatrix:
                _renderer.bindShaderParameter(parameter, camera.viewMatrix);
                break;
            case ShaderParameterBinding_ProjectionMatrix:
                _renderer.bindShaderParameter(parameter, camera.projectionMatrix);
                break;
            case ShaderParameterBinding_ViewProjectionMatrix:
                _renderer.bindShaderParameter(parameter, camera.projectionMatrix * camera.viewMatrix);
                break;
            case ShaderParameterBinding_ModelMatrix:
                _renderer.bindShaderParameter(parameter, model);
                break;
            case ShaderParameterBinding_ModelViewMatrix:
                _renderer.bindShaderParameter(parameter, camera.viewMatrix * model);
                break;
            case ShaderParameterBinding_ModelViewProjectionMatrix:
                _renderer.bindShaderParameter(parameter, camera.projectionMatrix * (camera.viewMatrix * model));
                break;
            }
        }
    }
}