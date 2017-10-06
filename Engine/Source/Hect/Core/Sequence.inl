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

template <typename Type, typename Container>
Sequence<Type, Container>::Sequence(const Container& container) :
    _container(const_cast<Container&>(container))
{
}

template <typename Type, typename Container>
Sequence<Type, Container>::Sequence(Sequence&& sequence) :
    _container(sequence._container)
{
}

template <typename Type, typename Container>
typename Container::iterator Sequence<Type, Container>::begin()
{
    return _container.begin();
}

template <typename Type, typename Container>
typename Container::iterator Sequence<Type, Container>::end()
{
    return _container.end();
}

template <typename Type, typename Container>
typename Container::const_iterator Sequence<Type, Container>::begin() const
{
    return _container.begin();
}

template <typename Type, typename Container>
typename Container::const_iterator Sequence<Type, Container>::end() const
{
    return _container.end();
}

template <typename Type, typename Container>
size_t Sequence<Type, Container>::size() const
{
    size_t count = 0;

    auto it = begin();
    while (it != end())
    {
        ++count;
        ++it;
    }

    return count;
}

template <typename Type, typename Container>
bool Sequence<Type, Container>::empty() const
{
    return size() == 0;
}

template <typename Type, typename Container>
Type& Sequence<Type, Container>::operator[](size_t index)
{
    auto it = begin();
    for (size_t i = 0; i < index; ++i)
    {
        ++it;
    }
    return *it;
}

template <typename Type, typename Container>
const Type& Sequence<Type, Container>::operator[](size_t index) const
{
    auto it = begin();
    for (size_t i = 0; i < index; ++i)
    {
        ++it;
    }
    return *it;
}

}
