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
namespace hect
{

template <typename T>
ComponentIteratorBase<T>::ComponentIteratorBase()
{
}

template <typename T>
ComponentIteratorBase<T>::ComponentIteratorBase(ComponentPool<T>& pool, ComponentId id) :
    _pool(&pool),
    _id(id)
{
}

template <typename T>
void ComponentIteratorBase<T>::increment()
{
    ++this->_id;

    size_t maxId = this->_pool->maxId();
    while (this->_id < maxId)
    {
        if (this->isValid())
        {
            const Entity& entity = this->_pool->entityForComponent(this->_id);
            if (entity.isActivated())
            {
                break;
            }
        }

        ++this->_id;
    }
}

template <typename T>
bool ComponentIteratorBase<T>::isValid() const
{
    if (this->_pool && this->_pool->componentHasEntity(this->_id))
    {
        return true;
    }
    return false;
}

template <typename T>
void ComponentIteratorBase<T>::ensureValid() const
{
    if (!this->isValid())
    {
        throw InvalidOperation("Invalid component iterator");
    }
}

template <typename T>
bool ComponentIteratorBase<T>::equals(const ComponentIteratorBase<T>& other) const
{
    return this->_pool == other._pool && this->_id == other._id;
}

template <typename T>
ComponentIterator<T>::ComponentIterator() :
    ComponentIteratorBase<T>()
{
}

template <typename T>
ComponentIterator<T>::ComponentIterator(ComponentPool<T>& pool, ComponentId id) :
    ComponentIteratorBase<T>(pool, id)
{
}

template <typename T>
T& ComponentIterator<T>::operator*() const
{
    this->ensureValid();
    return this->_pool->withId(this->_id);
}

template <typename T>
T* ComponentIterator<T>::operator->() const
{
    this->ensureValid();
    return &this->_pool->withId(this->_id);
}

template <typename T>
typename ComponentIterator<T>& ComponentIterator<T>::operator++()
{
    this->increment();
    return *this;
}

template <typename T>
bool ComponentIterator<T>::operator==(const ComponentIterator<T>& other) const
{
    return this->equals(other);
}

template <typename T>
bool ComponentIterator<T>::operator!=(const ComponentIterator<T>& other) const
{
    return !this->equals(other);
}

template <typename T>
ComponentIterator<T>::operator bool() const
{
    return this->isValid();
}

template <typename T>
ConstComponentIterator<T>::ConstComponentIterator() :
    ComponentIteratorBase<T>()
{
}

template <typename T>
ConstComponentIterator<T>::ConstComponentIterator(const ComponentPool<T>& pool, ComponentId id) :
    ComponentIteratorBase<T>(*const_cast<ComponentPool<T>*>(&pool), id)
{
}

template <typename T>
const T& ConstComponentIterator<T>::operator*() const
{
    this->ensureValid();
    return this->_pool->withId(this->_id);
}

template <typename T>
const T* ConstComponentIterator<T>::operator->() const
{
    this->ensureValid();
    return &this->_pool->withId(this->_id);
}

template <typename T>
typename ConstComponentIterator<T>& ConstComponentIterator<T>::operator++()
{
    this->increment();
    return *this;
}

template <typename T>
bool ConstComponentIterator<T>::operator==(const ConstComponentIterator<T>& other) const
{
    return this->equals(other);
}

template <typename T>
bool ConstComponentIterator<T>::operator!=(const ConstComponentIterator<T>& other) const
{
    return !this->equals(other);
}

template <typename T>
ConstComponentIterator<T>::operator bool() const
{
    return this->isValid();
}

}
