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
#include "Engine.h"

#include "Hect/Core/Configuration.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/IO/DataValueDecoder.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentRegistry.h"
#include "Hect/Logic/Scene.h"
#include "Hect/Logic/SceneRegistry.h"
#include "Hect/Runtime/Platform.h"
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

namespace
{

static Engine* _instance = nullptr;

}

Engine& Engine::instance()
{
    if (!_instance)
    {
        throw InvalidOperation("An engine instance has not been instantiated");
    }
    return *_instance;
}

void Engine::preInitialize()
{
}

void Engine::postUninitialize()
{
}

Engine::Engine(int argc, char* const argv[])
{
    if (_instance)
    {
        throw InvalidOperation("An engine instance has already been instantiated");
    }
    _instance = this;

    // Parse command line arguments
    CommandLineArguments arguments = parseCommandLineArgument(argc, argv);

    // Load the settings specified on the command-line
    if (!arguments.settingsFilePath.empty())
    {
        _settings = loadConfig(arguments.settingsFilePath);
    }

    setConfiguredLogLevels();

    // Register all of the Hect types
    registerTypes();

    // Ignore false positive memory leaks from static variables
#ifdef HECT_USE_VLD
    VLDMarkAllLeaksAsReported();
#endif

    // Create file system
    _fileSystem.reset(new FileSystem(argc, argv));
    _fileSystem->setWriteDirectory(_fileSystem->baseDirectory());

    // Create platform
    _platform.reset(new Platform());

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

        // Create window and renderers
        _window.reset(new Window("Hect", videoMode));
        _renderer.reset(new Renderer());
        _vectorRenderer.reset(new VectorRenderer(*_renderer));
    }
}

Engine::~Engine()
{
    _instance = nullptr;
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
        const Path scenePath = sceneValue.asString();

        SceneTypeId typeId;
        AssetDecoder decoder(assetCache(), scenePath);
        decoder >> decodeValue("sceneType", typeId, true);

        auto scene = SceneRegistry::create(typeId);
        scene->decode(decoder);

        playScene(*scene);
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

bool Engine::hasMouse()
{
    assert(_platform);
    return _platform->hasMouse();
}

Mouse& Engine::mouse()
{
    assert(_platform);
    return _platform->mouse();
}

bool Engine::hasKeyboard()
{
    assert(_platform);
    return _platform->hasKeyboard();
}

Keyboard& Engine::keyboard()
{
    assert(_platform);
    return _platform->keyboard();
}

bool Engine::hasJoystick(JoystickIndex index)
{
    assert(_platform);
    return _platform->hasJoystick(index);
}

Joystick& Engine::joystick(JoystickIndex index)
{
    assert(_platform);
    return _platform->joystick(index);
}

FileSystem& Engine::fileSystem()
{
    assert(_fileSystem);
    return *_fileSystem;
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

VectorRenderer& Engine::vectorRenderer()
{
    assert(_vectorRenderer);
    return *_vectorRenderer;
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
            std::ifstream stream(settingsFilePath.asString().data());
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
        throw FatalError(format("Failed to load settings file '%s': %s", settingsFilePath.asString().data(), exception.what()));
    }
}

void Engine::setConfiguredLogLevels()
{
    std::unordered_map<std::string, LogLevel> stringToLogLevel;
    stringToLogLevel["Info"] = LogLevel::Info;
    stringToLogLevel["Debug"] = LogLevel::Debug;
    stringToLogLevel["Warning"] = LogLevel::Warning;
    stringToLogLevel["Error"] = LogLevel::Error;
    stringToLogLevel["Trace"] = LogLevel::Trace;

    const DataValue& levels = _settings["logging"]["levels"];
    if (!levels.isNull())
    {
        for (const DataValue& level : levels)
        {
            if (level.isString())
            {
                const std::string& levelString = level.asString();
                auto it = stringToLogLevel.find(levelString);
                if (it != stringToLogLevel.end())
                {
                    LogLevel logLevel = it->second;
                    setLogLevelEnabled(logLevel, true);
                }
            }
        }
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
