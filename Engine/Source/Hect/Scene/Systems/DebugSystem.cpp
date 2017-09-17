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
#include "DebugSystem.h"

#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/PhysicallyBasedSceneRenderer.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

DebugSystem::DebugSystem(Scene& scene, AssetCache& asset_cache, InterfaceSystem& interface_system) :
    System(scene),
    lines_material(asset_cache, HECT_ASSET("Hect/Materials/DebugLines.material")),
    _interface_system(interface_system),
    _lines_mesh("DebugLines")
{
    VertexLayout vertex_layout;
    VertexAttribute position_attribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3);
    vertex_layout.add_attribute(position_attribute);
    VertexAttribute color_attribute(VertexAttributeSemantic::Color, VertexAttributeType::Float32, 4);
    vertex_layout.add_attribute(color_attribute);

    _lines_mesh.set_vertex_layout(vertex_layout);
    _lines_mesh.set_primitive_type(PrimitiveType::Lines);
}

void DebugSystem::clear_enqueued_debug_geometry()
{
    _lines_mesh.clear_vertex_data();
    _lines_mesh.clear_index_data();
}

void DebugSystem::render_line(Color color, Vector3 start_position, Vector3 end_position, double duration)
{
    MeshWriter writer(mesh_for_duration(duration));

    // Start vertex
    size_t start_index = writer.add_vertex();
    writer.write_attribute_data(VertexAttributeSemantic::Position, start_position);
    writer.write_attribute_data(VertexAttributeSemantic::Color, color);

    // End vertex
    size_t end_index = writer.add_vertex();
    writer.write_attribute_data(VertexAttributeSemantic::Position, end_position);
    writer.write_attribute_data(VertexAttributeSemantic::Color, color);

    // Add indices
    writer.add_index(start_index);
    writer.add_index(end_index);
}

void DebugSystem::render_box(Color color, Box box, Vector3 position, Quaternion rotation, double duration)
{
    // The lines of an axis-aligned box with a width of one
    static const std::array<std::pair<Vector3, Vector3>, 12> unit_box_lines =
    {
        std::make_pair(Vector3(-0.5, -0.5, -0.5), Vector3(0.5, -0.5, -0.5)),
        std::make_pair(Vector3(-0.5, 0.5, -0.5), Vector3(0.5, 0.5, -0.5)),
        std::make_pair(Vector3(-0.5, -0.5, 0.5), Vector3(0.5, -0.5, 0.5)),
        std::make_pair(Vector3(-0.5, 0.5, 0.5), Vector3(0.5, 0.5, 0.5)),
        std::make_pair(Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, -0.5, 0.5)),
        std::make_pair(Vector3(0.5, -0.5, -0.5), Vector3(0.5, -0.5, 0.5)),
        std::make_pair(Vector3(-0.5, 0.5, -0.5), Vector3(-0.5, 0.5, 0.5)),
        std::make_pair(Vector3(0.5, 0.5, -0.5), Vector3(0.5, 0.5, 0.5)),
        std::make_pair(Vector3(-0.5, -0.5, -0.5), Vector3(-0.5, 0.5, -0.5)),
        std::make_pair(Vector3(0.5, -0.5, -0.5), Vector3(0.5, 0.5, -0.5)),
        std::make_pair(Vector3(-0.5, -0.5, 0.5), Vector3(-0.5, 0.5, 0.5)),
        std::make_pair(Vector3(0.5, -0.5, 0.5), Vector3(0.5, 0.5, 0.5))
    };

    for (auto& line : unit_box_lines)
    {
        const Vector3 scale = box.scale();
        const Vector3 start = position + rotation * (line.first * scale);
        const Vector3 end = position + rotation * (line.second * scale);
        render_line(color, start, end, duration);
    }
}

void DebugSystem::add_render_calls(PhysicallyBasedSceneRenderer& scene_renderer)
{
    if (!_lines_mesh.vertex_data().empty())
    {
        scene_renderer.enqueue_render_call(TransformComponent::Identity, _lines_mesh, *lines_material);
    }
}

Mesh& DebugSystem::mesh_for_duration(double duration)
{
    (void)duration;
    return _lines_mesh;
}
