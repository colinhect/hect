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
#include "Hect/Core/Enum.h"

using namespace hect;

#define ENUM_VALUES \
    ENUM_VALUE(UniformType, Int) \
    ENUM_VALUE(UniformType, Float) \
    ENUM_VALUE(UniformType, Vector2) \
    ENUM_VALUE(UniformType, Vector3) \
    ENUM_VALUE(UniformType, Vector4) \
    ENUM_VALUE(UniformType, Matrix4) \
    ENUM_VALUE(UniformType, Texture)

#define ENUM_VALUE(type, value) HECT_ENUM_TO_STRING(type, value)
HECT_ENUM_DEFINE_TO_STRING(UniformType, ENUM_VALUES)
#undef ENUM_VALUE

#define ENUM_VALUE(type, value) HECT_ENUM_FROM_STRING(type, value)
HECT_ENUM_DEFINE_FROM_STRING(UniformType, ENUM_VALUES)
#undef ENUM_VALUE

#undef ENUM_TYPE