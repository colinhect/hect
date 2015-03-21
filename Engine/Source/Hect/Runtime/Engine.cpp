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
    const DataValue& sceneValue = _settings["scene"];
    if (sceneValue.isNull())
    {
        HECT_ERROR("No scene specified in settings");
    }
    else
    {
        // Load the specified scene
        Scene scene(*this);
        Path scenePath = sceneValue.asString();
        AssetDecoder decoder(*_assetCache, scenePath);
        decoder >> decodeValue(scene);

        // Play the scene
        playScene(scene);
    }

    return 0;
}

void Engine::playScene(Scene& scene)
{
    Timer timer;
    TimeSpan accumulator;
    TimeSpan delta;

    TimeSpan timeStep = TimeSpan::fromSeconds(1.0 / 60.0);

    double timeStepSeconds = timeStep.seconds();
    int64_t timeStepMicroseconds = timeStep.microseconds();

    while (_platform->handleEvents() && scene.active())
    {
        TimeSpan deltaTime = timer.elapsed();
        timer.reset();

        accumulator += deltaTime;
        delta += deltaTime;

        while (scene.active() && accumulator.microseconds() >= timeStepMicroseconds)
        {
            scene.tick(timeStepSeconds);

            delta = TimeSpan();
            accumulator -= timeStep;
        }

        scene.render(*_window);
        _window->swapBuffers();
    }
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

Window& Engine::window()
{
    if (!_window)
    {
        throw InvalidOperation("No available window");
    }
    return *_window;
}

Renderer& Engine::renderer()
{
    assert(_renderer);
    return *_renderer;
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
        for (const DataValue& settingsFilePath : settings["include"])
        {
            DataValue settings = loadConfig(settingsFilePath.asString());
            includedConfigs.push_back(std::move(settings));
        }

        // Merge additional settingss back to the main settings
        for (const DataValue& includedConfig : includedConfigs)
        {
            for (std::string& memberName : includedConfig.memberNames())
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
