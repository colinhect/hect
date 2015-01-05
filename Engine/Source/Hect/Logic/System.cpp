///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "System.h"

using namespace hect;

System::System(Scene& scene, SystemTickStage tickStage) :
    _scene(scene),
    _tickStage(tickStage)
{
}

Scene& System::scene()
{
    return _scene;
}

const Scene& System::scene() const
{
    return _scene;
}

void System::tick(Real timeStep)
{
    (void)timeStep;
}

SystemTickStage System::tickStage() const
{
    return _tickStage;
}

void System::encode(Encoder& encoder) const
{
    // Default to the encoding registered with the reflected type
    const Type& type = Type::of(*this);
    type.encode(this, encoder);
}

void System::decode(Decoder& decoder)
{
    // Default to the decoding registered with the reflected type
    const Type& type = Type::of(*this);
    type.decode(this, decoder);
}