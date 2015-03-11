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
    System(engine, scene, SystemTickStage_Subsequent),
    _renderer(&engine.renderer()),
    _taskPool(&engine.taskPool()),
    _buffersInitialized(false)
{
}

void RenderSystem::initialize()
{
    _skyBoxMaterial = Material::Handle(new Material("Skybox"));
    _skyBoxMaterial->setShader(skyBoxShader);

    for (auto& model : scene().components<Model>())
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

    for (auto& skyBox : scene().components<SkyBox>())
    {
        _renderer->uploadTexture(*skyBox.texture);
    }
}

void RenderSystem::render(RenderTarget& target)
{
    auto cameraSystem = scene().system<CameraSystem>();
    if (cameraSystem)
    {
        Camera::Iterator camera = cameraSystem->activeCamera();
        if (camera)
        {
            prepareFrame(*camera, scene(), target);
            renderFrame(*camera, target);
        }
    }
}

void RenderSystem::addRenderCall(Transform& transform, Mesh& mesh, Material& material)
{
    const Shader::Handle& shader = material.shader();
    if (shader)
    {
        switch (shader->renderStage())
        {
        case RenderStage_PrePhysicalGeometry:
            _frameData.prePhysicalGeometry.emplace_back(transform, mesh, material);
            break;
        case RenderStage_PhysicalGeometry:
            _frameData.opaquePhysicalGeometry.emplace_back(transform, mesh, material);
            break;
        case RenderStage_None:
        case RenderStage_PostPhysicalGeometry:
            _frameData.postPhysicalGeometry.emplace_back(transform, mesh, material);
            break;
        }
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

        auto cameraSystem = scene.system<CameraSystem>();
        if (cameraSystem)
        {
            cameraSystem->update(camera);
        }
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
    for (auto& entity : scene.entities())
    {
        if (!entity.parent())
        {
            buildRenderCalls(camera, entity);
        }
    }

    // Add render calls for debug geometry
    auto debugSystem = scene.system<DebugSystem>();
    if (debugSystem && debugSystem->isEnabled())
    {
        debugSystem->addRenderCalls(*this);
    }

    // Add each directional light to the frame data
    for (const auto& light : scene.components<DirectionalLight>())
    {
        _frameData.directionalLights.push_back(light.iterator());
    }

    std::vector<Task::Handle> sortingTasks;

    // Spin up a task to sort the pre-physical geometry render calls
    sortingTasks.push_back(_taskPool->enqueue([this]
    {
        std::vector<RenderCall>& renderCalls = _frameData.prePhysicalGeometry;
        std::sort(renderCalls.begin(), renderCalls.end());
    }));

    // Spin up a task to sort the opaque physical geometry render calls
    sortingTasks.push_back(_taskPool->enqueue([this]
    {
        std::vector<RenderCall>& renderCalls = _frameData.opaquePhysicalGeometry;
        std::sort(renderCalls.begin(), renderCalls.end());
    }));

    // Spin up a task to sort the translucent physical geometry render calls
    sortingTasks.push_back(_taskPool->enqueue([this]
    {
        std::vector<RenderCall>& renderCalls = _frameData.translucentPhysicalGeometry;
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

        // Render pre-physical geometry
        for (RenderCall& renderCall : _frameData.prePhysicalGeometry)
        {
            renderMesh(frame, camera, target, *renderCall.material, *renderCall.mesh, *renderCall.transform);
        }

        // Render opaque physical geometry
        for (RenderCall& renderCall : _frameData.opaquePhysicalGeometry)
        {
            renderMesh(frame, camera, target, *renderCall.material, *renderCall.mesh, *renderCall.transform);
        }
    }

    // Light rendering
    {
        Renderer::Frame frame = _renderer->beginFrame(backFrameBuffer());
        frame.clear(false);

        // Render environment light
        if (_frameData.lightProbeCubeMap)
        {
            frame.setShader(*environmentShader);
            setBoundUniforms(frame, *environmentShader, camera, target, _identityTransform);
            frame.renderMesh(*screenMesh);
        }

        // Render each directional light in the scene
        frame.setShader(*directionalLightShader);
        for (DirectionalLight::ConstIterator light : _frameData.directionalLights)
        {
            _frameData.primaryLightDirection = light->direction;
            _frameData.primaryLightColor = light->color;
            setBoundUniforms(frame, *directionalLightShader, camera, target, _identityTransform);
            frame.renderMesh(*screenMesh);
        }
    }

    swapBackBuffer();

    // Composite
    {
        Renderer::Frame frame = _renderer->beginFrame(backFrameBuffer());
        frame.clear(false);

        // Composite
        frame.setShader(*compositeShader);
        setBoundUniforms(frame, *compositeShader, camera, target, _identityTransform);
        frame.renderMesh(*screenMesh);

        // Render translucent geometry
        for (RenderCall& renderCall : _frameData.translucentPhysicalGeometry)
        {
            renderMesh(frame, camera, target, *renderCall.material, *renderCall.mesh, *renderCall.transform);
        }

        // Render post-physical geometry
        for (RenderCall& renderCall : _frameData.postPhysicalGeometry)
        {
            renderMesh(frame, camera, target, *renderCall.material, *renderCall.mesh, *renderCall.transform);
        }
    }

    swapBackBuffer();

    // Expose
    {
        Renderer::Frame frame = _renderer->beginFrame(target);
        frame.clear();
        frame.setShader(*exposeShader);
        setBoundUniforms(frame, *exposeShader, camera, target, _identityTransform);

        frame.renderMesh(*screenMesh);
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

    // Position buffer: X Y Z
    _positionBuffer = Texture("PositionBuffer", width, height, PixelType_Float32, PixelFormat_Rgb, nearest, nearest, false, false);

    // Normal buffer: X Y Z Depth
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
        // If the entity has a model component
        auto model = entity.component<Model>();
        if (model)
        {
            // Render the model
            for (const ModelSurface& surface : model->surfaces)
            {
                Mesh& mesh = *surface.mesh;
                Material& material = *surface.material;

                // If the entity has a transform component
                auto transform = entity.component<Transform>();
                if (transform)
                {
                    addRenderCall(*transform, mesh, material);
                }
                else
                {
                    addRenderCall(_identityTransform, mesh, material);
                }
            }
        }

        // If the entity has a skybox component
        auto skyBox = entity.component<SkyBox>();
        if (skyBox)
        {
            addRenderCall(_frameData.cameraTransform, *skyBoxMesh, *_skyBoxMaterial);
        }

        // Render all children
        for (auto& child : entity.children())
        {
            buildRenderCalls(camera, child, frustumTest);
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
    prePhysicalGeometry.clear();
    opaquePhysicalGeometry.clear();
    translucentPhysicalGeometry.clear();
    postPhysicalGeometry.clear();
    directionalLights.clear();

    cameraTransform = Transform();
    primaryLightDirection = Vector3();
    primaryLightColor = Color();
    lightProbeCubeMap =  nullptr;
    skyBoxCubeMap = nullptr;
    backBufferIndex = 0;
}