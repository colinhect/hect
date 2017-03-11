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
T& Entity::addComponent(Args&&... args)
{
    ensureInPool();
    ComponentPool<T>& componentPool = _pool->_scene.components<T>();
    return *componentPool.add(*this, T(args...));
}

template <typename T, typename... Args>
T& Entity::replaceComponent(Args&&... args)
{
    ensureInPool();
    ComponentPool<T>& componentPool = _pool->_scene.components<T>();
    return *componentPool.replace(*this, T(args...));
}

template <typename T>
void Entity::removeComponent()
{
    ensureInPool();
    ComponentPool<T>& componentPool = _pool->_scene.components<T>();
    componentPool.remove(*this);
}

template <typename T>
bool Entity::hasComponent() const
{
    ensureInPool();
    ComponentPool<T>& componentPool = _pool->_scene.components<T>();
    return componentPool.has(*this);
}

template <typename T>
T& Entity::component()
{
    ensureInPool();
    ComponentPool<T>& componentPool = _pool->_scene.components<T>();
    return *componentPool.get(*this);
}

template <typename T>
const T& Entity::component() const
{
    ensureInPool();
    ComponentPool<T>& componentPool = _pool->_scene.components<T>();
    return *componentPool.get(*this);
}

template <typename T>
Entity::Handle Entity::findFirstChild(T&& predicate) const
{
    if (hasChildren())
    {
        for (const Entity& child : children())
        {
            if (predicate(child))
            {
                return child.handle();
            }
        }
    }

    return _pool->end();
}

template <typename T>
Entity::Handle Entity::findFirstDescendant(T&& predicate) const
{
    if (hasChildren())
    {
        for (const Entity& child : children())
        {
            if (predicate(child))
            {
                return child.handle();
            }
            else if (child.hasChildren())
            {
                Entity::Handle nextGeneration = child.findFirstDescendant(predicate);
                if (nextGeneration)
                {
                    return nextGeneration;
                }
            }
        }
    }

    return _pool->end();
}

template <typename T>
Entity::Handle Entity::findFirstAncestor(T&& predicate) const
{
    Entity::Handle handle = parent();
    if (handle)
    {
        if (predicate(*handle))
        {
            return handle;
        }
        else
        {
            return handle->findFirstAncestor(predicate);
        }
    }

    return _pool->end();
}

template <typename T>
std::vector<Entity::Handle> Entity::findChildren(T&& predicate) const
{
    std::vector<Entity::Handle> results;

    if (hasChildren())
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
std::vector<Entity::Handle> Entity::findDescendants(T&& predicate) const
{
    std::vector<Entity::Handle> results;

    if (hasChildren())
    {
        forDescendants([&](const Entity& entity)
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
std::vector<Entity::Handle> Entity::findAncestors(T&& predicate) const
{
    std::vector<Entity::Handle> results;

    Entity::Handle handle = parent();
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
void Entity::forChildren(T&& action)
{
    if (hasChildren())
    {
        for (Entity& child : children())
        {
            action(child);
        }
    }
}

template <typename T>
void Entity::forChildren(T&& action) const
{
    if (hasChildren())
    {
        for (const Entity& child : children())
        {
            action(child);
        }
    }
}

template <typename T>
void Entity::forDescendants(T&& action)
{
    if (hasChildren())
    {
        for (Entity& child : children())
        {
            action(child);

            if (child.hasChildren())
            {
                child.forDescendants(action);
            }
        }
    }
}

template <typename T>
void Entity::forDescendants(T&& action) const
{
    if (hasChildren())
    {
        for (const Entity& child : children())
        {
            action(child);

            if (child.hasChildren())
            {
                child.forDescendants(action);
            }
        }
    }
}

template <typename T>
void Entity::forAncestors(T&& action)
{
    Entity::Handle handle = parent();
    while (handle)
    {
        action(*handle);
        handle = handle->parent();
    }
}

template <typename T>
void Entity::forAncestors(T&& action) const
{
    Entity::Handle handle = parent();
    while (handle)
    {
        action(*handle);
        handle = handle->parent();
    }
}

}
