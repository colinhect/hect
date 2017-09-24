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
#pragma once

#include <array>

#include "Hect/Core/Export.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/GeometryBuffer.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Scene/System.h"
#include "Hect/Scene/Systems/CameraSystem.h"
#include "Hect/Scene/Systems/DebugSystem.h"
#include "Hect/Scene/Components/CameraComponent.h"
#include "Hect/Scene/Components/DirectionalLightComponent.h"
#include "Hect/Scene/Components/TransformComponent.h"
#include "Hect/Scene/Components/LightProbeComponent.h"

namespace hect
{

///
/// Performs the high-level rendering algorithms (physically-based shading,
/// deferred shading, shadows, reflections, etc).
class HECT_EXPORT PhysicallyBasedSceneRenderer :
    public Uncopyable
{
public:
    PhysicallyBasedSceneRenderer(AssetCache& asset_cache, TaskPool& task_pool);

    ///
    /// Enqueues a render call to be rendered on the upcoming frame.
    ///
    /// \param transform The world-space transform.
    /// \param mesh The mesh to render.
    void enqueue_render_call(const TransformComponent& transform, Mesh& mesh);

    ///
    /// Enqueues a render call to be rendered on the upcoming frame.
    ///
    /// \param transform The world-space transform.
    /// \param mesh The mesh to render.
    /// \param material The material to use.
    void enqueue_render_call(const TransformComponent& transform, Mesh& mesh, Material& material);

    ///
    /// Renders the scene to a texture cube at the specified location.
    ///
    /// \note The properties of the camera camera are used.
    ///
    /// \param scene The scene to render.
    /// \param camera_system The camera system.
    /// \param renderer The renderer.
    /// \param position The position to render from.
    /// \param texture The texture to render to.
    void render_to_texture_cube(Scene& scene, CameraSystem& camera_system, Renderer& renderer, Vector3 position, TextureCube& texture);

    ///
    /// Renders the scene to the specified target.
    ///
    /// \param scene The scene to render.
    /// \param camera_system The camera system.
    /// \param renderer The renderer.
    /// \param target The target to render to.
    void render(Scene& scene, CameraSystem& camera_system, Renderer& renderer, RenderTarget& target);

private:
    void prepare_frame(Scene& scene, CameraSystem& camera_system, CameraComponent& camera, RenderTarget& target, GeometryBuffer& geometry_buffer);
    void render_frame(CameraComponent& camera, Renderer& renderer, RenderTarget& target);

    void upload_render_objects_for_scene(Scene& scene, Renderer& renderer);

    void build_render_calls(CameraComponent& camera, Entity& entity, bool frustum_test = true);
    void render_mesh(Renderer::Frame& frame, const CameraComponent& camera, const RenderTarget& target, Material& material, Mesh& mesh, const TransformComponent& transform);
    void set_bound_uniforms(Renderer::Frame& frame, Shader& shader, const CameraComponent& camera, const RenderTarget& target, const TransformComponent& transform);

    class RenderCall
    {
    public:
        RenderCall();
        RenderCall(const TransformComponent& transform, Mesh& mesh, Material& material);

        const TransformComponent* transform { nullptr };
        Mesh* mesh { nullptr };
        Material* material { nullptr };

        bool operator<(const RenderCall& other) const;
    };

    // Data required to render a frame
    class FrameData
    {
    public:
        void clear();

        std::vector<RenderCall> pre_physical_geometry;
        std::vector<RenderCall> opaque_physical_geometry;
        std::vector<RenderCall> translucent_physical_geometry;
        std::vector<RenderCall> post_physical_geometry;

        std::vector<DirectionalLightComponent::ConstIterator> directional_lights;

        TransformComponent camera_transform;
        Vector3 primary_light_direction;
        Color primary_light_color;
        TextureCube* light_probe_texture { nullptr };
        TextureCube* sky_box_texture { nullptr };
        GeometryBuffer* geometry_buffer { nullptr };
    } _frame_data;

    TaskPool& _task_pool;

    // The shader used to composite all components of the image into the final
    // image
    AssetHandle<Shader> _composite_shader;

    // The default material to fall-back on if no material is provided
    AssetHandle<Material> _default_material;

    // The shader used to perform directional lighting on physically lit
    // objects
    AssetHandle<Shader> _directional_light_shader;

    // The shader used to perform environmental lighting on physically lit
    // objects
    AssetHandle<Shader> _environment_shader;

    // The shader used to expose the final image to the window
    AssetHandle<Shader> _expose_shader;

    // The material used to render sky boxes
    AssetHandle<Material> _sky_box_material;

    // The mesh used to render sky boxes
    AssetHandle<Mesh> _sky_box_mesh;

    // The shader used to render sky boxes
    AssetHandle<Shader> _sky_box_shader;

    std::unique_ptr<GeometryBuffer> _geometry_buffer;
};

}
