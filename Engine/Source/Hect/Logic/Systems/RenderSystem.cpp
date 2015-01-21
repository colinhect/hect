///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

#include "Hect/Logic/Components/BoundingBox.h"
#include "Hect/Logic/Components/LightProbe.h"
#include "Hect/Logic/Components/Model.h"
#include "Hect/Logic/Components/SkyBox.h"
#include "Hect/Logic/Systems/CameraSystem.h"
#include "Hect/Logic/Systems/DebugSystem.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

RenderSystem::RenderSystem(Engine& engine, Scene& scene) :
    System(scene, SystemTickStage_Subsequent),
    _renderer(&engine.renderer()),
    _taskPool(&engine.taskPool()),
    _buffersInitialized(false)
{
    AssetCache& assetCache = engine.assetCache();

    _exposeShader = assetCache.getHandle<Shader>("Hect/Expose.shader");
    _compositeShader = assetCache.getHandle<Shader>("Hect/Composite.shader");
    _environmentShader = assetCache.getHandle<Shader>("Hect/Environment.shader");
    _directionalLightShader = assetCache.getHandle<Shader>("Hect/DirectionalLight.shader");

    _skyBoxMaterial = assetCache.getHandle<Material>("Hect/SkyBox.material");

    _screenMesh = assetCache.getHandle<Mesh>("Hect/Screen.mesh");
    _skyBoxMesh = assetCache.getHandle<Mesh>("Hect/SkyBox.mesh");
}

void RenderSystem::render(Engine& engine, RenderTarget& target)
{
    (void)engine;

    CameraSystem& cameraSystem = scene().system<CameraSystem>();
    Camera::Iterator camera = cameraSystem.activeCamera();
    if (camera)
    {
        prepareFrame(*camera, scene(), target);
        renderFrame(*camera, target);
    }
}

void RenderSystem::addRenderCall(Transform& transform, Mesh& mesh, Material& material)
{
    const AssetHandle<Shader>& shader = material.shader();
    if (shader)
    {
        switch (shader->schema())
        {
        case ShaderSchema_None:
        case ShaderSchema_OpaquePhysicalGeometry:
            _frameData.opaquePhysicalGeometry.emplace_back(transform, mesh, material);
            break;
        case ShaderSchema_TransparentPhysicalGeometry:
            _frameData.transparentPhysicalGeometry.emplace_back(transform, mesh, material);
            break;
        }
    }
}

void RenderSystem::uploadRendererObjects()
{
    for (Model& model : scene().components<Model>())
    {
        for (ModelSurface& surface : model.surfaces)
        {
            Mesh& mesh = *surface.mesh;
            Material& material = *surface.material;

            _renderer->uploadMesh(mesh);
            _renderer->uploadShader(*material.shader());
            for (UniformValue& uniformValue : material.uniformValues())
            {
                if (uniformValue.type() == UniformType_Texture)
                {
                    _renderer->uploadTexture(*uniformValue.asTexture());
                }
            }
        }
    }

    for (SkyBox& skyBox : scene().components<SkyBox>())
    {
        _renderer->uploadTexture(*skyBox.texture);
    }
}

void RenderSystem::prepareFrame(Camera& camera, Scene& scene, RenderTarget& target)
{
    // Clear the state from the last frame
    _frameData.clear();

    // Update the camer transform
    _frameData.cameraTransform.globalPosition = camera.position;

    // Update the camera's aspect ratio if needed
    if (camera.aspectRatio != target.aspectRatio())
    {
        camera.aspectRatio = target.aspectRatio();

        CameraSystem& cameraSystem = scene.system<CameraSystem>();
        cameraSystem.update(camera);
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
        _frameData.lightProbeCubeMap = &*lightProbe->texture;
    }

    // Get the cube map of the active sky box
    auto skyBox = scene.components<SkyBox>().begin();
    if (skyBox)
    {
        _frameData.skyBoxCubeMap = &*skyBox->texture;
    }

    // Build all render calls and sort by priority
    for (Entity& entity : scene.entities())
    {
        if (!entity.parent())
        {
            buildRenderCalls(camera, entity);
        }
    }

    // Add render calls for debug geometry
    if (scene.hasSystemType<DebugSystem>())
    {
        DebugSystem& debugSystem = scene.system<DebugSystem>();
        debugSystem.addRenderCalls(*this);
    }

    // Add each directional light to the frame data
    for (const DirectionalLight& light : scene.components<DirectionalLight>())
    {
        _frameData.directionalLights.push_back(light.iterator());
    }

    std::vector<Task::Handle> sortingTasks;

    // Spin up a task to sort the opaque render calls
    sortingTasks.push_back(_taskPool->enqueue([this]
    {
        std::vector<RenderCall>& renderCalls = _frameData.opaquePhysicalGeometry;
        std::sort(renderCalls.begin(), renderCalls.end());
    }));

    // Spin up a task to sort the transparent render calls
    sortingTasks.push_back(_taskPool->enqueue([this]
    {
        std::vector<RenderCall>& renderCalls = _frameData.transparentPhysicalGeometry;
        std::sort(renderCalls.begin(), renderCalls.end());
    }));

    // Wait until all sorting tasks complete
    for (Task::Handle& task : sortingTasks)
    {
        task->wait();
    }
}

