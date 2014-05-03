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

#include "Hect/Core/Export.h"
#include "Hect/Graphics/RendererObject.h"

namespace hect
{

///
/// A shader module type.
namespace ShaderModuleType
{
    enum Enum
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
}

///
/// A shader module.
class HECT_API ShaderModule :
    public RendererObject
{
public:

    ///
    /// Constructs an empty shader module.
    ShaderModule();

    ///
    /// Constructs a shader module as a copy of another.
    ///
    /// \param shaderModule The shader module to copy.
    ShaderModule(const ShaderModule& shaderModule);

    ///
    /// Constructs a shader module moved from another.
    ///
    /// \param shaderModule The shader module to move.
    ShaderModule(ShaderModule&& shaderModule);

    ///
    /// Destroys the shader if it is uploaded.
    ~ShaderModule();

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Returns the type.
    ShaderModuleType::Enum type() const;

    ///
    /// Sets the type.
    ///
    /// \note If the shader module is uploaded to a renderer then it will be
    /// destroyed before the type is set.
    ///
    /// \warning Setting the type of an uploaded shader module can cause
    /// undefined behavior if the shader module is uploaded as a module of
    /// a shader.
    ///
    /// \param type The new type.
    void setType(ShaderModuleType::Enum type);

    /// Returns the source.
    const std::string& source() const;

    ///
    /// Sets the source.
    ///
    /// \warning Setting the source of an uploaded shader module can cause
    /// undefined behavior if the shader module is uploaded as a module of
    /// a shader.
    ///
    /// \param source The new source.
    void setSource(const std::string& source);

    ///
    /// Returns whether the shader module is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param shaderModule The other shader module.
    bool operator==(const ShaderModule& shaderModule) const;

    ///
    /// Returns whether the shader module is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param shaderModule The other shader module.
    bool operator!=(const ShaderModule& shaderModule) const;

    ///
    /// Replaces the shader module with a copy of another.
    ///
    /// \note If the shader module was uploaded to a renderer then it will be
    /// destroyed before assigned.
    ///
    /// \warning Setting the source of an uploaded shader module can cause
    /// undefined behavior if the shader module is uploaded as a module of
    /// a shader.
    ///
    /// \param shaderModule The shader module to copy.
    ///
    /// \returns A reference to the shader module.
    ShaderModule& operator=(const ShaderModule& shaderModule);

    ///
    /// Replaces the shader module by moving another.
    ///
    /// \note If the shader module was uploaded to a renderer then it will be
    /// destroyed before assigned.
    ///
    /// \warning Setting the source of an uploaded shader module can cause
    /// undefined behavior if the shader module is uploaded as a module of
    /// a shader.
    ///
    /// \param shaderModule The shader module to move.
    ///
    /// \returns A reference to the shader module.
    ShaderModule& operator=(ShaderModule&& shaderModule);

private:
    std::string _name;
    ShaderModuleType::Enum _type;
    std::string _source;
};

}