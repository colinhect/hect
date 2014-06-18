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
#include <Hect/Core/Configuration.h>
#include <Hect/Core/LogicFlow.h>
#include <Hect/IO/JsonValue.h>
#include <Hect/IO/FileSystem.h>
#include <Hect/Graphics/Renderer.h>
#include <Hect/Graphics/Window.h>
#include <Hect/Reflection/RegisterTypes.h>

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
        hect::registerTypes();

        // Create file system
        hect::FileSystem fileSystem;

        // Add the working directory as a data source
        hect::Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);

        // Set the working directory as the write directory
        fileSystem.setWriteDirectory(workingDirectory);

        // Load the settings
        hect::JsonValue settings;
        {
            hect::FileReadStream stream = fileSystem.openFileForRead("Settings.json");
            settings.decodeFromJson(stream);
        }

        // Add the data sources listed in the settings
        for (const hect::JsonValue& dataSource : settings["dataSources"])
        {
            fileSystem.addDataSource(dataSource.asString());
        }

        // Load video mode
        hect::VideoMode videoMode;
        videoMode.decodeFromJsonValue(settings["videoMode"]);

        // Create window/renderer
        hect::Window window("Sample", videoMode);
        hect::Renderer renderer(window);

        // Load the input axes from the settings
        hect::InputAxis::Array axes;
        for (const hect::JsonValue& axisValue : settings["inputAxes"])
        {
            hect::InputAxis axis;
            axis.decodeFromJsonValue(axisValue);
            axes.push_back(axis);
        }

        // Create the input system
        hect::InputSystem inputSystem(axes);

        hect::AssetCache assetCache(fileSystem);

        // Create the logic flow
        MainLogicLayer main(assetCache, inputSystem, window, renderer);
        hect::LogicFlow logicFlow(hect::TimeSpan::fromSeconds((hect::Real)1 / (hect::Real)60));
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
    catch (hect::Error& error)
    {
        hect::Window::showFatalError(error.what());
    }

    return 0;
}