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

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Hect/Logic/GameMode.h"

namespace hect
{

///
/// Provides the ability to dynamically create game modes based on type
/// information.
class GameModeRegistry
{
public:

    ///
    /// Creates a game mode of the specified type.
    ///
    /// \param typeName The type name of type of game mode to create.
    /// \param engine The engine that the game mode is being created for.
    ///
    /// \throws Error If the specified type name does not correspond to a
    /// registered game mode type.
    static std::unique_ptr<GameMode> create(const std::string& typeName, Engine& engine);

    ///
    /// Registers a game mode type.
    template <typename T>
    static void registerType();

private:
    GameModeRegistry();

    typedef std::function<std::unique_ptr<GameMode>(Engine&)> GameModeConstructor;

    static std::map<std::string, GameModeConstructor> _constructors;
};

}

#include "GameModeRegistry.inl"