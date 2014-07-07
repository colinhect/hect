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
#include "VertexAttributeType.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Enum.h"

using namespace hect;

template <>
VertexAttributeType::Enum Enum::fromString<VertexAttributeType::Enum>(const std::string& string)
{
	static std::map<std::string, VertexAttributeType::Enum> _stringToValue;
	if (_stringToValue.empty())
	{
		_stringToValue["Byte"] = hect::VertexAttributeType::Byte;
		_stringToValue["UnsignedByte"] = hect::VertexAttributeType::UnsignedByte;
		_stringToValue["Short"] = hect::VertexAttributeType::Short;
		_stringToValue["UnsignedShort"] = hect::VertexAttributeType::UnsignedShort;
		_stringToValue["Int"] = hect::VertexAttributeType::Int;
		_stringToValue["UnsignedInt"] = hect::VertexAttributeType::UnsignedInt;
		_stringToValue["Half"] = hect::VertexAttributeType::Half;
		_stringToValue["Float"] = hect::VertexAttributeType::Float;
	}
	auto it = _stringToValue.find(string);
	if (it == _stringToValue.end())
	{
		throw Error(format("Invalid string '%s' for type 'VertexAttributeType::Enum'", string.c_str()));
	}
	return it->second;
}

template <>
const std::string& Enum::toString<VertexAttributeType::Enum>(VertexAttributeType::Enum value)
{
	static std::map<VertexAttributeType::Enum, std::string> _valueToString;
	if (_valueToString.empty())
	{
		_valueToString[hect::VertexAttributeType::Byte] = "Byte";
		_valueToString[hect::VertexAttributeType::UnsignedByte] = "UnsignedByte";
		_valueToString[hect::VertexAttributeType::Short] = "Short";
		_valueToString[hect::VertexAttributeType::UnsignedShort] = "UnsignedShort";
		_valueToString[hect::VertexAttributeType::Int] = "Int";
		_valueToString[hect::VertexAttributeType::UnsignedInt] = "UnsignedInt";
		_valueToString[hect::VertexAttributeType::Half] = "Half";
		_valueToString[hect::VertexAttributeType::Float] = "Float";
	}
	auto it = _valueToString.find(value);
	if (it == _valueToString.end())
	{
		throw Error("Invalid value for type 'VertexAttributeType::Enum'");
	}
	return it->second;
}