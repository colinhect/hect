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
#include "Hect/Interface/Widgets/CheckBoxWidget.h"
#include "Hect/Interface/Widgets/LabelWidget.h"
#include "Hect/Interface/Widgets/TableWidget.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

DebugSystem::DebugSystem(Engine& engine, Scene& scene, InterfaceSystem& interfaceSystem) :
    System(engine, scene),
    _interfaceSystem(interfaceSystem),
    _linesMesh("DebugLines")
{
    VertexLayout vertexLayout;
    VertexAttribute positionAttribute(VertexAttributeSemantic::Position, VertexAttributeType::Float32, 3);
    vertexLayout.addAttribute(positionAttribute);
    VertexAttribute colorAttribute(VertexAttributeSemantic::Color, VertexAttributeType::Float32, 4);
    vertexLayout.addAttribute(colorAttribute);

    _linesMesh.setVertexLayout(vertexLayout);
    _linesMesh.setPrimitiveType(PrimitiveType::Lines);
}

void DebugSystem::clearEnqueuedDebugGeometry()
{
    _linesMesh.clearVertexData();
    _linesMesh.clearIndexData();
}

void DebugSystem::renderLine(Color color, Vector3 startPosition, Vector3 endPosition, double duration)
{
    MeshWriter writer(meshForDuration(duration));

    // Start vertex
    size_t startIndex = writer.addVertex();
    writer.writeAttributeData(VertexAttributeSemantic::Position, startPosition);
    writer.writeAttributeData(VertexAttributeSemantic::Color, color);

    // End vertex
    size_t endIndex = writer.addVertex();
    writer.writeAttributeData(VertexAttributeSemantic::Position, endPosition);
    writer.writeAttributeData(VertexAttributeSemantic::Color, color);

    // Add indices
    writer.addIndex(startIndex);
    writer.addIndex(endIndex);
}

void DebugSystem::renderBox(Color color, Box box, Vector3 position, Quaternion rotation, double duration)
{
    // The lines of an axis-aligned box with a width of one
    static const std::array<std::pair<Vector3, Vector3>, 12> unitBoxLines =
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

    for (auto& line : unitBoxLines)
    {
        Vector3 scale = box.scale();
        const Vector3 start = position + rotation * (line.first * scale);
        const Vector3 end = position + rotation * (line.second * scale);
        renderLine(color, start, end, duration);
    }
}

void DebugSystem::addRenderCalls(PhysicallyBasedSceneRenderer& sceneRenderer)
{
    if (!_linesMesh.vertexData().empty())
    {
        sceneRenderer.enqueueRenderCall(TransformComponent::Identity, _linesMesh, *linesMaterial);
    }
}

void DebugSystem::toggleShowInterface()
{
    if (!_interface)
    {
        Window& mainWindow = engine().mainWindow();
        _interface = _interfaceSystem.createInterface(mainWindow);
    }
}

Mesh& DebugSystem::meshForDuration(double duration)
{
    (void)duration;
    return _linesMesh;
}
