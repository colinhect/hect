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

#include "Hect/Reflection/Type.h"

using namespace hect;

void InputAxisEncoder::encode(const InputAxis& inputAxis, ObjectEncoder& encoder)
{
    inputAxis;
    encoder;
    throw Error("Cannot encode an input axis");
}

void InputAxisEncoder::decode(InputAxis& inputAxis, ObjectDecoder& decoder)
{
    if (decoder.hasMember("source"))
    {
        std::string value = decoder.decodeString("source");
        inputAxis.setSource(Enum::fromString<InputAxisSource::Enum>(value));
    }

    if (decoder.hasMember("positiveMouseButton"))
    {
        std::string value = decoder.decodeString("positiveMouseButton");
        inputAxis.setPositiveMouseButton(Enum::fromString<MouseButton::Enum>(value));
    }

    if (decoder.hasMember("negativeMouseButton"))
    {
        std::string value = decoder.decodeString("negativeMouseButton");
        inputAxis.setNegativeMouseButton(Enum::fromString<MouseButton::Enum>(value));
    }

    if (decoder.hasMember("positiveKey"))
    {
        std::string value = decoder.decodeString("positiveKey");
        inputAxis.setPositiveKey(Enum::fromString<Key::Enum>(value));
    }

    if (decoder.hasMember("negativeKey"))
    {
        std::string value = decoder.decodeString("negativeKey");
        inputAxis.setNegativeKey(Enum::fromString<Key::Enum>(value));
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