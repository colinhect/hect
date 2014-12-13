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

SceneRenderer::SceneRenderer(Renderer& renderer, AssetCache& assetCache) :
    _renderer(renderer),
    _renderCalls(1024),
    _buffersInitialized(false)
{
    _exposeMaterial = assetCache.getHandle<Material>("Hect/Expose.material");
    _compositeMaterial = assetCache.getHandle<Material>("Hect/Composite.material");
    _environmentMaterial = assetCache.getHandle<Material>("Hect/Environment.material");
    _directionalLightMaterial = assetCache.getHandle<Material>("Hect/DirectionalLight.material");

    _skyBoxMaterial = assetCache.getHandle<Material>("Hect/SkyBox.material");

    _screenMesh = assetCache.getHandle<Mesh>("Hect/Screen.mesh");
    _skyBoxMesh = assetCache.getHandle<Mesh>("Hect/SkyBox.mesh");
}

void SceneRenderer::renderScene(Scene& scene, RenderTarget& target)
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
        initializeBuffers(target.width(), target.height());
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
        _renderer.beginFrame();
        _renderer.selectTarget(_geometryFrameBuffer);
        _renderer.clear();

        for (RenderCall& renderCall : _renderCalls)
        {
            renderMesh(*camera, _geometryFrameBuffer, *renderCall.material, *renderCall.mesh, *renderCall.transform);
        }

        _renderer.endFrame();
    }

    // Clear debug geometry now that it is rendered
    if (scene.hasSystemType<DebugSystem>())
    {
        DebugSystem& debugSystem = scene.system<DebugSystem>();
        debugSystem.clear();
    }

    // Light rendering
    {
        _renderer.beginFrame();
        _renderer.selectTarget(backFrameBuffer());
        _renderer.clear();

        _renderer.selectMesh(*_screenMesh);

        // Render environment light
        if (_lightProbeCubeMap)
        {
            _renderer.selectMaterial(*_environmentMaterial);
            setBoundMaterialParameters(*_environmentMaterial, *camera, target, _identityTransform);

            _renderer.draw();
        }

        // Render directional lights
        {
            _renderer.selectMaterial(*_directionalLightMaterial);

            // Render each directional light in the scene
            for (const DirectionalLight& light : scene.components<DirectionalLight>())
            {
                _primaryLightDirection = light.direction;
                _primaryLightColor = light.color;
                setBoundMaterialParameters(*_directionalLightMaterial, *camera, target, _identityTransform);
                _renderer.draw();
            }
        }

        // Render point lights
        {
        }

        _renderer.endFrame();
    }

    swapBackBuffer();

    // Composite
    {
        _renderer.beginFrame();
        _renderer.selectTarget(backFrameBuffer());
        _renderer.clear();
        _renderer.selectMaterial(*_compositeMaterial);
        setBoundMaterialParameters(*_compositeMaterial, *camera, target, _identityTransform);

        _renderer.selectMesh(*_screenMesh);
        _renderer.draw();

        _renderer.endFrame();
    }

    swapBackBuffer();

    // Expose
    {
        _renderer.beginFrame();
        _renderer.selectTarget(target);
        _renderer.clear();
        _renderer.selectMaterial(*_exposeMaterial);
        setBoundMaterialParameters(*_exposeMaterial, *camera, target, _identityTransform);

        _renderer.selectMesh(*_screenMesh);
        _renderer.draw();

        _renderer.endFrame();
    }
}

void SceneRenderer::addRenderCall(Transform& transform, Mesh& mesh, Material& material)
{
    _renderCalls.emplace_back(transform, mesh, material);
}

void SceneRenderer::initializeBuffers(unsigned width, unsigned height)
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

    size_t memoryUsageBefore = _renderer.statistics().memoryUsage;

    _renderer.uploadFrameBuffer(_geometryFrameBuffer);
    _renderer.uploadFrameBuffer(_backFrameBuffers[0]);
    _renderer.uploadFrameBuffer(_backFrameBuffers[1]);

    // Log the memory usage for the buffers
    size_t memoryUsage = _renderer.statistics().memoryUsage - memoryUsageBefore;
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

