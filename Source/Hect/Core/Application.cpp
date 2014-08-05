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
#include "Application.h"

#include <SDL.h>

using namespace hect;

class HectTypes;

Application::Application(const std::string& name, const Path& settingsFilePath)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw Error(format("Failed to initialize SDL: %s", SDL_GetError()));
    }

    SDL_version version;
    SDL_GetVersion(&version);
    HECT_INFO(format("SDL version %d.%d.%d", version.major, version.minor, version.patch));

    Type::registerTypes<HectTypes>();

    _storage.reset(new Storage());

    // Add the working directory as a data source
    Path workingDirectory = _storage->workingDirectory();
    _storage->addDataSource(workingDirectory);

    // Set the working directory as the write directory
    _storage->setWriteDirectory(workingDirectory);

    // Load the settings
    {
        FileReadStream stream = _storage->openFileForRead(settingsFilePath);
        _settings.decodeFromJson(stream);
    }

    // Add the data sources listed in the settings
    for (const JsonValue& dataSource : _settings["dataSources"])
    {
        _storage->addDataSource(dataSource.asString());
    }

    // Load video mode
    VideoMode videoMode;
    videoMode.decodeFromJsonValue(_settings["videoMode"]);

    // Create window/renderer
    _window.reset(new Window(name, videoMode));
    _renderer.reset(new Renderer(*_window));

    // Load the input axes from the settings
    _input.reset(new Input());
    for (const JsonValue& axisValue : _settings["inputAxes"])
    {
        InputAxis axis;
        axis.decodeFromJsonValue(axisValue);
        _input->addAxis(axis);
    }
}

Application::~Application()
{
    _renderer.reset();
    _window.reset();
    _input.reset();
    SDL_Quit();
}

Input& Application::input()
{
    assert(_input);
    return *_input;
}

Renderer& Application::renderer()
{
    assert(_renderer);
    return *_renderer;
}

Storage& Application::storage()
{
    assert(_renderer);
    return *_storage;
}

Window& Application::window()
{
    assert(_window);
    return *_window;
}