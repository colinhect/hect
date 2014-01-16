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
#include "Hect.h"

using namespace hect;

BasicRenderSystem::BasicRenderSystem(Renderer& renderer) :
    _renderer(&renderer)
{
}

bool BasicRenderSystem::includesEntity(const Entity& entity) const
{
    return entity.hasComponent<Transform>() && entity.hasComponent<Geometry>();
}

void BasicRenderSystem::renderAll(Camera& camera, RenderTarget& target)
{
    _renderer->beginFrame();
    _renderer->bindTarget(target);
    _renderer->clear();

    // Render geometery
    for (Entity& entity : entities())
    {
        Geometry& geometry = entity.component<Geometry>();
        Transform& transform = entity.component<Transform>();

        size_t surfaceCount = geometry.surfaceCount();
        for (size_t surfaceIndex = 0; surfaceIndex < surfaceCount; ++surfaceIndex)
        {
            Mesh& mesh = geometry.meshes()[surfaceIndex].get();
            Material& material = geometry.materials()[surfaceIndex].get();

            // Render the mesh for each pass
            for (const Pass& pass : material.techniques()[0].passes())
            {
                _renderMeshPass(camera, target, pass, mesh, transform);
            }
        }
    }

    _renderer->endFrame();
}

void BasicRenderSystem::_renderMeshPass(const Camera& camera, const RenderTarget& target, const Pass& pass, Mesh& mesh, const Transform& transform)
{
    // Prepare the pass
    pass.prepare(*_renderer);

    // Buid the model matrix
    Matrix4<> model;
    transform.buildMatrix(model);

    // Set uniforms with bindings
    Shader& shader = pass.shader().get();
    for (const Uniform& uniform : shader.uniforms())
    {
        if (uniform.hasBinding())
        {
            UniformBinding binding = uniform.binding();

            switch (binding)
            {
            case UniformBinding::RenderTargetSize:
                _renderer->setUniform(uniform, Vector2<float>((float)target.width(), (float)target.height()));
                break;
            case UniformBinding::CameraPosition:
                _renderer->setUniform(uniform, (Vector3<float>)camera.position());
                break;
            case UniformBinding::CameraUp:
                _renderer->setUniform(uniform, (Vector3<float>)camera.up());
                break;
            case UniformBinding::ViewMatrix:
                _renderer->setUniform(uniform, (Matrix4<float>)camera.viewMatrix());
                break;
            case UniformBinding::ProjectionMatrix:
                _renderer->setUniform(uniform, (Matrix4<float>)camera.projectionMatrix());
                break;
            case UniformBinding::ViewProjectionMatrix:
                _renderer->setUniform(uniform, (Matrix4<float>)(camera.projectionMatrix() * camera.viewMatrix()));
                break;
            case UniformBinding::ModelMatrix:
                _renderer->setUniform(uniform, (Matrix4<float>)model);
                break;
            case UniformBinding::ModelViewMatrix:
                _renderer->setUniform(uniform, (Matrix4<float>)(camera.viewMatrix() * model));
                break;
            case UniformBinding::ModelViewProjectionMatrix:
                _renderer->setUniform(uniform, (Matrix4<float>)(camera.projectionMatrix() * (camera.viewMatrix() * model)));
                break;
            }
        }
    }

    // Bind and draw the mesh
    _renderer->bindMesh(mesh);
    _renderer->draw();
}