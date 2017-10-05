///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "PhysicallyBasedSceneRenderer.h"

#include <algorithm>

#include "Hect/Math/Constants.h"
#include "Hect/Runtime/Engine.h"
#include "Hect/Scene/Components/BoundingBoxComponent.h"
#include "Hect/Scene/Components/GeometryComponent.h"
#include "Hect/Scene/Components/LightProbeComponent.h"
#include "Hect/Scene/Components/SkyBoxComponent.h"
#include "Hect/Units/Angle.h"

using namespace hect;

PhysicallyBasedSceneRenderer::PhysicallyBasedSceneRenderer(AssetCache& asset_cache, TaskPool& task_pool) :
    _task_pool(task_pool),
    _composite_shader(asset_cache, HECT_ASSET("Hect/Rendering/Composite.shader")),
    _default_material(asset_cache, HECT_ASSET("Hect/Materials/Default.material")),
    _directional_light_shader(asset_cache, HECT_ASSET("Hect/Rendering/DirectionalLight.shader")),
    _environment_shader(asset_cache, HECT_ASSET("Hect/Rendering/Environment.shader")),
    _expose_shader(asset_cache, HECT_ASSET("Hect/Rendering/Expose.shader")),
    _sky_box_mesh(asset_cache, HECT_ASSET("Hect/Rendering/SkyBox.mesh")),
    _sky_box_shader(asset_cache, HECT_ASSET("Hect/Shaders/SkyBox.shader"))
{
    _sky_box_material.reset(new Material("Skybox"));
    _sky_box_material->set_shader(_sky_box_shader);
    _sky_box_material->set_cull_mode(CullMode::None);
}

void PhysicallyBasedSceneRenderer::enqueue_render_call(const TransformComponent& transform, Mesh& mesh)
{
    return enqueue_render_call(transform, mesh, *_default_material);
}

void PhysicallyBasedSceneRenderer::enqueue_render_call(const TransformComponent& transform, Mesh& mesh, Material& material)
{
    const AssetHandle<Shader>& shader = material.shader();
    if (shader)
    {
        switch (shader->render_stage())
        {
        case RenderStage::PrePhysicalGeometry:
            _frame_data.pre_physical_geometry.emplace_back(transform, mesh, material);
            break;
        case RenderStage::PhysicalGeometry:
            _frame_data.opaque_physical_geometry.emplace_back(transform, mesh, material);
            break;
        case RenderStage::PostPhysicalGeometry:
        case RenderStage::None:
            _frame_data.post_physical_geometry.emplace_back(transform, mesh, material);
            break;
        }
    }
}

void PhysicallyBasedSceneRenderer::render_to_texture_cube(Scene& scene, CameraSystem& camera_system, Renderer& renderer, Vector3 position, TextureCube& texture)
{
    // These values are specific to OpenGL's cube map conventions (issue #189)
    static std::vector<std::pair<Vector3, Vector3>> camera_vectors =
    {
        std::pair<Vector3, Vector3>(Vector3::UnitX, -Vector3::UnitY),
        std::pair<Vector3, Vector3>(-Vector3::UnitX, -Vector3::UnitY),
        std::pair<Vector3, Vector3>(Vector3::UnitY, Vector3::UnitZ),
        std::pair<Vector3, Vector3>(-Vector3::UnitY, -Vector3::UnitZ),
        std::pair<Vector3, Vector3>(Vector3::UnitZ, -Vector3::UnitY),
        std::pair<Vector3, Vector3>(-Vector3::UnitZ, -Vector3::UnitY),
    };

    // Create a geometry buffer for rendering to the texture cube
    unsigned width = texture.width();
    unsigned height = texture.height();
    GeometryBuffer geometry_buffer(width, height);

    // Create a transient entity for holding our camera
    Entity& entity = scene.create_entity();
    entity.set_transient(true);

    // Create the camera
    auto& camera = entity.add_component<CameraComponent>();
    camera.position = position;
    camera.exposure = -1.0;
    camera.field_of_view = Degrees(180.0);

    const CameraComponent* active_camera = camera_system.active_camera();
    if (active_camera)
    {
        camera.near_clip = active_camera->near_clip;
        camera.far_clip = active_camera->far_clip;
    }

    // For each side of the cube face
    for (unsigned i = 0; i < 6; ++i)
    {
        // Update the camera's matrices
        camera.front = camera_vectors[i].first;
        camera.up = camera_vectors[i].second;
        camera_system.update_camera(camera);

        // Create the frame buffer and attach the corresponding face
        // of the cubic texture
        FrameBuffer frame_buffer(width, height);
        frame_buffer.attach(FrameBufferSlot::Color0, static_cast<CubeSide>(i), texture);

        // Render the frame
        prepare_frame(scene, camera_system, camera, frame_buffer, geometry_buffer);
        render_frame(camera, renderer, frame_buffer);
    }

    // Destroy the transient entity holding the camera
    entity.destroy();
}

