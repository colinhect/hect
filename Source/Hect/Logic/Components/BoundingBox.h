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

#include "Hect/Logic/Component.h"
#include "Hect/Math/AxisAlignedBox.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Matrix4.h"
#include "Hect/Math/Quaternion.h"

namespace hect
{

///
/// The bounds of an entity.
class BoundingBox :
    public Component<BoundingBox>
{
public:
    BoundingBox();

    ///
    /// Returns the bounds as an axis aligned box.
    AxisAlignedBox& axisAlignedBox();

    ///
    /// Returns the bounds as an axis aligned box.
    const AxisAlignedBox& axisAlignedBox() const;

    ///
    /// Sets the bounds from an axis aligned box.
    void setAxisAlignedBox(const AxisAlignedBox& box);

    void encode(ObjectEncoder& encoder) const;
    void decode(ObjectDecoder& decoder, AssetCache& assetCache);

private:
    AxisAlignedBox _axisAlignedBox;
};

}