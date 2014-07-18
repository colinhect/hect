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
#include <cassert>

namespace hect
{

template <typename T, typename Container>
CollectionAccessor<T, Container>::CollectionAccessor(const Container& container) :
    _container(const_cast<Container*>(&container))
{
}

template <typename T, typename Container>
typename Container::iterator CollectionAccessor<T, Container>::begin()
{
    assert(_container);
    return _container->begin();
}

template <typename T, typename Container>
typename Container::const_iterator CollectionAccessor<T, Container>::begin() const
{
    assert(_container);
    return _container->begin();
}

template <typename T, typename Container>
typename Container::iterator CollectionAccessor<T, Container>::end()
{
    assert(_container);
    return _container->end();
}

template <typename T, typename Container>
typename Container::const_iterator CollectionAccessor<T, Container>::end() const
{
    assert(_container);
    return _container->end();
}

template <typename T, typename Container>
size_t CollectionAccessor<T, Container>::size() const
{
    assert(_container);
    return _container->size();
}

template <typename T, typename Container>
T& CollectionAccessor<T, Container>::operator[](typename Container::size_type index)
{
    assert(_container);
    return (*_container)[index];
}

template <typename T, typename Container>
const T& CollectionAccessor<T, Container>::operator[](typename Container::size_type index) const
{
    assert(_container);
    return (*_container)[index];
}

}