void PhysicallyBasedSceneRenderer::render(Scene& scene, CameraSystem& camera_system, Renderer& renderer, RenderTarget& target)
{
    CameraComponent* camera = camera_system.active_camera();
    if (camera)
    {
        if (!_geometry_buffer)
        {
            _geometry_buffer.reset(new GeometryBuffer(target.width(), target.height()));
        }

        prepare_frame(scene, camera_system, *camera, target, *_geometry_buffer);
        render_frame(*camera, renderer, target);
    }
    else
    {
        // If there is no camera then just clear the target
        Renderer::Frame frame = renderer.begin_frame(target);
        frame.clear();
    }
}

void PhysicallyBasedSceneRenderer::prepare_frame(Scene& scene, CameraSystem& camera_system, CameraComponent& camera, RenderTarget& target, GeometryBuffer& geometry_buffer)
{
    // Clear the state from the last frame and begin initializing it for the
    // next frame
    _frame_data.clear();
    _frame_data.geometry_buffer = &geometry_buffer;

    // Update the camera transform
    _frame_data.camera_transform.global_position = camera.position;

    // Update the camera's aspect ratio if needed
    if (camera.aspect_ratio != target.aspect_ratio())
    {
        camera.aspect_ratio = target.aspect_ratio();

        camera_system.update_camera(camera);
    }

    // Get the cube map of the active light probe
    auto light_probe = scene.components<LightProbeComponent>().begin();
    if (light_probe && light_probe->texture)
    {
        _frame_data.light_probe_texture = &*light_probe->texture;
    }

    // Get the cube map of the active sky box
    auto sky_box = scene.components<SkyBoxComponent>().begin();
    if (sky_box && sky_box->texture)
    {
        _frame_data.sky_box_texture = &*sky_box->texture;
    }

    // Build all render calls and sort by priority
    for (Entity& entity : scene.entities())
    {
        if (!entity.parent())
        {
            build_render_calls(camera, entity);
        }
    }

    // Add each directional light to the frame data
    for (const DirectionalLightComponent& light : scene.components<DirectionalLightComponent>())
    {
        _frame_data.directional_lights.push_back(&light);
    }

    std::vector<Task::Handle> sorting_tasks;

    // Spin up a task to sort the pre-physical geometry render calls
    sorting_tasks.push_back(_task_pool.enqueue([this]
    {
        std::vector<RenderCall>& render_calls = _frame_data.pre_physical_geometry;
        std::sort(render_calls.begin(), render_calls.end());
    }));

    // Spin up a task to sort the opaque physical geometry render calls
    sorting_tasks.push_back(_task_pool.enqueue([this]
    {
        std::vector<RenderCall>& render_calls = _frame_data.opaque_physical_geometry;
        std::sort(render_calls.begin(), render_calls.end());
    }));

    // Spin up a task to sort the translucent physical geometry render calls
    sorting_tasks.push_back(_task_pool.enqueue([this]
    {
        std::vector<RenderCall>& render_calls = _frame_data.translucent_physical_geometry;
        std::sort(render_calls.begin(), render_calls.end());
    }));

    // Wait until all sorting tasks complete
    for (Task::Handle& task : sorting_tasks)
    {
        task->wait();
    }
}

