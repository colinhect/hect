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

namespace hect
{

///
/// The source of an input axis.
enum class InputAxisSource
{
    ///
    /// A mouse movement along the x axis.
    MouseMoveX,

    ///
    /// A mouse movement along the y axis.
    MouseMoveY,

    ///
    /// A mouse button press.
    MouseButton,

    ///
    /// A mouse scroll.
    MouseScroll,

    ///
    /// A key press.
    Key
};

///
/// An axis manipulated by an input device.
class InputAxis
{
public:

    ///
    /// An array of input axes.
    typedef std::vector<InputAxis> Array;

    ///
    /// Constructs an input axis.
    ///
    /// \param name The name of the axis (must be unique).
    /// \param source The source of the axis.
    InputAxis(const std::string& name, InputAxisSource source);

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Returns the source.
    InputAxisSource source() const;

    ///
    /// Returns the mouse button which causes the axis to move in the positive
    /// direction.
    MouseButton positiveMouseButton() const;

    ///
    /// Sets the mouse button which causes the axis to move in the positive
    /// direction.
    ///
    /// \param button The new button.
    void setPositiveMouseButton(MouseButton button);

    ///
    /// Returns the mouse button which causes the axis to move in the negative
    /// direction.
    MouseButton negativeMouseButton() const;

    ///
    /// Sets the mouse button which causes the axis to move in the negative
    /// direction.
    ///
    /// \param button The new button.
    void setNegativeMouseButton(MouseButton button);

    ///
    /// Returns the key which causes the axis to move in the positive
    /// direction.
    Key positiveKey() const;

    ///
    /// Sets the key which causes the axis to move in the positive direction.
    ///
    /// \param key The new key.
    void setPositiveKey(Key key);

    ///
    /// Returns the key which causes the axis to move in the negative
    /// direction.
    Key negativeKey() const;

    ///
    /// Sets the key which causes the axis to move in the negative direction.
    ///
    /// \param key The new key.
    void setNegativeKey(Key key);

    ///
    /// Returns the current value.
    double value() const;

    ///
    /// Sets the current value.
    ///
    /// \param value The new value.
    void setValue(double value);

    ///
    /// Returns the acceleration.
    double acceleration() const;

    ///
    /// Sets the acceleration.
    ///
    /// \param acceleration The new acceleration.
    void setAcceleration(double acceleration);

    ///
    /// Returns the gravity.
    double gravity() const;

    ///
    /// Sets the gravity.
    ///
    /// \param gravity The new gravity.
    void setGravity(double gravity);

private:
    std::string _name;
    InputAxisSource _source;

    MouseButton _positiveMouseButton;
    MouseButton _negativeMouseButton;

    Key _positiveKey;
    Key _negativeKey;

    double _value;
    double _acceleration;
    double _gravity;
};

}