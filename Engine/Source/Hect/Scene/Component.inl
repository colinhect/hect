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
namespace hect
{

template <typename T>
Component<T>::Component()
{
}

template <typename T>
Component<T>::Component(const Component<T>& component)
{
    (void)component;
}

template <typename T>
Component<T>::Component(Component<T>&& component)
{
    (void)component;
}

template <typename T>
ComponentPool<T>& Component<T>::pool()
{
    this->ensureInPool();
    return *this->_pool;
}

template <typename T>
const ComponentPool<T>& Component<T>::pool() const
{
    this->ensureInPool();
    return *this->_pool;
}

template <typename T>
EntityIterator Component<T>::entity()
{
    this->ensureInPool();

    EntityPool& entityPool = this->_pool->_scene._entityPool;
    EntityId entityId = this->_pool->componentIdToEntityId(this->_id);
    return EntityIterator(entityPool, entityId);
}

template <typename T>
EntityConstIterator Component<T>::entity() const
{
    this->ensureInPool();

    EntityPool& entityPool = this->_pool->_scene._entityPool;
    EntityId entityId = this->_pool->componentIdToEntityId(this->_id);
    return EntityConstIterator(entityPool, entityId);
}

template <typename T>
typename Component<T>::Iterator Component<T>::iterator()
{
    this->ensureInPool();
    return typename Component<T>::Iterator(*this->_pool, this->_id);
}

template <typename T>
typename Component<T>::ConstIterator Component<T>::iterator() const
{
    this->ensureInPool();
    return typename Component<T>::ConstIterator(*this->_pool, this->_id);
}

template <typename T>
ComponentId Component<T>::id() const
{
    return this->_id;
}

template <typename T>
ComponentTypeId Component<T>::typeId() const
{
    return ComponentRegistry::typeIdOf<T>();
}

template <typename T>
Component<T>& Component<T>::operator=(const Component& component)
{
    (void)component;

    this->_pool = nullptr;
    this->_id = ComponentId(-1);

    return *this;
}

template <typename T>
Component<T>& Component<T>::operator=(Component&& component)
{
    (void)component;

    this->_pool = nullptr;
    this->_id = ComponentId(-1);

    return *this;
}

template <typename T>
void Component<T>::enterPool(ComponentPool<T>& pool, ComponentId id)
{
    this->_pool = &pool;
    this->_id = id;
}

template <typename T>
void Component<T>::exitPool()
{
    this->_pool = nullptr;
    this->_id = ComponentId(-1);
}

template <typename T>
bool Component<T>::inPool() const
{
    return this->_pool && this->_id != ComponentId(-1);
}

template <typename T>
void Component<T>::ensureInPool() const
{
    if (!this->inPool())
    {
        throw InvalidOperation("Component is not in a pool");
    }
}

}