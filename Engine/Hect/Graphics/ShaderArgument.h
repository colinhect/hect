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

#include "Hect/Graphics/ShaderValue.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

namespace hect
{

///
/// An instance of a specific value bound to a shader parameter by name.
class ShaderArgument
{
public:
    ShaderArgument();

    ///
    /// Constructs shader argument given the name of that parameter that the
    /// value is bound to and the value itself.
    ///
    /// \param name The name of the shader parameter to bind to.
    /// \param value The value.
    ShaderArgument(const std::string& name, const ShaderValue& value);

    ///
    /// Returns the name of the parameter.
    const std::string& name() const;

    ///
    /// Returns the value.
    const ShaderValue& value() const;

    ///
    /// Returns whether the shader argument is equivalent to another.
    ///
    /// \param shaderArgument The other shader argument.
    bool operator==(const ShaderArgument& shaderArgument) const;

    ///
    /// Returns whether the shader argument is different from another.
    ///
    /// \param shaderArgument The other shader argument.
    bool operator!=(const ShaderArgument& shaderArgument) const;

    friend Encoder& operator<<(Encoder& encoder, const ShaderArgument& shaderArgument);
    friend Decoder& operator>>(Decoder& decoder, ShaderArgument& shaderArgument);

private:
    std::string _name;
    ShaderValue _value;
};

}