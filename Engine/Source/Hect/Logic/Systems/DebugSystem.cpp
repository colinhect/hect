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

#include "Hect/Interface/Widgets/CheckBox.h"
#include "Hect/Interface/Widgets/Grid.h"
#include "Hect/Interface/Widgets/Label.h"
#include "Hect/Logic/Systems/RenderSystem.h"
#include "Hect/Runtime/Engine.h"

using namespace hect;

DebugSystem::DebugSystem(Engine& engine, Scene& scene) :
    System(engine, scene, SystemTickStage::Precedent),
    _renderer(engine.renderer()),
    _window(engine.window()),
    _interfaceSystem(scene.system<InterfaceSystem>())
{
}

void DebugSystem::renderBox(DebugColor color, const Box& box, const Vector3& position, const Quaternion& rotation)
{
    _boxes.emplace_back(box, position, rotation, color);
}

void DebugSystem::addRenderCalls(RenderSystem& renderSystem)
{
    for (DebugBox& box : _boxes)
    {
        renderSystem.addRenderCall(box.transform, *boxMesh, _coloredMaterials[static_cast<size_t>(box.color)]);
    }
}

void DebugSystem::toggleShowInterface()
{
    if (_interfaceSystem)
    {
        if (!_form)
        {
            _form = _interfaceSystem->createForm();
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

void DebugSystem::initialize()
{
    addColoredMaterial(Color(100, 0, 0)); // Primary
    addColoredMaterial(Color(0, 100, 0)); // Secondary
    addColoredMaterial(Color(0, 0, 100)); // Tertiary

    _renderer.uploadShader(*coloredLineShader);
    _renderer.uploadMesh(*boxMesh);
}

void DebugSystem::tick(double timeStep)
{
    (void)timeStep;
    _boxes.clear();
}

DebugSystem::DebugBox::DebugBox()
{
}

DebugSystem::DebugBox::DebugBox(const Box& box, const Vector3& position, const Quaternion& rotation, DebugColor color) :
    box(box),
    color(color)
{
    transform.globalPosition = position;
    transform.globalScale = box.scale();
    transform.globalRotation = rotation;
}

void DebugSystem::addColoredMaterial(const Color& color)
{
    Material material;
    material.setShader(coloredLineShader);
    material.setUniformValue("color", color);

    _coloredMaterials.push_back(material);
}

void DebugSystem::createSystemPanel()
{
    assert(_form);

    const double checkBoxColumnWidth = 16;
    const double rowHeight = 18;
    const double panelMargin = 10;

    Grid::Handle grid = _form->createWidget<Grid>();
    grid->setHorizontalAlign(HorizontalAlign::Center);
    grid->setVerticalAlign(VerticalAlign::Center);

    Grid::ColumnId checkBoxColumnId = grid->addColumn(checkBoxColumnWidth);
    Grid::ColumnId systemNameColumnId = grid->addColumn();

    double maxSystemNameLength = 0;

    SystemRegistry::SystemTypeIdSequence typeIds = SystemRegistry::typeIds();
    std::sort(typeIds.begin(), typeIds.end(), [] (SystemTypeId a, SystemTypeId b)
    {
        return SystemRegistry::typeNameOf(a) < SystemRegistry::typeNameOf(b);
    });

    for (SystemTypeId typeId : typeIds)
    {
        if (scene().hasSystemOfTypeId(typeId))
        {
            Grid::RowId rowId = grid->addRow(rowHeight);

            const std::string& systemName = SystemRegistry::typeNameOf(typeId);

            CheckBox::Handle checkBox = _form->createWidget<CheckBox>();
            checkBox->setHorizontalAlign(HorizontalAlign::Center);
            checkBox->setVerticalAlign(VerticalAlign::Center);
            checkBox->setPressAction([this, typeId, checkBox]
            {
                SystemBase& system = scene().systemOfTypeId(typeId);
                system.setDebugEnabled(checkBox->isChecked());
            });

            SystemBase& system = scene().systemOfTypeId(typeId);
            checkBox->setChecked(system.isDebugEnabled());

            grid->setCell(checkBoxColumnId, rowId, checkBox);

            Label::Handle label = _form->createWidget<Label>();
            label->setText(systemName);
            label->setHorizontalAlign(HorizontalAlign::Left);
            label->setVerticalAlign(VerticalAlign::Bottom);
            grid->setCell(systemNameColumnId, rowId, label);

            maxSystemNameLength = std::max(maxSystemNameLength, label->dimensions().x);
        }
    }

    grid->resizeColumn(systemNameColumnId, maxSystemNameLength);

    _systemPanel = _form->createWidget<Panel>();
    _systemPanel->setDimensions(grid->dimensions() + panelMargin);
    _systemPanel->addChild(grid);

    Vector2 windowDimensions(_window.width(), _window.height());
    Vector2 systemPanelPosition = windowDimensions * 0.5 - _systemPanel->dimensions() * 0.5;
    _systemPanel->setPosition(systemPanelPosition);
}

void DebugSystem::destroySystemPanel()
{
    assert(_form);

    if (_systemPanel)
    {
        _form->destroyWidget(_systemPanel);
        _systemPanel.reset();
    }
}
