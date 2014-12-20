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
#include "Material.h"

using namespace hect;

Material::Material()
{
}

Material::Material(const std::string& name) :
    Asset(name)
{
}

const AssetHandle<Shader>& Material::shader() const
{
    return _shader;
}

void Material::setShader(const AssetHandle<Shader>& shader)
{
    _shader = shader;
}

CullMode Material::cullMode() const
{
    return _cullMode;
}

void Material::setCullMode(CullMode cullMode)
{
    _cullMode = cullMode;
}

bool Material::operator==(const Material& material) const
{
    if (_shader != material._shader)
    {
        return false;
    }

    return true;
}

bool Material::operator!=(const Material& material) const
{
    return !(*this == material);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Material& material)
{
    encoder << beginObject()
            << encodeValue("shader", material._shader)
            << endObject();

    return encoder;
}

Decoder& operator>>(Decoder& decoder, Material& material)
{
    decoder >> beginObject()
            >> decodeValue("shader", material._shader)
            >> endObject();

    return decoder;
}

}
