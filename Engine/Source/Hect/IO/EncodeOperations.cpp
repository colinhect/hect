///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "EncodeOperations.h"

using namespace hect;

BeginArray::BeginArray()
{
}

BeginArray::BeginArray(const char* name) :
    name(name)
{
}

BeginObject::BeginObject()
{
}

BeginObject::BeginObject(const char* name) :
    name(name)
{
}

namespace hect
{

BeginArray begin_array()
{
    return BeginArray();
}

BeginArray begin_array(const char* name)
{
    return BeginArray(name);
}

EndArray end_array()
{
    return EndArray();
}

BeginObject begin_object()
{
    return BeginObject();
}

BeginObject begin_object(const char* name)
{
    return BeginObject(name);
}

EndObject end_object()
{
    return EndObject();
}

}