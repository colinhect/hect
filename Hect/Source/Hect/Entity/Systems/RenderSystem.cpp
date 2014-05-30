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
#include "RenderSystem.h"

#include "Hect/Entity/Scene.h"
#include "Hect/Entity/Components/Camera.h"
#include "Hect/Entity/Components/Geometry.h"
#include "Hect/Entity/Components/Transform.h"

using namespace hect;

RenderSystem::RenderSystem(Scene& scene, Renderer& renderer) :
    System(scene),
    _renderer(&renderer)
{
}

void RenderSystem::renderAll(Camera& camera, RenderTarget& target)
{
    _renderer->beginFrame();
    _renderer->bindTarget(target);
    _renderer->clear();

    // Render geometery
    for (Geometry& geometry : scene().componentPool<Geometry>())
    {
        EntityId entityId = geometry.entityId();
        auto transform = scene().entityComponent<Transform>(entityId);
        if (transform.isValid())
        {
            size_t surfaceCount = geometry.surfaceCount();
            for (size_t surfaceIndex = 0; surfaceIndex < surfaceCount; ++surfaceIndex)
            {
                Mesh& mesh = *geometry.meshes()[surfaceIndex];
                Material& material = *geometry.materials()[surfaceIndex];

                // Render the mesh for each pass
                for (const Pass& pass : material.techniques()[0].passes())
                {
                    renderMeshPass(camera, target, pass, mesh, *transform);
                }
            }
        }
    }

    _renderer->endFrame();
}

void RenderSystem::renderMeshPass(const Camera& camera, const RenderTarget& target, const Pass& pass, Mesh& mesh, const Transform& transform)
{
    // Prepare the pass
    pass.prepare(*_renderer);

    // Buid the model matrix
    Matrix4 model;
    transform.buildMatrix(model);

    // Set uniforms with bindings
    Shader& shader = *pass.shader();
    for (const Uniform& uniform : shader.uniforms())
    {
        if (uniform.hasBinding())
        {
            UniformBinding::Enum binding = uniform.binding();

            switch (binding)
            {
            case UniformBinding::RenderTargetSize:
                _renderer->setUniform(uniform, Vector2((Real)target.width(), (Real)target.height()));
                break;
            case UniformBinding::CameraPosition:
                _renderer->setUniform(uniform, camera.position());
                break;
            case UniformBinding::CameraUp:
                _renderer->setUniform(uniform, camera.up());
                break;
            case UniformBinding::ViewMatrix:
                _renderer->setUniform(uniform, camera.viewMatrix());
                break;
            case UniformBinding::ProjectionMatrix:
                _renderer->setUniform(uniform, camera.projectionMatrix());
                break;
            case UniformBinding::ViewProjectionMatrix:
                _renderer->setUniform(uniform, camera.projectionMatrix() * camera.viewMatrix());
                break;
            case UniformBinding::ModelMatrix:
                _renderer->setUniform(uniform, model);
                break;
            case UniformBinding::ModelViewMatrix:
                _renderer->setUniform(uniform, camera.viewMatrix() * model);
                break;
            case UniformBinding::ModelViewProjectionMatrix:
                _renderer->setUniform(uniform, camera.projectionMatrix() * (camera.viewMatrix() * model));
                break;
            }
        }
    }

    // Bind and draw the mesh
    _renderer->bindMesh(mesh);
    _renderer->draw();
}