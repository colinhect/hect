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
#include "Context.h"

namespace
{
    Context* _instance = nullptr;
}

Context& Context::instance()
{
    assert(_instance);
    return *_instance;
}

Context::Context() :
    scene(Engine::instance())
{
    Type::create<TestA>(Kind::Class, "TestA");
    ComponentRegistry::registerType<TestA>();
    scene.addComponentType<TestA>();

    _instance = this;

    for (int i = 0; i < 256; ++i)
    {
        Entity::Iterator entity = scene.createEntity();
        entity->addComponent<TestA>();

        for (int j = 0; j < i; ++j)
        {
            Entity::Iterator child = scene.createEntity();
            child->addComponent<TestA>();
            entity->addChild(*child);
        }

        entity->activate();
        entitiesWithChildren.push_back(entity);
    }

    scene.refresh();
}

Context::~Context()
{
    _instance = nullptr;
}
