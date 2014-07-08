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

#define ENUM_TYPE Key

#define ENUM_VALUES \
    ENUM_VALUE(Unknown) \
    ENUM_VALUE(A) \
    ENUM_VALUE(B) \
    ENUM_VALUE(C) \
    ENUM_VALUE(D) \
    ENUM_VALUE(E) \
    ENUM_VALUE(F) \
    ENUM_VALUE(G) \
    ENUM_VALUE(H) \
    ENUM_VALUE(I) \
    ENUM_VALUE(J) \
    ENUM_VALUE(K) \
    ENUM_VALUE(L) \
    ENUM_VALUE(M) \
    ENUM_VALUE(N) \
    ENUM_VALUE(O) \
    ENUM_VALUE(P) \
    ENUM_VALUE(Q) \
    ENUM_VALUE(R) \
    ENUM_VALUE(S) \
    ENUM_VALUE(T) \
    ENUM_VALUE(U) \
    ENUM_VALUE(V) \
    ENUM_VALUE(W) \
    ENUM_VALUE(X) \
    ENUM_VALUE(Y) \
    ENUM_VALUE(Z) \
    ENUM_VALUE(Num0) \
    ENUM_VALUE(Num1) \
    ENUM_VALUE(Num2) \
    ENUM_VALUE(Num3) \
    ENUM_VALUE(Num4) \
    ENUM_VALUE(Num5) \
    ENUM_VALUE(Num6) \
    ENUM_VALUE(Num7) \
    ENUM_VALUE(Num8) \
    ENUM_VALUE(Num9) \
    ENUM_VALUE(Esc) \
    ENUM_VALUE(Space) \
    ENUM_VALUE(Enter) \
    ENUM_VALUE(Backspace) \
    ENUM_VALUE(Tab) \
    ENUM_VALUE(Tick) \
    ENUM_VALUE(F1) \
    ENUM_VALUE(F2) \
    ENUM_VALUE(F3) \
    ENUM_VALUE(F4) \
    ENUM_VALUE(F5) \
    ENUM_VALUE(F6) \
    ENUM_VALUE(F7) \
    ENUM_VALUE(F8) \
    ENUM_VALUE(F9) \
    ENUM_VALUE(F10) \
    ENUM_VALUE(F11) \
    ENUM_VALUE(F12) \
    ENUM_VALUE(LeftCtrl) \
    ENUM_VALUE(LeftShift) \
    ENUM_VALUE(LeftAlt)

#define ENUM_VALUE(value) HECT_ENUM_TO_STRING(value)
HECT_ENUM_DEFINE_TO_STRING(ENUM_VALUES)
#undef ENUM_VALUE

#define ENUM_VALUE(value) HECT_ENUM_FROM_STRING(value)
HECT_ENUM_DEFINE_FROM_STRING(ENUM_VALUES)
#undef ENUM_VALUE

#undef ENUM_TYPE