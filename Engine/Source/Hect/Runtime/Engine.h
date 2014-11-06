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
#pragma once

#include <memory>

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/IO/FileSystem.h"
#include "Hect/IO/JsonValue.h"
#include "Hect/Runtime/Window.h"

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

class Engine :
    public Uncopyable
{
public:
    Engine(int argc, char* const argv[]);

    int main();

    FileSystem& fileSystem();
    Platform& platform();
    Renderer& renderer();
    Window& window();

    AssetCache& assetCache();

    const JsonValue& config();

private:
    struct CommandLineArguments
    {
        std::string configFilePath;
    };

    JsonValue loadConfig(const Path& configFilePath);

    CommandLineArguments parseCommandLineArgument(int argc, char* const argv[]);

    FileSystem _fileSystem;

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
    JsonValue _config;
};

}