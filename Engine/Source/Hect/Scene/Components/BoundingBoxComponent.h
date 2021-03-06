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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Math/AxisAlignedBox.h"
#include "Hect/Scene/Scene.h"

namespace hect
{

///
/// The spacial extents of an Entity.
///
/// \component
class HECT_EXPORT BoundingBoxComponent :
    public Component<BoundingBoxComponent>
{
public:

    ///
    /// Whether the bounding box adapts to the bounds of the components of its
    /// entity.
    ///
    /// \property
    bool adaptive { true };

    ///
    /// The local extents of the bounding box.
    ///
    /// \property
    AxisAlignedBox local_extents;

    ///
    /// The global extents of the bounding box.
    ///
    /// \warning The global extents are calculated as a function of the local
    /// extents and the transform hierarchy.  This should not be directly.
    AxisAlignedBox globalExtents;
};

}
