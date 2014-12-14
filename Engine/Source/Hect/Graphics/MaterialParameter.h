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

#include "Hect/Graphics/MaterialParameterBinding.h"
#include "Hect/Graphics/MaterialValue.h"

namespace hect
{

///
/// A parameter of a material.
///
/// \note A material parameter must either have a binding or a type.
class MaterialParameter
{
    friend class Material;
public:
    MaterialParameter();

    ///
    /// Returns the value type the parameter accepts.
    MaterialValueType type() const;

    ///
    /// Returns the binding.
    MaterialParameterBinding binding() const;

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Returns the compiled location.
    int location() const;

    ///
    /// Sets the compiled location.
    ///
    /// \param location The compiled location.
    void setLocation(int location);

    ///
    /// Returns the associated texture index.
    ///
    /// \throws Error If the parameter is not of type
    /// ::MaterialValueType_Texture.
    unsigned textureIndex() const;

    ///
    /// Returns whether the material parameter is equivalent to another.
    ///
    /// \param materialParameter The other material parameter.
    bool operator==(const MaterialParameter& materialParameter) const;

    ///
    /// Returns whether the material parameter is different from another.
    ///
    /// \param materialParameter The other material parameter.
    bool operator!=(const MaterialParameter& materialParameter) const;

private:
    MaterialParameter(size_t index, unsigned textureIndex, const std::string& name, MaterialValueType type, MaterialParameterBinding binding);

    void resolveTypeFromBinding();

    size_t _index { 0 };
    unsigned _textureIndex { 0 };

    std::string _name;

    MaterialValueType _type { MaterialValueType_Float };
    MaterialParameterBinding _binding { MaterialParameterBinding_None };

    int _location { -1 };
};

}