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
#include <Hect/Logic/Scene.h>
#include <Hect/IO/BinaryDecoder.h>
#include <Hect/IO/BinaryEncoder.h>
#include <Hect/IO/JsonDecoder.h>
#include <Hect/IO/JsonEncoder.h>
#include <Hect/IO/MemoryReadStream.h>
#include <Hect/IO/MemoryWriteStream.h>

using namespace hect;

#include <catch.hpp>

extern Engine* engine;

class String :
    public Component<String>
{
public:
    String()
    {
    }

    String(const std::string& value) :
        value(value)
    {
    }

    void encode(Encoder& encoder) const
    {
        encoder << beginObject() << encodeValue("value", value) << endObject();
    }

    void decode(Decoder& decoder)
    {
        decoder >> beginObject() >> decodeValue("value", value) >> endObject();
    }

    std::string value;
};

class ComponentPoolListener :
    public Listener<ComponentEvent<String>>
{
public:
    void receiveEvent(const ComponentEvent<String>& event)
    {
        receivedEvents.push_back(event);
    }

    std::vector<ComponentEvent<String>> receivedEvents;
};

void testEncodeDecode(std::function<void(Scene& scene)> createScene, std::function<void(Scene& scene)> verifyScene)
{
    // Json
    {
        JsonValue jsonValue;

        {
            Scene scene(*engine);

            createScene(scene);

            JsonEncoder encoder;
            encoder << encodeValue(scene);
            jsonValue = encoder.jsonValues()[0];
        }

        {
            Scene scene(*engine);

            JsonDecoder decoder(jsonValue);
            decoder >> decodeValue(scene);

            verifyScene(scene);
        }
    }

    // Binary
    {
        std::vector<uint8_t> data;

        {
            Scene scene(*engine);
            createScene(scene);

            MemoryWriteStream writeStream(data);
            BinaryEncoder encoder(writeStream);
            encoder << encodeValue(scene);
        }

        {
            Scene scene(*engine);

            MemoryReadStream readStream(data);
            BinaryDecoder decoder(readStream);
            decoder >> decodeValue(scene);

            verifyScene(scene);
        }
    }
}

TEST_CASE("Scene_RegisterComponent")
{
    ComponentRegistry::registerType<String>();
}

TEST_CASE("Scene_CreateAndDestroyEntities")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    REQUIRE(a);
    REQUIRE(a->id() == 0);

    Entity::Iterator b = scene.createEntity();
    REQUIRE(b);
    REQUIRE(b->id() == 1);

    a->destroy();
    REQUIRE(!a);

    REQUIRE(b);
    REQUIRE(b->id() == 1);
}

TEST_CASE("Scene_DereferenceInvalidEntityIter")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.entities().end();
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, Error);
}

TEST_CASE("Scene_DereferenceDestroyedEntityIter")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    REQUIRE(a);
    a->destroy();
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, Error);
}

TEST_CASE("Scene_CreateAndActivateEntities")
{
    Scene scene(*engine);

    REQUIRE(scene.entityCount() == 0);

    Entity::Iterator a = scene.createEntity();
    REQUIRE(scene.entityCount() == 0);

    Entity::Iterator b = scene.createEntity();
    REQUIRE(scene.entityCount() == 0);

    a->activate();
    REQUIRE(scene.entityCount() == 1);

    a->destroy();
    REQUIRE(scene.entityCount() == 0);

    b->destroy();
    REQUIRE(scene.entityCount() == 0);
}

