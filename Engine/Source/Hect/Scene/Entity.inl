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

template <typename T, typename... Args>
T& Entity::add_component(Args&&... args)
{
    ensure_in_pool();
    ComponentPool<T>& component_pool = _pool->_scene.components<T>();
    return component_pool.add(*this, T(args...));
}

template <typename T, typename... Args>
T& Entity::replace_component(Args&&... args)
{
    ensure_in_pool();
    ComponentPool<T>& component_pool = _pool->_scene.components<T>();
    return component_pool.replace(*this, T(args...));
}

template <typename T>
void Entity::remove_component()
{
    ensure_in_pool();
    ComponentPool<T>& component_pool = _pool->_scene.components<T>();
    component_pool.remove(*this);
}

template <typename T>
bool Entity::has_component() const
{
    ensure_in_pool();
    ComponentPool<T>& component_pool = _pool->_scene.components<T>();
    return component_pool.has(*this);
}

template <typename T>
T& Entity::component()
{
    ensure_in_pool();
    ComponentPool<T>& component_pool = _pool->_scene.components<T>();
    return component_pool.get(*this);
}

template <typename T>
const T& Entity::component() const
{
    ensure_in_pool();
    ComponentPool<T>& component_pool = _pool->_scene.components<T>();
    return component_pool.get(*this);
}

template <typename T>
EntityHandle Entity::find_first_child(T&& predicate) const
{
    if (has_children())
    {
        for (const Entity& child : children())
        {
            if (predicate(child))
            {
                return child.handle();
            }
        }
    }

    return EntityHandle();
}

template <typename T>
EntityHandle Entity::find_first_descendant(T&& predicate) const
{
    if (has_children())
    {
        for (const Entity& child : children())
        {
            if (predicate(child))
            {
                return child.handle();
            }
            else if (child.has_children())
            {
                EntityHandle next_generation = child.find_first_descendant(predicate);
                if (next_generation)
                {
                    return next_generation;
                }
            }
        }
    }

    return EntityHandle();
}

template <typename T>
EntityHandle Entity::find_first_ancestor(T&& predicate) const
{
    EntityHandle handle = parent();
    if (handle)
    {
        if (predicate(*handle))
        {
            return handle;
        }
        else
        {
            return handle->find_first_ancestor(predicate);
        }
    }

    return EntityHandle();
}

template <typename T>
std::vector<EntityHandle> Entity::find_children(T&& predicate) const
{
    std::vector<EntityHandle> results;

    if (has_children())
    {
        for (const Entity& child : children())
        {
            if (predicate(child))
            {
                results.emplace_back(child.handle());
            }
        }
    }

    return results;
}

template <typename T>
std::vector<EntityHandle> Entity::find_descendants(T&& predicate) const
{
    std::vector<EntityHandle> results;

    if (has_children())
    {
        for_descendants([&](const Entity& entity)
        {
            if (predicate(entity))
            {
                results.emplace_back(entity.handle());
            }
        });
    }

    return results;
}

template <typename T>
std::vector<EntityHandle> Entity::find_ancestors(T&& predicate) const
{
    std::vector<EntityHandle> results;

    EntityHandle handle = parent();
    while (handle)
    {
        if (predicate(*handle))
        {
            results.push_back(handle);
        }

        handle = handle->parent();
    }

    return results;
}

template <typename T>
void Entity::for_children(T&& action)
{
    if (has_children())
    {
        for (Entity& child : children())
        {
            action(child);
        }
    }
}

template <typename T>
void Entity::for_children(T&& action) const
{
    if (has_children())
    {
        for (const Entity& child : children())
        {
            action(child);
        }
    }
}

template <typename T>
void Entity::for_descendants(T&& action)
{
    if (has_children())
    {
        for (Entity& child : children())
        {
            action(child);

            if (child.has_children())
            {
                child.for_descendants(action);
            }
        }
    }
}

template <typename T>
void Entity::for_descendants(T&& action) const
{
    if (has_children())
    {
        for (const Entity& child : children())
        {
            action(child);

            if (child.has_children())
            {
                child.for_descendants(action);
            }
        }
    }
}

template <typename T>
void Entity::for_ancestors(T&& action)
{
    EntityHandle handle = parent();
    while (handle)
    {
        action(*handle);
        handle = handle->parent();
    }
}

template <typename T>
void Entity::for_ancestors(T&& action) const
{
    EntityHandle handle = parent();
    while (handle)
    {
        action(*handle);
        handle = handle->parent();
    }
}

}
