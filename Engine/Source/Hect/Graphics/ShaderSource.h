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

#include <string>

#include "Hect/IO/Path.h"
#include "Hect/Graphics/ShaderSourceType.h"

namespace hect
{

///
/// A shader source module.
class ShaderSource
{
public:

    ///
    /// Constructs a shader source module.
    ///
    /// \param type The source type.
    /// \param path The path to the source file (for compile error reporting).
    /// \param source The source code of the module.
    ShaderSource(ShaderSourceType type, const Path& path, const std::string& source);

    ///
    /// Returns the type.
    ShaderSourceType type() const;

    ///
    /// Returns the path to the source file.
    const Path& path() const;

    ///
    /// Returns the source.
    const std::string& source() const;

    ///
    /// Returns whether the shader source is equivalent to another.
    ///
    /// \param shaderSource The other shader source.
    bool operator==(const ShaderSource& shaderSource) const;

    ///
    /// Returns whether the shader source is different from another.
    ///
    /// \param shaderSource The other shader source.
    bool operator!=(const ShaderSource& shaderSource) const;

private:
    ShaderSourceType _type;
    Path _path;
    std::string _source;
};

}