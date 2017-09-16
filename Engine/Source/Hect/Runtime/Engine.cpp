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
#include "Hect/Runtime/Platform.h"
#include "Hect/Scene/Component.h"
#include "Hect/Scene/ComponentRegistry.h"
#include "Hect/Scene/Scene.h"
#include "Hect/Scene/SceneRegistry.h"
#include "Hect/Timing/Timer.h"

#include "Hect/Generated/RegisterTypes.h"

#include <fstream>
#include <streambuf>
#include <tclap/CmdLine.h>
#include <unordered_map>

#ifdef HECT_USE_VLD
#include <vld.h>
#endif

using namespace hect;

namespace
{
    Engine* _instance = nullptr;

}

Engine& Engine::instance()
{
    if (!_instance)
    {
        throw InvalidOperation("An engine instance has not been instantiated");
    }
    return *_instance;
}

void Engine::pre_initialize()
{
}

void Engine::post_uninitialize()
{
}

Engine::Engine(int argc, char* const argv[], const Path& settings_files_path)
{
    if (_instance)
    {
        throw InvalidOperation("An engine instance has already been instantiated");
    }
    _instance = this;

    // Parse command line arguments
    CommandLineArguments arguments = parse_command_line_argument(argc, argv);

    // Load the settings specified on the command-line
    if (!arguments.settings_file_path.empty())
    {
        _settings = load_config(arguments.settings_file_path);
    }
    else if (!settings_files_path.empty())
    {
        _settings = load_config(settings_files_path);
    }

    set_configured_log_levels();

    // Register all of the Hect types
    register_types();

    // Ignore false positive memory leaks from static variables
#ifdef HECT_USE_VLD
    VLDMarkAllLeaksAsReported();
#endif

    // Create file system
    _file_system.reset(new FileSystem(argc, argv));
    _file_system->set_write_directory(_file_system->base_directory());

    // Create platform
    _platform.reset(new Platform());

    // Mount the archive of built-in assets
    _file_system->mount_archive("Hect.data", "Hect");

    // Create the task pool
    size_t thread_count = _settings["task_pool"]["thread_count"].or_default(2).as_int();
    _task_pool.reset(new TaskPool(thread_count));

    // Create the asset cache
    bool concurrent = _settings["asset_cache"]["concurrent"].or_default(false).as_bool();
    _asset_cache.reset(new AssetCache(*_file_system, concurrent));

    // Mount the archives specified in the settings
    for (const DataValue& archive : _settings["archives"])
    {
        if (!archive["path"].is_null())
        {
            _file_system->mount_archive(archive["path"].as_string(), archive["mount_point"].as_string());
        }
    }

    const DataValue& video_mode_value = _settings["video_mode"];
    if (!video_mode_value.is_null())
    {
        // Load video mode
        VideoMode video_mode;
        try
        {
            DataValueDecoder decoder(video_mode_value);
            decoder >> decode_value(video_mode);
        }
        catch (const DecodeError& error)
        {
            HECT_ERROR(format("Invalid video mode: %s", error.what()));
        }

        // Create window and renderers
        _window.reset(new Window("Hect", video_mode));
        _renderer.reset(new Renderer());
        _vector_renderer.reset(new VectorRenderer(*_renderer));
    }
}

Engine::~Engine()
{
    _instance = nullptr;
}

int Engine::main()
{
    const DataValue& scene_value = _settings["scene"];
    if (scene_value.is_null())
    {
        HECT_ERROR("No scene specified in settings");
    }
    else
    {
        const Path scene_path = scene_value.as_string();

        // Peek at the scene type to be able to create the scene object of the
        // correct type
        SceneTypeId type_id;
        {
            AssetDecoder decoder(asset_cache(), scene_path);
            if (decoder.is_binary_stream())
            {
                decoder >> decode_value("scene_type", type_id);
            }
            else
            {
                Name type_name;
                decoder >> decode_value("scene_type", type_name);
                type_id = SceneRegistry::type_id_of(type_name);
            }
        }

        // Create the scene
        auto scene = SceneRegistry::create(type_id);
        scene->load(scene_path);

        play_scene(*scene);
    }

    return 0;
}

