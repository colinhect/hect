///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

namespace hect
{

///
/// A base exception.
class HECT_EXPORT Name
{
public:

    ///
    /// An index value of a name.
    typedef uint32_t Index;

    ///
    /// Constructs an empty name.
    Name();

    ///
    /// Constructs a name from a string.
    ///
    /// \param name The name string.
    Name(const char* name);

    ///
    /// Constructs a name from a string.
    ///
    /// \param name The name string.
    Name(const std::string& name);

    ///
    /// Returns the name as a string.
    const std::string& asString() const;

    ///
    /// Returns a pointer to the raw data of the name's string.
    const char* data() const;

    ///
    /// Returns the index of the name.
    Index index() const;

    ///
    /// Returns whether the name is empty.
    bool empty() const;

    ///
    /// Returns true if the name is less than another.
    ///
    /// \param name The other name.
    bool operator<(Name name) const;

    ///
    /// Returns whether the name is equivalent to another.
    ///
    /// \param name The other name.
    bool operator==(Name name) const;

    ///
    /// Returns whether the name is different from another.
    ///
    /// \param name The other name.
    bool operator!=(Name name) const;

private:
    static Index lookUpIndex(const std::string& string);

    Index _index;
};

class Encoder;
class Decoder;

HECT_EXPORT Encoder& operator<<(Encoder& encoder, const Name& name);
HECT_EXPORT Decoder& operator>>(Decoder& decoder, Name& name);

}

namespace std
{

template <>
struct hash<hect::Name>
{
    std::size_t operator()(const hect::Name& name) const;
};

}
