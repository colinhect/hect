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
#include "Hect/Logic/Components/DirectionalLight.h"
#include "Hect/Logic/Components/LightProbe.h"
#include "Hect/Logic/Components/Model.h"
#include "Hect/Logic/Components/SkyBox.h"
#include "Hect/Logic/Systems/CameraSystem.h"
#include "Hect/Logic/Systems/DebugSystem.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

SceneRenderer::SceneRenderer(AssetCache& assetCache) :
    _renderCalls(1024),
    _buffersInitialized(false)
{
    _exposeShader = assetCache.getHandle<Shader>("Hect/Expose.shader");
    _compositeShader = assetCache.getHandle<Shader>("Hect/Composite.shader");
    _environmentShader = assetCache.getHandle<Shader>("Hect/Environment.shader");
    _directionalLightShader = assetCache.getHandle<Shader>("Hect/DirectionalLight.shader");

    _skyBoxMaterial = assetCache.getHandle<Material>("Hect/SkyBox.material");

    _screenMesh = assetCache.getHandle<Mesh>("Hect/Screen.mesh");
    _skyBoxMesh = assetCache.getHandle<Mesh>("Hect/SkyBox.mesh");
}

void SceneRenderer::renderScene(Renderer& renderer, Scene& scene, RenderTarget& target)
{
    CameraSystem& cameraSystem = scene.system<CameraSystem>();
    Component<Camera>::Iterator camera = cameraSystem.activeCamera();
    if (!camera)
    {
        // Nothing to render
        return;
    }

    // Update the camer transform
    _cameraTransform.globalPosition = camera->position;

    // Update the camera's aspect ratio if needed
    if (camera->aspectRatio != target.aspectRatio())
    {
        camera->aspectRatio = target.aspectRatio();
        cameraSystem.updateCamera(*camera);
    }

    // Initialize buffers if needed
    if (!_buffersInitialized)
    {
        initializeBuffers(renderer, target.width(), target.height());
    }

    // Get the cube map of the active light probe
    auto lightProbe = scene.components<LightProbe>().begin();
    if (lightProbe)
    {
        _lightProbeCubeMap = &*lightProbe->texture;
    }
    else
    {
        _lightProbeCubeMap = nullptr;
    }

    // Get the cube map of the active sky box
    auto skyBox = scene.components<SkyBox>().begin();
    if (skyBox)
    {
        _skyBoxCubeMap = &*skyBox->texture;
    }
    else
    {
        _skyBoxCubeMap = nullptr;
    }

    // Build all render calls and sort by priority
    _renderCalls.clear();
    for (Entity& entity : scene.entities())
    {
        if (!entity.parent())
        {
            buildRenderCalls(*camera, entity);
        }
    }

    // Add render calls for debug geometry
    if (scene.hasSystemType<DebugSystem>())
    {
        DebugSystem& debugSystem = scene.system<DebugSystem>();
        debugSystem.addRenderCalls(*this);
    }

    // Sort render calls by priority
    std::sort(_renderCalls.begin(), _renderCalls.end());

    // Geometry buffer rendering
    {
        renderer.beginFrame();
        renderer.selectTarget(_geometryFrameBuffer);
        renderer.clear();

        for (RenderCall& renderCall : _renderCalls)
        {
            renderMesh(renderer, *camera, _geometryFrameBuffer, *renderCall.material, *renderCall.mesh, *renderCall.transform);
        }

        renderer.endFrame();
    }

    // Light rendering
    {
        renderer.beginFrame();
        renderer.selectTarget(backFrameBuffer());
        renderer.clear();

        renderer.selectMesh(*_screenMesh);

        // Render environment light
        if (_lightProbeCubeMap)
        {
            renderer.selectShader(*_environmentShader);
            setBoundUniforms(renderer, *_environmentShader, *camera, target, _identityTransform);

            renderer.draw();
        }

        // Render directional lights
        {
            renderer.selectShader(*_directionalLightShader);

            // Render each directional light in the scene
            for (const DirectionalLight& light : scene.components<DirectionalLight>())
            {
                _primaryLightDirection = light.direction;
                _primaryLightColor = light.color;
                setBoundUniforms(renderer, *_directionalLightShader, *camera, target, _identityTransform);
                renderer.draw();
            }
        }

        // Render point lights
        {
        }

        renderer.endFrame();
    }

    swapBackBuffer();

    // Composite
    {
        renderer.beginFrame();
        renderer.selectTarget(backFrameBuffer());
        renderer.clear();
        renderer.selectShader(*_compositeShader);
        setBoundUniforms(renderer, *_compositeShader, *camera, target, _identityTransform);

        renderer.selectMesh(*_screenMesh);
        renderer.draw();

        renderer.endFrame();
    }

    swapBackBuffer();

    // Expose
    {
        renderer.beginFrame();
        renderer.selectTarget(target);
        renderer.clear();
        renderer.selectShader(*_exposeShader);
        setBoundUniforms(renderer, *_exposeShader, *camera, target, _identityTransform);

        renderer.selectMesh(*_screenMesh);
        renderer.draw();

        renderer.endFrame();
    }
}

