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
#include "MouseButton.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Enum.h"

using namespace hect;

template <>
MouseButton::Enum Enum::fromString<MouseButton::Enum>(const std::string& string)
{
	static std::map<std::string, MouseButton::Enum> _stringToValue;
	if (_stringToValue.empty())
	{
		_stringToValue["Left"] = hect::MouseButton::Left;
		_stringToValue["Right"] = hect::MouseButton::Right;
		_stringToValue["Middle"] = hect::MouseButton::Middle;
	}
	auto it = _stringToValue.find(string);
	if (it == _stringToValue.end())
	{
		throw Error(format("Invalid string '%s' for type 'MouseButton::Enum'", string.c_str()));
	}
	return it->second;
}

template <>
const std::string& Enum::toString<MouseButton::Enum>(MouseButton::Enum value)
{
	static std::map<MouseButton::Enum, std::string> _valueToString;
	if (_valueToString.empty())
	{
		_valueToString[hect::MouseButton::Left] = "Left";
		_valueToString[hect::MouseButton::Right] = "Right";
		_valueToString[hect::MouseButton::Middle] = "Middle";
	}
	auto it = _valueToString.find(value);
	if (it == _valueToString.end())
	{
		throw Error("Invalid value for type 'MouseButton::Enum'");
	}
	return it->second;
}