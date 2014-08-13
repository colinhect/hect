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
#include "InputAxisSourceEncoder.h"

#include "Hect/Reflection/Enum.h"

using namespace hect;

void InputAxisSourceEncoder::encode(const InputAxisSource& inputAxisSource, ObjectEncoder& encoder)
{
    inputAxisSource;
    encoder;
    throw Error("Not supported");
}

void InputAxisSourceEncoder::decode(InputAxisSource& inputAxisSource, ObjectDecoder& decoder, AssetCache& assetCache)
{
    assetCache;

    if (decoder.hasMember("type"))
    {
        inputAxisSource.type = decoder.decodeEnum<InputAxisSourceType>("type");
    }

    if (decoder.hasMember("mouseButton"))
    {
        inputAxisSource.mouseButton = decoder.decodeEnum<MouseButton>("mouseButton");
    }

    if (decoder.hasMember("key"))
    {
        inputAxisSource.key = decoder.decodeEnum<Key>("key");
    }

    if (decoder.hasMember("gamepadIndex"))
    {
        inputAxisSource.gamepadIndex = decoder.decodeUnsignedInt("gamepadIndex");
    }

    if (decoder.hasMember("gamepadAxis"))
    {
        inputAxisSource.gamepadAxis = decoder.decodeEnum<GamepadAxis>("gamepadAxis");
    }

    if (decoder.hasMember("gamepadAxisDeadZone"))
    {
        inputAxisSource.gamepadAxisDeadZone = decoder.decodeReal("gamepadAxisDeadZone");
    }

    if (decoder.hasMember("gamepadButton"))
    {
        inputAxisSource.gamepadButton = decoder.decodeEnum<GamepadButton>("gamepadButton");
    }

    if (decoder.hasMember("acceleration"))
    {
        inputAxisSource.acceleration = decoder.decodeReal("acceleration");
    }
    
    if (decoder.hasMember("range"))
    {
        inputAxisSource.range = decoder.decodeVector2("range");
    }
}