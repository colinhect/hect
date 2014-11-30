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

#include "Hect/Logic/Scene.h"
#include "Hect/Logic/Components/Transform.h"
#include "Hect/Spacial/Box.h"

namespace hect
{

///
/// Provides the functionality for rendering debug geometry.
///
/// \system
class DebugRenderSystem :
    public System
{
public:

    struct DebugBox
    {
        Box box;
        Vector3 color;
        Vector3 position;
        Quaternion rotation;
    };

private:
    typedef std::vector<DebugBox> DebugBoxContainer;

public:

    ///
    /// A sequence of debug boxes.
    typedef Sequence<DebugBox, DebugBoxContainer> DebugBoxSequence;

    DebugRenderSystem(Scene& scene);

    void clear();

    bool isEnabled() const;
    void setEnabled(bool enabled);

    void renderBox(const Box& box, const Vector3& color, const Vector3& position, const Quaternion& rotation = Quaternion());
    
    const DebugBoxSequence boxes() const;

private:
    bool _enabled { false };
    DebugBoxContainer _boxes;
};

}