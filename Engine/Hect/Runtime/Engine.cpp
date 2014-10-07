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

#include "Hect/Platform/Platform.h"
#include "Hect/Timing/Timer.h"
#include "Hect/Timing/TimeSpan.h"
#include "Hect/IO/JsonDecoder.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentRegistry.h"
#include "Hect/Logic/GameMode.h"
#include "Hect/Logic/GameModeRegistry.h"

#include <tclap/CmdLine.h>

#include "Hect/Generated/_reflect_hect.h"

using namespace hect;

Engine::Engine(int argc, char* const argv[])
{
    auto arguments = parseCommandLineArgument(argc, argv);

    // Register all of the Hect types
    registerTypes();

    Platform::initialize();

    // Mount the base and working directory
    auto baseDirectory = FileSystem::baseDirectory();
    FileSystem::mount(baseDirectory);
    auto workingDirectory = FileSystem::workingDirectory();
    FileSystem::mount(workingDirectory);

    // Set the working directory as the write directory
    FileSystem::setWriteDirectory(workingDirectory);

    // Load the configs specified on the command-line
    for (auto& configFilePath : arguments.configFilePaths)
    {
        ReadStream stream = FileSystem::openFileForRead(configFilePath);
        _config.decodeFromJson(stream);
    }

    // Load additional config files
    for (auto& configFilePath : _config["additional"])
    {
        ReadStream stream = FileSystem::openFileForRead(configFilePath.asString());
        _config.decodeFromJson(stream);
    }
    
    // Mount the paths specified in the config
    for (auto& path : _config["paths"])
    {
        FileSystem::mount(path.asString());
    }

    _assetCache.reset(new AssetCache());

    const JsonValue& videoModeValue = _config["videoMode"];
    if (!videoModeValue.isNull())
    {
        // Load video mode
        VideoMode videoMode;

        {
            JsonDecoder decoder(videoModeValue);
            decoder >> decodeValue(videoMode);
        }

        // Create window/renderer/input devices
        _window = Platform::createWindow("Hect", videoMode);
        _renderer.reset(new Renderer(*_window));
        _renderSystem.reset(new RenderSystem(*_renderer, *_assetCache));
    }
}

Engine::~Engine()
{
    _renderSystem.reset();
    _assetCache.reset();
    _renderer.reset();
    _window.reset();

    Platform::deinitialize();
}

int Engine::main()
{
    const std::string& gameModeTypeName = _config["defaultGameMode"].asString();

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

        _renderSystem->renderAll(*_window);
        _window->swapBuffers();
    }

    return 0;
}

Renderer& Engine::renderer()
{
    assert(_renderer);
    return *_renderer;
}

RenderSystem& Engine::renderSystem()
{
    assert(_renderSystem);
    return *_renderSystem;
}

Window& Engine::window()
{
    assert(_window);
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

Engine::CommandLineArguments Engine::parseCommandLineArgument(int argc, char* const argv[])
{
    CommandLineArguments arguments;

    try
    {
        TCLAP::CmdLine cmd{ "Hect Engine" };
        TCLAP::MultiArg<std::string> configsArg
        {
            "c", "config",
            "A config file load",
            false,
            "string"
        };
        
        cmd.add(configsArg);
        cmd.parse(argc, argv);

        for (auto& config : configsArg.getValue())
        {
            arguments.configFilePaths.push_back(config);
        }
    }
    catch (std::exception& exception)
    {
        HECT_ERROR(exception.what());
    }

    return arguments;
}