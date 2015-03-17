///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "Hect/IO/Encodable.h"
#include "Hect/Graphics/ShaderModuleType.h"

namespace hect
{

///
/// A shader module.
class HECT_EXPORT ShaderModule :
    public Encodable
{
public:

    ///
    /// Constructs an empty shader module.
    ShaderModule();

    ///
    /// Constructs a shader module.
    ///
    /// \param type The module type.
    /// \param name The name of the module.
    /// \param source The source code of the module.
    ShaderModule(ShaderModuleType type, const std::string& name, const std::string& source);

    ///
    /// Returns the type.
    ShaderModuleType type() const;

    ///
    /// Returns the name of the module.
    const std::string& name() const;

    ///
    /// Returns the source code.
    const std::string& source() const;

    ///
    /// Returns whether the shader module is equivalent to another.
    ///
    /// \param shaderModule The other shader module.
    bool operator==(const ShaderModule& shaderModule) const;

    ///
    /// Returns whether the shader source is different from another.
    ///
    /// \param shaderModule The other shader module.
    bool operator!=(const ShaderModule& shaderModule) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder) override;

private:
    ShaderModuleType _type { ShaderModuleType_Vertex };
    std::string _name;
    std::string _source;
};

}