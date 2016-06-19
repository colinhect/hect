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
#include "Name.h"

#include <memory>
#include <mutex>
#include <unordered_map>

#include "Hect/Core/Exception.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

using namespace hect;

namespace
{

static std::mutex _nameIndexLookUpMutex;
static std::unordered_map<std::string, Name::Index> _nameStringToIndex;
static std::vector<std::string> _nameIndexToString;

}

Name::Name() :
    _index(-1)
{
}

Name::Name(const char* name) :
    _index(lookUpIndex(name))
{
}

Name::Name(const std::string& name) :
    _index(lookUpIndex(name))
{
}

const std::string& Name::asString() const
{
    static const std::string emptyString = "";
    if (empty())
    {
        return emptyString;
    }
    else
    {
        return _nameIndexToString[_index];
    }
}

const char* Name::data() const
{
    return asString().data();
}

Name::Index Name::index() const
{
    return _index;
}

bool Name::empty() const
{
    return _index == Index(-1);
}

bool Name::operator<(const Name& name) const
{
    return _index < name._index;
}

bool Name::operator==(const Name& name) const
{
    return _index == name._index;
}

bool Name::operator!=(const Name& name) const
{
    return _index != name._index;
}

Name::Index Name::lookUpIndex(const std::string& string)
{
    Name::Index index = -1;

    // If this is the first encounter of this name
    auto it = _nameStringToIndex.find(string);
    if (it == _nameStringToIndex.end())
    {
        std::lock_guard<std::mutex> lock(_nameIndexLookUpMutex);

        // Add the string to the string look-up table
        index = static_cast<Name::Index>(_nameIndexToString.size());
        _nameIndexToString.push_back(string);

        // Add the index to the index look-up table
        _nameStringToIndex[string] = index;
    }
    else
    {
        index = it->second;
    }

    return index;
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Name& name)
{
    encoder << encodeValue(name.asString());
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Name& name)
{
    std::string nameString;
    decoder >> decodeValue(nameString);
    name = Name(nameString);
    return decoder;
}

}

namespace std
{

std::size_t hash<hect::Name>::operator()(const hect::Name& name) const
{
    return static_cast<std::size_t>(name.index());
}

}
