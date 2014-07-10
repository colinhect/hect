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
#include "Hect/Core/Enum.h"

using namespace hect;

#define ENUM_VALUES \
    ENUM_VALUE(Key, Unknown) \
    ENUM_VALUE(Key, A) \
    ENUM_VALUE(Key, B) \
    ENUM_VALUE(Key, C) \
    ENUM_VALUE(Key, D) \
    ENUM_VALUE(Key, E) \
    ENUM_VALUE(Key, F) \
    ENUM_VALUE(Key, G) \
    ENUM_VALUE(Key, H) \
    ENUM_VALUE(Key, I) \
    ENUM_VALUE(Key, J) \
    ENUM_VALUE(Key, K) \
    ENUM_VALUE(Key, L) \
    ENUM_VALUE(Key, M) \
    ENUM_VALUE(Key, N) \
    ENUM_VALUE(Key, O) \
    ENUM_VALUE(Key, P) \
    ENUM_VALUE(Key, Q) \
    ENUM_VALUE(Key, R) \
    ENUM_VALUE(Key, S) \
    ENUM_VALUE(Key, T) \
    ENUM_VALUE(Key, U) \
    ENUM_VALUE(Key, V) \
    ENUM_VALUE(Key, W) \
    ENUM_VALUE(Key, X) \
    ENUM_VALUE(Key, Y) \
    ENUM_VALUE(Key, Z) \
    ENUM_VALUE(Key, Num0) \
    ENUM_VALUE(Key, Num1) \
    ENUM_VALUE(Key, Num2) \
    ENUM_VALUE(Key, Num3) \
    ENUM_VALUE(Key, Num4) \
    ENUM_VALUE(Key, Num5) \
    ENUM_VALUE(Key, Num6) \
    ENUM_VALUE(Key, Num7) \
    ENUM_VALUE(Key, Num8) \
    ENUM_VALUE(Key, Num9) \
    ENUM_VALUE(Key, Esc) \
    ENUM_VALUE(Key, Space) \
    ENUM_VALUE(Key, Enter) \
    ENUM_VALUE(Key, Backspace) \
    ENUM_VALUE(Key, Tab) \
    ENUM_VALUE(Key, Tick) \
    ENUM_VALUE(Key, F1) \
    ENUM_VALUE(Key, F2) \
    ENUM_VALUE(Key, F3) \
    ENUM_VALUE(Key, F4) \
    ENUM_VALUE(Key, F5) \
    ENUM_VALUE(Key, F6) \
    ENUM_VALUE(Key, F7) \
    ENUM_VALUE(Key, F8) \
    ENUM_VALUE(Key, F9) \
    ENUM_VALUE(Key, F10) \
    ENUM_VALUE(Key, F11) \
    ENUM_VALUE(Key, F12) \
    ENUM_VALUE(Key, LeftCtrl) \
    ENUM_VALUE(Key, LeftShift) \
    ENUM_VALUE(Key, LeftAlt)

#define ENUM_VALUE(type, value) HECT_ENUM_TO_STRING(type, value)
HECT_ENUM_DEFINE_TO_STRING(Key, ENUM_VALUES)
#undef ENUM_VALUE

#define ENUM_VALUE(type, value) HECT_ENUM_FROM_STRING(type, value)
HECT_ENUM_DEFINE_FROM_STRING(Key, ENUM_VALUES)
#undef ENUM_VALUE