void PhysicallyBasedSceneRenderer::render_frame(CameraComponent& camera, Renderer& renderer, RenderTarget& target)
{
    GeometryBuffer& geometry_buffer = *_frame_data.geometry_buffer;

    // Opaque geometry rendering
    {
        Renderer::Frame frame = renderer.begin_frame(geometry_buffer.frame_buffer());
        frame.clear(camera.clear_color);

        // Render pre-physical geometry
        for (RenderCall& render_call : _frame_data.pre_physical_geometry)
        {
            render_mesh(frame, camera, target, *render_call.material, *render_call.mesh, *render_call.transform);
        }

        // Render opaque physical geometry
        for (RenderCall& render_call : _frame_data.opaque_physical_geometry)
        {
            render_mesh(frame, camera, target, *render_call.material, *render_call.mesh, *render_call.transform);
        }
    }

    // Light rendering
    {
        Renderer::Frame frame = renderer.begin_frame(geometry_buffer.back_frame_buffer());
        frame.clear(Color::Zero, false);

        // Render environment light
        if (_frame_data.light_probe_texture)
        {
            frame.set_shader(*_environment_shader);
            set_bound_uniforms(frame, *_environment_shader, camera, target, TransformComponent::Identity);
            frame.render_viewport();
        }

        // Render each directional light in the scene
        if (!_frame_data.directional_lights.empty())
        {
            frame.set_shader(*_directional_light_shader);
            for (const DirectionalLightComponent* light : _frame_data.directional_lights)
            {
                _frame_data.primary_light_direction = light->direction;
                _frame_data.primary_light_color = light->color;
                set_bound_uniforms(frame, *_directional_light_shader, camera, target, TransformComponent::Identity);
                frame.render_viewport();
            }
        }
    }

    geometry_buffer.swap_back_buffers();

    // Composite
    {
        Renderer::Frame frame = renderer.begin_frame(geometry_buffer.back_frame_buffer());
        frame.clear(Color::Zero, false);

        // Composite
        frame.set_shader(*_composite_shader);
        set_bound_uniforms(frame, *_composite_shader, camera, target, TransformComponent::Identity);
        frame.render_viewport();

        // Render translucent geometry
        for (RenderCall& render_call : _frame_data.translucent_physical_geometry)
        {
            render_mesh(frame, camera, target, *render_call.material, *render_call.mesh, *render_call.transform);
        }

        // Render post-physical geometry
        for (RenderCall& render_call : _frame_data.post_physical_geometry)
        {
            render_mesh(frame, camera, target, *render_call.material, *render_call.mesh, *render_call.transform);
        }
    }

    geometry_buffer.swap_back_buffers();

    // Expose
    {
        Renderer::Frame frame = renderer.begin_frame(target);
        frame.clear(camera.clear_color);
        frame.set_shader(*_expose_shader);
        set_bound_uniforms(frame, *_expose_shader, camera, target, TransformComponent::Identity);

        frame.render_viewport();
    }
}

void PhysicallyBasedSceneRenderer::upload_render_objects_for_scene(Scene& scene, Renderer& renderer)
{
    for (GeometryComponent& geometry : scene.components<GeometryComponent>())
    {
        for (GeometrySurface& surface : geometry.surfaces)
        {
            Mesh& mesh = *surface.mesh;
            Material& material = *surface.material;

            renderer.upload_mesh(mesh);
            renderer.upload_shader(*material.shader());
            for (UniformValue& uniform_value : material.uniform_values())
            {
                if (uniform_value.type() == UniformType::Texture2)
                {
                    renderer.upload_texture(*uniform_value.as_texture2());
                }
            }
        }
    }

    for (SkyBoxComponent& sky_box : scene.components<SkyBoxComponent>())
    {
        renderer.upload_texture(*sky_box.texture);
    }
}