void SceneRenderer::addRenderCall(Transform& transform, Mesh& mesh, Material& material)
{
    _renderCalls.emplace_back(transform, mesh, material);
}

void SceneRenderer::initializeBuffers(Renderer& renderer, unsigned width, unsigned height)
{
    _buffersInitialized = true;

    TextureFilter nearest = TextureFilter_Nearest;

    // Depth buffer
    _depthBuffer = RenderBuffer(RenderBufferFormat_DepthComponent, width, height);

    // Diffuse buffer: Red Green Blue Lighting
    _diffuseBuffer = Texture("DiffuseBuffer", width, height, PixelType_Float32, PixelFormat_Rgba, nearest, nearest, false, false);

    // Material buffer: Roughness Metallic ?
    _materialBuffer = Texture("MaterialBuffer", width, height, PixelType_Float32, PixelFormat_Rgb, nearest, nearest, false, false);

    // World position buffer: X Y Z
    _positionBuffer = Texture("PositionBuffer", width, height, PixelType_Float32, PixelFormat_Rgb, nearest, nearest, false, false);

    // World normal buffer: X Y Z Depth
    _normalBuffer = Texture("NormalBuffer", width, height, PixelType_Float16, PixelFormat_Rgba, nearest, nearest, false, false);

    // Back buffers
    _backBuffers[0] = Texture("BackBuffer0", width, height, PixelType_Float32, PixelFormat_Rgb, nearest, nearest, false, false);
    _backBuffers[1] = Texture("BackBuffer1", width, height, PixelType_Float32, PixelFormat_Rgb, nearest, nearest, false, false);

    // Geometry frame buffer
    _geometryFrameBuffer = FrameBuffer(width, height);
    _geometryFrameBuffer.attachTexture(FrameBufferSlot_Color0, _diffuseBuffer);
    _geometryFrameBuffer.attachTexture(FrameBufferSlot_Color1, _materialBuffer);
    _geometryFrameBuffer.attachTexture(FrameBufferSlot_Color2, _positionBuffer);
    _geometryFrameBuffer.attachTexture(FrameBufferSlot_Color3, _normalBuffer);
    _geometryFrameBuffer.attachRenderBuffer(FrameBufferSlot_Depth, _depthBuffer);

    // Back frame buffers
    _backFrameBuffers[0] = FrameBuffer(width, height);
    _backFrameBuffers[0].attachTexture(FrameBufferSlot_Color0, _backBuffers[0]);
    _backFrameBuffers[0].attachRenderBuffer(FrameBufferSlot_Depth, _depthBuffer);

    _backFrameBuffers[1] = FrameBuffer(width, height);
    _backFrameBuffers[1].attachTexture(FrameBufferSlot_Color0, _backBuffers[1]);
    _backFrameBuffers[1].attachRenderBuffer(FrameBufferSlot_Depth, _depthBuffer);

    size_t memoryUsageBefore = renderer.statistics().memoryUsage;

    renderer.uploadFrameBuffer(_geometryFrameBuffer);
    renderer.uploadFrameBuffer(_backFrameBuffers[0]);
    renderer.uploadFrameBuffer(_backFrameBuffers[1]);

    // Log the memory usage for the buffers
    size_t memoryUsage = renderer.statistics().memoryUsage - memoryUsageBefore;
    Real memoryUsageInMegabytes = static_cast<Real>(memoryUsage) / Real(1024 * 1024);
    HECT_TRACE(format("Scene renderer GPU memory usage: %dMB", static_cast<int>(memoryUsageInMegabytes)));
}

void SceneRenderer::buildRenderCalls(Camera& camera, Entity& entity, bool frustumTest)
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

                    addRenderCall(*transform, mesh, material);
                }

                // Render all children
                for (Entity& child : entity.children())
                {
                    buildRenderCalls(camera, child, frustumTest);
                }
            }
        }
    }
    else
    {
        auto skyBox = entity.component<SkyBox>();
        if (skyBox)
        {
            addRenderCall(_cameraTransform, *_skyBoxMesh, *_skyBoxMaterial);
        }
    }
}

