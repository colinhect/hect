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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Logic/EntityIterator.h"

namespace hect
{

///
/// An EntityEvent type.
enum EntityEventType
{
    ///
    /// An Entity was created in the Scene.
    ///
    /// This event type is triggered when an entity pending creation is
    /// created during Scene::refresh().
    EntityEventType_Create,

    ///
    /// An Entity was activated in the Scene.
    ///
    /// This event type is triggered when an entity pending activation is
    /// activated during Scene::refresh().
    EntityEventType_Activate,

    ///
    /// An Entity was destroyed in the Scene.
    ///
    /// This event type is triggered when an entity pending destruction is
    /// destroyed during Scene::refresh().
    EntityEventType_Destroy
};

///
/// An Entity related event.
class HECT_EXPORT EntityEvent
{
public:

    ///
    /// The type of event.
    EntityEventType type { EntityEventType_Create };

    ///
    /// An iterator to the Entity that the event is for.
    EntityIterator entity;
};

}