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
#include "Game.h"

#include "Hect/Logic/Scene.h"
#include "Hect/Platform/FileSystem.h"
#include "Hect/Platform/Platform.h"
#include "Hect/Timing/Timer.h"

using namespace hect;

class HectTypes;

Game::Game(const std::string& name, const Path& settingsFilePath)
{
    Platform::initialize();

    Type::registerTypes<HectTypes>();
    
    // Add the working directory as a data source
    Path workingDirectory = FileSystem::workingDirectory();
    FileSystem::mount(workingDirectory);

    // Set the working directory as the write directory
    FileSystem::setWriteDirectory(workingDirectory);

    // Load the settings
    {
        ReadStream::Pointer stream = FileSystem::openFileForRead(settingsFilePath);
        _settings.decodeFromJson(*stream);
    }

    // Add the data sources listed in the settings
    for (const JsonValue& dataSource : _settings["dataSources"])
    {
        FileSystem::mount(dataSource.asString());
    }

    // Load video mode
    VideoMode videoMode;
    videoMode.decodeFromJsonValue(_settings["videoMode"]);

    // Create window/renderer/input devices
    _window.reset(new Window(name, videoMode));
    _renderer.reset(new Renderer(*_window));
}

Game::~Game()
{
    _renderer.reset();
    _window.reset();

    Platform::deinitialize();
}

void Game::playScene(Scene& scene)
{
    while (Platform::handleEvents())
    {
        scene.tick();
        _window->swapBuffers();
    }
}

Renderer& Game::renderer()
{
    assert(_renderer);
    return *_renderer;
}

Window& Game::window()
{
    assert(_window);
    return *_window;
}

const JsonValue& Game::settings() const
{
    return _settings;
}