void Engine::play_scene(Scene& scene)
{
    HECT_INFO(format("Playing scene '%s'...", scene.name().data()));

    const Seconds time_step(1.0 / 60.0);
    const Microseconds time_step_microseconds(time_step);

    Timer timer;
    Microseconds accumulator(0);
    Microseconds delta(0);

    // Initialize the scene if needed
    if (!scene.is_initialized())
    {
        scene.initialize();
    }

    while (_platform->handle_events() && scene.active())
    {
        Microseconds delta_time = timer.elapsed();
        timer.reset();

        accumulator += delta_time;
        delta += delta_time;

        while (scene.active() && accumulator >= time_step_microseconds)
        {
            scene.tick(time_step);

            delta = Microseconds(0);
            accumulator -= time_step;
        }

        scene.render(*_window);
        _window->swap_buffers();
    }
}

Platform& Engine::platform()
{
    assert(_platform);
    return *_platform;
}

FileSystem& Engine::file_system()
{
    assert(_file_system);
    return *_file_system;
}

Window& Engine::main_window()
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

VectorRenderer& Engine::vector_renderer()
{
    assert(_vector_renderer);
    return *_vector_renderer;
}

TaskPool& Engine::task_pool()
{
    assert(_task_pool);
    return *_task_pool;
}

AssetCache& Engine::asset_cache()
{
    assert(_asset_cache);
    return *_asset_cache;
}

const DataValue& Engine::settings()
{
    return _settings;
}

DataValue Engine::load_config(const Path& settings_file_path)
{
    try
    {
        // Read the file to a YAML string
        std::string yaml;
        {
            std::ifstream stream(settings_file_path.as_string().data());
            yaml.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        }

        DataValue settings;
        settings.decode_from_yaml(yaml);

        // Load additional settings files
        std::vector<DataValue> included_configs;
        for (const DataValue& include_file_path : settings["include"])
        {
            DataValue settings_data = load_config(include_file_path.as_string());
            included_configs.push_back(std::move(settings_data));
        }

        // Merge additional settingss back to the main settings
        for (const DataValue& included_config : included_configs)
        {
            for (std::string& member_name : included_config.member_names())
            {
                settings.add_member(member_name, included_config[member_name]);
            }
        }

        return settings;
    }
    catch (const std::exception& exception)
    {
        throw FatalError(format("Failed to load settings file '%s': %s", settings_file_path.as_string().data(), exception.what()));
    }
}

void Engine::set_configured_log_levels()
{
    std::unordered_map<std::string, LogLevel> string_to_log_level;
    string_to_log_level["Info"] = LogLevel::Info;
    string_to_log_level["Debug"] = LogLevel::Debug;
    string_to_log_level["Warning"] = LogLevel::Warning;
    string_to_log_level["Error"] = LogLevel::Error;
    string_to_log_level["Trace"] = LogLevel::Trace;

    const DataValue& levels = _settings["logging"]["levels"];
    if (!levels.is_null())
    {
        for (const DataValue& level : levels)
        {
            if (level.is_string())
            {
                const std::string& level_string = level.as_string();
                auto it = string_to_log_level.find(level_string);
                if (it != string_to_log_level.end())
                {
                    LogLevel log_level = it->second;
                    set_log_level_enabled(log_level, true);
                }
            }
        }
    }
}

Engine::CommandLineArguments Engine::parse_command_line_argument(int argc, char* const argv[])
{
    std::vector<std::string> argument_strings { argv[0] };

    // If there is only one argument given then assume it is a settings file path
    if (argc == 2)
    {
        argument_strings.push_back("--settings");
    }

    // Add the remaining arguments
    for (int i = 1; i < argc; ++i)
    {
        argument_strings.push_back(argv[i]);
    }

    try
    {
        TCLAP::CmdLine cmd("Hect Engine");
        TCLAP::ValueArg<std::string> settings_arg
        {
            "s", "settings",
            "A settings file load",
            false,
            "",
            "string"
        };

        cmd.add(settings_arg);
        cmd.parse(argument_strings);

        CommandLineArguments arguments;
        arguments.settings_file_path = settings_arg.getValue();
        return arguments;
    }
    catch (const std::exception& exception)
    {
        throw FatalError(exception.what());
    }
}
