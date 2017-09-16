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

#include <deque>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Logging.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"

using namespace hect;

namespace
{

std::mutex _name_index_look_up_mutex;
std::unordered_map<std::string, Name::Index> _name_string_to_index;
std::deque<std::string> _name_strings;

}

const Name Name::Unnamed("<unnamed>");

Name::Name() :
    _index(-1)
#ifdef HECT_DEBUG_BUILD
    , _value(nullptr)
#endif
{
}

Name::Name(const char* name) :
    _index(look_up_index(name))
#ifdef HECT_DEBUG_BUILD
    , _value(data())
#endif
{
}

Name::Name(const std::string& name) :
    _index(look_up_index(name))
#ifdef HECT_DEBUG_BUILD
    , _value(data())
#endif
{
}

const std::string& Name::as_string() const
{
    static const std::string empty_string = "";
    if (empty())
    {
        return empty_string;
    }
    else
    {
        return _name_strings[_index];
    }
}

const char* Name::data() const
{
    return as_string().data();
}

Name::Index Name::index() const
{
    return _index;
}

bool Name::empty() const
{
    return _index == Index(-1);
}

bool Name::operator<(Name name) const
{
    return _index < name._index;
}

bool Name::operator==(Name name) const
{
    return _index == name._index;
}

bool Name::operator!=(Name name) const
{
    return _index != name._index;
}

Name::Index Name::look_up_index(const std::string& string)
{
    std::lock_guard<std::mutex> lock(_name_index_look_up_mutex);

    Name::Index index;

    // If this is the first encounter of this name
    auto it = _name_string_to_index.find(string);
    if (it == _name_string_to_index.end())
    {
        // Add the string to the string look-up table
        index = static_cast<Name::Index>(_name_strings.size());
        _name_strings.emplace_back(string);

        // Add the index to the index look-up table
        _name_string_to_index[string] = index;

        HECT_TRACE(format("Indexed '%s' to name table at index %i", string.data(), index))
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
    encoder << encode_value(name.as_string());
    return encoder;
}

Decoder& operator>>(Decoder& decoder, Name& name)
{
    std::string name_string;
    decoder >> decode_value(name_string);
    name = Name(name_string);
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
