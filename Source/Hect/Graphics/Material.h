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

#include "Hect/Core/Sequence.h"
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
    Sequence<Technique> techniques();

    ///
    /// Returns the techniques.
    ConstSequence<Technique> techniques() const;

    ///
    /// Adds a technique.
    ///
    /// \param technique The technique to add.
    void addTechnique(const Technique& technique);

    ///
    /// Removes all techniques.
    void clearTechniques();

    ///
    /// Returns the prefered technique.
    ///
    /// \throws Error If the material does not have any techniques.
    Technique& preferedTechnique();

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    std::string _name;
    std::vector<Technique> _techniques;
};

}