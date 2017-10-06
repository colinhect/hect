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

template <typename ComponentType>
Component<ComponentType>::Component()
{
}

template <typename ComponentType>
Component<ComponentType>::Component(const Component<ComponentType>& component)
{
    (void)component;
}

template <typename ComponentType>
Component<ComponentType>::Component(Component<ComponentType>&& component)
{
    (void)component;
}

template <typename ComponentType>
ComponentPool<ComponentType>& Component<ComponentType>::pool()
{
    this->ensure_in_pool();
    return *this->_pool;
}

template <typename ComponentType>
const ComponentPool<ComponentType>& Component<ComponentType>::pool() const
{
    this->ensure_in_pool();
    return *this->_pool;
}

template <typename ComponentType>
Entity& Component<ComponentType>::entity()
{
    this->ensure_in_pool();

    EntityPool& entity_pool = this->_pool->_scene._entity_pool;
    const EntityId entity_id = this->_pool->component_id_to_entity_id(this->_id);
    return entity_pool.with_id(entity_id);
}

template <typename ComponentType>
const Entity& Component<ComponentType>::entity() const
{
    this->ensure_in_pool();

    EntityPool& entity_pool = this->_pool->_scene._entity_pool;
    const EntityId entity_id = this->_pool->component_id_to_entity_id(this->_id);
    return entity_pool.with_id(entity_id);
}

template <typename ComponentType>
ComponentHandle<ComponentType> Component<ComponentType>::handle() const
{
    if (!this->in_pool())
    {
        return ComponentHandle<ComponentType>();
    }

    if (!_handle)
    {
        _handle = ComponentHandle<ComponentType>(*const_cast<ComponentType*>(reinterpret_cast<const ComponentType*>(this)));
    }

    return _handle;
}

template <typename ComponentType>
ComponentIterator<ComponentType> Component<ComponentType>::iterator()
{
    this->ensure_in_pool();
    return ComponentIterator<ComponentType>(*this->_pool, this->_id);
}

template <typename ComponentType>
ComponentConstIterator<ComponentType> Component<ComponentType>::iterator() const
{
    this->ensure_in_pool();
    return ComponentConstIterator<ComponentType>(*this->_pool, this->_id);
}

template <typename ComponentType>
ComponentId Component<ComponentType>::id() const
{
    return this->_id;
}

template <typename ComponentType>
ComponentTypeId Component<ComponentType>::type_id() const
{
    return ComponentRegistry::type_id_of<ComponentType>();
}

template <typename ComponentType>
Component<ComponentType>& Component<ComponentType>::operator=(const Component& component)
{
    (void)component;
    return *this;
}

template <typename ComponentType>
Component<ComponentType>& Component<ComponentType>::operator=(Component&& component)
{
    (void)component;

    this->_pool = component._pool;
    this->_id = component._id;

    component._pool = nullptr;
    component._id = ComponentId(-1);

    return *this;
}

template <typename ComponentType>
void Component<ComponentType>::enter_pool(ComponentPool<ComponentType>& pool, ComponentId id)
{
    this->_pool = &pool;
    this->_id = id;
}

template <typename ComponentType>
void Component<ComponentType>::exit_pool()
{
    this->_pool = nullptr;
    this->_id = ComponentId(-1);

    if (this->_handle)
    {
        this->_handle.invalidate();
    }
}

template <typename ComponentType>
bool Component<ComponentType>::in_pool() const
{
    return this->_pool && this->_id != ComponentId(-1);
}

template <typename ComponentType>
void Component<ComponentType>::ensure_in_pool() const
{
    if (!this->in_pool())
    {
        throw InvalidOperation("Component is not in a pool");
    }
}

}
