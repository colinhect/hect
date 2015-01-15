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
#pragma once

#include <array>

#include "Hect/Core/Export.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/RenderBuffer.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Logic/Scene.h"
#include "Hect/Logic/Components/Camera.h"
#include "Hect/Logic/Components/DirectionalLight.h"
#include "Hect/Logic/Components/Transform.h"

namespace hect
{

///
/// Performs the high-level rendering algorithms (physically-based shading,
/// deferred shading, shadows, reflections, etc).
///
/// \system
class HECT_EXPORT RenderSystem :
    public System
{
public:
    RenderSystem(Engine& engine, Scene& scene);

    ///
    /// Enqueues a render call to be rendered on the upcoming frame.
    ///
    /// \param transform The world-space transform.
    /// \param mesh The mesh to render.
    /// \param material The material to use.
    void addRenderCall(Transform& transform, Mesh& mesh, Material& material);

    ///
    /// Immediately uploads all render objects in the scene.
    void uploadRendererObjects();

    void render(Engine& engine, RenderTarget& target) override;

private:
    void prepareFrame(Camera& camera, Scene& scene, RenderTarget& target);
    void renderFrame(Camera& camera, RenderTarget& target);

    void initializeBuffers(unsigned width, unsigned height);
    void buildRenderCalls(Camera& camera, Entity& entity, bool frustumTest = true);
    void renderMesh(const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform);
    void setBoundUniforms(Shader& shader, const Camera& camera, const RenderTarget& target, const Transform& transform);

    void swapBackBuffer();
    Texture& backBuffer();
    Texture& lastBackBuffer();
    FrameBuffer& backFrameBuffer();

    class RenderCall
    {
    public:
        RenderCall();
        RenderCall(Transform& transform, Mesh& mesh, Material& material);

        Transform* transform { nullptr };
        Mesh* mesh { nullptr };
        Material* material { nullptr };

        bool operator<(const RenderCall& other) const;
    };

    // Data required to render a frame
    class FrameData
    {
    public:
        void clear();

        std::vector<RenderCall> opaquePhysicalGeometry;
        std::vector<RenderCall> transparentPhysicalGeometry;

        std::vector<DirectionalLight::ConstIterator> directionalLights;

        Transform cameraTransform;
        Vector3 primaryLightDirection;
        Color primaryLightColor;
        Texture* lightProbeCubeMap { nullptr };
        Texture* skyBoxCubeMap { nullptr };
        size_t backBufferIndex { 0 };
    } _frameData;

    Renderer* _renderer { nullptr };
    TaskPool* _taskPool { nullptr };

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

    bool _buffersInitialized { false };
};

}