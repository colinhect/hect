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

Material::TechniqueSequence Material::techniques()
{
    return _techniques;
}

const Material::TechniqueSequence Material::techniques() const
{
    return _techniques;
}

void Material::addTechnique(const Technique& technique)
{
    _techniques.push_back(technique);
}

void Material::clearTechniques()
{
    _techniques.clear();
}

Technique& Material::preferedTechnique()
{
    if (_techniques.empty())
    {
        throw Error("The material has no techniques");
    }
    return _techniques[0];
}

bool Material::operator==(const Material& material) const
{
    if (_techniques.size() != material._techniques.size())
    {
        return false;
    }

    for (size_t i = 0; i < _techniques.size(); ++i)
    {
        if (_techniques[i] != material._techniques[i])
        {
            return false;
        }
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
    return encoder << beginObject()
        << encodeVector("techniques", material._techniques)
        << endObject();
}

Decoder& operator>>(Decoder& decoder, Material& material)
{
    return decoder >> beginObject()
        >> decodeVector("techniques", material._techniques)
        >> endObject();
}

}