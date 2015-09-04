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
#include "Hect/Math/Constants.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

RenderSystem::RenderSystem(Engine& engine, Scene& scene) :
    System(engine, scene, SystemTickStage::Subsequent),
    _renderer(engine.renderer()),
    _taskPool(engine.taskPool()),
    _cameraSystem(scene.system<CameraSystem>()),
    _debugSystem(scene.system<DebugSystem>())
{
}

void RenderSystem::addRenderCall(Transform& transform, Mesh& mesh, Material& material)
{
    const Shader::Handle& shader = material.shader();
    if (shader)
    {
        switch (shader->renderStage())
        {
        case RenderStage::PrePhysicalGeometry:
            _frameData.prePhysicalGeometry.emplace_back(transform, mesh, material);
            break;
        case RenderStage::PhysicalGeometry:
            _frameData.opaquePhysicalGeometry.emplace_back(transform, mesh, material);
            break;
        case RenderStage::None:
        case RenderStage::PostPhysicalGeometry:
            _frameData.postPhysicalGeometry.emplace_back(transform, mesh, material);
            break;
        }
    }
}

void RenderSystem::renderToTextureCube(const Vector3& position, double nearClip, double farClip, TextureCube& texture)
{
    // These values are specific to OpenGL's cube map conventions (issue #189)
    static std::vector<std::pair<Vector3, Vector3>> cameraVectors =
    {
        std::pair<Vector3, Vector3>(Vector3::UnitX, -Vector3::UnitY),
        std::pair<Vector3, Vector3>(-Vector3::UnitX, -Vector3::UnitY),
        std::pair<Vector3, Vector3>(Vector3::UnitY, Vector3::UnitZ),
        std::pair<Vector3, Vector3>(-Vector3::UnitY, -Vector3::UnitZ),
        std::pair<Vector3, Vector3>(Vector3::UnitZ, -Vector3::UnitY),
        std::pair<Vector3, Vector3>(-Vector3::UnitZ, -Vector3::UnitY),
    };

    if (_cameraSystem)
    {
        // Create a geometry buffer for rendering to the texture cube
        unsigned width = texture.width();
        unsigned height = texture.height();
        GeometryBuffer geometryBuffer(width, height);

        // Create a transient entity for holding our camera
        Entity::Iterator entity = scene().createEntity();
        entity->setTransient(true);

        // Create the camera
        Camera::Iterator camera = entity->addComponent<Camera>();
        camera->position = position;
        camera->exposure = -1.0;
        camera->nearClip = nearClip;
        camera->farClip = farClip;
        camera->fieldOfView = Angle::fromRadians(Pi / 2);

        // For each side of the cube face
        for (unsigned i = 0; i < 6; ++i)
        {
            // Update the camera's matrices
            camera->front = cameraVectors[i].first;
            camera->up = cameraVectors[i].second;
            _cameraSystem->update(*camera);

            // Create the frame buffer and attach the corresponding face
            // of the cubic texture
            FrameBuffer frameBuffer(width, height);
            frameBuffer.attach(FrameBufferSlot::Color0, static_cast<CubeSide>(i), texture);

            // Render the frame
            prepareFrame(*camera, scene(), frameBuffer, geometryBuffer);
            renderFrame(*camera, frameBuffer);
        }

        // Destroy the transient entity holding the camera
        entity->destroy();
    }
}

void RenderSystem::initialize()
{
    _skyBoxMaterial = new Material("Skybox");
    _skyBoxMaterial->setShader(skyBoxShader);
    _skyBoxMaterial->setCullMode(CullMode::None);

    for (Model& model : scene().components<Model>())
    {
        for (ModelSurface& surface : model.surfaces)
        {
            Mesh& mesh = *surface.mesh;
            Material& material = *surface.material;

            _renderer.uploadMesh(mesh);
            _renderer.uploadShader(*material.shader());
            for (UniformValue& uniformValue : material.uniformValues())
            {
                if (uniformValue.type() == UniformType::Texture2)
                {
                    _renderer.uploadTexture(*uniformValue.asTexture2());
                }
            }
        }
    }

    for (SkyBox& skyBox : scene().components<SkyBox>())
    {
        _renderer.uploadTexture(*skyBox.texture);
    }
}

void RenderSystem::render(RenderTarget& target)
{
    if (_cameraSystem)
    {
        Camera::Iterator camera = _cameraSystem->activeCamera();
        if (camera)
        {
            if (!_geometryBuffer)
            {
                _geometryBuffer.reset(new GeometryBuffer(target.width(), target.height()));
            }

            prepareFrame(*camera, scene(), target, *_geometryBuffer);
            renderFrame(*camera, target);
        }
        else
        {
            // If there is no camera then just clear the target
            Renderer::Frame frame = _renderer.beginFrame(target);
            frame.clear();
        }
    }
}

