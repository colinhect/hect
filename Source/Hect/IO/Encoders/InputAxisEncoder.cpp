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
#include "InputAxisEncoder.h"

#include "Hect/Core/Enum.h"

using namespace hect;

void InputAxisEncoder::encode(const InputAxis& inputAxis, ObjectEncoder& encoder)
{
    inputAxis;
    encoder;
    throw Error("Cannot encode an input axis");
}

void InputAxisEncoder::decode(InputAxis& inputAxis, ObjectDecoder& decoder)
{
    if (decoder.hasMember("name"))
    {
        std::string value = decoder.decodeString("name");
        inputAxis.setName(value);
    }

    if (decoder.hasMember("source"))
    {
        auto source = decoder.decodeEnum<InputAxisSource>("source");
        inputAxis.setSource(source);
    }

    if (decoder.hasMember("positiveMouseButton"))
    {
        auto positiveMouseButton = decoder.decodeEnum<MouseButton>("positiveMouseButton");
        inputAxis.setPositiveMouseButton(positiveMouseButton);
    }

    if (decoder.hasMember("negativeMouseButton"))
    {
        auto negativeMouseButton = decoder.decodeEnum<MouseButton>("negativeMouseButton");
        inputAxis.setNegativeMouseButton(negativeMouseButton);
    }

    if (decoder.hasMember("positiveKey"))
    {
        auto positiveKey = decoder.decodeEnum<Key>("positiveKey");
        inputAxis.setPositiveKey(positiveKey);
    }

    if (decoder.hasMember("negativeKey"))
    {
        auto negativeKey = decoder.decodeEnum<Key>("negativeKey");
        inputAxis.setNegativeKey(negativeKey);
    }

    if (decoder.hasMember("acceleration"))
    {
        inputAxis.setAcceleration(decoder.decodeReal("acceleration"));
    }

    if (decoder.hasMember("gravity"))
    {
        inputAxis.setGravity(decoder.decodeReal("gravity"));
    }
}