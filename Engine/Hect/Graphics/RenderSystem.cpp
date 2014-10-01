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
#include "RenderSystem.h"

#include <algorithm>

#include "Hect/Logic/World.h"
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

RenderSystem::RenderSystem(Renderer& renderer, AssetCache& assetCache) :
    _renderer(&renderer),
    _buffersInitialized(false)
{
    _compositorShader = assetCache.getHandle<Shader>("Hect/PhysicallyBased/Compositor.shader");
    _environmentShader = assetCache.getHandle<Shader>("Hect/PhysicallyBased/Environment.shader");
    _directionalLightShader = assetCache.getHandle<Shader>("Hect/PhysicallyBased/DirectionalLight.shader");

    _skyBoxMaterial = assetCache.getHandle<Material>("Hect/PhysicallyBased/SkyBox.material");

    _screenMesh = assetCache.getHandle<Mesh>("Hect/PhysicallyBased/Screen.mesh");
    _skyBoxMesh = assetCache.getHandle<Mesh>("Hect/PhysicallyBased/SkyBox.mesh");
}

void RenderSystem::addWorld(World& world)
{
    auto it = std::find(_worlds.begin(), _worlds.end(), &world);
    if (it == _worlds.end())
    {
        _worlds.push_back(&world);
    }
}

void RenderSystem::removeWorld(World& world)
{
    auto it = std::find(_worlds.begin(), _worlds.end(), &world);
    if (it != _worlds.end())
    {
        _worlds.erase(it);
    }
}

void RenderSystem::renderAll(RenderTarget& target)
{
    World& world = *_worlds.front();

    CameraSystem& cameraSystem = world.system<CameraSystem>();
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

        // Model buffer rendering
        {
            renderer().beginFrame();
            renderer().bindTarget(_geometryBuffer);
            renderer().clear();

            // Render the sky box if there is one
            auto skyBox = world.components<SkyBox>().begin();
            if (skyBox)
            {
                // Construct a transform at the camera's position
                Transform transform;
                transform.globalPosition = camera->position;

                // Update the sky box material to use this sky box's texture
                for (Technique& technique : _skyBoxMaterial->techniques())
                {
                    for (Pass& pass : technique.passes())
                    {
                        pass.clearTextures();
                        pass.addTexture(skyBox->texture);
                    }
                }

                // Render the sky box
                renderMesh(*camera, _geometryBuffer, *_skyBoxMaterial, *_skyBoxMesh, transform);
            }

            // Render each entity in hierarchical order
            for (Entity& entity : world.entities())
            {
                if (!entity.parent())
                {
                    render(*camera, target, entity);
                }
            }

            renderer().endFrame();
        }

        // Accumulation buffer rendering
        {
            renderer().beginFrame();
            renderer().bindTarget(_accumulationBuffer);
            renderer().clear();

            RenderState state;
            state.enable(RenderStateFlag_Blend);
            state.disable(RenderStateFlag_DepthTest);
            state.disable(RenderStateFlag_DepthWrite);
            renderer().bindState(state);

            // Get the first light probe
            auto lightProbe = world.components<LightProbe>().begin();
            if (!lightProbe)
            {
                throw Error("No light probe in world");
            }

            unsigned int index = 0;
            for (Texture& target : _geometryBuffer.targets())
            {
                renderer().bindTexture(target, index++);
            }
            renderer().bindTexture(*lightProbe->texture, index++);
            renderer().bindMesh(*_screenMesh);

            Transform identity;

            // Render environment light
            {
                renderer().bindShader(*_environmentShader);
                setBoundUniforms(*_environmentShader, *camera, target, identity);

                renderer().draw();
            }

            // Render directional lights
            {
                renderer().bindShader(*_directionalLightShader);
                setBoundUniforms(*_directionalLightShader, *camera, target, identity);

                // Get the uniforms required for directional lights
                const Uniform& colorUniform = _directionalLightShader->uniformWithName("lightColor");
                const Uniform& directionUniform = _directionalLightShader->uniformWithName("lightDirection");

                // Render each directional light in the world
                for (const DirectionalLight& light : world.components<DirectionalLight>())
                {
                    renderer().setUniform(colorUniform, light.color);
                    renderer().setUniform(directionUniform, light.direction);
                    renderer().draw();
                }
            }

            // Render point lights
            {
            }

            renderer().endFrame();
        }

        // Compositor rendering
        {
            renderer().beginFrame();
            renderer().bindTarget(target);
            renderer().clear();

            RenderState state;
            state.disable(RenderStateFlag_DepthTest);
            renderer().bindState(state);

            renderer().bindShader(*_compositorShader);

            renderer().bindTexture(*_geometryBuffer.targets().begin(), 0);
            renderer().bindTexture(*_accumulationBuffer.targets().begin(), 1);

            // Bind and draw the composited image
            renderer().bindMesh(*_screenMesh);
            renderer().draw();

            renderer().endFrame();
        }
    }
}

