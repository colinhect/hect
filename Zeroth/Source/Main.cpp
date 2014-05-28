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
#include <Hect.h>
using namespace hect;

#ifdef HECT_WINDOWS
#ifdef HECT_DEBUG
#include <vld.h>
#endif
#endif

#include "MainLogicLayer.h"

int main(int argc, const char* argv[])
{
    argc;
    argv;

    try
    {
        Type::registerTypes();

        // Create file system
        FileSystem fileSystem;

        // Add the working directory as a data source
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);

        // Set the working directory as the write directory
        fileSystem.setWriteDirectory(workingDirectory);

        // Load the settings
        JsonValue settings;
        {
            FileReadStream stream = fileSystem.openFileForRead("Settings.json");
            settings.decodeFromJson(stream);
        }

        // Add the data sources listed in the settings
        for (const JsonValue& dataSource : settings["dataSources"])
        {
            fileSystem.addDataSource(dataSource.asString());
        }

        // Load video mode
        VideoMode videoMode;
        videoMode.decodeFromJsonValue(settings["videoMode"]);

        // Create window/renderer
        Window window("Sample", videoMode);
        Renderer renderer(window);

        // Load the input axes from the settings
        InputAxis::Array axes;
        for (const JsonValue& axisValue : settings["inputAxes"])
        {
            InputAxis axis;
            axis.decodeFromJsonValue(axisValue);
            axes.push_back(axis);
        }

        // Create the input system
        InputSystem inputSystem(axes);

        AssetCache assetCache(fileSystem);

        // Create the logic flow
        MainLogicLayer main(assetCache, inputSystem, window, renderer);
        LogicFlow logicFlow(TimeSpan::fromSeconds((Real)1 / (Real)60));
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
    catch (Error& error)
    {
        Window::showFatalError(error.what());
    }

    return 0;
}