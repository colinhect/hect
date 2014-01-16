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
#include "Hect.h"

using namespace hect;

enum class EntityDataBit
{
    NotSerializable = 60,
    Destroyed = 61,
    Activated = 62,
    NotNull = 63
};

bool EntityData::isActivated() const
{
    return _bitset.test((size_t)EntityDataBit::Activated);
}

void EntityData::setActivated(bool activated)
{
    _bitset.set((size_t)EntityDataBit::Activated, activated);
}

bool EntityData::isDestroyed() const
{
    return _bitset.test((size_t)EntityDataBit::Destroyed);
}

void EntityData::setDestroyed(bool destroyed)
{
    _bitset.set((size_t)EntityDataBit::Destroyed, destroyed);
}

bool EntityData::isNull() const
{
    return !_bitset.test((size_t)EntityDataBit::NotNull);
}

void EntityData::setNull(bool null)
{
    _bitset.set((size_t)EntityDataBit::NotNull, !null);
}

bool EntityData::isSerializable() const
{
    return !_bitset.test((size_t)EntityDataBit::NotSerializable);
}

void EntityData::setSerializable(bool serializable)
{
    _bitset.set((size_t)EntityDataBit::NotSerializable, !serializable);
}

bool EntityData::hasComponent(ComponentTypeId typeId) const
{
    return _bitset.test(typeId);
}

void EntityData::setHasComponent(ComponentTypeId typeId, bool value)
{
    _bitset.set(typeId, value);
}