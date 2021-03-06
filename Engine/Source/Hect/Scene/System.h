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

#include "Hect/Scene/ComponentListener.h"

namespace hect
{

///
/// A template list of the component types that a system is notified about.
///
/// \note This class inherits from ComponentListener for each component type
/// listed.
template <typename... ComponentTypes>
class Components :
    public ComponentListener<ComponentTypes>...
{
public:
    Components(Scene& scene);

    virtual ~Components() { }
};

///
/// A system affecting the behavior of a scene by simulating and relating
/// Component%s and \link Entity Entities \endlink within it.
template <typename SystemType, typename ComponentListenersType = Components<>>
class System :
    public SystemBase,
    public ComponentListenersType
{
public:

    ///
    /// Constructs a system.
    ///
    /// \param scene The scene that the system is a part of.
    System(Scene& scene);

    virtual ~System() { }
};

}

#include "System.inl"
