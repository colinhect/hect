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
#include "Key.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Enum.h"

using namespace hect;

template <>
Key::Enum Enum::fromString<Key::Enum>(const std::string& string)
{
	static std::map<std::string, Key::Enum> _stringToValue;
	if (_stringToValue.empty())
	{
		_stringToValue["Unknown"] = hect::Key::Unknown;
		_stringToValue["A"] = hect::Key::A;
		_stringToValue["B"] = hect::Key::B;
		_stringToValue["C"] = hect::Key::C;
		_stringToValue["D"] = hect::Key::D;
		_stringToValue["E"] = hect::Key::E;
		_stringToValue["F"] = hect::Key::F;
		_stringToValue["G"] = hect::Key::G;
		_stringToValue["H"] = hect::Key::H;
		_stringToValue["I"] = hect::Key::I;
		_stringToValue["J"] = hect::Key::J;
		_stringToValue["K"] = hect::Key::K;
		_stringToValue["L"] = hect::Key::L;
		_stringToValue["M"] = hect::Key::M;
		_stringToValue["N"] = hect::Key::N;
		_stringToValue["O"] = hect::Key::O;
		_stringToValue["P"] = hect::Key::P;
		_stringToValue["Q"] = hect::Key::Q;
		_stringToValue["R"] = hect::Key::R;
		_stringToValue["S"] = hect::Key::S;
		_stringToValue["T"] = hect::Key::T;
		_stringToValue["U"] = hect::Key::U;
		_stringToValue["V"] = hect::Key::V;
		_stringToValue["W"] = hect::Key::W;
		_stringToValue["X"] = hect::Key::X;
		_stringToValue["Y"] = hect::Key::Y;
		_stringToValue["Z"] = hect::Key::Z;
		_stringToValue["Num0"] = hect::Key::Num0;
		_stringToValue["Num1"] = hect::Key::Num1;
		_stringToValue["Num2"] = hect::Key::Num2;
		_stringToValue["Num3"] = hect::Key::Num3;
		_stringToValue["Num4"] = hect::Key::Num4;
		_stringToValue["Num5"] = hect::Key::Num5;
		_stringToValue["Num6"] = hect::Key::Num6;
		_stringToValue["Num7"] = hect::Key::Num7;
		_stringToValue["Num8"] = hect::Key::Num8;
		_stringToValue["Num9"] = hect::Key::Num9;
		_stringToValue["Esc"] = hect::Key::Esc;
		_stringToValue["Space"] = hect::Key::Space;
		_stringToValue["Enter"] = hect::Key::Enter;
		_stringToValue["Backspace"] = hect::Key::Backspace;
		_stringToValue["Tab"] = hect::Key::Tab;
		_stringToValue["Tick"] = hect::Key::Tick;
		_stringToValue["F1"] = hect::Key::F1;
		_stringToValue["F2"] = hect::Key::F2;
		_stringToValue["F3"] = hect::Key::F3;
		_stringToValue["F4"] = hect::Key::F4;
		_stringToValue["F5"] = hect::Key::F5;
		_stringToValue["F6"] = hect::Key::F6;
		_stringToValue["F7"] = hect::Key::F7;
		_stringToValue["F8"] = hect::Key::F8;
		_stringToValue["F9"] = hect::Key::F9;
		_stringToValue["F10"] = hect::Key::F10;
		_stringToValue["F11"] = hect::Key::F11;
		_stringToValue["F12"] = hect::Key::F12;
		_stringToValue["LeftCtrl"] = hect::Key::LeftCtrl;
		_stringToValue["LeftShift"] = hect::Key::LeftShift;
		_stringToValue["LeftAlt"] = hect::Key::LeftAlt;
	}
	auto it = _stringToValue.find(string);
	if (it == _stringToValue.end())
	{
		throw Error(format("Invalid string '%s' for type 'Key::Enum'", string.c_str()));
	}
	return it->second;
}

template <>
const std::string& Enum::toString<Key::Enum>(Key::Enum value)
{
	static std::map<Key::Enum, std::string> _valueToString;
	if (_valueToString.empty())
	{
		_valueToString[hect::Key::Unknown] = "Unknown";
		_valueToString[hect::Key::A] = "A";
		_valueToString[hect::Key::B] = "B";
		_valueToString[hect::Key::C] = "C";
		_valueToString[hect::Key::D] = "D";
		_valueToString[hect::Key::E] = "E";
		_valueToString[hect::Key::F] = "F";
		_valueToString[hect::Key::G] = "G";
		_valueToString[hect::Key::H] = "H";
		_valueToString[hect::Key::I] = "I";
		_valueToString[hect::Key::J] = "J";
		_valueToString[hect::Key::K] = "K";
		_valueToString[hect::Key::L] = "L";
		_valueToString[hect::Key::M] = "M";
		_valueToString[hect::Key::N] = "N";
		_valueToString[hect::Key::O] = "O";
		_valueToString[hect::Key::P] = "P";
		_valueToString[hect::Key::Q] = "Q";
		_valueToString[hect::Key::R] = "R";
		_valueToString[hect::Key::S] = "S";
		_valueToString[hect::Key::T] = "T";
		_valueToString[hect::Key::U] = "U";
		_valueToString[hect::Key::V] = "V";
		_valueToString[hect::Key::W] = "W";
		_valueToString[hect::Key::X] = "X";
		_valueToString[hect::Key::Y] = "Y";
		_valueToString[hect::Key::Z] = "Z";
		_valueToString[hect::Key::Num0] = "Num0";
		_valueToString[hect::Key::Num1] = "Num1";
		_valueToString[hect::Key::Num2] = "Num2";
		_valueToString[hect::Key::Num3] = "Num3";
		_valueToString[hect::Key::Num4] = "Num4";
		_valueToString[hect::Key::Num5] = "Num5";
		_valueToString[hect::Key::Num6] = "Num6";
		_valueToString[hect::Key::Num7] = "Num7";
		_valueToString[hect::Key::Num8] = "Num8";
		_valueToString[hect::Key::Num9] = "Num9";
		_valueToString[hect::Key::Esc] = "Esc";
		_valueToString[hect::Key::Space] = "Space";
		_valueToString[hect::Key::Enter] = "Enter";
		_valueToString[hect::Key::Backspace] = "Backspace";
		_valueToString[hect::Key::Tab] = "Tab";
		_valueToString[hect::Key::Tick] = "Tick";
		_valueToString[hect::Key::F1] = "F1";
		_valueToString[hect::Key::F2] = "F2";
		_valueToString[hect::Key::F3] = "F3";
		_valueToString[hect::Key::F4] = "F4";
		_valueToString[hect::Key::F5] = "F5";
		_valueToString[hect::Key::F6] = "F6";
		_valueToString[hect::Key::F7] = "F7";
		_valueToString[hect::Key::F8] = "F8";
		_valueToString[hect::Key::F9] = "F9";
		_valueToString[hect::Key::F10] = "F10";
		_valueToString[hect::Key::F11] = "F11";
		_valueToString[hect::Key::F12] = "F12";
		_valueToString[hect::Key::LeftCtrl] = "LeftCtrl";
		_valueToString[hect::Key::LeftShift] = "LeftShift";
		_valueToString[hect::Key::LeftAlt] = "LeftAlt";
	}
	auto it = _valueToString.find(value);
	if (it == _valueToString.end())
	{
		throw Error("Invalid value for type 'Key::Enum'");
	}
	return it->second;
}