TEST_CASE("Scene_EntityIterationEmpty")
{
    Scene scene(*engine);

    size_t count = 0;
    for (const Entity& entity : scene.entities())
    {
        (void)entity;
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Scene_EntityIterationNoneActivated")
{
    Scene scene(*engine);

    scene.createEntity();
    scene.createEntity();
    scene.createEntity();

    size_t count = 0;
    for (const Entity& entity : scene.entities())
    {
        (void)entity;
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Scene_EntityIterationSomeActivated")
{
    Scene scene(*engine);

    scene.createEntity();
    scene.createEntity()->activate();
    scene.createEntity()->activate();
    scene.createEntity();
    scene.createEntity()->activate();
    scene.createEntity();

    std::vector<EntityId> ids;
    for (const Entity& entity : scene.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 1);
    REQUIRE(ids[1] == 2);
    REQUIRE(ids[2] == 4);
}

TEST_CASE("Scene_EntityIterationFirstActivated")
{
    Scene scene(*engine);

    scene.createEntity()->activate();
    scene.createEntity();
    scene.createEntity()->activate();
    scene.createEntity();

    std::vector<EntityId> ids;
    for (const Entity& entity : scene.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 2);
}

TEST_CASE("Scene_EntityIterationLastActivated")
{
    Scene scene(*engine);

    scene.createEntity();
    scene.createEntity()->activate();
    scene.createEntity();
    scene.createEntity()->activate();

    std::vector<EntityId> ids;
    for (const Entity& entity : scene.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 1);
    REQUIRE(ids[1] == 3);
}

TEST_CASE("Scene_EntityIterationFirstAndLastActivated")
{
    Scene scene(*engine);

    scene.createEntity()->activate();
    scene.createEntity();
    scene.createEntity()->activate();

    std::vector<EntityId> ids;
    for (const Entity& entity : scene.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 2);
}

TEST_CASE("Scene_CreateManyEntities")
{
    Scene scene(*engine);

    std::vector<Entity::Iterator> entities;
    for (EntityId id = 0; id < 64; ++id)
    {
        {
            Entity::Iterator entity = scene.createEntity();
            entity->activate();
            entities.push_back(entity);
        }

        {
            EntityId id = 0;
            for (const Entity::Iterator& entity : entities)
            {
                REQUIRE(entity);
                REQUIRE(entity->id() == id);
                ++id;
            }
        }
    }
}


TEST_CASE("Scene_AddRemoveEntityChildren")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->activate();
    REQUIRE(!a->parent());

    Entity::Iterator b = scene.createEntity();
    b->activate();
    REQUIRE(!b->parent());

    Entity::Iterator c = scene.createEntity();
    c->activate();
    REQUIRE(!c->parent());

    a->addChild(*b);
    REQUIRE(&*b->parent() == &*a);

    a->removeChild(*b);
    REQUIRE(!b->parent());
}

TEST_CASE("Scene_AddChildEntityAsChild")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->activate();

    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->activate();

    REQUIRE_THROWS_AS(c->addChild(*b), Error);
}

TEST_CASE("Scene_AddEntityChildActivationRestrictions")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->activate();

    Entity::Iterator b = scene.createEntity();

    REQUIRE_THROWS_AS(a->addChild(*b), Error);

    Entity::Iterator c = scene.createEntity();
    c->activate();

    REQUIRE_THROWS_AS(c->addChild(*b), Error);
}

TEST_CASE("Scene_AddChildFromAnotherScene")
{
    Scene sceneA(*engine);

    Entity::Iterator a = sceneA.createEntity();
    a->activate();

    Scene sceneB(*engine);

    Entity::Iterator b = sceneB.createEntity();
    b->activate();

    REQUIRE_THROWS_AS(a->addChild(*b), Error);
}

TEST_CASE("Scene_ChildEntityActivation")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    Entity::Iterator c = scene.createEntity();
    Entity::Iterator d = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);
    c->addChild(*d);

    a->activate();

    REQUIRE(a->isActivated());
    REQUIRE(b->isActivated());
    REQUIRE(c->isActivated());
    REQUIRE(d->isActivated());
}

TEST_CASE("Scene_ChildEntityIterationEmpty")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    size_t count = 0;
    for (Entity& child : a->children())
    {
        (void)child;
        ++count;
    }
    REQUIRE(count == 0);
}

TEST_CASE("Scene_ChildEntityIterationNonEmpty")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    Entity::Iterator c = scene.createEntity();
    Entity::Iterator d = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);
    c->addChild(*d);

    std::vector<EntityId> ids;
    for (Entity& child : a->children())
    {
        ids.push_back(child.id());
    }
    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 1);
    REQUIRE(ids[1] == 2);
    ids.clear();

    a->removeChild(*b);

    for (Entity& child : a->children())
    {
        ids.push_back(child.id());
    }
    REQUIRE(ids.size() == 1);
    REQUIRE(ids[0] == 2);
}

TEST_CASE("Scene_DestroyEntityWithChildren")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    Entity::Iterator c = scene.createEntity();
    Entity::Iterator d = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);
    c->addChild(*d);

    a->activate();

    REQUIRE(scene.entityCount() == 4);

    a->destroy();
    REQUIRE(!a);
    REQUIRE(!b);
    REQUIRE(!c);
    REQUIRE(!d);

    REQUIRE(scene.entityCount() == 0);
}

