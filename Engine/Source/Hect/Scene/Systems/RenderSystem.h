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
///
/// \system
class HECT_EXPORT RenderSystem :
    public System<RenderSystem>
{
public:
    RenderSystem(Engine& engine, Scene& scene);

    ///
    /// Enqueues a render call to be rendered on the upcoming frame.
    ///
    /// \param transform The world-space transform.
    /// \param mesh The mesh to render.
    /// \param material The material to use.
    void addRenderCall(const TransformComponent& transform, Mesh& mesh, Material& material);

    ///
    /// Renders the scene to a texture cube at the specified location.
    ///
    /// \note The properties of the camera camera are used.
    ///
    /// \param position The position to render from.
    /// \param texture The texture to render to.
    void renderToTextureCube(Vector3 position, TextureCube& texture);

    void initialize() override;
    void render(RenderTarget& target) override;

    ///
    /// The shader used to expose the final image to the window.
    ///
    /// \property{required}
    Shader::Handle exposeShader;

    ///
    /// The shader used to composite all components of the image into the
    /// final image.
    ///
    /// \property{required}
    Shader::Handle compositeShader;

    ///
    /// The shader used to perform environmental lighting on physically lit
    /// objects.
    ///
    /// \property{required}
    Shader::Handle environmentShader;

    ///
    /// The shader used to perform directional lighting on physically lit
    /// objects.
    ///
    /// \property{required}
    Shader::Handle directionalLightShader;

    ///
    /// The shader used to render sky boxes.
    ///
    /// \property{required}
    Shader::Handle skyBoxShader;

    ///
    /// The mesh used to render sky boxes.
    ///
    /// \property{required}
    Mesh::Handle skyBoxMesh;

private:
    void prepareFrame(CameraComponent& camera, Scene& scene, RenderTarget& target, GeometryBuffer& geometryBuffer);
    void renderFrame(CameraComponent& camera, RenderTarget& target);

    void buildRenderCalls(CameraComponent& camera, Entity& entity, bool frustumTest = true);
    void renderMesh(Renderer::Frame& frame, const CameraComponent& camera, const RenderTarget& target, Material& material, Mesh& mesh, const TransformComponent& transform);
    void setBoundUniforms(Renderer::Frame& frame, Shader& shader, const CameraComponent& camera, const RenderTarget& target, const TransformComponent& transform);

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

        std::vector<RenderCall> prePhysicalGeometry;
        std::vector<RenderCall> opaquePhysicalGeometry;
        std::vector<RenderCall> translucentPhysicalGeometry;
        std::vector<RenderCall> postPhysicalGeometry;

        std::vector<DirectionalLightComponent::ConstIterator> directionalLights;

        TransformComponent cameraTransform;
        Vector3 primaryLightDirection;
        Color primaryLightColor;
        TextureCube* lightProbeTexture { nullptr };
        TextureCube* skyBoxTexture { nullptr };
        GeometryBuffer* geometryBuffer { nullptr };
    } _frameData;

    Renderer& _renderer;
    TaskPool& _taskPool;

    std::unique_ptr<GeometryBuffer> _geometryBuffer;

    CameraSystem::Handle _cameraSystem;
    DebugSystem::Handle _debugSystem;

    Material::Handle _skyBoxMaterial;
};

}