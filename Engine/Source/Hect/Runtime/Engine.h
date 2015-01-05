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
#pragma once

#include <memory>

#include "Hect/Concurrency/TaskPool.h"
#include "Hect/Core/Configuration.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/IO/DataValue.h"
#include "Hect/Runtime/Window.h"

#ifdef HECT_FILESYSTEM_PHYSFS
#include "Hect/IO/FileSystems/PhysFSFileSystem.h"
#endif

#ifdef HECT_PLATFORM_SDL
#include "Hect/Runtime/Platforms/SdlPlatform.h"
#else
#include "Hect/Runtime/Platforms/NullPlatform.h"
#endif

#ifdef HECT_RENDERER_OPENGL
#include "Hect/Graphics/Renderers/OpenGLRenderer.h"
#else
#include "Hect/Graphics/Renderers/NullRenderer.h"
#endif

namespace hect
{

///
/// An instance of Hect %Engine.
///
/// \warning Only one engine instance can be instantiated at a given time.
class HECT_EXPORT Engine :
    public Uncopyable
{
public:

    ///
    /// Constructs an engine.
    ///
    /// \param argc The number of command-line arguments passed to main().
    /// \param argv The array of command-line arguments passed to main().
    Engine(int argc, char* const argv[]);

    ///
    /// Executes the engine to completion.
    ///
    /// \returns The process return code.
    int main();

    ///
    /// Returns the file system.
    FileSystem& fileSystem();

    ///
    /// Returns the platform.
    Platform& platform();

    ///
    /// Returns the renderer.
    Renderer& renderer();

    ///
    /// Returns the main window.
    Window& window();

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
    struct HECT_EXPORT CommandLineArguments
    {
        std::string settingsFilePath;
    };

    DataValue loadConfig(const Path& settingsFilePath);

    CommandLineArguments parseCommandLineArgument(int argc, char* const argv[]);

#ifdef HECT_FILESYSTEM_PHYSFS
    PhysFSFileSystem _fileSystem;
#endif

#ifdef HECT_PLATFORM_SDL
    SdlPlatform _platform;
#else
    NullPlatform _platform;
#endif

#ifdef HECT_RENDERER_OPENGL
    OpenGLRenderer _renderer;
#else
    NullRenderer _renderer;
#endif

    std::unique_ptr<Window> _window;
    std::unique_ptr<AssetCache> _assetCache;
    std::unique_ptr<TaskPool> _taskPool;
    DataValue _settings;
};

}