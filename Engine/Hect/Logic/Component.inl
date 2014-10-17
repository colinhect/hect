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
namespace hect
{
    
template <typename T>
Component<T>::IteratorBase::IteratorBase() :
    _pool(nullptr),
    _id((ComponentId)-1)
{
}

template <typename T>
Component<T>::IteratorBase::IteratorBase(ComponentPool<T>& pool, ComponentId id) :
    _pool(&pool),
    _id(id)
{
}

template <typename T>
void Component<T>::IteratorBase::increment()
{
    ++_id;

    size_t maxId = _pool->maxId();
    while (_id < maxId)
    {
        if (isValid())
        {
            const Entity& entity = _pool->entityForComponent(_id);
            if (entity.isActivated())
            {
                break;
            }
        }

        ++_id;
    }
}

template <typename T>
bool Component<T>::IteratorBase::isValid() const
{
    if (_pool->componentHasEntity(_id))
    {
        return true;
    }
    return false;
}

template <typename T>
void Component<T>::IteratorBase::ensureValid() const
{
    if (!isValid())
    {
        throw Error("Invalid component iterator");
    }
}

template <typename T>
bool Component<T>::IteratorBase::equals(const IteratorBase& other) const
{
    return _pool == other._pool && _id == other._id;
}

template <typename T>
Component<T>::Iterator::Iterator() :
    IteratorBase()
{
}

template <typename T>
Component<T>::Iterator::Iterator(ComponentPool<T>& pool, ComponentId id) :
    IteratorBase(pool, id)
{
}

template <typename T>
T& Component<T>::Iterator::operator*() const
{
    ensureValid();
    return _pool->componentWithId(_id);
}

template <typename T>
T* Component<T>::Iterator::operator->() const
{
    ensureValid();
    return &_pool->componentWithId(_id);
}

template <typename T>
typename Component<T>::Iterator& Component<T>::Iterator::operator++()
{
    increment();
    return *this;
}

template <typename T>
bool Component<T>::Iterator::operator==(const Iterator& other) const
{
    return equals(other);
}

template <typename T>
bool Component<T>::Iterator::operator!=(const Iterator& other) const
{
    return !equals(other);
}

template <typename T>
Component<T>::Iterator::operator bool() const
{
    return isValid();
}

template <typename T>
Component<T>::ConstIterator::ConstIterator() :
    IteratorBase()
{
}

template <typename T>
Component<T>::ConstIterator::ConstIterator(const ComponentPool<T>& pool, ComponentId id) :
    IteratorBase(*const_cast<ComponentPool<T>*>(&pool), id)
{
}

template <typename T>
const T& Component<T>::ConstIterator::operator*() const
{
    ensureValid();
    return _pool->componentWithId(_id);
}

template <typename T>
const T* Component<T>::ConstIterator::operator->() const
{
    ensureValid();
    return &_pool->componentWithId(_id);
}

template <typename T>
typename Component<T>::ConstIterator& Component<T>::ConstIterator::operator++()
{
    increment();
    return *this;
}

template <typename T>
bool Component<T>::ConstIterator::operator==(const ConstIterator& other) const
{
    return equals(other);
}

template <typename T>
bool Component<T>::ConstIterator::operator!=(const ConstIterator& other) const
{
    return !equals(other);
}

template <typename T>
Component<T>::ConstIterator::operator bool() const
{
    return isValid();
}

template <typename T>
Component<T>::Component() :
    _pool(nullptr),
    _id((ComponentId)-1)
{
}

template <typename T>
Component<T>::Component(const Component<T>& component) :
    _pool(nullptr),
    _id((ComponentId)-1)
{
    component;
}

template <typename T>
Component<T>::Component(Component<T>&& component) :
    _pool(nullptr),
    _id((ComponentId)-1)
{
    component;
}

template <typename T>
ComponentPool<T>& Component<T>::pool()
{
    ensureInPool();
    return *_pool;
}

template <typename T>
const ComponentPool<T>& Component<T>::pool() const
{
    ensureInPool();
    return *_pool;
}

template <typename T>
Entity& Component<T>::entity()
{
    ensureInPool();
    return _pool->entityForComponent(_id);
}

template <typename T>
const Entity& Component<T>::entity() const
{
    ensureInPool();
    return _pool->entityForComponent(_id);
}

template <typename T>
ComponentId Component<T>::id() const
{
    return _id;
}

template <typename T>
ComponentTypeId Component<T>::typeId() const
{
    return ComponentRegistry::typeIdOf<T>();
}

template <typename T>
Component<T>& Component<T>::operator=(const Component& component)
{
    component;

    _pool = nullptr;
    _id = (ComponentId)-1;

    return *this;
}

template <typename T>
Component<T>& Component<T>::operator=(Component&& component)
{
    component;

    _pool = nullptr;
    _id = (ComponentId)-1;

    return *this;
}

template <typename T>
void Component<T>::enterPool(ComponentPool<T>& pool, ComponentId id)
{
    _pool = &pool;
    _id = id;
}

template <typename T>
void Component<T>::exitPool()
{
    _pool = nullptr;
    _id = (ComponentId)-1;
}

template <typename T>
bool Component<T>::inPool() const
{
    return _pool && _id != (ComponentId)-1;
}

template <typename T>
void Component<T>::ensureInPool() const
{
    if (!inPool())
    {
        throw Error("Component is not in a pool");
    }
}

}