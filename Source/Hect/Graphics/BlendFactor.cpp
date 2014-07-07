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
#include "BlendFactor.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Enum.h"

using namespace hect;

template <>
BlendFactor::Enum Enum::fromString<BlendFactor::Enum>(const std::string& string)
{
	static std::map<std::string, BlendFactor::Enum> _stringToValue;
	if (_stringToValue.empty())
	{
		_stringToValue["Zero"] = hect::BlendFactor::Zero;
		_stringToValue["One"] = hect::BlendFactor::One;
		_stringToValue["SourceColor"] = hect::BlendFactor::SourceColor;
		_stringToValue["OneMinusSourceColor"] = hect::BlendFactor::OneMinusSourceColor;
		_stringToValue["DestColor"] = hect::BlendFactor::DestColor;
		_stringToValue["OneMinusDestColor"] = hect::BlendFactor::OneMinusDestColor;
		_stringToValue["SourceAlpha"] = hect::BlendFactor::SourceAlpha;
		_stringToValue["OneMinusSourceAlpha"] = hect::BlendFactor::OneMinusSourceAlpha;
		_stringToValue["DestAlpha"] = hect::BlendFactor::DestAlpha;
		_stringToValue["OneMinusDestAlpha"] = hect::BlendFactor::OneMinusDestAlpha;
	}
	auto it = _stringToValue.find(string);
	if (it == _stringToValue.end())
	{
		throw Error(format("Invalid string '%s' for type 'BlendFactor::Enum'", string.c_str()));
	}
	return it->second;
}

template <>
const std::string& Enum::toString<BlendFactor::Enum>(BlendFactor::Enum value)
{
	static std::map<BlendFactor::Enum, std::string> _valueToString;
	if (_valueToString.empty())
	{
		_valueToString[hect::BlendFactor::Zero] = "Zero";
		_valueToString[hect::BlendFactor::One] = "One";
		_valueToString[hect::BlendFactor::SourceColor] = "SourceColor";
		_valueToString[hect::BlendFactor::OneMinusSourceColor] = "OneMinusSourceColor";
		_valueToString[hect::BlendFactor::DestColor] = "DestColor";
		_valueToString[hect::BlendFactor::OneMinusDestColor] = "OneMinusDestColor";
		_valueToString[hect::BlendFactor::SourceAlpha] = "SourceAlpha";
		_valueToString[hect::BlendFactor::OneMinusSourceAlpha] = "OneMinusSourceAlpha";
		_valueToString[hect::BlendFactor::DestAlpha] = "DestAlpha";
		_valueToString[hect::BlendFactor::OneMinusDestAlpha] = "OneMinusDestAlpha";
	}
	auto it = _valueToString.find(value);
	if (it == _valueToString.end())
	{
		throw Error("Invalid value for type 'BlendFactor::Enum'");
	}
	return it->second;
}