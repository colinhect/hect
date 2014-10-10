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
#include "UniformValueInstance.h"

using namespace hect;

UniformValueInstance::UniformValueInstance()
{
}

UniformValueInstance::UniformValueInstance(const std::string& name, const UniformValue& value) :
    _name(name),
    _value(value)
{
}

const std::string& UniformValueInstance::name() const
{
    return _name;
}

const UniformValue& UniformValueInstance::value() const
{
    return _value;
}

bool UniformValueInstance::operator==(const UniformValueInstance& uniformValueInstance) const
{
    return _name == uniformValueInstance.name()
           && _value == uniformValueInstance.value();
}

bool UniformValueInstance::operator!=(const UniformValueInstance& uniformValueInstance) const
{
    return !(*this == uniformValueInstance);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const UniformValueInstance& uniformValueInstance)
{
    return encoder << beginObject()
        << encodeValue("name", uniformValueInstance._name)
        << encodeValue(uniformValueInstance._value)
        << endObject();
}

Decoder& operator>>(Decoder& decoder, UniformValueInstance& uniformValueInstance)
{
    return decoder >> beginObject()
        >> decodeValue("name", uniformValueInstance._name)
        >> decodeValue(uniformValueInstance._value)
        >> endObject();
}

}