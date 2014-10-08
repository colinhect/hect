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
#include "InputAxis.h"

#include <algorithm>

#include "Hect/Math/Utilities.h"

using namespace hect;

InputAxis::InputAxis() :
    _value(0)
{
}

InputAxis::InputAxis(const std::string& name) :
    _name(name),
    _value(0)
{
}

const std::string& InputAxis::name() const
{
    return _name;
}

void InputAxis::setName(const std::string& name)
{
    _name = name;
}

void InputAxis::addBinding(const InputAxisBinding& binding)
{
    _bindings.push_back(binding);
}

void InputAxis::update(Real timeStepInSeconds)
{
    _value = 0;
    for (InputAxisBinding& binding : _bindings)
    {
        binding.update(timeStepInSeconds);
        _value += binding.value();
    }
    _value = clamp<Real>(_value, -1, 1);
}

Real InputAxis::value() const
{
    return _value;
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const InputAxis& inputAxis)
{
    encoder << encodeValue("name", inputAxis._name);

    encoder << beginArray("bindings");
    for (const InputAxisBinding& binding : inputAxis._bindings)
    {
        encoder << beginObject()
                << binding
                << endObject();
    }
    return encoder << endArray();
}

Decoder& operator>>(Decoder& decoder, InputAxis& inputAxis)
{
    decoder >> decodeValue("name", inputAxis._name, true);
    decoder >> beginArray("bindings");
    while (decoder.hasMoreElements())
    {
        InputAxisBinding binding;
        decoder >> beginObject()
                >> binding
                >> endObject();
        inputAxis._bindings.push_back(binding);
    }
    return decoder >> endArray();
}

}