void SceneRenderer::renderMesh(Renderer& renderer, const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform)
{
    Shader& shader = *material.shader();

    // Select the shader
    renderer.selectShader(shader);
    setBoundUniforms(renderer, shader, camera, target, transform);

    // Select and draw the mesh
    renderer.selectMesh(mesh);
    renderer.draw();
}

void SceneRenderer::setBoundUniforms(Renderer& renderer, Shader& shader, const Camera& camera, const RenderTarget& target, const Transform& transform)
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

    for (const Uniform& uniform : shader.uniforms())
    {
        UniformBinding binding = uniform.binding();
        switch (binding)
        {
        case UniformBinding_None:
            break;
        case UniformBinding_RenderTargetSize:
            renderer.setUniform(uniform, Vector2(static_cast<Real>(target.width()), static_cast<Real>(target.height())));
            break;
        case UniformBinding_CameraPosition:
            renderer.setUniform(uniform, camera.position);
            break;
        case UniformBinding_CameraFront:
            renderer.setUniform(uniform, camera.front);
            break;
        case UniformBinding_CameraUp:
            renderer.setUniform(uniform, camera.up);
            break;
        case UniformBinding_CameraExposure:
            renderer.setUniform(uniform, camera.exposure);
            break;
        case UniformBinding_CameraOneOverGamma:
            renderer.setUniform(uniform, Real(1) / camera.gamma);
            break;
        case UniformBinding_PrimaryLightDirection:
            renderer.setUniform(uniform, _primaryLightDirection);
            break;
        case UniformBinding_PrimaryLightColor:
            renderer.setUniform(uniform, _primaryLightColor);
            break;
        case UniformBinding_ViewMatrix:
            renderer.setUniform(uniform, camera.viewMatrix);
            break;
        case UniformBinding_ProjectionMatrix:
            renderer.setUniform(uniform, camera.projectionMatrix);
            break;
        case UniformBinding_ViewProjectionMatrix:
            renderer.setUniform(uniform, camera.projectionMatrix * camera.viewMatrix);
            break;
        case UniformBinding_ModelMatrix:
            renderer.setUniform(uniform, model);
            break;
        case UniformBinding_ModelViewMatrix:
            renderer.setUniform(uniform, camera.viewMatrix * model);
            break;
        case UniformBinding_ModelViewProjectionMatrix:
            renderer.setUniform(uniform, camera.projectionMatrix * (camera.viewMatrix * model));
            break;
        case UniformBinding_LightProbeCubeMap:
            assert(_lightProbeCubeMap);
            renderer.setUniform(uniform, *_lightProbeCubeMap);
            break;
        case UniformBinding_SkyBoxCubeMap:
            assert(_skyBoxCubeMap);
            renderer.setUniform(uniform, *_skyBoxCubeMap);
            break;
        case UniformBinding_DiffuseBuffer:
            renderer.setUniform(uniform, _diffuseBuffer);
            break;
        case UniformBinding_MaterialBuffer:
            renderer.setUniform(uniform, _materialBuffer);
            break;
        case UniformBinding_PositionBuffer:
            renderer.setUniform(uniform, _positionBuffer);
            break;
        case UniformBinding_NormalBuffer:
            renderer.setUniform(uniform, _normalBuffer);
            break;
        case UniformBinding_BackBuffer:
            renderer.setUniform(uniform, lastBackBuffer());
            break;
        }
    }
}

void SceneRenderer::swapBackBuffer()
{
    _backBufferIndex = (_backBufferIndex + 1) % 2;
}

Texture& SceneRenderer::backBuffer()
{
    return _backBuffers[_backBufferIndex];
}

Texture& SceneRenderer::lastBackBuffer()
{
    return _backBuffers[(_backBufferIndex + 1) % 2];
}

FrameBuffer& SceneRenderer::backFrameBuffer()
{
    return _backFrameBuffers[_backBufferIndex];
}

bool SceneRenderer::RenderCall::operator<(const RenderCall& other) const
{
    return material->shader()->priority() > other.material->shader()->priority();
}

SceneRenderer::RenderCall::RenderCall()
{
}

SceneRenderer::RenderCall::RenderCall(Transform& transform, Mesh& mesh, Material& material) :
    transform(&transform),
    mesh(&mesh),
    material(&material)
{
}