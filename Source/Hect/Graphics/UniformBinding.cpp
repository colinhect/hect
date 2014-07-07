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
#include "UniformBinding.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Enum.h"

using namespace hect;

template <>
UniformBinding::Enum Enum::fromString<UniformBinding::Enum>(const std::string& string)
{
	static std::map<std::string, UniformBinding::Enum> _stringToValue;
	if (_stringToValue.empty())
	{
		_stringToValue["None"] = hect::UniformBinding::None;
		_stringToValue["RenderTargetSize"] = hect::UniformBinding::RenderTargetSize;
		_stringToValue["CameraPosition"] = hect::UniformBinding::CameraPosition;
		_stringToValue["CameraUp"] = hect::UniformBinding::CameraUp;
		_stringToValue["ViewMatrix"] = hect::UniformBinding::ViewMatrix;
		_stringToValue["ProjectionMatrix"] = hect::UniformBinding::ProjectionMatrix;
		_stringToValue["ViewProjectionMatrix"] = hect::UniformBinding::ViewProjectionMatrix;
		_stringToValue["ModelMatrix"] = hect::UniformBinding::ModelMatrix;
		_stringToValue["ModelViewMatrix"] = hect::UniformBinding::ModelViewMatrix;
		_stringToValue["ModelViewProjectionMatrix"] = hect::UniformBinding::ModelViewProjectionMatrix;
	}
	auto it = _stringToValue.find(string);
	if (it == _stringToValue.end())
	{
		throw Error(format("Invalid string '%s' for type 'UniformBinding::Enum'", string.c_str()));
	}
	return it->second;
}

template <>
const std::string& Enum::toString<UniformBinding::Enum>(UniformBinding::Enum value)
{
	static std::map<UniformBinding::Enum, std::string> _valueToString;
	if (_valueToString.empty())
	{
		_valueToString[hect::UniformBinding::None] = "None";
		_valueToString[hect::UniformBinding::RenderTargetSize] = "RenderTargetSize";
		_valueToString[hect::UniformBinding::CameraPosition] = "CameraPosition";
		_valueToString[hect::UniformBinding::CameraUp] = "CameraUp";
		_valueToString[hect::UniformBinding::ViewMatrix] = "ViewMatrix";
		_valueToString[hect::UniformBinding::ProjectionMatrix] = "ProjectionMatrix";
		_valueToString[hect::UniformBinding::ViewProjectionMatrix] = "ViewProjectionMatrix";
		_valueToString[hect::UniformBinding::ModelMatrix] = "ModelMatrix";
		_valueToString[hect::UniformBinding::ModelViewMatrix] = "ModelViewMatrix";
		_valueToString[hect::UniformBinding::ModelViewProjectionMatrix] = "ModelViewProjectionMatrix";
	}
	auto it = _valueToString.find(value);
	if (it == _valueToString.end())
	{
		throw Error("Invalid value for type 'UniformBinding::Enum'");
	}
	return it->second;
}