void RenderSystem::prepareFrame(Camera& camera, Scene& scene, RenderTarget& target, GeometryBuffer& geometryBuffer)
{
    // Clear the state from the last frame and begin initializing it for the
    // next frame
    _frameData.clear();
    _frameData.geometryBuffer = &geometryBuffer;

    // Update the camera transform
    _frameData.cameraTransform.globalPosition = camera.position;

    // Update the camera's aspect ratio if needed
    if (camera.aspectRatio != target.aspectRatio())
    {
        camera.aspectRatio = target.aspectRatio();

        if (_cameraSystem)
        {
            _cameraSystem->update(camera);
        }
    }

    // Get the cube map of the active light probe
    LightProbe::Iterator lightProbe = scene.components<LightProbe>().begin();
    if (lightProbe && lightProbe->texture)
    {
        _frameData.lightProbeTexture = &*lightProbe->texture;
    }

    // Get the cube map of the active sky box
    SkyBox::Iterator skyBox = scene.components<SkyBox>().begin();
    if (skyBox && skyBox->texture)
    {
        _frameData.skyBoxTexture = &*skyBox->texture;
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
    if (_debugSystem && _debugSystem->isEnabled())
    {
        _debugSystem->addRenderCalls(*this);
    }

    // Add each directional light to the frame data
    for (const DirectionalLight& light : scene.components<DirectionalLight>())
    {
        _frameData.directionalLights.push_back(light.iterator());
    }

    std::vector<Task::Handle> sortingTasks;

    // Spin up a task to sort the pre-physical geometry render calls
    sortingTasks.push_back(_taskPool.enqueue([this]
    {
        std::vector<RenderCall>& renderCalls = _frameData.prePhysicalGeometry;
        std::sort(renderCalls.begin(), renderCalls.end());
    }));

    // Spin up a task to sort the opaque physical geometry render calls
    sortingTasks.push_back(_taskPool.enqueue([this]
    {
        std::vector<RenderCall>& renderCalls = _frameData.opaquePhysicalGeometry;
        std::sort(renderCalls.begin(), renderCalls.end());
    }));

    // Spin up a task to sort the translucent physical geometry render calls
    sortingTasks.push_back(_taskPool.enqueue([this]
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
    GeometryBuffer& geometryBuffer = *_frameData.geometryBuffer;

    // Opaque geometry rendering
    {
        Renderer::Frame frame = _renderer.beginFrame(geometryBuffer.frameBuffer());
        frame.clear(camera.clearColor);

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
        Renderer::Frame frame = _renderer.beginFrame(geometryBuffer.backFrameBuffer());
        frame.clear(Color::Zero, false);

        // Render environment light
        if (_frameData.lightProbeTexture)
        {
            frame.setShader(*environmentShader);
            setBoundUniforms(frame, *environmentShader, camera, target, _identityTransform);
            frame.renderViewport();
        }

        // Render each directional light in the scene
        frame.setShader(*directionalLightShader);
        for (DirectionalLight::ConstIterator light : _frameData.directionalLights)
        {
            _frameData.primaryLightDirection = light->direction;
            _frameData.primaryLightColor = light->color;
            setBoundUniforms(frame, *directionalLightShader, camera, target, _identityTransform);
            frame.renderViewport();
        }
    }

    geometryBuffer.swapBackBuffers();

    // Composite
    {
        Renderer::Frame frame = _renderer.beginFrame(geometryBuffer.backFrameBuffer());
        frame.clear(Color::Zero, false);

        // Composite
        frame.setShader(*compositeShader);
        setBoundUniforms(frame, *compositeShader, camera, target, _identityTransform);
        frame.renderViewport();

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

    geometryBuffer.swapBackBuffers();

    // Expose
    {
        Renderer::Frame frame = _renderer.beginFrame(target);
        frame.clear(camera.clearColor);
        frame.setShader(*exposeShader);
        setBoundUniforms(frame, *exposeShader, camera, target, _identityTransform);

        frame.renderViewport();
    }
}

void RenderSystem::buildRenderCalls(Camera& camera, Entity& entity, bool frustumTest)
{
    // By default, assume that the entity is visible
    bool visible = true;

    // If we need to test this entity against the frustum
    if (frustumTest)
    {
        // If the entity has a bounding box
        BoundingBox::Iterator boundingBox = entity.component<BoundingBox>();
        if (boundingBox)
        {
            // Test the bounding box against the frustum
            FrustumTestResult result = camera.frustum.testAxisAlignedBox(boundingBox->extents);
            if (result == FrustumTestResult::Inside)
            {
                // No need to test any children
                frustumTest = false;
            }
            else if (result == FrustumTestResult::Intersect)
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
        Model::Iterator model = entity.component<Model>();
        if (model && model->visible)
        {
            // Render the model
            for (const ModelSurface& surface : model->surfaces)
            {
                if (!surface.visible)
                {
                    continue;
                }

                Mesh& mesh = *surface.mesh;
                Material& material = *surface.material;

                // If the entity has a transform component
                Transform::Iterator transform = entity.component<Transform>();
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
        SkyBox::Iterator skyBox = entity.component<SkyBox>();
        if (skyBox)
        {
            addRenderCall(_frameData.cameraTransform, *skyBoxMesh, *_skyBoxMaterial);
        }

        // Render all children
        for (Entity& child : entity.children())
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
    if (transform.globalPosition != Vector3::Zero)
    {
        model.translate(transform.globalPosition);
    }

    // Scale the matrix by the global scale
    if (transform.globalScale != Vector3::Zero)
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
        case UniformBinding::None:
            break;
        case UniformBinding::RenderTargetSize:
            frame.setUniform(uniform, Vector2(static_cast<double>(target.width()), static_cast<double>(target.height())));
            break;
        case UniformBinding::CameraPosition:
            frame.setUniform(uniform, camera.position);
            break;
        case UniformBinding::CameraFront:
            frame.setUniform(uniform, camera.front);
            break;
        case UniformBinding::CameraUp:
            frame.setUniform(uniform, camera.up);
            break;
        case UniformBinding::CameraExposure:
            frame.setUniform(uniform, camera.exposure);
            break;
        case UniformBinding::CameraOneOverGamma:
            frame.setUniform(uniform, 1.0 / camera.gamma);
            break;
        case UniformBinding::PrimaryLightDirection:
            frame.setUniform(uniform, _frameData.primaryLightDirection);
            break;
        case UniformBinding::PrimaryLightColor:
            frame.setUniform(uniform, _frameData.primaryLightColor);
            break;
        case UniformBinding::ViewMatrix:
            frame.setUniform(uniform, camera.viewMatrix);
            break;
        case UniformBinding::ProjectionMatrix:
            frame.setUniform(uniform, camera.projectionMatrix);
            break;
        case UniformBinding::ViewProjectionMatrix:
            frame.setUniform(uniform, camera.projectionMatrix * camera.viewMatrix);
            break;
        case UniformBinding::ModelMatrix:
            frame.setUniform(uniform, model);
            break;
        case UniformBinding::ModelViewMatrix:
            frame.setUniform(uniform, camera.viewMatrix * model);
            break;
        case UniformBinding::ModelViewProjectionMatrix:
            frame.setUniform(uniform, camera.projectionMatrix * (camera.viewMatrix * model));
            break;
        case UniformBinding::LightProbeTexture:
            assert(_frameData.lightProbeTexture);
            frame.setUniform(uniform, *_frameData.lightProbeTexture);
            break;
        case UniformBinding::SkyBoxTexture:
            assert(_frameData.skyBoxTexture);
            frame.setUniform(uniform, *_frameData.skyBoxTexture);
            break;
        case UniformBinding::DiffuseBuffer:
            assert(_frameData.geometryBuffer);
            frame.setUniform(uniform, _frameData.geometryBuffer->diffuseBuffer());
            break;
        case UniformBinding::MaterialBuffer:
            assert(_frameData.geometryBuffer);
            frame.setUniform(uniform, _frameData.geometryBuffer->materialBuffer());
            break;
        case UniformBinding::PositionBuffer:
            assert(_frameData.geometryBuffer);
            frame.setUniform(uniform, _frameData.geometryBuffer->positionBuffer());
            break;
        case UniformBinding::NormalBuffer:
            assert(_frameData.geometryBuffer);
            frame.setUniform(uniform, _frameData.geometryBuffer->normalBuffer());
            break;
        case UniformBinding::BackBuffer:
            assert(_frameData.geometryBuffer);
            frame.setUniform(uniform, _frameData.geometryBuffer->lastBackBuffer());
            break;
        }
    }
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
    lightProbeTexture =  nullptr;
    skyBoxTexture = nullptr;
    geometryBuffer = nullptr;
}
