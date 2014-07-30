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

using namespace hect;

class HectTypes;

Engine::Engine(const std::string& name, const Path& configFilePath)
{
    Type::registerTypes<HectTypes>();

    // Add the working directory as a data source
    Path workingDirectory = _fileSystem.workingDirectory();
    _fileSystem.addDataSource(workingDirectory);

    // Set the working directory as the write directory
    _fileSystem.setWriteDirectory(workingDirectory);

    // Load the settings
    {
        FileReadStream stream = _fileSystem.openFileForRead(configFilePath);
        _settings.decodeFromJson(stream);
    }

    // Add the data sources listed in the settings
    for (const JsonValue& dataSource : _settings["dataSources"])
    {
        _fileSystem.addDataSource(dataSource.asString());
    }

    // Load video mode
    VideoMode videoMode;
    videoMode.decodeFromJsonValue(_settings["videoMode"]);

    // Create window/renderer
    _window.reset(new Window(name, videoMode));
    _renderer.reset(new Renderer(*_window));

    // Load the input axes from the settings
    for (const JsonValue& axisValue : _settings["inputAxes"])
    {
        InputAxis axis;
        axis.decodeFromJsonValue(axisValue);
        _inputSystem.addAxis(axis);
    }

    size_t threadCount = _settings["assetCache"]["threadCount"].or(8).asUnsigned();
    _assetCache.reset(new AssetCache(_fileSystem, threadCount));
}

Engine::~Engine()
{
}

JsonValue& Engine::settings()
{
    return _settings;
}

FileSystem& Engine::fileSystem()
{
    return _fileSystem;
}

InputSystem& Engine::inputSystem()
{
    return _inputSystem;
}

Window& Engine::window()
{
    assert(_window);
    return *_window;
}

Renderer& Engine::renderer()
{
    assert(_renderer);
    return *_renderer;
}

AssetCache& Engine::assetCache()
{

    return *_assetCache;
}