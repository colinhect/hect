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

#include "Hect/Core/Export.h"
#include "Hect/Input/MouseButton.h"
#include "Hect/Input/MouseEventType.h"
#include "Hect/Math/Vector2.h"

namespace hect
{

///
/// An event caused by the alteration of the Mouse.
class HECT_EXPORT MouseEvent
{
public:

    ///
    /// The type of the event.
    MouseEventType type { MouseEventType::Movement };

    ///
    /// The mouse button related to the event.
    ///
    /// \note Only relevant for a ::MouseEventType::ButtonDown or
    /// ::MouseEventType::ButtonUp event.
    MouseButton button { MouseButton::Button0 };

    ///
    /// The coordinates of the cursor.
    IntVector2 cursor_position;

    ///
    /// The movement of the cursor.
    IntVector2 cursor_movement;
};

}