void SceneRenderer::renderMesh(const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform)
{
    // Select the material
    _renderer.selectMaterial(material);
    setBoundMaterialParameters(material, camera, target, transform);

    // Select and draw the mesh
    _renderer.selectMesh(mesh);
    _renderer.draw();
}

void SceneRenderer::setBoundMaterialParameters(Material& material, const Camera& camera, const RenderTarget& target, const Transform& transform)
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

    for (const MaterialParameter& parameter : material.parameters())
    {
        MaterialParameterBinding binding = parameter.binding();
        switch (binding)
        {
        case MaterialParameterBinding_None:
            break;
        case MaterialParameterBinding_RenderTargetSize:
            _renderer.setMaterialParameter(parameter, Vector2(static_cast<Real>(target.width()), static_cast<Real>(target.height())));
            break;
        case MaterialParameterBinding_CameraPosition:
            _renderer.setMaterialParameter(parameter, camera.position);
            break;
        case MaterialParameterBinding_CameraFront:
            _renderer.setMaterialParameter(parameter, camera.front);
            break;
        case MaterialParameterBinding_CameraUp:
            _renderer.setMaterialParameter(parameter, camera.up);
            break;
        case MaterialParameterBinding_CameraExposure:
            _renderer.setMaterialParameter(parameter, camera.exposure);
            break;
        case MaterialParameterBinding_CameraOneOverGamma:
            _renderer.setMaterialParameter(parameter, Real(1) / camera.gamma);
            break;
        case MaterialParameterBinding_PrimaryLightDirection:
            _renderer.setMaterialParameter(parameter, _primaryLightDirection);
            break;
        case MaterialParameterBinding_PrimaryLightColor:
            _renderer.setMaterialParameter(parameter, _primaryLightColor);
            break;
        case MaterialParameterBinding_ViewMatrix:
            _renderer.setMaterialParameter(parameter, camera.viewMatrix);
            break;
        case MaterialParameterBinding_ProjectionMatrix:
            _renderer.setMaterialParameter(parameter, camera.projectionMatrix);
            break;
        case MaterialParameterBinding_ViewProjectionMatrix:
            _renderer.setMaterialParameter(parameter, camera.projectionMatrix * camera.viewMatrix);
            break;
        case MaterialParameterBinding_ModelMatrix:
            _renderer.setMaterialParameter(parameter, model);
            break;
        case MaterialParameterBinding_ModelViewMatrix:
            _renderer.setMaterialParameter(parameter, camera.viewMatrix * model);
            break;
        case MaterialParameterBinding_ModelViewProjectionMatrix:
            _renderer.setMaterialParameter(parameter, camera.projectionMatrix * (camera.viewMatrix * model));
            break;
        case MaterialParameterBinding_LightProbeCubeMap:
            assert(_lightProbeCubeMap);
            _renderer.setMaterialParameter(parameter, *_lightProbeCubeMap);
            break;
        case MaterialParameterBinding_SkyBoxCubeMap:
            assert(_skyBoxCubeMap);
            _renderer.setMaterialParameter(parameter, *_skyBoxCubeMap);
            break;
        case MaterialParameterBinding_DiffuseBuffer:
            _renderer.setMaterialParameter(parameter, _diffuseBuffer);
            break;
        case MaterialParameterBinding_MaterialBuffer:
            _renderer.setMaterialParameter(parameter, _materialBuffer);
            break;
        case MaterialParameterBinding_PositionBuffer:
            _renderer.setMaterialParameter(parameter, _positionBuffer);
            break;
        case MaterialParameterBinding_NormalBuffer:
            _renderer.setMaterialParameter(parameter, _normalBuffer);
            break;
        case MaterialParameterBinding_BackBuffer:
            _renderer.setMaterialParameter(parameter, lastBackBuffer());
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
    return material->priority() > other.material->priority();
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