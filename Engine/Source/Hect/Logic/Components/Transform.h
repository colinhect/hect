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

#include "Hect/Core/Export.h"
#include "Hect/Logic/Scene.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Quaternion.h"

namespace hect
{

///
/// A combination of a position, scale, and rotation.
///
/// \component
class HECT_EXPORT Transform :
    public Component<Transform>
{
public:

    ///
    /// The local position.
    ///
    /// \property
    Vector3 localPosition;

    ///
    /// The local scale.
    ///
    /// \property
    Vector3 localScale { Vector3::one() };

    ///
    /// The local rotation.
    ///
    /// \property
    Quaternion localRotation;

    ///
    /// The global position.
    ///
    /// \note The global position is calculated as a function of the local
    /// position and the transform hierarchy.  This should not be directly
    /// modified unless the local position and parent transforms are known
    /// to be static.
    Vector3 globalPosition;

    ///
    /// The global scale.
    ///
    /// \note The global scale is calculated as a function of the local
    /// scale and the transform hierarchy.  This should not be directly
    /// modified unless the local scale and parent transforms are known
    /// to be static.
    Vector3 globalScale { Vector3::one() };

    ///
    /// The global rotation.
    ///
    /// \note The global rotation is calculated as a function of the local
    /// rotation and the transform hierarchy.  This should not be directly
    /// modified unless the local rotation and parent transforms are known
    /// to be static.
    Quaternion globalRotation;
};

}