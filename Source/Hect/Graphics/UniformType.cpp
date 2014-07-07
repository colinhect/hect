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
#include "UniformType.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Enum.h"

using namespace hect;

template <>
UniformType::Enum Enum::fromString<UniformType::Enum>(const std::string& string)
{
	static std::map<std::string, UniformType::Enum> _stringToValue;
	if (_stringToValue.empty())
	{
		_stringToValue["Int"] = hect::UniformType::Int;
		_stringToValue["Float"] = hect::UniformType::Float;
		_stringToValue["Vector2"] = hect::UniformType::Vector2;
		_stringToValue["Vector3"] = hect::UniformType::Vector3;
		_stringToValue["Vector4"] = hect::UniformType::Vector4;
		_stringToValue["Matrix4"] = hect::UniformType::Matrix4;
		_stringToValue["Texture"] = hect::UniformType::Texture;
	}
	auto it = _stringToValue.find(string);
	if (it == _stringToValue.end())
	{
		throw Error(format("Invalid string '%s' for type 'UniformType::Enum'", string.c_str()));
	}
	return it->second;
}

template <>
const std::string& Enum::toString<UniformType::Enum>(UniformType::Enum value)
{
	static std::map<UniformType::Enum, std::string> _valueToString;
	if (_valueToString.empty())
	{
		_valueToString[hect::UniformType::Int] = "Int";
		_valueToString[hect::UniformType::Float] = "Float";
		_valueToString[hect::UniformType::Vector2] = "Vector2";
		_valueToString[hect::UniformType::Vector3] = "Vector3";
		_valueToString[hect::UniformType::Vector4] = "Vector4";
		_valueToString[hect::UniformType::Matrix4] = "Matrix4";
		_valueToString[hect::UniformType::Texture] = "Texture";
	}
	auto it = _valueToString.find(value);
	if (it == _valueToString.end())
	{
		throw Error("Invalid value for type 'UniformType::Enum'");
	}
	return it->second;
}