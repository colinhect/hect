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
    this->ensure_in_pool();
    return *this->_pool;
}

template <typename T>
const ComponentPool<T>& Component<T>::pool() const
{
    this->ensure_in_pool();
    return *this->_pool;
}

template <typename T>
Entity& Component<T>::entity()
{
    this->ensure_in_pool();

    EntityPool& entity_pool = this->_pool->_scene._entity_pool;
    const EntityId entity_id = this->_pool->component_id_to_entity_id(this->_id);
    return entity_pool.with_id(entity_id);
}

template <typename T>
const Entity& Component<T>::entity() const
{
    this->ensure_in_pool();

    EntityPool& entity_pool = this->_pool->_scene._entity_pool;
    const EntityId entity_id = this->_pool->component_id_to_entity_id(this->_id);
    return entity_pool.with_id(entity_id);
}

template <typename T>
typename ComponentHandle<T> Component<T>::handle() const
{
    if (!this->in_pool())
    {
        return ComponentHandle<T>();
    }

    if (!_handle)
    {
        _handle = ComponentHandle<T>(*const_cast<T*>(reinterpret_cast<const T*>(this)));
    }

    return _handle;
}

template <typename T>
typename ComponentIterator<T> Component<T>::iterator()
{
    this->ensure_in_pool();
    return typename ComponentIterator<T>(*this->_pool, this->_id);
}

template <typename T>
typename ComponentConstIterator<T> Component<T>::iterator() const
{
    this->ensure_in_pool();
    return typename ComponentConstIterator<T>(*this->_pool, this->_id);
}

template <typename T>
ComponentId Component<T>::id() const
{
    return this->_id;
}

template <typename T>
ComponentTypeId Component<T>::type_id() const
{
    return ComponentRegistry::type_id_of<T>();
}

template <typename T>
Component<T>& Component<T>::operator=(const Component& component)
{
    (void)component;
    return *this;
}

template <typename T>
Component<T>& Component<T>::operator=(Component&& component)
{
    (void)component;

    this->_pool = component._pool;
    this->_id = component._id;

    component._pool = nullptr;
    component._id = ComponentId(-1);

    return *this;
}

template <typename T>
void Component<T>::enter_pool(ComponentPool<T>& pool, ComponentId id)
{
    this->_pool = &pool;
    this->_id = id;
}

template <typename T>
void Component<T>::exit_pool()
{
    this->_pool = nullptr;
    this->_id = ComponentId(-1);

    if (this->_handle)
    {
        this->_handle.invalidate();
    }
}

template <typename T>
bool Component<T>::in_pool() const
{
    return this->_pool && this->_id != ComponentId(-1);
}

template <typename T>
void Component<T>::ensure_in_pool() const
{
    if (!this->in_pool())
    {
        throw InvalidOperation("Component is not in a pool");
    }
}

}