void RenderSystem::renderFrame(Camera& camera, RenderTarget& target)
{
    // Opaque geometry rendering
    {
        Renderer::Frame frame = _renderer->beginFrame(_geometryFrameBuffer);
        frame.clear();

        for (RenderCall& renderCall : _frameData.opaquePhysicalGeometry)
        {
            renderMesh(frame, camera, target, *renderCall.material, *renderCall.mesh, *renderCall.transform);
        }
    }

    // Light rendering
    {
        Renderer::Frame frame = _renderer->beginFrame(backFrameBuffer());
        frame.clear();

        // Render environment light
        if (_frameData.lightProbeCubeMap)
        {
            frame.setShader(*_environmentShader);
            setBoundUniforms(frame, *_environmentShader, camera, target, _identityTransform);
            frame.renderMesh(*_screenMesh);
        }

        // Render directional lights
        {
            frame.setShader(*_directionalLightShader);

            // Render each directional light in the scene
            for (DirectionalLight::ConstIterator light : _frameData.directionalLights)
            {
                _frameData.primaryLightDirection = light->direction;
                _frameData.primaryLightColor = light->color;
                setBoundUniforms(frame, *_directionalLightShader, camera, target, _identityTransform);
                frame.renderMesh(*_screenMesh);
            }
        }
    }

    swapBackBuffer();

    // Composite
    {
        Renderer::Frame frame = _renderer->beginFrame(backFrameBuffer());
        frame.clear();
        frame.setShader(*_compositeShader);
        setBoundUniforms(frame, *_compositeShader, camera, target, _identityTransform);

        frame.renderMesh(*_screenMesh);

        // Transparent geometry rendering
        {
            for (RenderCall& renderCall : _frameData.transparentPhysicalGeometry)
            {
                renderMesh(frame, camera, target, *renderCall.material, *renderCall.mesh, *renderCall.transform);
            }
        }
    }

    swapBackBuffer();

    // Expose
    {
        Renderer::Frame frame = _renderer->beginFrame(target);
        frame.clear();
        frame.setShader(*_exposeShader);
        setBoundUniforms(frame, *_exposeShader, camera, target, _identityTransform);

        frame.renderMesh(*_screenMesh);
    }
}

void RenderSystem::initializeBuffers(unsigned width, unsigned height)
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

    _renderer->uploadFrameBuffer(_geometryFrameBuffer);
    _renderer->uploadFrameBuffer(_backFrameBuffers[0]);
    _renderer->uploadFrameBuffer(_backFrameBuffers[1]);
}

void RenderSystem::buildRenderCalls(Camera& camera, Entity& entity, bool frustumTest)
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
                    FrustumTestResult result = camera.frustum.testAxisAlignedBox(boundingBox->extents);
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
            addRenderCall(_frameData.cameraTransform, *_skyBoxMesh, *_skyBoxMaterial);
        }
    }
}

void RenderSystem::renderMesh(Renderer::Frame& frame, const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform)
{
    Shader& shader = *material.shader();

    // Set the shader
    frame.setShader(shader);
    setBoundUniforms(frame, shader, camera, target, transform);

    // Set the uniform values of the material
    UniformIndex index = 0;
    for (const UniformValue& value : material.uniformValues())
    {
        if (value)
        {
            const Uniform& uniform = shader.uniform(index);
            frame.setUniform(uniform, value);
        }
        ++index;
    }

    // Render the mesh
    frame.setCullMode(material.cullMode());
    frame.renderMesh(mesh);
}