TEST_CASE("Scene_DestroyEntityWithParent")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    Entity::Iterator c = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);

    a->activate();

    REQUIRE(scene.entityCount() == 3);

    b->destroy();

    REQUIRE(scene.entityCount() == 2);

    std::vector<EntityId> ids;
    for (Entity& child : a->children())
    {
        ids.push_back(child.id());
    }
    REQUIRE(ids.size() == 1);
    REQUIRE(ids[0] == 2);
}

TEST_CASE("Scene_CloneEntity")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Component<String>::Iterator stringA = a->addComponent<String>("Test");
    a->activate();

    Entity::Iterator b = a->clone();
    Component<String>::Iterator stringB = b->component<String>();

    REQUIRE(a->isActivated());
    REQUIRE(!b->isActivated());
    REQUIRE(&stringA->entity() == &*a);
    REQUIRE(&stringB->entity() == &*b);
    REQUIRE(&*stringA != &*stringB);
    REQUIRE(stringA->id() != stringB->id());
}

TEST_CASE("Scene_CloneEntityWithChildren")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    Entity::Iterator c = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);

    Entity::Iterator cloneA = a->clone();
    Entity::Children::Iterator childIter = cloneA->children().begin();
    REQUIRE(&*childIter->parent() == &*cloneA);
    REQUIRE(&*childIter != &*b);
    REQUIRE(++childIter);
    REQUIRE(&*childIter->parent() == &*cloneA);
    REQUIRE(&*childIter != &*c);
    REQUIRE(!++childIter);
}

TEST_CASE("Scene_AddAndRemoveComponent")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Component<String>::Iterator string = a->addComponent<String>("Test");
    REQUIRE(string == a->component<String>());
    REQUIRE(string);
    REQUIRE(string->value == "Test");
    REQUIRE(&string->entity() == &*a);

    a->removeComponent<String>();
    REQUIRE(!string);
    REQUIRE(!a->component<String>());
}

TEST_CASE("Scene_ReplaceComponent")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Component<String>::Iterator string = a->addComponent<String>("Test");
    Component<String>::Iterator replacedString = a->replaceComponent<String>("Replaced");
    REQUIRE(string == a->component<String>());
    REQUIRE(replacedString == a->component<String>());
    REQUIRE(string == replacedString);
    REQUIRE(replacedString);
    REQUIRE(replacedString->value == "Replaced");
    REQUIRE(&string->entity() == &*a);
}

TEST_CASE("Scene_AddExistingComponent")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    REQUIRE_THROWS_AS(a->removeComponent<String>(), Error);
}

TEST_CASE("Scene_RemoveNonExistingComponent")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("Test");
    REQUIRE_THROWS_AS(a->addComponent<String>("Test"), Error);
}

TEST_CASE("Scene_RemoveNonExistingUnregisteredComponent")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    REQUIRE_THROWS_AS(a->removeComponent<String>(), Error);
}

TEST_CASE("Scene_EncodeDecodeSimple")
{
    testEncodeDecode([](Scene& scene)
    {
        Entity::Iterator a = scene.createEntity();
        a->addComponent<String>("Test");
        a->activate();
    }, [](Scene& scene)
    {
        REQUIRE(scene.entityCount() == 1);

        Entity::Iterator a = scene.entities().begin();
        REQUIRE(a);
        REQUIRE(a->component<String>()->value == "Test");
    });
}

TEST_CASE("Scene_EncodeDecodeWithChildren")
{
    testEncodeDecode([](Scene& scene)
    {
        Entity::Iterator a = scene.createEntity();
        Entity::Iterator b = scene.createEntity();
        a->addChild(*b);
        a->activate();
    }, [](Scene& scene)
    {
        REQUIRE(scene.entityCount() == 2);

        Entity::Iterator a = scene.entities().begin();
        REQUIRE(a);

        Entity::Children::Iterator b = a->children().begin();
        REQUIRE(b);
    });
}

