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
ComponentIteratorBase<ComponentType>::ComponentIteratorBase()
{
}

template <typename ComponentType>
ComponentIteratorBase<ComponentType>::ComponentIteratorBase(ComponentPool<ComponentType>& pool, ComponentId id) :
    _pool(&pool),
    _id(id)
{
}

template <typename ComponentType>
void ComponentIteratorBase<ComponentType>::invalidate()
{
    _pool = nullptr;
    _id = ComponentId(-1);
}

template <typename ComponentType>
ComponentType& ComponentIteratorBase<ComponentType>::dereference() const
{
    if (!this->is_valid())
    {
        throw InvalidOperation("Invalid component iterator");
    }

    return this->_pool->with_id(this->_id);
}

template <typename ComponentType>
void ComponentIteratorBase<ComponentType>::increment()
{
    ++this->_id;

    size_t max_id = this->_pool->max_id();
    while (this->_id < max_id)
    {
        if (this->is_valid())
        {
            const Entity& entity = this->_pool->entity_for_component(this->_id);
            if (entity.is_activated())
            {
                break;
            }
        }

        ++this->_id;
    }
}

template <typename ComponentType>
bool ComponentIteratorBase<ComponentType>::is_valid() const
{
    if (this->_pool && this->_pool->component_has_entity(this->_id))
    {
        return true;
    }
    return false;
}

template <typename ComponentType>
bool ComponentIteratorBase<ComponentType>::equals(const ComponentIteratorBase<ComponentType>& other) const
{
    return this->_pool == other._pool && this->_id == other._id;
}

template <typename ComponentType>
ComponentIterator<ComponentType>::ComponentIterator() :
    ComponentIteratorBase<ComponentType>()
{
}

template <typename ComponentType>
ComponentIterator<ComponentType>::ComponentIterator(ComponentPool<ComponentType>& pool, ComponentId id) :
    ComponentIteratorBase<ComponentType>(pool, id)
{
}

template <typename ComponentType>
ComponentType& ComponentIterator<ComponentType>::operator*() const
{
    return this->dereference();
}

template <typename ComponentType>
ComponentType* ComponentIterator<ComponentType>::operator->() const
{
    return &this->dereference();
}

template <typename ComponentType>
ComponentIterator<ComponentType>& ComponentIterator<ComponentType>::operator++()
{
    this->increment();
    return *this;
}

template <typename ComponentType>
bool ComponentIterator<ComponentType>::operator==(const ComponentIterator<ComponentType>& other) const
{
    return this->equals(other);
}

template <typename ComponentType>
bool ComponentIterator<ComponentType>::operator!=(const ComponentIterator<ComponentType>& other) const
{
    return !this->equals(other);
}

template <typename ComponentType>
ComponentIterator<ComponentType>::operator bool() const
{
    return this->is_valid();
}

template <typename ComponentType>
ComponentConstIterator<ComponentType>::ComponentConstIterator() :
    ComponentIteratorBase<ComponentType>()
{
}

template <typename ComponentType>
ComponentConstIterator<ComponentType>::ComponentConstIterator(const ComponentPool<ComponentType>& pool, ComponentId id) :
    ComponentIteratorBase<ComponentType>(*const_cast<ComponentPool<ComponentType>*>(&pool), id)
{
}

template <typename ComponentType>
ComponentConstIterator<ComponentType>::ComponentConstIterator(const ComponentIterator<ComponentType>& iterator) :
    ComponentIteratorBase<ComponentType>(*iterator._pool, iterator._id)
{
}

template <typename ComponentType>
const ComponentType& ComponentConstIterator<ComponentType>::operator*() const
{
    return this->dereference();
}

template <typename ComponentType>
const ComponentType* ComponentConstIterator<ComponentType>::operator->() const
{
    return &this->dereference();
}

template <typename ComponentType>
ComponentConstIterator<ComponentType>& ComponentConstIterator<ComponentType>::operator++()
{
    this->increment();
    return *this;
}

template <typename ComponentType>
bool ComponentConstIterator<ComponentType>::operator==(const ComponentConstIterator<ComponentType>& other) const
{
    return this->equals(other);
}

template <typename ComponentType>
bool ComponentConstIterator<ComponentType>::operator!=(const ComponentConstIterator<ComponentType>& other) const
{
    return !this->equals(other);
}

template <typename ComponentType>
ComponentConstIterator<ComponentType>::operator bool() const
{
    return this->is_valid();
}

}
