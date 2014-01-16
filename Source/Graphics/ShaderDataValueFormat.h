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
/// Provides functionality for loading shaders from data values.
class ShaderDataValueFormat
{
public:

    ///
    /// Loads a shader from a data value.
    ///
    /// \param shader The shader to load to.
    /// \param name The name of the shader.
    /// \param dataValue The root data value.
    /// \param assetCache The asset cache to use to load referenced assets.
    static void load(Shader& shader, const std::string& name, const DataValue& dataValue, AssetCache& assetCache);

    ///
    /// Parses a uniform value of a certain type.
    ///
    /// \param type The uniform value type.
    /// \param dataValue The data value to parse from.
    ///
    /// \returns The parsed uniform value.
    static UniformValue parseValue(UniformType type, const DataValue& dataValue);

private:
    static Uniform _parseUniform(const std::string& name, const DataValue& dataValue);
    static UniformBinding _parseUniformBinding(const DataValue& dataValue);
    static UniformType _parseType(const DataValue& dataValue);
};

}