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

#include "Hect/Graphics/MaterialValue.h"

namespace hect
{

///
/// A binding from a material parameter to a built-in value in the rendering
/// pipeline.
enum MaterialParameterBinding
{
    ///
    /// No binding.
    MaterialParameterBinding_None,

    ///
    /// Bound to the size of the active render target.
    MaterialParameterBinding_RenderTargetSize,

    ///
    /// Bound to the world-space position of the active camera.
    MaterialParameterBinding_CameraPosition,

    ///
    /// Bound to the world-space front direction of the active camera.
    MaterialParameterBinding_CameraFront,

    ///
    /// Bound to the world-space up direction of the active camera.
    MaterialParameterBinding_CameraUp,

    ///
    /// Bound to the exposure of the active camera.
    MaterialParameterBinding_CameraExposure,

    ///
    /// Bound to the one divided by the gamma of the active camera.
    MaterialParameterBinding_CameraOneOverGamma,

    ///
    /// Bound to the view matrix.
    MaterialParameterBinding_ViewMatrix,

    ///
    /// Bound to the projection matrix.
    MaterialParameterBinding_ProjectionMatrix,

    ///
    /// Bound to the product of the view and projection matrices.
    MaterialParameterBinding_ViewProjectionMatrix,

    ///
    /// Bound to the model matrix.
    MaterialParameterBinding_ModelMatrix,

    ///
    /// Bound to the product of the model and the view matrix.
    MaterialParameterBinding_ModelViewMatrix,

    ///
    /// Bound to the product of the model, view, and projection matrices.
    MaterialParameterBinding_ModelViewProjectionMatrix
};

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