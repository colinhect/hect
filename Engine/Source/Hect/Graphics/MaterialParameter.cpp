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
#include "MaterialParameter.h"

using namespace hect;

MaterialParameter::MaterialParameter()
{
}

MaterialValueType MaterialParameter::type() const
{
    return _type;
}

MaterialParameterBinding MaterialParameter::binding() const
{
    return _binding;
}

const std::string& MaterialParameter::name() const
{
    return _name;
}

int MaterialParameter::location() const
{
    return _location;
}

void MaterialParameter::setLocation(int location)
{
    _location = location;
}

unsigned MaterialParameter::textureIndex() const
{
    if (_type != MaterialValueType_Texture)
    {
        throw Error("Parameter is not a texture");
    }

    return _textureIndex;
}

MaterialParameter::MaterialParameter(size_t index, unsigned textureIndex, const std::string& name, MaterialValueType type, MaterialParameterBinding binding) :
    _name(name),
    _index(index),
    _textureIndex(textureIndex),
    _type(type),
    _binding(binding)
{
    resolveTypeFromBinding();
}

void MaterialParameter::resolveTypeFromBinding()
{
    switch (_binding)
    {
    case MaterialParameterBinding_None:
        break;
    case MaterialParameterBinding_CameraExposure:
    case MaterialParameterBinding_CameraOneOverGamma:
        _type = MaterialValueType_Float;
        break;
    case MaterialParameterBinding_RenderTargetSize:
        _type = MaterialValueType_Vector2;
        break;
    case MaterialParameterBinding_CameraPosition:
    case MaterialParameterBinding_CameraFront:
    case MaterialParameterBinding_CameraUp:
        _type = MaterialValueType_Vector3;
        break;
    case MaterialParameterBinding_ViewMatrix:
    case MaterialParameterBinding_ProjectionMatrix:
    case MaterialParameterBinding_ViewProjectionMatrix:
    case MaterialParameterBinding_ModelMatrix:
    case MaterialParameterBinding_ModelViewMatrix:
    case MaterialParameterBinding_ModelViewProjectionMatrix:
        _type = MaterialValueType_Matrix4;
        break;
    }
}