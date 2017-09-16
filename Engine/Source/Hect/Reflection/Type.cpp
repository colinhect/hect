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
#include "Type.h"

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Exception.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encoder.h"
#include "Hect/Reflection/Enum.h"

using namespace hect;

Type::Type()
{
}

Kind Type::kind() const
{
    return _kind;
}

Name Type::name() const
{
    return _name;
}

Enum& Type::as_enum()
{
    if (!_enum)
    {
        throw InvalidOperation("Type is not an enum");
    }
    return *_enum;
}

const Enum& Type::as_enum() const
{
    if (!_enum)
    {
        throw InvalidOperation("Type is not an enum");
    }
    return *_enum;
}

void Type::set_encode_function(EncodeFunction function)
{
    _encode_function = function;
}

void Type::encode(const void* value, Encoder& encoder) const
{
    _encode_function(value, encoder);
}

void Type::set_decode_function(DecodeFunction function)
{
    _decode_function = function;
}

void Type::decode(void* value, Decoder& decoder) const
{
    _decode_function(value, decoder);
}

Type::Type(Kind kind, Name name) :
    _kind(kind),
    _name(name),
    _enum(new Enum(name)),
    _encode_function([](const void*, Encoder&) { }),
    _decode_function([](void*, Decoder&) { })
{
}

const Type& Type::from_type_info(const std::type_info& type_info)
{
    std::type_index type_index(type_info);

    auto it = _registered_types.find(type_index);
    if (it != _registered_types.end())
    {
        return it->second;
    }
    else
    {
        throw InvalidOperation("Unknown type");
    }
}

std::map<std::type_index, Type> Type::_registered_types;
