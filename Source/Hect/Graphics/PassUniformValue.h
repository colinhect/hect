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

#include "Hect/Graphics/UniformValue.h"

namespace hect
{

///
/// A pass's value for a uniform.
class PassUniformValue
{
public:

    ///
    /// Constructs an pass uniform value given a name an a value.
    ///
    /// \param name The name of the uniform.
    /// \param value The value.
    PassUniformValue(const std::string& name, const UniformValue& value);

    ///
    /// Returns the name of the uniform.
    const std::string& name() const;

    ///
    /// Returns the value.
    const UniformValue& value() const;

    ///
    /// Returns whether the pass uniform value is equivalent to another.
    ///
    /// \param passUniformValue The other pass uniform value.
    bool operator==(const PassUniformValue& passUniformValue) const;

    ///
    /// Returns whether the pass uniform value is different from another.
    ///
    /// \param passUniformValue The other pass uniform value.
    bool operator!=(const PassUniformValue& passUniformValue) const;

private:
    std::string _name;
    UniformValue _value;
};

}