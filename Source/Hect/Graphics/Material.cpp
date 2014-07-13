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

#include "Hect/IO/Encoders/MaterialEncoder.h"

using namespace hect;

Material::Material()
{
}

Material::Material(const std::string& name) :
    _name(name)
{
}

const std::string& Material::name() const
{
    return _name;
}

void Material::setName(const std::string& name)
{
    _name = name;
}

void Material::addTechnique(const Technique& technique)
{
    _techniques.push_back(technique);
}

Technique::Array& Material::techniques()
{
    return _techniques;
}

const Technique::Array& Material::techniques() const
{
    return _techniques;
}

void Material::encode(ObjectEncoder& encoder) const
{
    MaterialEncoder::encode(*this, encoder);
}

void Material::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    MaterialEncoder::decode(*this, decoder, assetCache);
}

bool Material::operator==(const Material& material) const
{
    // Technique count
    if (_techniques.size() != material.techniques().size())
    {
        return false;
    }

    // Techniques
    size_t techniqueCount = _techniques.size();
    for (size_t i = 0; i < techniqueCount; ++i)
    {
        if (_techniques[i] != material.techniques()[i])
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