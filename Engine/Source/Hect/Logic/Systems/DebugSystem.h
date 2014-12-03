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
class DebugSystem :
    public System
{
public:

    ///
    /// Debug geometry for a box.
    struct DebugBox
    {
        ///
        /// The box.
        Box box;

        ///
        /// The color.
        Vector3 color;

        ///
        /// The world-space position.
        Vector3 position;

        ///
        /// The world-space rotation.
        Quaternion rotation;
    };

private:
    typedef std::vector<DebugBox> DebugBoxContainer;

public:

    ///
    /// A sequence of debug boxes.
    typedef Sequence<DebugBox, DebugBoxContainer> DebugBoxSequence;

    DebugSystem(Scene& scene);

    ///
    /// Clear all enqueued debug geometry.
    void clear();

    ///
    /// Returns whether the system is enabled.
    bool isEnabled() const;

    ///
    /// Sets whether the system is enabled.
    ///
    /// \param enabled True if the system is enabled; false otherwise.
    void setEnabled(bool enabled);

    ///
    /// Enqueues debug geometry for a box.
    ///
    /// \param box The box.
    /// \param color The color.
    /// \param position The position.
    /// \param rotation The rotation.
    void renderBox(const Box& box, const Vector3& color, const Vector3& position, const Quaternion& rotation = Quaternion());

    ///
    /// Returns all of the enqueued box geometry.
    const DebugBoxSequence boxes() const;

private:
    bool _enabled { false };
    DebugBoxContainer _boxes;
};

}