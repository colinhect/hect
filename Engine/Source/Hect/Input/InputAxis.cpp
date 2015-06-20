///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

using namespace hect;

InputAxis::InputAxis()
{
}

InputAxis::InputAxis(const std::string& name) :
    _name(name)
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

void InputAxis::update(Engine& engine, double timeStep)
{
    _value = 0;
    for (InputAxisBinding& binding : _bindings)
    {
        binding.update(engine, timeStep);
        _value += binding.value();
    }
    _value = std::max(-1.0, std::min(_value, 1.0));
}

double InputAxis::value() const
{
    return _value;
}

void InputAxis::encode(Encoder& encoder) const
{
    encoder << encodeValue("name", _name)
            << encodeVector("bindings", _bindings);
}

void InputAxis::decode(Decoder& decoder)
{
    decoder >> decodeValue("name", _name, true)
            >> decodeVector("bindings", _bindings);
}
