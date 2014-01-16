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
/// A shader module type.
enum class ShaderModuleType
{

    ///
    /// A vertex program module.
    Vertex,

    ///
    /// A pixel program module.
    Pixel,

    ///
    /// A geometry program module.
    Geometry
};

///
/// A shader module.
class ShaderModule :
    public RendererObject
{
public:

    ///
    /// Constructs an empty shader module.
    ShaderModule();

    ///
    /// Constructs a shader module given its type and source.
    ///
    /// \param name The name.
    /// \param type The type.
    /// \param source The source.
    ShaderModule(const std::string& name, ShaderModuleType type, const std::string& source);

    ///
    /// Destroys the shader if it is uploaded.
    ~ShaderModule();

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Returns the type.
    ShaderModuleType type() const;

    /// Returns the source.
    const std::string& source() const;

private:
    std::string _name;
    ShaderModuleType _type;
    std::string _source;
};

}