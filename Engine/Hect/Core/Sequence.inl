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

template <typename T, typename Container>
Sequence<T, Container>::Sequence(typename const Container& container) :
    _container(const_cast<typename Container*>(&container))
{
}

template <typename T, typename Container>
Sequence<T, Container>::Sequence(Sequence&& sequence) :
    _container(sequence._container)
{
}

template <typename T, typename Container>
typename Container::iterator Sequence<T, Container>::begin()
{
    return _container->begin();
}

template <typename T, typename Container>
typename Container::iterator Sequence<T, Container>::end()
{
    return _container->end();
}

template <typename T, typename Container>
typename Container::const_iterator Sequence<T, Container>::begin() const
{
    return _container->begin();
}

template <typename T, typename Container>
typename Container::const_iterator Sequence<T, Container>::end() const
{
    return _container->end();
}

template <typename T, typename Container>
size_t Sequence<T, Container>::size() const
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

template <typename T, typename Container>
bool Sequence<T, Container>::empty() const
{
    return size() == 0;
}

template <typename T, typename Container>
T& Sequence<T, Container>::operator[](size_t index)
{
    auto it = begin();
    for (size_t i = 0; i < index; ++i)
    {
        ++it;
    }
    return *it;
}

template <typename T, typename Container>
const T& Sequence<T, Container>::operator[](size_t index) const
{
    auto it = begin();
    for (size_t i = 0; i < index; ++i)
    {
        ++it;
    }
    return *it;
}

}