void PhysicallyBasedSceneRenderer::build_render_calls(CameraComponent& camera, Entity& entity, bool frustum_test)
{
    // By default, assume that the entity is visible
    bool visible = true;

    // If we need to test this entity against the frustum
    if (frustum_test)
    {
        if (entity.has_component<BoundingBoxComponent>())
        {
            auto& bounding_box = entity.component<BoundingBoxComponent>();

            // Test the bounding box against the frustum
            FrustumTestResult result = camera.frustum.test_axis_aligned_box(bounding_box.globalExtents);
            if (result == FrustumTestResult::Inside)
            {
                // No need to test any children
                frustum_test = false;
            }
            else if (result == FrustumTestResult::Intersect)
            {
                // Need to test children
                frustum_test = true;
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
        if (entity.has_component<GeometryComponent>())
        {
            auto& geometry = entity.component<GeometryComponent>();
            if (geometry.visible)
            {
                // Render the mesh surfaces
                for (const GeometrySurface& surface : geometry.surfaces)
                {
                    if (!surface.visible)
                    {
                        continue;
                    }

                    Mesh& mesh = *surface.mesh;

                    TransformComponent transform = TransformComponent::Identity;

                    if (entity.has_component<TransformComponent>())
                    {
                        auto& transform_component = entity.component<TransformComponent>();
                        transform = transform_component;
                    }

                    if (surface.material)
                    {
                        enqueue_render_call(transform, mesh, *surface.material);
                    }
                    else
                    {
                        enqueue_render_call(transform, mesh);
                    }
                }
            }
        }

        if (entity.has_component<SkyBoxComponent>())
        {
            enqueue_render_call(_frame_data.camera_transform, *_sky_box_mesh, *_sky_box_material);
        }

        // Render all children
        for (Entity& child : entity.children())
        {
            build_render_calls(camera, child, frustum_test);
        }
    }
}

void PhysicallyBasedSceneRenderer::render_mesh(Renderer::Frame& frame, const CameraComponent& camera, const RenderTarget& target, Material& material, Mesh& mesh, const TransformComponent& transform)
{
    Shader& shader = *material.shader();

    // Set the shader
    frame.set_shader(shader);
    set_bound_uniforms(frame, shader, camera, target, transform);

    // Set the uniform values of the material
    UniformIndex index = 0;
    for (const UniformValue& value : material.uniform_values())
    {
        if (value)
        {
            const Uniform& uniform = shader.uniform(index);
            frame.set_uniform(uniform, value);
        }
        ++index;
    }

    // Render the mesh
    frame.set_cull_mode(material.cull_mode());
    frame.render_mesh(mesh);
}

void PhysicallyBasedSceneRenderer::set_bound_uniforms(Renderer::Frame& frame, Shader& shader, const CameraComponent& camera, const RenderTarget& target, const TransformComponent& transform)
{
    // Buid the model matrix
    Matrix4 model;

    // Translate the matrix to the global position
    if (transform.global_position != Vector3::Zero)
    {
        model.translate(transform.global_position);
    }

    // Scale the matrix by the global scale
    if (transform.global_scale != Vector3::Zero)
    {
        model.scale(transform.global_scale);
    }

    // Rotate the matrix by the global rotation
    if (transform.global_rotation != Quaternion())
    {
        model.rotate(transform.global_rotation);
    }

    for (const Uniform& uniform : shader.uniforms())
    {
        UniformBinding binding = uniform.binding();
        switch (binding)
        {
        case UniformBinding::None:
            break;
        case UniformBinding::RenderTargetSize:
            frame.set_uniform(uniform, Vector2(static_cast<double>(target.width()), static_cast<double>(target.height())));
            break;
        case UniformBinding::CameraPosition:
            frame.set_uniform(uniform, camera.position);
            break;
        case UniformBinding::CameraFront:
            frame.set_uniform(uniform, camera.front);
            break;
        case UniformBinding::CameraUp:
            frame.set_uniform(uniform, camera.up);
            break;
        case UniformBinding::CameraExposure:
            frame.set_uniform(uniform, camera.exposure);
            break;
        case UniformBinding::CameraOneOverGamma:
            frame.set_uniform(uniform, 1.0 / camera.gamma);
            break;
        case UniformBinding::PrimaryLightDirection:
            frame.set_uniform(uniform, _frame_data.primary_light_direction);
            break;
        case UniformBinding::PrimaryLightColor:
            frame.set_uniform(uniform, _frame_data.primary_light_color);
            break;
        case UniformBinding::ViewMatrix:
            frame.set_uniform(uniform, camera.view_matrix);
            break;
        case UniformBinding::ProjectionMatrix:
            frame.set_uniform(uniform, camera.projection_matrix);
            break;
        case UniformBinding::ViewProjectionMatrix:
            frame.set_uniform(uniform, camera.projection_matrix * camera.view_matrix);
            break;
        case UniformBinding::ModelMatrix:
            frame.set_uniform(uniform, model);
            break;
        case UniformBinding::ModelViewMatrix:
            frame.set_uniform(uniform, camera.view_matrix * model);
            break;
        case UniformBinding::ModelViewProjectionMatrix:
            frame.set_uniform(uniform, camera.projection_matrix * (camera.view_matrix * model));
            break;
        case UniformBinding::LightProbeTexture:
            assert(_frame_data.light_probe_texture);
            frame.set_uniform(uniform, *_frame_data.light_probe_texture);
            break;
        case UniformBinding::SkyBoxTexture:
            assert(_frame_data.sky_box_texture);
            frame.set_uniform(uniform, *_frame_data.sky_box_texture);
            break;
        case UniformBinding::DiffuseBuffer:
            assert(_frame_data.geometry_buffer);
            frame.set_uniform(uniform, _frame_data.geometry_buffer->diffuse_buffer());
            break;
        case UniformBinding::MaterialBuffer:
            assert(_frame_data.geometry_buffer);
            frame.set_uniform(uniform, _frame_data.geometry_buffer->material_buffer());
            break;
        case UniformBinding::PositionBuffer:
            assert(_frame_data.geometry_buffer);
            frame.set_uniform(uniform, _frame_data.geometry_buffer->position_buffer());
            break;
        case UniformBinding::NormalBuffer:
            assert(_frame_data.geometry_buffer);
            frame.set_uniform(uniform, _frame_data.geometry_buffer->normal_buffer());
            break;
        case UniformBinding::BackBuffer:
            assert(_frame_data.geometry_buffer);
            frame.set_uniform(uniform, _frame_data.geometry_buffer->last_back_buffer());
            break;
        }
    }
}

bool PhysicallyBasedSceneRenderer::RenderCall::operator<(const RenderCall& other) const
{
    return material->shader()->priority() > other.material->shader()->priority();
}

PhysicallyBasedSceneRenderer::RenderCall::RenderCall()
{
}

PhysicallyBasedSceneRenderer::RenderCall::RenderCall(const TransformComponent& transform, Mesh& mesh, Material& material) :
    transform(&transform),
    mesh(&mesh),
    material(&material)
{
}

void PhysicallyBasedSceneRenderer::FrameData::clear()
{
    pre_physical_geometry.clear();
    opaque_physical_geometry.clear();
    translucent_physical_geometry.clear();
    post_physical_geometry.clear();

    directional_lights.clear();

    camera_transform = TransformComponent();
    primary_light_direction = Vector3();
    primary_light_color = Color();
    light_probe_texture = nullptr;
    sky_box_texture = nullptr;
    geometry_buffer = nullptr;
}