void RenderSystem::setBoundUniforms(Renderer::Frame& frame, Shader& shader, const Camera& camera, const RenderTarget& target, const Transform& transform)
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
            frame.setUniform(uniform, Vector2(static_cast<double>(target.width()), static_cast<double>(target.height())));
            break;
        case UniformBinding_CameraPosition:
            frame.setUniform(uniform, camera.position);
            break;
        case UniformBinding_CameraFront:
            frame.setUniform(uniform, camera.front);
            break;
        case UniformBinding_CameraUp:
            frame.setUniform(uniform, camera.up);
            break;
        case UniformBinding_CameraExposure:
            frame.setUniform(uniform, camera.exposure);
            break;
        case UniformBinding_CameraOneOverGamma:
            frame.setUniform(uniform, 1.0 / camera.gamma);
            break;
        case UniformBinding_PrimaryLightDirection:
            frame.setUniform(uniform, _frameData.primaryLightDirection);
            break;
        case UniformBinding_PrimaryLightColor:
            frame.setUniform(uniform, _frameData.primaryLightColor);
            break;
        case UniformBinding_ViewMatrix:
            frame.setUniform(uniform, camera.viewMatrix);
            break;
        case UniformBinding_ProjectionMatrix:
            frame.setUniform(uniform, camera.projectionMatrix);
            break;
        case UniformBinding_ViewProjectionMatrix:
            frame.setUniform(uniform, camera.projectionMatrix * camera.viewMatrix);
            break;
        case UniformBinding_ModelMatrix:
            frame.setUniform(uniform, model);
            break;
        case UniformBinding_ModelViewMatrix:
            frame.setUniform(uniform, camera.viewMatrix * model);
            break;
        case UniformBinding_ModelViewProjectionMatrix:
            frame.setUniform(uniform, camera.projectionMatrix * (camera.viewMatrix * model));
            break;
        case UniformBinding_LightProbeCubeMap:
            assert(_frameData.lightProbeCubeMap);
            frame.setUniform(uniform, *_frameData.lightProbeCubeMap);
            break;
        case UniformBinding_SkyBoxCubeMap:
            assert(_frameData.skyBoxCubeMap);
            frame.setUniform(uniform, *_frameData.skyBoxCubeMap);
            break;
        case UniformBinding_DiffuseBuffer:
            frame.setUniform(uniform, _diffuseBuffer);
            break;
        case UniformBinding_MaterialBuffer:
            frame.setUniform(uniform, _materialBuffer);
            break;
        case UniformBinding_PositionBuffer:
            frame.setUniform(uniform, _positionBuffer);
            break;
        case UniformBinding_NormalBuffer:
            frame.setUniform(uniform, _normalBuffer);
            break;
        case UniformBinding_BackBuffer:
            frame.setUniform(uniform, lastBackBuffer());
            break;
        }
    }
}

void RenderSystem::swapBackBuffer()
{
    _frameData.backBufferIndex = (_frameData.backBufferIndex + 1) % 2;
}

Texture& RenderSystem::backBuffer()
{
    return _backBuffers[_frameData.backBufferIndex];
}

Texture& RenderSystem::lastBackBuffer()
{
    return _backBuffers[(_frameData.backBufferIndex + 1) % 2];
}

FrameBuffer& RenderSystem::backFrameBuffer()
{
    return _backFrameBuffers[_frameData.backBufferIndex];
}

bool RenderSystem::RenderCall::operator<(const RenderCall& other) const
{
    return material->shader()->priority() > other.material->shader()->priority();
}

RenderSystem::RenderCall::RenderCall()
{
}

RenderSystem::RenderCall::RenderCall(Transform& transform, Mesh& mesh, Material& material) :
    transform(&transform),
    mesh(&mesh),
    material(&material)
{
}

void RenderSystem::FrameData::clear()
{
    opaquePhysicalGeometry.clear();
    transparentPhysicalGeometry.clear();
    directionalLights.clear();

    cameraTransform = Transform();
    primaryLightDirection = Vector3();
    primaryLightColor = Color();
    lightProbeCubeMap =  nullptr;
    skyBoxCubeMap = nullptr;
    backBufferIndex = 0;
}