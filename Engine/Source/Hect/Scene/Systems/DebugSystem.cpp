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
#include "Hect/Interface/Widgets/CheckBoxWidget.h"
#include "Hect/Interface/Widgets/GridWidget.h"
#include "Hect/Interface/Widgets/LabelWidget.h"
#include "Hect/Runtime/Engine.h"
#include "Hect/Scene/Systems/RenderSystem.h"

using namespace hect;

DebugSystem::DebugSystem(Engine& engine, Scene& scene) :
    System(engine, scene),
    _renderer(engine.renderer()),
    _window(engine.window()),
    _interfaceSystem(scene.system<InterfaceSystem>()),
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

void DebugSystem::addRenderCalls(RenderSystem& renderSystem)
{
    if (!_linesMesh.vertexData().empty())
    {
        renderSystem.addRenderCall(TransformComponent::Identity, _linesMesh, *linesMaterial);
    }
}

void DebugSystem::toggleShowInterface()
{
    if (_interfaceSystem)
    {
        if (!_interface)
        {
            _interface = _interfaceSystem->createInterface(_window);
        }

        if (_systemPanel)
        {
            destroySystemPanel();
        }
        else
        {
            createSystemPanel();
        }
    }
}

Mesh& DebugSystem::meshForDuration(double duration)
{
    (void)duration;
    return _linesMesh;
}

void DebugSystem::createSystemPanel()
{
    assert(_interface);

    const double checkBoxColumnWidth = 16;
    const double systemNameColumnWidth = 256;
    const double rowHeight = 18;
    const double panelMargin = 10;

    _systemPanel = _interface->createChild<PanelWidget>();

    GridWidget::Handle grid = _systemPanel->createChild<GridWidget>();
    grid->setHorizontalAlign(HorizontalAlign::Center);
    grid->setVerticalAlign(VerticalAlign::Center);

    GridWidget::Column checkBoxColumn = grid->addColumn(checkBoxColumnWidth);
    GridWidget::Column systemNameColumn = grid->addColumn(systemNameColumnWidth);

    SystemRegistry::SystemTypeIdSequence typeIds = SystemRegistry::typeIds();
    std::sort(typeIds.begin(), typeIds.end(), [] (SystemTypeId a, SystemTypeId b)
    {
        return SystemRegistry::typeNameOf(a) < SystemRegistry::typeNameOf(b);
    });

    for (SystemTypeId typeId : typeIds)
    {
        if (scene().hasSystemOfTypeId(typeId))
        {
            GridWidget::Row row = grid->addRow(rowHeight);

            Name systemName = SystemRegistry::typeNameOf(typeId);

            GridWidget::Cell::Handle checkBoxCell = grid->createCell(checkBoxColumn, row);
            CheckBoxWidget::Handle checkBox = checkBoxCell->createChild<CheckBoxWidget>();
            checkBox->setHorizontalAlign(HorizontalAlign::Center);
            checkBox->setVerticalAlign(VerticalAlign::Center);
            checkBox->setPressAction([this, typeId, checkBox]
            {
                //SystemBase& system = scene().systemOfTypeId(typeId);
                // system.setDebugEnabled(checkBox->isChecked()); This doesn't work anymore
            });

            //SystemBase& system = scene().systemOfTypeId(typeId);

            // Neither does this
            //checkBox->setChecked(system.isDebugEnabled());

            GridWidget::Cell::Handle labelCell = grid->createCell(systemNameColumn, row);
            LabelWidget::Handle label = labelCell->createChild<LabelWidget>();
            label->setText(systemName.asString());
            label->setHorizontalAlign(HorizontalAlign::Left);
            label->setVerticalAlign(VerticalAlign::Bottom);
        }
    }

    _systemPanel->setDimensions(grid->dimensions() + panelMargin);

    Vector2 windowDimensions(_window.width(), _window.height());
    Vector2 systemPanelPosition = windowDimensions * 0.5 - _systemPanel->dimensions() * 0.5;
    _systemPanel->setPosition(systemPanelPosition);
}

void DebugSystem::destroySystemPanel()
{
    assert(_interface);

    if (_systemPanel)
    {
        _interface->destroyChild(_systemPanel);
        _systemPanel.reset();
    }
}
