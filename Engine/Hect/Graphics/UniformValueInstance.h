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

#include "Hect/Graphics/UniformValue.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// An instance of a specific value bound to a uniform by name.
class UniformValueInstance
{
public:
    UniformValueInstance();

    ///
    /// Constructs an uniform value instance given the name of that uniform
    /// that the value is an instance for and the value itself.
    ///
    /// \param name The name of the uniform to bind to.
    /// \param value The value.
    UniformValueInstance(const std::string& name, const UniformValue& value);

    ///
    /// Returns the name of the uniform.
    const std::string& name() const;

    ///
    /// Returns the value.
    const UniformValue& value() const;

    ///
    /// Returns whether the uniform value instance is equivalent to another.
    ///
    /// \param uniformValueInstance The other uniform value instance.
    bool operator==(const UniformValueInstance& uniformValueInstance) const;

    ///
    /// Returns whether the uniform value instance is different from another.
    ///
    /// \param uniformValueInstance The other uniform value instance.
    bool operator!=(const UniformValueInstance& uniformValueInstance) const;

    friend Encoder& operator<<(Encoder& encoder, const UniformValueInstance& uniformValueInstance);
    friend Decoder& operator>>(Decoder& decoder, UniformValueInstance& uniformValueInstance);

private:
    std::string _name;
    UniformValue _value;
};

}