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
#include "InputAxis.h"

#include <algorithm>

using namespace hect;

InputAxis::InputAxis()
{
}

InputAxis::InputAxis(Name name) :
    _name(name)
{
}

Name InputAxis::name() const
{
    return _name;
}

void InputAxis::set_name(Name name)
{
    _name = name;
}

void InputAxis::add_binding(const InputAxisBinding& binding)
{
    _bindings.push_back(binding);
}

void InputAxis::update(Platform& platform, Seconds time_step)
{
    _value = 0;
    for (InputAxisBinding& binding : _bindings)
    {
        binding.update(platform, time_step);
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
    encoder << encode_value("name", _name)
            << encode_vector("bindings", _bindings);
}

void InputAxis::decode(Decoder& decoder)
{
    decoder >> decode_value("name", _name, true)
            >> decode_vector("bindings", _bindings);
}
