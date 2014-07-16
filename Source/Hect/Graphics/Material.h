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

#include "Hect/Graphics/Technique.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// The manner in which a surface is rendered.
class Material :
    public Encodable
{
public:

    ///
    /// Constructs a material without any techniques.
    Material();

    ///
    /// Constructs a material without any techniques.
    ///
    /// \param name The name of the material.
    Material(const std::string& name);

    ///
    /// Returns the name.
    const std::string& name() const;

    ///
    /// Sets the name.
    ///
    /// \param name The new name.
    void setName(const std::string& name);

    ///
    /// Returns the techniques.
    Technique::Array& techniques();

    ///
    /// Returns the techniques.
    const Technique::Array& techniques() const;

    ///
    /// Sets the techniques that the material has.
    ///
    /// \param technique The technique to add.
    void setTechniques(const Technique::Array& techniques);

    ///
    /// Encodes the mesh.
    ///
    /// \param encoder The encoder to use.
    void encode(ObjectEncoder& encoder) const;

    ///
    /// Decodes the mesh.
    ///
    /// \param decoder The decoder to use.
    /// \param assetCache The asset cache to get referenced assets from.
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

    ///
    /// Returns whether the material is equivalent to another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param material The other material.
    bool operator==(const Material& material) const;

    ///
    /// Returns whether the material is different from another.
    ///
    /// \note Does not compare the name.
    ///
    /// \param material The other material.
    bool operator!=(const Material& material) const;

private:
    std::string _name;
    Technique::Array _techniques;
};

}