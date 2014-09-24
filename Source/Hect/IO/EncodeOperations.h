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
#pragma once

#include "Hect/Core/Uncopyable.h"

namespace hect
{

struct BeginArray :
    public Uncopyable
{
    BeginArray();
    BeginArray(const char* name);

    const char* name;
};

struct EndArray :
    public Uncopyable
{
};

struct BeginObject :
    public Uncopyable
{
    BeginObject();
    BeginObject(const char* name);

    const char* name;
};

struct EndObject :
    public Uncopyable
{
};

template <typename T>
struct EncodeMember :
    public Uncopyable
{
    EncodeMember(const T& value);
    EncodeMember(const char* name, const T& value);

    const char* name;
    const T& value;
};

BeginArray beginArray();
BeginArray beginArray(const char* name);
EndArray endArray();

BeginObject beginObject();
BeginObject beginObject(const char* name);
EndObject endObject();

template <typename T>
EncodeMember<T> member(const char* name, const T& value);

}

#include "EncodeOperations.inl"