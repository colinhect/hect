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

#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Logic/Components/Camera.h"
#include "Hect/Logic/System.h"

namespace hect
{

class RenderSystem
{
public:
    RenderSystem(Renderer& renderer, AssetCache& assetCache);

    void addWorld(World& world);
    void removeWorld(World& world);

    void renderAll(RenderTarget& target);

    void render(Camera& camera, RenderTarget& target, Entity& entity, bool frustumTest = true);
    void renderMesh(const Camera& camera, const RenderTarget& target, Material& material, Mesh& mesh, const Transform& transform);
    void renderMeshPass(const Camera& camera, const RenderTarget& target, Pass& pass, Mesh& mesh, const Transform& transform);

    void setBoundUniforms(Shader& shader, const Camera& camera, const RenderTarget& target, const Transform& transform);

    Renderer& renderer();

private:
    void initializeBuffers(unsigned width, unsigned height);

    Renderer* _renderer;

    std::vector<World*> _worlds;

    FrameBuffer _geometryBuffer;
    FrameBuffer _accumulationBuffer;

    AssetHandle<Shader> _compositorShader;
    AssetHandle<Shader> _environmentShader;
    AssetHandle<Shader> _directionalLightShader;

    AssetHandle<Material> _skyBoxMaterial;

    AssetHandle<Mesh> _screenMesh;
    AssetHandle<Mesh> _skyBoxMesh;

    bool _buffersInitialized;
};

}