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
Component<T>::IterBase::IterBase() :
    _pool(nullptr),
    _id((ComponentId)-1)
{
}

template <typename T>
Component<T>::IterBase::IterBase(ComponentPool<T>* pool, ComponentId id) :
    _pool(pool),
    _id(id)
{
}

template <typename T>
void Component<T>::IterBase::_nextValidComponent()
{
    size_t maxComponentId = _pool->maxId();
    while (_id < maxComponentId)
    {
        if (_isValid())
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
void Component<T>::IterBase::_increment()
{
    ++_id;
    _nextValidComponent();
}

template <typename T>
bool Component<T>::IterBase::_isValid() const
{
    if (_pool->componentHasEntity(_id))
    {
        return true;
    }
    return false;
}

template <typename T>
void Component<T>::IterBase::_ensureValid() const
{
    if (!_isValid())
    {
        throw Error("Invalid component iterator");
    }
}

template <typename T>
bool Component<T>::IterBase::_equals(const IterBase& other) const
{
    return _pool == other._pool && _id == other._id;
}

template <typename T>
Component<T>::Iter::Iter() :
    IterBase()
{
}

template <typename T>
Component<T>::Iter::Iter(ComponentPool<T>* pool, ComponentId id) :
    IterBase(pool, id)
{
}

template <typename T>
T& Component<T>::Iter::operator*() const
{
    _ensureValid();
    return _pool->componentWithId(_id);
}

template <typename T>
T* Component<T>::Iter::operator->() const
{
    _ensureValid();
    return &_pool->componentWithId(_id);
}

template <typename T>
typename Component<T>::Iter& Component<T>::Iter::operator++()
{
    _increment();
    return *this;
}

template <typename T>
bool Component<T>::Iter::operator==(const Iter& other) const
{
    return _equals(other);
}

template <typename T>
bool Component<T>::Iter::operator!=(const Iter& other) const
{
    return !_equals(other);
}

template <typename T>
Component<T>::Iter::operator bool() const
{
    return _isValid();
}

template <typename T>
Component<T>::ConstIter::ConstIter() :
    IterBase()
{
}

template <typename T>
Component<T>::ConstIter::ConstIter(const ComponentPool<T>* pool, ComponentId id) :
    IterBase(const_cast<ComponentPool<T>*>(pool), id)
{
}

template <typename T>
const T& Component<T>::ConstIter::operator*() const
{
    _ensureValid();
    return _pool->componentWithId(_id);
}

template <typename T>
const T* Component<T>::ConstIter::operator->() const
{
    _ensureValid();
    return &_pool->componentWithId(_id);
}

template <typename T>
typename Component<T>::ConstIter& Component<T>::ConstIter::operator++()
{
    _increment();
    return *this;
}

template <typename T>
bool Component<T>::ConstIter::operator==(const ConstIter& other) const
{
    return _equals(other);
}

template <typename T>
bool Component<T>::ConstIter::operator!=(const ConstIter& other) const
{
    return !_equals(other);
}

template <typename T>
Component<T>::ConstIter::operator bool() const
{
    return _isValid();
}

template <typename T>
Component<T>::Component() :
    _pool(nullptr),
    _id((ComponentId)-1)
{
}

template <typename T>
void Component<T>::enterPool(ComponentPool<T>* pool, ComponentId id)
{
    _pool = pool;
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
ComponentPool<T>& Component<T>::pool()
{
    _ensureInPool();
    return *_pool;
}

template <typename T>
const ComponentPool<T>& Component<T>::pool() const
{
    _ensureInPool();
    return *_pool;
}

template <typename T>
Entity& Component<T>::entity()
{
    _ensureInPool();
    return _pool->entityForComponent(_id);
}

template <typename T>
const Entity& Component<T>::entity() const
{
    _ensureInPool();
    return _pool->entityForComponent(_id);
}

template <typename T>
ComponentId Component<T>::id() const
{
    return _id;
}

template <typename T>
std::type_index Component<T>::typeIndex() const
{
    return std::type_index(typeid(T));
}

template <typename T>
void Component<T>::_ensureInPool() const
{
    if (!inPool())
    {
        throw Error("The component is not in a pool");
    }
}

}