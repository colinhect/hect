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
#pragma once

#include <limits>

namespace hect
{

///
/// A simple implementation of the allocator concept for use in STL containers.
template <typename T>
class Allocator
{
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template <class U>
    struct rebind
    {
        typedef Allocator<U> other;
    };

    pointer address(reference value) const
    {
        return &value;
    }

    const_pointer address(const_reference value) const
    {
        return &value;
    }

    Allocator() throw() { }
    Allocator(const Allocator&) throw() { }

    template <class U>
    Allocator(const Allocator<U>&) throw() { }

    ~Allocator() throw() { }

    size_type max_size() const throw()
    {
        return std::numeric_limits<std::size_t>::max() / sizeof(T);
    }

    pointer allocate(size_type count, const void* = nullptr)
    {
        return reinterpret_cast<pointer>(::operator new(count * sizeof(T)));
    }

    void construct(pointer p, const T& value)
    {
        new (reinterpret_cast<void*>(p)) T(value);
    }

    void destroy(pointer p)
    {
        p->~T();
    }

    void deallocate(pointer p, size_type count)
    {
        (void)count;
        ::operator delete(reinterpret_cast<void*>(p));
    }
};

template <class T, class U>
bool operator==(const Allocator<T>&, const Allocator<U>&) throw()
{
    return true;
}

template <class T, class U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) throw()
{
    return false;
}

}