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

#include "Hect/Core/Configuration.h"
#include "Hect/Platform/Platform.h"
#include "Hect/Timing/Timer.h"
#include "Hect/Timing/TimeSpan.h"
#include "Hect/IO/JsonDecoder.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentRegistry.h"
#include "Hect/Logic/GameMode.h"
#include "Hect/Logic/GameModeRegistry.h"

#include "Hect/Generated/RegisterTypes.h"

#include <fstream>
#include <streambuf>
#include <tclap/CmdLine.h>

using namespace hect;

Engine::Engine(int argc, char* const argv[])
{
    auto arguments = parseCommandLineArgument(argc, argv);

    // Register all of the Hect types
    registerTypes();

    Platform::initialize(argc, argv);

#ifdef HECT_WINDOWS_BUILD
    // Set the base directory as the write directory
    auto baseDirectory = FileSystem::baseDirectory();
    FileSystem::setWriteDirectory(baseDirectory);
#endif

    // Load the configs specified on the command-line
    if (!arguments.configFilePath.empty())
    {
        _config = loadConfig(arguments.configFilePath);
    }

    // Mount the archives specified in the config
    for (auto& archive : _config["archives"])
    {
        if (!archive["path"].isNull())
        {
            FileSystem::mountArchive(archive["path"].asString(), archive["mountPoint"].asString());
        }
    }

    bool concurrent = _config["assetCache"]["concurrent"].orDefault(false).asBool();
    _assetCache.reset(new AssetCache(concurrent));

    const JsonValue& videoModeValue = _config["videoMode"];
    if (!videoModeValue.isNull())
    {
        // Load video mode
        VideoMode videoMode;
        try
        {
            JsonDecoder decoder(videoModeValue);
            decoder >> decodeValue(videoMode);
        }
        catch (Error& error)
        {
            throw Error(format("Invalid video mode: %s", error.what()));
        }

        // Create window and graphics context
        _window = Platform::createWindow("Hect", videoMode);
        _graphicsContext.reset(new GraphicsContext(*_window));
        _renderer.reset(new Renderer(*_graphicsContext, *_assetCache));
    }
}

Engine::~Engine()
{
    _renderer.reset();
    _assetCache.reset();
    _graphicsContext.reset();
    _window.reset();

    Platform::deinitialize();
}

int Engine::main()
{
    const JsonValue& gameModeValue = _config["gameMode"];
    if (gameModeValue.isNull())
    {
        throw Error("No game mode specified in config");
    }

    const std::string& gameModeTypeName = gameModeValue.asString();

    GameMode::Pointer gameMode = GameModeRegistry::create(gameModeTypeName, *this);

    Timer timer;
    TimeSpan accumulator;
    TimeSpan delta;

    while (Platform::handleEvents())
    {
        TimeSpan deltaTime = timer.elapsed();
        timer.reset();

        accumulator += deltaTime;
        delta += deltaTime;

        while (accumulator.microseconds() >= gameMode->timeStep().microseconds())
        {
            gameMode->tick();

            delta = TimeSpan();
            accumulator -= gameMode->timeStep();
        }

        gameMode->render(*_window);
        _window->swapBuffers();
    }

    return 0;
}

GraphicsContext& Engine::graphicsContext()
{
    if (!_graphicsContext)
    {
        throw Error("No available graphics context");
    }
    return *_graphicsContext;
}

Renderer& Engine::renderer()
{
    if (!_renderer)
    {
        throw Error("No available renderer");
    }
    return *_renderer;
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

const JsonValue& Engine::config()
{
    return _config;
}

JsonValue Engine::loadConfig(const Path& configFilePath)
{
    try
    {
        // Read the file to a JSON string
        std::string json;
        {
            std::ifstream stream(configFilePath.asString().c_str());
            json.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        }

        JsonValue config;
        config.decodeFromJson(json);

        // Load additional config files
        std::vector<JsonValue> includedConfigs;
        for (auto& configFilePath : config["include"])
        {
            JsonValue config = loadConfig(configFilePath.asString());
            includedConfigs.push_back(std::move(config));
        }

        // Merge additional configs back to the main config
        for (auto& includedConfig : includedConfigs)
        {
            for (auto& memberName : includedConfig.memberNames())
            {
                config.addMember(memberName, includedConfig[memberName]);
            }
        }

        return config;
    }
    catch (std::exception& exception)
    {
        throw Error(format("Failed to load config file '%s': %s", configFilePath.asString().c_str(), exception.what()));
    }
}

Engine::CommandLineArguments Engine::parseCommandLineArgument(int argc, char* const argv[])
{
    std::vector<std::string> argumentStrings { argv[0] };

    // If there is only one argument given then assume it is a config file path
    if (argc == 2)
    {
        argumentStrings.push_back("--config");
    }

    // Add the remaining arguments
    for (int i = 1; i < argc; ++i)
    {
        argumentStrings.push_back(argv[i]);
    }

    try
    {
        TCLAP::CmdLine cmd("Hect Engine");
        TCLAP::ValueArg<std::string> configArg
        {
            "c", "config",
            "A config file load",
            false,
            "",
            "string"
        };

        cmd.add(configArg);
        cmd.parse(argumentStrings);

        CommandLineArguments arguments;
        arguments.configFilePath = configArg.getValue();
        return arguments;
    }
    catch (std::exception& exception)
    {
        throw Error(exception.what());
    }
}
