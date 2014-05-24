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

#include <vector>

#include "Hect/Core/Export.h"
#include "Hect/Core/Real.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// The source of an input axis.
namespace InputAxisSource
{
enum Enum
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
}

///
/// An axis manipulated by an input device.
class HECT_API InputAxis :
    public Encodable
{
public:

    ///
    /// An array of input axes.
    typedef std::vector<InputAxis> Array;

    ///
    /// Constructs an input axis.
    InputAxis();

    ///
    /// Constructs an input axis.
    ///
    /// \param name The name of the axis (must be unique).
    InputAxis(const std::string& name);

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Returns the source.
    InputAxisSource::Enum source() const;

    ///
    /// Sets the source.
    ///
    /// \param source The new source.
    void setSource(InputAxisSource::Enum source);

    ///
    /// Returns the mouse button which causes the axis to move in the positive
    /// direction.
    MouseButton::Enum positiveMouseButton() const;

    ///
    /// Sets the mouse button which causes the axis to move in the positive
    /// direction.
    ///
    /// \param button The new button.
    void setPositiveMouseButton(MouseButton::Enum button);

    ///
    /// Returns the mouse button which causes the axis to move in the negative
    /// direction.
    MouseButton::Enum negativeMouseButton() const;

    ///
    /// Sets the mouse button which causes the axis to move in the negative
    /// direction.
    ///
    /// \param button The new button.
    void setNegativeMouseButton(MouseButton::Enum button);

    ///
    /// Returns the key which causes the axis to move in the positive
    /// direction.
    Key::Enum positiveKey() const;

    ///
    /// Sets the key which causes the axis to move in the positive direction.
    ///
    /// \param key The new key.
    void setPositiveKey(Key::Enum key);

    ///
    /// Returns the key which causes the axis to move in the negative
    /// direction.
    Key::Enum negativeKey() const;

    ///
    /// Sets the key which causes the axis to move in the negative direction.
    ///
    /// \param key The new key.
    void setNegativeKey(Key::Enum key);

    ///
    /// Returns the current value.
    Real value() const;

    ///
    /// Sets the current value.
    ///
    /// \param value The new value.
    void setValue(Real value);

    ///
    /// Returns the acceleration.
    Real acceleration() const;

    ///
    /// Sets the acceleration.
    ///
    /// \param acceleration The new acceleration.
    void setAcceleration(Real acceleration);

    ///
    /// Returns the gravity.
    Real gravity() const;

    ///
    /// Sets the gravity.
    ///
    /// \param gravity The new gravity.
    void setGravity(Real gravity);

    ///
    /// Encodes the input axis.
    ///
    /// \param encoder The encoder to use.
    void encode(ObjectEncoder& encoder) const;

    ///
    /// Decodes the input axis.
    ///
    /// \param decoder The decoder to use.
    /// \param assetCache The asset cache to get referenced assets from.
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    std::string _name;
    InputAxisSource::Enum _source;

    MouseButton::Enum _positiveMouseButton;
    MouseButton::Enum _negativeMouseButton;

    Key::Enum _positiveKey;
    Key::Enum _negativeKey;

    Real _value;
    Real _acceleration;
    Real _gravity;
};

}