TEST_CASE("Scene_ComponentIterationEmpty")
{
    Scene scene(*engine);

    size_t count = 0;
    for (const String& string : scene.components<String>())
    {
        (void)string;
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Scene_ComponentIterationNoneActivated")
{
    Scene scene(*engine);

    scene.createEntity()->addComponent<String>("Test");

    size_t count = 0;
    for (const String& string : scene.components<String>())
    {
        (void)string;
        ++count;
    }

    REQUIRE(count == 0);
}


TEST_CASE("Scene_ComponentIterationSomeActivated")
{
    Scene scene(*engine);

    scene.createEntity();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity();

    std::vector<ComponentId> ids;
    for (const String& string : scene.components<String>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("Scene_ComponentIterationFirstActivated")
{
    Scene scene(*engine);

    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity();

    std::vector<ComponentId> ids;
    for (const String& string : scene.components<String>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("Scene_ComponentIterationLastActivated")
{
    Scene scene(*engine);

    scene.createEntity();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();

    std::vector<ComponentId> ids;
    for (const String& string : scene.components<String>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("Scene_ComponentIterationFirstAndLastActivated")
{
    Scene scene(*engine);

    String string("Test");

    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();
    scene.createEntity();
    scene.createEntity()->addComponent<String>("Test")->entity().activate();

    std::vector<ComponentId> ids;
    for (const String& string : scene.components<String>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("Scene_ComponentPoolListeners")
{
    Scene scene(*engine);

    ComponentPoolListener listener;
    scene.components<String>().addListener(listener);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("A");
    REQUIRE(listener.receivedEvents.size() == 0);
    a->activate();
    REQUIRE(listener.receivedEvents.size() == 1);

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("B");
    REQUIRE(listener.receivedEvents.size() == 1);
    b->activate();

    REQUIRE(listener.receivedEvents.size() == 2);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType_Add);
    REQUIRE(&listener.receivedEvents[0].entity() == &*a);
    REQUIRE(listener.receivedEvents[1].type == ComponentEventType_Add);
    REQUIRE(&listener.receivedEvents[1].entity() == &*b);
    listener.receivedEvents.clear();

    a->destroy();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType_Remove);
    REQUIRE(listener.receivedEvents[0].entity().id() == (EntityId)-1);
    listener.receivedEvents.clear();

    b->removeComponent<String>();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType_Remove);
    REQUIRE(&listener.receivedEvents[0].entity() == &*b);
}

TEST_CASE("Scene_ComponentPoolFindFirstWithMatch")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    ComponentPool<String>& strings = scene.components<String>();
    Component<String>::Iterator iterator = strings.findFirst([](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iterator);
    REQUIRE(iterator->value == "Match");
    REQUIRE(&*iterator == &*b->component<String>());
}

TEST_CASE("Scene_ComponentPoolFindFirstWithoutMatch")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    ComponentPool<String>& strings = scene.components<String>();
    Component<String>::Iterator iterator = strings.findFirst([](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(!iterator);
}

TEST_CASE("Scene_ComponentPoolFindWithMatches")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    ComponentPool<String>& strings = scene.components<String>();
    Component<String>::Iterator::Vector iters = strings.find([](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iters.size() == 2);

    REQUIRE(iters[0]);
    REQUIRE(iters[0]->value == "Match");
    REQUIRE(&*iters[0] == &*b->component<String>());

    REQUIRE(iters[1]);
    REQUIRE(iters[1]->value == "Match");
    REQUIRE(&*iters[1] == &*c->component<String>());
}

TEST_CASE("Scene_ComponentPoolFindWithoutMatches")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    ComponentPool<String>& strings = scene.components<String>();
    Component<String>::Iterator::Vector iters = strings.find([](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("Scene_EntityPoolFindFirstWithMatch")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    Entity::Iterator iterator = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(iterator);
    REQUIRE(&*iterator == &*b);
}

TEST_CASE("Scene_EntityPoolFindFirstWithoutMatch")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    Entity::Iterator iterator = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(!iterator);
}

TEST_CASE("Scene_EntityPoolFindWithMatches")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    Entity::Iterator::Vector iters = scene.entities().find([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(iters.size() == 2);

    REQUIRE(iters[0]);
    REQUIRE(&*iters[0] == &*b);

    REQUIRE(iters[1]);
    REQUIRE(&*iters[1] == &*c);
}

TEST_CASE("Scene_EntityPoolFindWithoutMatches")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    Entity::Iterator::Vector iters = scene.entities().find([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("Scene_EntityFindFirstChild")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("D");
    c->addChild(*d);

    a->activate();

    Entity::Iterator iterator = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<String>()->value == "B";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == b);

    iterator = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<String>()->value == "C";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == c);

    iterator = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<String>()->value == "D";
    });
    REQUIRE(!iterator);
}

TEST_CASE("Scene_EntityFindChildren")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("D");
    c->addChild(*d);

    a->activate();

    Entity::Iterator::Vector iters = a->findChildren([](const Entity& entity)
    {
        return entity.component<String>()->value == "B" ||
               entity.component<String>()->value == "C";
    });
    REQUIRE(iters.size() == 2);
    REQUIRE(iters[0] == b);
    REQUIRE(iters[1] == c);

    iters = a->findChildren([](const Entity& entity)
    {
        return entity.component<String>()->value == "C";
    });
    REQUIRE(iters.size() == 1);
    REQUIRE(iters[0] == c);

    iters = a->findChildren([](const Entity& entity)
    {
        return entity.component<String>()->value == "D";
    });
    REQUIRE(iters.size() == 0);
}

TEST_CASE("Scene_EntityFindFirstDescendant")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("D");
    c->addChild(*d);

    a->activate();

    Entity::Iterator iterator = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<String>()->value == "B";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == b);

    iterator = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<String>()->value == "C";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == c);

    iterator = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<String>()->value == "D";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == d);

    iterator = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<String>()->value == "E";
    });
    REQUIRE(!iterator);
}

