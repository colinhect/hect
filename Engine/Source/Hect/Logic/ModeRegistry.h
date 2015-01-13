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

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Hect/Core/Export.h"
#include "Hect/Logic/Mode.h"

namespace hect
{

///
/// Provides the ability to dynamically create Mode%s based on type
/// information.
class HECT_EXPORT ModeRegistry
{
public:

    ///
    /// Creates a Mode of the specified type.
    ///
    /// \param typeName The type name of type of mode to create.
    /// \param engine The engine that the mode is being created for.
    ///
    /// \throws InvalidOperation If the specified type name does not correspond
    /// to a registered mode type.
    static std::unique_ptr<Mode> create(const std::string& typeName, Engine& engine);

    ///
    /// Registers a mode type.
    ///
    /// \warning The type must be registered with Type.
    template <typename T>
    static void registerType();

private:
    ModeRegistry();

    typedef std::function<std::unique_ptr<Mode>(Engine&)> ModeConstructor;

    static std::map<std::string, ModeConstructor> _constructors;
};

}

#include "ModeRegistry.inl"
