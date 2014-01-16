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
#include "Hect.h"

using namespace hect;

InputAxis InputAxisDataValueFormat::load(const std::string& name, const DataValue& dataValue)
{
    InputAxisSource source = _parseSource(dataValue["source"]);
    InputAxis axis(name, source);

    if (!dataValue["positiveMouseButton"].isNull())
    {
        MouseButton button = _parseMouseButton(dataValue["positiveMouseButton"]);
        axis.setPositiveMouseButton(button);
    }

    if (!dataValue["negativeMouseButton"].isNull())
    {
        MouseButton button = _parseMouseButton(dataValue["negativeMouseButton"]);
        axis.setNegativeMouseButton(button);
    }

    if (!dataValue["positiveKey"].isNull())
    {
        Key key = _parseKey(dataValue["positiveKey"]);
        axis.setPositiveKey(key);
    }

    if (!dataValue["negativeKey"].isNull())
    {
        Key key = _parseKey(dataValue["negativeKey"]);
        axis.setNegativeKey(key);
    }

    if (dataValue["acceleration"].isNumber())
    {
        axis.setAcceleration(dataValue["acceleration"].asDouble());
    }

    if (dataValue["gravity"].isNumber())
    {
        axis.setGravity(dataValue["gravity"].asDouble());
    }

    return axis;
}

InputAxisSource InputAxisDataValueFormat::_parseSource(const DataValue& dataValue)
{
    static std::map<std::string, InputAxisSource> sources;

    if (sources.empty())
    {
        sources["MouseMoveX"] = InputAxisSource::MouseMoveX;
        sources["MouseMoveY"] = InputAxisSource::MouseMoveY;
        sources["MouseButton"] = InputAxisSource::MouseButton;
        sources["MouseScroll"] = InputAxisSource::MouseScroll;
        sources["Key"] = InputAxisSource::Key;
    }

    auto it = sources.find(dataValue.asString());
    if (it == sources.end())
    {
        throw Error(format("Invalid input axis source '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}

Key InputAxisDataValueFormat::_parseKey(const DataValue& dataValue)
{
    static std::map<std::string, Key> keys;

    if (keys.empty())
    {
        keys["A"] = Key::A;
        keys["B"] = Key::B;
        keys["C"] = Key::C;
        keys["D"] = Key::D;
        keys["E"] = Key::E;
        keys["F"] = Key::F;
        keys["G"] = Key::G;
        keys["H"] = Key::H;
        keys["I"] = Key::I;
        keys["J"] = Key::J;
        keys["K"] = Key::K;
        keys["L"] = Key::L;
        keys["M"] = Key::M;
        keys["N"] = Key::N;
        keys["O"] = Key::O;
        keys["P"] = Key::P;
        keys["Q"] = Key::Q;
        keys["R"] = Key::R;
        keys["S"] = Key::S;
        keys["T"] = Key::T;
        keys["U"] = Key::U;
        keys["V"] = Key::V;
        keys["W"] = Key::W;
        keys["X"] = Key::X;
        keys["Y"] = Key::Y;
        keys["Z"] = Key::Z;
        keys["0"] = Key::Num0;
        keys["1"] = Key::Num1;
        keys["2"] = Key::Num2;
        keys["3"] = Key::Num3;
        keys["4"] = Key::Num4;
        keys["5"] = Key::Num5;
        keys["6"] = Key::Num6;
        keys["7"] = Key::Num7;
        keys["8"] = Key::Num8;
        keys["9"] = Key::Num9;
        keys["Esc"] = Key::Esc;
        keys["Ctrl"] = Key::Ctrl;
        keys["Shift"] = Key::Shift;
        keys["Alt"] = Key::Alt;
        keys["Space"] = Key::Space;
        keys["Enter"] = Key::Enter;
        keys["Backspace"] = Key::Backspace;
        keys["Tab"] = Key::Tab;
        keys["Tick"] = Key::Tick;
        keys["F1"] = Key::F1;
        keys["F2"] = Key::F2;
        keys["F3"] = Key::F3;
        keys["F4"] = Key::F4;
        keys["F5"] = Key::F5;
        keys["F6"] = Key::F6;
        keys["F7"] = Key::F7;
        keys["F8"] = Key::F8;
        keys["F9"] = Key::F9;
        keys["F10"] = Key::F10;
        keys["F11"] = Key::F11;
        keys["F12"] = Key::F12;
    }

    auto it = keys.find(dataValue.asString());
    if (it == keys.end())
    {
        throw Error(format("Invalid key '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}

MouseButton InputAxisDataValueFormat::_parseMouseButton(const DataValue& dataValue)
{
    static std::map<std::string, MouseButton> mouseButtons;

    if (mouseButtons.empty())
    {
        mouseButtons["Left"] = MouseButton::Left;
        mouseButtons["Right"] = MouseButton::Right;
        mouseButtons["Middle"] = MouseButton::Middle;
    }

    auto it = mouseButtons.find(dataValue.asString());
    if (it == mouseButtons.end())
    {
        throw Error(format("Invalid mouse button '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}
