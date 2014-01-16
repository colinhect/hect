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
#include "MainLogicLayer.h"

void runSample(FileSystem& fileSystem, Window& window, Renderer& renderer, const DataValue& settings)
{
    // Create the asset cache
    AssetCache assetCache(fileSystem);

    // Load the input axes from the settings
    InputAxis::Array axes;
    for (const std::string& axisName : settings["inputAxes"].memberNames())
    {
        InputAxis axis = InputAxisDataValueFormat::load(axisName, settings["inputAxes"][axisName]);
        axes.push_back(axis);
    }

    // Create the input system
    InputSystem inputSystem(axes);

    // Create the logic flow
    MainLogicLayer main(assetCache, inputSystem, window, renderer);
    LogicFlow logicFlow(TimeSpan::fromSeconds(1.0 / 60.0));
    logicFlow.addLayer(main);

    // Update until the flow is complete
    while (window.pollEvents(inputSystem))
    {
        if (!logicFlow.update())
        {
            break;
        }
    }
}