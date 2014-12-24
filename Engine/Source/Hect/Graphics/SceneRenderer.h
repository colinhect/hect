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
#pragma once

#include <array>

#include "Hect/Core/Export.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/RenderBuffer.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Logic/Components/Camera.h"
#include "Hect/Logic/Components/Transform.h"

namespace hect
{

///
/// Provides the functionality for high level rendering algorithms (physically-
/// based shading, deferred shading, shadows, reflections, etc).
class HECT_EXPORT SceneRenderer
{
public:

    ///
    /// Constructs a scene renderer.
    ///
    /// \param assetCache The asset cache to get needed assets from.
    SceneRenderer(AssetCache& assetCache);

    ///
    /// Renders a scene in its current state to a given render target.
    ///
    /// \param renderer The renderer to render using.
    /// \param scene The scene to render.
    /// \param target The target to render to.
    void renderScene(Renderer& renderer, Scene& scene, RenderTarget& target);

    ///
    /// Enqueues a render call to be rendered on the upcoming frame.
    ///
    /// \param transform The world-space transform.
    /// \param mesh The mesh to render.
    /// \param material The material to use.
    void addRenderCall(Transform& transform, Mesh& mesh, Material& material);

private:
    void initializeBuffers(Renderer& renderer, unsigned width, unsigned height);
    void buildRenderCalls(Camera& camera, Entity& entity, bool frustumTest = true);
    void renderMesh(Renderer& renderer, const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform);
    void setBoundUniforms(Renderer& renderer, Shader& shader, const Camera& camera, const RenderTarget& target, const Transform& transform);

    void swapBackBuffer();
    Texture& backBuffer();
    Texture& lastBackBuffer();
    FrameBuffer& backFrameBuffer();

    class HECT_EXPORT RenderCall
    {
    public:
        RenderCall();
        RenderCall(Transform& transform, Mesh& mesh, Material& material);

        Transform* transform { nullptr };
        Mesh* mesh { nullptr };
        Material* material { nullptr };

        bool operator<(const RenderCall& other) const;
    };
    
    RenderBuffer _depthBuffer;

    Texture _diffuseBuffer;
    Texture _materialBuffer;
    Texture _positionBuffer;
    Texture _normalBuffer;
    std::array<Texture, 2> _backBuffers;

    FrameBuffer _geometryFrameBuffer;
    std::array<FrameBuffer, 2> _backFrameBuffers;

    AssetHandle<Shader> _exposeShader;
    AssetHandle<Shader> _compositeShader;
    AssetHandle<Shader> _environmentShader;
    AssetHandle<Shader> _directionalLightShader;

    AssetHandle<Material> _skyBoxMaterial;

    AssetHandle<Mesh> _screenMesh;
    AssetHandle<Mesh> _skyBoxMesh;

    Transform _identityTransform;
    Transform _cameraTransform;
    Vector3 _primaryLightDirection;
    Vector3 _primaryLightColor;
    Texture* _lightProbeCubeMap { nullptr };
    Texture* _skyBoxCubeMap { nullptr };

    std::vector<RenderCall> _renderCalls;

    bool _buffersInitialized { false };
    size_t _backBufferIndex { 0 };
};

}