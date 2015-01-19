///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "Engine.h"

#include "Hect/Core/Configuration.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/IO/DataValueDecoder.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentRegistry.h"
#include "Hect/Logic/Mode.h"
#include "Hect/Logic/ModeRegistry.h"
#include "Hect/Timing/Timer.h"
#include "Hect/Timing/TimeSpan.h"

#include "Hect/Generated/RegisterTypes.h"

#include <fstream>
#include <streambuf>
#include <tclap/CmdLine.h>

#ifdef HECT_USE_VLD
#include <vld.h>
#endif

using namespace hect;

Engine::Engine(int argc, char* const argv[])
{
    // Register all of the Hect types
    registerTypes();

    // Ignore false positive memory leaks from static variables
#ifdef HECT_USE_VLD
    VLDMarkAllLeaksAsReported();
#endif

    // Create file system
    _fileSystem.reset(new FileSystem(argc, argv));

    // Create platform
    _platform.reset(new Platform());

    // Parse command line arguments
    CommandLineArguments arguments = parseCommandLineArgument(argc, argv);

    // Load the settings specified on the command-line
    if (!arguments.settingsFilePath.empty())
    {
        _settings = loadConfig(arguments.settingsFilePath);
    }

    // Mount the archives specified in the settings
    for (const DataValue& archive : _settings["archives"])
    {
        if (!archive["path"].isNull())
        {
            _fileSystem->mountArchive(archive["path"].asString(), archive["mountPoint"].asString());
        }
    }

    // Create the task pool
    size_t threadCount = _settings["taskPool"]["threadCount"].orDefault(2).asInt();
    _taskPool.reset(new TaskPool(threadCount));

    // Create the asset cache
    bool concurrent = _settings["assetCache"]["concurrent"].orDefault(false).asBool();
    _assetCache.reset(new AssetCache(*_fileSystem, concurrent));

    const DataValue& videoModeValue = _settings["videoMode"];
    if (!videoModeValue.isNull())
    {
        // Load video mode
        VideoMode videoMode;
        try
        {
            DataValueDecoder decoder(videoModeValue);
            decoder >> decodeValue(videoMode);
        }
        catch (const DecodeError& error)
        {
            HECT_ERROR(format("Invalid video mode: %s", error.what()));
        }

        // Create window and renderer
        _window.reset(new Window("Hect", videoMode));
        _renderer.reset(new Renderer());
    }
}

int Engine::main()
{
    const DataValue& modeValue = _settings["mode"];
    if (modeValue.isNull())
    {
        HECT_ERROR("No mode specified in settings");
    }
    else
    {
        const std::string& modeTypeName = modeValue.asString();

        auto mode = ModeRegistry::create(modeTypeName, *this);

        Timer timer;
        TimeSpan accumulator;
        TimeSpan delta;

        double timeStep = mode->timeStep().seconds();

        bool active = true;
        while (_platform->handleEvents() && active)
        {
            TimeSpan deltaTime = timer.elapsed();
            timer.reset();

            accumulator += deltaTime;
            delta += deltaTime;

            while (active && accumulator.microseconds() >= mode->timeStep().microseconds())
            {
                active = mode->tick(*this, timeStep);

                delta = TimeSpan();
                accumulator -= mode->timeStep();
            }

            mode->render(*this, *_window);
            _window->swapBuffers();
        }
    }

    return 0;
}

FileSystem& Engine::fileSystem()
{
    assert(_fileSystem);
    return *_fileSystem;
}

Platform& Engine::platform()
{
    assert(_platform);
    return *_platform;
}

Renderer& Engine::renderer()
{
    assert(_renderer);
    return *_renderer;
}

Window& Engine::window()
{
    if (!_window)
    {
        throw InvalidOperation("No available window");
    }
    return *_window;
}

TaskPool& Engine::taskPool()
{
    assert(_taskPool);
    return *_taskPool;
}

AssetCache& Engine::assetCache()
{
    assert(_assetCache);
    return *_assetCache;
}

const DataValue& Engine::settings()
{
    return _settings;
}

DataValue Engine::loadConfig(const Path& settingsFilePath)
{
    try
    {
        // Read the file to a YAML string
        std::string yaml;
        {
            std::ifstream stream(settingsFilePath.asString().c_str());
            yaml.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        }

        DataValue settings;
        settings.decodeFromYaml(yaml);

        // Load additional settings files
        std::vector<DataValue> includedConfigs;
        for (auto& settingsFilePath : settings["include"])
        {
            DataValue settings = loadConfig(settingsFilePath.asString());
            includedConfigs.push_back(std::move(settings));
        }

        // Merge additional settingss back to the main settings
        for (auto& includedConfig : includedConfigs)
        {
            for (auto& memberName : includedConfig.memberNames())
            {
                settings.addMember(memberName, includedConfig[memberName]);
            }
        }

        return settings;
    }
    catch (const std::exception& exception)
    {
        throw FatalError(format("Failed to load settings file '%s': %s", settingsFilePath.asString().c_str(), exception.what()));
    }
}

Engine::CommandLineArguments Engine::parseCommandLineArgument(int argc, char* const argv[])
{
    std::vector<std::string> argumentStrings { argv[0] };

    // If there is only one argument given then assume it is a settings file path
    if (argc == 2)
    {
        argumentStrings.push_back("--settings");
    }

    // Add the remaining arguments
    for (int i = 1; i < argc; ++i)
    {
        argumentStrings.push_back(argv[i]);
    }

    try
    {
        TCLAP::CmdLine cmd("Hect Engine");
        TCLAP::ValueArg<std::string> settingsArg
        {
            "s", "settings",
            "A settings file load",
            false,
            "",
            "string"
        };

        cmd.add(settingsArg);
        cmd.parse(argumentStrings);

        CommandLineArguments arguments;
        arguments.settingsFilePath = settingsArg.getValue();
        return arguments;
    }
    catch (const std::exception& exception)
    {
        throw FatalError(exception.what());
    }
}
