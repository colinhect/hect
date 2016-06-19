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
    static void preInitialize();

    ///
    /// Performs the post-uninitialization flow.
    static void postUninitialize();

    ///
    /// Constructs an engine.
    ///
    /// \param argc The number of command-line arguments passed to main().
    /// \param argv The array of command-line arguments passed to main().
    ///
    /// \throws InvalidOperation If an instance of Engine has already been
    /// instantiated.
    Engine(int argc, char* const argv[]);

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
    void playScene(Scene& scene);

    ///
    /// Returns whether the platform has a mouse connected.
    bool hasMouse();

    ///
    /// Returns the mouse.
    ///
    /// \throws InvalidOperation If the platform does not have a mouse
    /// connected.
    Mouse& mouse();

    ///
    /// Returns whether the platform has a keyboard connected.
    bool hasKeyboard();

    ///
    /// Returns the keyboard.
    ///
    /// \throws InvalidOperation If the platform does not have a keyboard
    /// connected.
    Keyboard& keyboard();

    ///
    /// Returns whether the platform has a joystick connected at the given
    /// index.
    ///
    /// \param index The index of the joystick.
    bool hasJoystick(JoystickIndex index);

    ///
    /// Returns the joystick connected at the specified index.
    ///
    /// \param index The index of the joystick.
    ///
    /// \throws InvalidOperation If no joystick is connected at the given
    // index.
    Joystick& joystick(JoystickIndex index);

    ///
    /// Returns the file system.
    FileSystem& fileSystem();

    ///
    /// Returns the main window.
    Window& window();

    ///
    /// Returns the renderer.
    Renderer& renderer();

    ///
    /// Returns the vector renderer.
    VectorRenderer& vectorRenderer();

    ///
    /// Returns the asset cache.
    AssetCache& assetCache();

    ///
    /// Returns the task pool.
    TaskPool& taskPool();

    ///
    /// Returns the settings data.
    const DataValue& settings();

private:
    struct CommandLineArguments
    {
        std::string settingsFilePath;
    };

    DataValue loadConfig(const Path& settingsFilePath);
    void setConfiguredLogLevels();

    CommandLineArguments parseCommandLineArgument(int argc, char* const argv[]);

    std::unique_ptr<FileSystem> _fileSystem;
    std::unique_ptr<Platform> _platform;
    std::unique_ptr<Window> _window;
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<VectorRenderer> _vectorRenderer;
    std::unique_ptr<AssetCache> _assetCache;
    std::unique_ptr<TaskPool> _taskPool;
    DataValue _settings;
};

}

#define HECT_PROJECT_MAIN(project) \
    int main(int argc, char* const argv[]) \
    { \
        try \
        { \
            hect::Engine::preInitialize(); \
            project::registerTypes(); \
            hect::Engine engine(argc, argv); \
            int code = engine.main(); \
            hect::Engine::postUninitialize(); \
            return code; \
        } \
        catch (hect::Exception& exception) \
        { \
            HECT_ERROR(exception.what()); \
            hect::Engine::postUninitialize(); \
        } \
        return 0; \
    }

#define HECT_PROJECT_MAIN_TEST_HARNESS(project, settingsFile, block) \
    int main(int argc, char* const argv[]) \
    { \
        try \
        { \
            hect::Engine::preInitialize(); \
            project::registerTypes(); \
            char settingsPath[] = settingsFile; \
            char* const engineArgv[] = { argv[0], settingsPath }; \
            hect::Engine engine(argc, argv); \
            int code = 0; \
            block; \
            hect::Engine::postUninitialize(); \
            return code; \
        } \
        catch (hect::Exception& exception) \
        { \
            HECT_ERROR(exception.what()); \
            hect::Engine::postUninitialize(); \
        } \
        return 0; \
    }

#define HECT_MAIN_TEST_HARNESS(settingsFile, block) \
    int main(int argc, char* const argv[]) \
    { \
        try \
        { \
            hect::Engine::preInitialize(); \
            char settingsPath[] = settingsFile; \
            char* const engineArgv[] = { argv[0], settingsPath }; \
            hect::Engine engine(argc, argv); \
            int code = 0; \
            block; \
            hect::Engine::postUninitialize(); \
            return code; \
        } \
        catch (hect::Exception& exception) \
        { \
            HECT_ERROR(exception.what()); \
            hect::Engine::postUninitialize(); \
        } \
        return 0; \
    }