TEST_CASE("Scene_EntityFindDescendants")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("D");
    c->addChild(*d);

    a->activate();

    Entity::Iterator::Vector iters = a->findDescendants([](const Entity& entity)
    {
        return entity.component<String>()->value == "B" ||
               entity.component<String>()->value == "C";
    });
    REQUIRE(iters.size() == 2);
    REQUIRE(iters[0] == b);
    REQUIRE(iters[1] == c);

    iters = a->findDescendants([](const Entity& entity)
    {
        return entity.component<String>()->value == "C";
    });
    REQUIRE(iters.size() == 1);
    REQUIRE(iters[0] == c);

    iters = a->findDescendants([](const Entity& entity)
    {
        return entity.component<String>()->value == "D";
    });
    REQUIRE(iters.size() == 1);
    REQUIRE(iters[0] == d);

    iters = a->findDescendants([](const Entity& entity)
    {
        (void)entity;
        return true;
    });
    REQUIRE(iters.size() == 3);
    REQUIRE(iters[0] == b);
    REQUIRE(iters[1] == c);
    REQUIRE(iters[2] == d);
}

TEST_CASE("Scene_EntityFindFirstAncestor")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("A");

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("D");
    c->addChild(*d);

    a->activate();

    Entity::Iterator iterator = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<String>()->value == "C";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == c);

    iterator = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<String>()->value == "A";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == a);

    iterator = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<String>()->value == "E";
    });
    REQUIRE(!iterator);
}

TEST_CASE("Scene_EntityFindAncestors")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<String>("A");

    Entity::Iterator b = scene.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<String>("D");
    c->addChild(*d);

    a->activate();

    Entity::Iterator::Vector iters = d->findAncestors([](const Entity& entity)
    {
        return entity.component<String>()->value == "C" ||
               entity.component<String>()->value == "A";
    });
    REQUIRE(iters.size() == 2);
    REQUIRE(iters[0] == c);
    REQUIRE(iters[1] == a);

    iters = d->findAncestors([](const Entity& entity)
    {
        return entity.component<String>()->value == "A";
    });
    REQUIRE(iters.size() == 1);
    REQUIRE(iters[0] == a);

    iters = d->findAncestors([](const Entity& entity)
    {
        return entity.component<String>()->value == "E";
    });
    REQUIRE(iters.size() == 0);
}

TEST_CASE("Scene_CreateEntityHandle")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Entity::Handle handle = a->createHandle();
    REQUIRE(handle);
    REQUIRE(&*handle == &*a);

    a->destroy();

    REQUIRE(!handle);
    REQUIRE_THROWS_AS(*handle, Error);
}

TEST_CASE("Scene_CopyEntityHandle")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Entity::Handle handle = a->createHandle();
    Entity::Handle handleCopy = handle;
    REQUIRE(handle == handleCopy);
    REQUIRE(&*handle == &*a);
    REQUIRE(&*handleCopy == &*a);

    a->destroy();

    REQUIRE(!handle);
    REQUIRE_THROWS_AS(*handle, Error);
    REQUIRE(!handleCopy);
    REQUIRE_THROWS_AS(*handleCopy, Error);
}