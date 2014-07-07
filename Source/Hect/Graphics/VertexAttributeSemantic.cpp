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
#include "VertexAttributeSemantic.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Enum.h"

using namespace hect;

template <>
VertexAttributeSemantic::Enum Enum::fromString<VertexAttributeSemantic::Enum>(const std::string& string)
{
	static std::map<std::string, VertexAttributeSemantic::Enum> _stringToValue;
	if (_stringToValue.empty())
	{
		_stringToValue["Position"] = hect::VertexAttributeSemantic::Position;
		_stringToValue["Normal"] = hect::VertexAttributeSemantic::Normal;
		_stringToValue["Color"] = hect::VertexAttributeSemantic::Color;
		_stringToValue["Tangent"] = hect::VertexAttributeSemantic::Tangent;
		_stringToValue["Binormal"] = hect::VertexAttributeSemantic::Binormal;
		_stringToValue["Weight0"] = hect::VertexAttributeSemantic::Weight0;
		_stringToValue["Weight1"] = hect::VertexAttributeSemantic::Weight1;
		_stringToValue["Weight2"] = hect::VertexAttributeSemantic::Weight2;
		_stringToValue["Weight3"] = hect::VertexAttributeSemantic::Weight3;
		_stringToValue["TextureCoords0"] = hect::VertexAttributeSemantic::TextureCoords0;
		_stringToValue["TextureCoords1"] = hect::VertexAttributeSemantic::TextureCoords1;
		_stringToValue["TextureCoords2"] = hect::VertexAttributeSemantic::TextureCoords2;
		_stringToValue["TextureCoords3"] = hect::VertexAttributeSemantic::TextureCoords3;
	}
	auto it = _stringToValue.find(string);
	if (it == _stringToValue.end())
	{
		throw Error(format("Invalid string '%s' for type 'VertexAttributeSemantic::Enum'", string.c_str()));
	}
	return it->second;
}

template <>
const std::string& Enum::toString<VertexAttributeSemantic::Enum>(VertexAttributeSemantic::Enum value)
{
	static std::map<VertexAttributeSemantic::Enum, std::string> _valueToString;
	if (_valueToString.empty())
	{
		_valueToString[hect::VertexAttributeSemantic::Position] = "Position";
		_valueToString[hect::VertexAttributeSemantic::Normal] = "Normal";
		_valueToString[hect::VertexAttributeSemantic::Color] = "Color";
		_valueToString[hect::VertexAttributeSemantic::Tangent] = "Tangent";
		_valueToString[hect::VertexAttributeSemantic::Binormal] = "Binormal";
		_valueToString[hect::VertexAttributeSemantic::Weight0] = "Weight0";
		_valueToString[hect::VertexAttributeSemantic::Weight1] = "Weight1";
		_valueToString[hect::VertexAttributeSemantic::Weight2] = "Weight2";
		_valueToString[hect::VertexAttributeSemantic::Weight3] = "Weight3";
		_valueToString[hect::VertexAttributeSemantic::TextureCoords0] = "TextureCoords0";
		_valueToString[hect::VertexAttributeSemantic::TextureCoords1] = "TextureCoords1";
		_valueToString[hect::VertexAttributeSemantic::TextureCoords2] = "TextureCoords2";
		_valueToString[hect::VertexAttributeSemantic::TextureCoords3] = "TextureCoords3";
	}
	auto it = _valueToString.find(value);
	if (it == _valueToString.end())
	{
		throw Error("Invalid value for type 'VertexAttributeSemantic::Enum'");
	}
	return it->second;
}