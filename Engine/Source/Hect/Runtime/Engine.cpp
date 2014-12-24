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
#include "Engine.h"

#include "Hect/Runtime/Platform.h"
#include "Hect/Timing/Timer.h"
#include "Hect/Timing/TimeSpan.h"
#include "Hect/IO/DataValueDecoder.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentRegistry.h"
#include "Hect/Logic/GameMode.h"
#include "Hect/Logic/GameModeRegistry.h"

#include "Hect/Generated/RegisterTypes.h"

#include <fstream>
#include <streambuf>
#include <tclap/CmdLine.h>

using namespace hect;

Engine::Engine(int argc, char* const argv[]) :
    _fileSystem(argc, argv)
{
    auto arguments = parseCommandLineArgument(argc, argv);

    // Register all of the Hect types
    registerTypes();

#ifdef HECT_WINDOWS_BUILD
    // Set the base directory as the write directory
    auto baseDirectory = _fileSystem.baseDirectory();
    _fileSystem.setWriteDirectory(baseDirectory);
#endif

    // Load the settings specified on the command-line
    if (!arguments.settingsFilePath.empty())
    {
        _settings = loadConfig(arguments.settingsFilePath);
    }

    // Mount the archives specified in the settings
    for (auto& archive : _settings["archives"])
    {
        if (!archive["path"].isNull())
        {
            _fileSystem.mountArchive(archive["path"].asString(), archive["mountPoint"].asString());
        }
    }

    // Create the asset cache
    bool concurrent = _settings["assetCache"]["concurrent"].orDefault(false).asBool();
    _assetCache.reset(new AssetCache(_fileSystem, concurrent));

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
        catch (Error& error)
        {
            throw Error(format("Invalid video mode: %s", error.what()));
        }

        // Create window and renderer
        _window = _platform.createWindow("Hect", videoMode);
        _renderer.initialize(*_window);
    }
}

int Engine::main()
{
    const DataValue& gameModeValue = _settings["gameMode"];
    if (gameModeValue.isNull())
    {
        throw Error("No game mode specified in settings");
    }

    const std::string& gameModeTypeName = gameModeValue.asString();

    auto gameMode = GameModeRegistry::create(gameModeTypeName, *this);

    Timer timer;
    TimeSpan accumulator;
    TimeSpan delta;

    Real timeStep = gameMode->timeStep().seconds();

    while (_platform.handleEvents())
    {
        TimeSpan deltaTime = timer.elapsed();
        timer.reset();

        accumulator += deltaTime;
        delta += deltaTime;

        while (accumulator.microseconds() >= gameMode->timeStep().microseconds())
        {
            gameMode->tick(timeStep);

            delta = TimeSpan();
            accumulator -= gameMode->timeStep();
        }

        gameMode->render(*_window);
        _window->swapBuffers();
    }

    return 0;
}

FileSystem& Engine::fileSystem()
{
    return _fileSystem;
}

Platform& Engine::platform()
{
    return _platform;
}

Renderer& Engine::renderer()
{
    return _renderer;
}

Window& Engine::window()
{
    if (!_window)
    {
        throw Error("No available window");
    }
    return *_window;
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
    catch (std::exception& exception)
    {
        throw Error(format("Failed to load settings file '%s': %s", settingsFilePath.asString().c_str(), exception.what()));
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
    catch (std::exception& exception)
    {
        throw Error(exception.what());
    }
}
