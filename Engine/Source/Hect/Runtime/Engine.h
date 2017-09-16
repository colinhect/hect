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
#pragma once

#include <memory>

#include "Hect/Concurrency/TaskPool.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Name.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/VectorRenderer.h"
#include "Hect/Input/Joystick.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"
#include "Hect/IO/DataValue.h"
#include "Hect/IO/FileSystem.h"
#include "Hect/Runtime/Window.h"

namespace hect
{

class Scene;
class Platform;

///
/// An instance of Hect %Engine.
///
/// \warning Only one engine instance can be instantiated at a given time.
class HECT_EXPORT Engine :
    public Uncopyable
{
public:

    ///
    /// Returns the primary instance of Engine.
    ///
    /// \throws InvalidOperation If an instance of Engine has not been
    /// instantiated.
    static Engine& instance();

    ///
    /// Performs the pre-initialization flow.
    static void pre_initialize();

    ///
    /// Performs the post-uninitialization flow.
    static void post_uninitialize();

    ///
    /// Constructs an engine.
    ///
    /// \param argc The number of command-line arguments passed to main().
    /// \param argv The array of command-line arguments passed to main().
    /// \param settings_file_path The path to the settings file to use if none
    /// is specified in the arguments.
    ///
    /// \throws InvalidOperation If an instance of Engine has already been
    /// instantiated.
    Engine(int argc, char* const argv[], const Path& settings_file_path = Path());

    ~Engine();

    ///
    /// Executes the engine to completion.
    ///
    /// \returns The process return code.
    int main();

    ///
    /// Plays a scene for as long as it remains active.
    ///
    /// \param scene The scene to play.
    void play_scene(Scene& scene);

    ///
    /// Returns the platform.
    Platform& platform();

    ///
    /// Returns the file system.
    FileSystem& file_system();

    ///
    /// Returns the main window.
    Window& main_window();

    ///
    /// Returns the renderer.
    Renderer& renderer();

    ///
    /// Returns the vector renderer.
    VectorRenderer& vector_renderer();

    ///
    /// Returns the asset cache.
    AssetCache& asset_cache();

    ///
    /// Returns the task pool.
    TaskPool& task_pool();

    ///
    /// Returns the settings data.
    const DataValue& settings();

private:
    struct CommandLineArguments
    {
        std::string settings_file_path;
    };

    DataValue load_config(const Path& settings_file_path);
    void set_configured_log_levels();

    CommandLineArguments parse_command_line_argument(int argc, char* const argv[]);

    std::unique_ptr<FileSystem> _file_system;
    std::unique_ptr<Platform> _platform;
    std::unique_ptr<Window> _window;
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<VectorRenderer> _vector_renderer;
    std::unique_ptr<AssetCache> _asset_cache;
    std::unique_ptr<TaskPool> _task_pool;
    DataValue _settings;
};

}

#define HECT_MAIN_PROJECT(project, settings_file) \
    int main(int argc, char* const argv[]) \
    { \
        try \
        { \
            hect::Engine::pre_initialize(); \
            project::register_types(); \
            hect::Engine engine(argc, argv, settings_file); \
            int code = engine.main(); \
            hect::Engine::post_uninitialize(); \
            return code; \
        } \
        catch (hect::Exception& exception) \
        { \
            HECT_ERROR(exception.what()); \
            hect::Engine::post_uninitialize(); \
        } \
        return 0; \
    }

#define HECT_MAIN_SYSTEM_TEST_HARNESS(settings_file, block) \
    int main(int argc, char* const argv[]) \
    { \
        try \
        { \
            hect::Engine::pre_initialize(); \
            char settings_path[] = settings_file; \
            char* const engine_argv[] = { argv[0], settings_path }; \
            hect::Engine engine(2, engine_argv); \
            int code = 0; \
            block; \
            hect::Engine::post_uninitialize(); \
            return code; \
        } \
        catch (hect::Exception& exception) \
        { \
            HECT_ERROR(exception.what()); \
            hect::Engine::post_uninitialize(); \
        } \
        return 0; \
    }

#define HECT_MAIN_SYSTEM_TEST_HARNESS_PROJECT(project, settings_file, block) \
    int main(int argc, char* const argv[]) \
    { \
        try \
        { \
            hect::Engine::pre_initialize(); \
            project::register_types(); \
            char settings_path[] = settings_file; \
            char* const engine_argv[] = { argv[0], settings_path }; \
            hect::Engine engine(2, engine_argv); \
            int code = 0; \
            block; \
            hect::Engine::post_uninitialize(); \
            return code; \
        } \
        catch (hect::Exception& exception) \
        { \
            HECT_ERROR(exception.what()); \
            hect::Engine::post_uninitialize(); \
        } \
        return 0; \
    }

#define HECT_MAIN_UNIT_TEST_HARNESS(block) \
    int main(int argc, char* const argv[]) \
    { \
        try \
        { \
            hect::Engine::pre_initialize(); \
            int code = 0; \
            block; \
            hect::Engine::post_uninitialize(); \
            return code; \
        } \
        catch (hect::Exception& exception) \
        { \
            HECT_ERROR(exception.what()); \
            hect::Engine::post_uninitialize(); \
        } \
        return 0; \
    }

#define HECT_MAIN_UNIT_TEST_HARNESS_PROJECT(project, block) \
    int main(int argc, char* const argv[]) \
    { \
        try \
        { \
            hect::Engine::pre_initialize(); \
            project::register_types(); \
            int code = 0; \
            block; \
            hect::Engine::post_uninitialize(); \
            return code; \
        } \
        catch (hect::Exception& exception) \
        { \
            HECT_ERROR(exception.what()); \
            hect::Engine::post_uninitialize(); \
        } \
        return 0; \
    }