void RenderSystem::render(Camera& camera, RenderTarget& target, Entity& entity, bool frustumTest)
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

void RenderSystem::renderMesh(const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform)
{
    // Render the mesh for each pass
    for (Pass& pass : material.preferedTechnique().passes())
    {
        renderMeshPass(camera, target, pass, mesh, transform);
    }
}

void RenderSystem::renderMeshPass(const Camera& camera, const RenderTarget& target, Pass& pass, Mesh& mesh, const Transform& transform)
{
    // Prepare the pass
    pass.prepare(*_renderer);

    // Set uniforms with bindings
    Shader& shader = *pass.shader();
    setBoundUniforms(shader, camera, target, transform);

    // Bind and draw the mesh
    _renderer->bindMesh(mesh);
    _renderer->draw();
}

void RenderSystem::setBoundUniforms(Shader& shader, const Camera& camera, const RenderTarget& target, const Transform& transform)
{
    // Buid the model matrix
    Matrix4 model;

    if (transform.globalPosition != Vector3::zero())
    {
        model.translate(transform.globalPosition);
    }

    if (transform.globalScale != Vector3::zero())
    {
        model.scale(transform.globalScale);
    }

    // TODO: Implement quaternion equality
    //if (transform.globalRotation != Quaternion())
    //{
    model.rotate(transform.globalRotation);
    //}

    for (const Uniform& uniform : shader.uniforms())
    {
        if (uniform.hasBinding())
        {
            UniformBinding binding = uniform.binding();

            switch (binding)
            {
            case UniformBinding_RenderTargetSize:
                _renderer->setUniform(uniform, Vector2((Real)target.width(), (Real)target.height()));
                break;
            case UniformBinding_CameraPosition:
                _renderer->setUniform(uniform, camera.position);
                break;
            case UniformBinding_CameraFront:
                _renderer->setUniform(uniform, camera.front);
                break;
            case UniformBinding_CameraUp:
                _renderer->setUniform(uniform, camera.up);
                break;
            case UniformBinding_ViewMatrix:
                _renderer->setUniform(uniform, camera.viewMatrix);
                break;
            case UniformBinding_ProjectionMatrix:
                _renderer->setUniform(uniform, camera.projectionMatrix);
                break;
            case UniformBinding_ViewProjectionMatrix:
                _renderer->setUniform(uniform, camera.projectionMatrix * camera.viewMatrix);
                break;
            case UniformBinding_ModelMatrix:
                _renderer->setUniform(uniform, model);
                break;
            case UniformBinding_ModelViewMatrix:
                _renderer->setUniform(uniform, camera.viewMatrix * model);
                break;
            case UniformBinding_ModelViewProjectionMatrix:
                _renderer->setUniform(uniform, camera.projectionMatrix * (camera.viewMatrix * model));
                break;
            }
        }
    }
}

Renderer& RenderSystem::renderer()
{
    assert(_renderer);
    return *_renderer;
}

void RenderSystem::initializeBuffers(unsigned width, unsigned height)
{
    _buffersInitialized = true;

    TextureFilter filter = TextureFilter_Nearest;

    _geometryBuffer = FrameBuffer();

    // Diffuse: Red Green Blue
    _geometryBuffer.addTarget(Texture("DiffuseBuffer", width, height, PixelType_Float16, PixelFormat_Rgb, filter, filter, false, false));

    // Material: Roughness Metallic ?
    _geometryBuffer.addTarget(Texture("MaterialBuffer", width, height, PixelType_Float16, PixelFormat_Rgb, filter, filter, false, false));

    // Position: X Y Z
    _geometryBuffer.addTarget(Texture("PositionBuffer", width, height, PixelType_Float32, PixelFormat_Rgb, filter, filter, false, false));

    // Normal: X Y Z Depth
    _geometryBuffer.addTarget(Texture("NormalBuffer", width, height, PixelType_Float16, PixelFormat_Rgba, filter, filter, false, false));

    _accumulationBuffer = FrameBuffer();
    _accumulationBuffer.addTarget(Texture("AccumulationBuffer", width, height, PixelType_Float16, PixelFormat_Rgba, filter, filter, false, false));
}