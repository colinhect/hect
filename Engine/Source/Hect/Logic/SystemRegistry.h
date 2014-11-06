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
#include <typeindex>
#include <vector>

#include "Hect/Logic/System.h"

namespace hect
{

///
/// A mapping from system id to system.
typedef std::vector<std::shared_ptr<System>> SystemMap;

///
/// Provides the ability to dynamically create systems based on type
/// information.
class SystemRegistry
{
public:

    ///
    /// Creates a system map containing a system for each of the registered
    /// system types.
    ///
    /// \param scene The scene that the systems are being created for.
    static SystemMap createMap(Scene& scene);

    ///
    /// Returns the system type id for the given system type index.
    ///
    /// \param typeIndex The type index of the system type to get the id of.
    ///
    /// \throws Error If the specified type index does not correspond to a
    /// registered system type.
    static SystemTypeId typeIdOf(std::type_index typeIndex);

    ///
    /// Registers a system type.
    template <typename T>
    static void registerType();

    ///
    /// Returns the system type id for the given system type.
    ///
    /// \throws Error If the specified type is not a registered system type.
    template <typename T>
    static SystemTypeId typeIdOf();

private:
    SystemRegistry();

    static std::map<std::type_index, SystemTypeId> _typeIndexToId;
    static std::vector<std::function<std::shared_ptr<System>(Scene&)>> _constructors;
};

}

#include "SystemRegistry.inl"