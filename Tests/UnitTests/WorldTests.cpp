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
#include <Hect/Logic/World.h>
#include <Hect/IO/BinaryEncoder.h>
#include <Hect/IO/BinaryDecoder.h>
#include <Hect/IO/JsonEncoder.h>
#include <Hect/IO/JsonDecoder.h>
#include <Hect/IO/MemoryWriteStream.h>
#include <Hect/IO/MemoryReadStream.h>
using namespace hect;

#include <catch.hpp>

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

void testEncodeDecode(std::function<void(World& world)> createWorld, std::function<void(World& world)> verifyWorld)
{
    // Json
    {
        JsonValue jsonValue;

        {
            World world;

            createWorld(world);

            JsonEncoder encoder;
            encoder << encodeValue(world);
            jsonValue = encoder.jsonValues()[0];
        }

        {
            World world;

            JsonDecoder decoder(jsonValue);
            decoder >> decodeValue(world);

            verifyWorld(world);
        }
    }

    // Binary
    {
        std::vector<uint8_t> data;

        {
            World world;
            createWorld(world);

            MemoryWriteStream writeStream(data);
            BinaryEncoder encoder(writeStream);
            encoder << encodeValue(world);
        }

        {
            World world;

            MemoryReadStream readStream(data);
            BinaryDecoder decoder(readStream);
            decoder >> decodeValue(world);

            verifyWorld(world);
        }
    }
}

TEST_CASE("World_RegisterComponent")
{
    ComponentRegistry::registerType<String>();
}

TEST_CASE("World_CreateAndDestroyEntities")
{
    World world;

    Entity::Iterator a = world.createEntity();
    REQUIRE(a);
    REQUIRE(a->id() == 0);

    Entity::Iterator b = world.createEntity();
    REQUIRE(b);
    REQUIRE(b->id() == 1);

    a->destroy();
    REQUIRE(!a);

    REQUIRE(b);
    REQUIRE(b->id() == 1);
}

TEST_CASE("World_DereferenceInvalidEntityIter")
{
    World world;

    Entity::Iterator a = world.entities().end();
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, Error);
}

TEST_CASE("World_DereferenceDestroyedEntityIter")
{
    World world;

    Entity::Iterator a = world.createEntity();
    REQUIRE(a);
    a->destroy();
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, Error);
}

TEST_CASE("World_CreateAndActivateEntities")
{
    World world;

    REQUIRE(world.entityCount() == 0);

    Entity::Iterator a = world.createEntity();
    REQUIRE(world.entityCount() == 0);

    Entity::Iterator b = world.createEntity();
    REQUIRE(world.entityCount() == 0);

    a->activate();
    REQUIRE(world.entityCount() == 1);

    a->destroy();
    REQUIRE(world.entityCount() == 0);

    b->destroy();
    REQUIRE(world.entityCount() == 0);
}

TEST_CASE("World_EntityIterationEmpty")
{
    World world;

    size_t count = 0;
    for (const Entity& entity : world.entities())
    {
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("World_EntityIterationNoneActivated")
{
    World world;

    world.createEntity();
    world.createEntity();
    world.createEntity();

    size_t count = 0;
    for (const Entity& entity : world.entities())
    {
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("World_EntityIterationSomeActivated")
{
    World world;

    world.createEntity();
    world.createEntity()->activate();
    world.createEntity()->activate();
    world.createEntity();
    world.createEntity()->activate();
    world.createEntity();

    std::vector<EntityId> ids;
    for (const Entity& entity : world.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 1);
    REQUIRE(ids[1] == 2);
    REQUIRE(ids[2] == 4);
}

TEST_CASE("World_EntityIterationFirstActivated")
{
    World world;

    world.createEntity()->activate();
    world.createEntity();
    world.createEntity()->activate();
    world.createEntity();

    std::vector<EntityId> ids;
    for (const Entity& entity : world.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 2);
}

TEST_CASE("World_EntityIterationLastActivated")
{
    World world;

    world.createEntity();
    world.createEntity()->activate();
    world.createEntity();
    world.createEntity()->activate();

    std::vector<EntityId> ids;
    for (const Entity& entity : world.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 1);
    REQUIRE(ids[1] == 3);
}

TEST_CASE("World_EntityIterationFirstAndLastActivated")
{
    World world;

    world.createEntity()->activate();
    world.createEntity();
    world.createEntity()->activate();

    std::vector<EntityId> ids;
    for (const Entity& entity : world.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 2);
}

TEST_CASE("World_CreateManyEntities")
{
    World world;

    std::vector<Entity::Iterator> entities;
    for (EntityId id = 0; id < 64; ++id)
    {
        {
            Entity::Iterator entity = world.createEntity();
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


TEST_CASE("World_AddRemoveEntityChildren")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->activate();
    REQUIRE(!a->parent());

    Entity::Iterator b = world.createEntity();
    b->activate();
    REQUIRE(!b->parent());

    Entity::Iterator c = world.createEntity();
    c->activate();
    REQUIRE(!c->parent());

    a->addChild(*b);
    REQUIRE(&*b->parent() == &*a);

    a->removeChild(*b);
    REQUIRE(!b->parent());
}

TEST_CASE("World_AddChildEntityAsChild")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->activate();

    a->addChild(*b);

    Entity::Iterator c = world.createEntity();
    c->activate();

    REQUIRE_THROWS_AS(c->addChild(*b), Error);
}

TEST_CASE("World_AddEntityChildActivationRestrictions")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->activate();

    Entity::Iterator b = world.createEntity();

    REQUIRE_THROWS_AS(a->addChild(*b), Error);

    Entity::Iterator c = world.createEntity();
    c->activate();

    REQUIRE_THROWS_AS(c->addChild(*b), Error);
}

TEST_CASE("World_AddChildFromAnotherWorld")
{
    World worldA;

    Entity::Iterator a = worldA.createEntity();
    a->activate();

    World worldB;

    Entity::Iterator b = worldB.createEntity();
    b->activate();

    REQUIRE_THROWS_AS(a->addChild(*b), Error);
}

TEST_CASE("World_ChildEntityActivation")
{
    World world;

    Entity::Iterator a = world.createEntity();
    Entity::Iterator b = world.createEntity();
    Entity::Iterator c = world.createEntity();
    Entity::Iterator d = world.createEntity();

    a->addChild(*b);
    a->addChild(*c);
    c->addChild(*d);

    a->activate();

    REQUIRE(a->isActivated());
    REQUIRE(b->isActivated());
    REQUIRE(c->isActivated());
    REQUIRE(d->isActivated());
}

TEST_CASE("World_ChildEntityIterationEmpty")
{
    World world;

    Entity::Iterator a = world.createEntity();

    size_t count = 0;
    for (Entity& child : a->children())
    {
        ++count;
    }
    REQUIRE(count == 0);
}

TEST_CASE("World_ChildEntityIterationNonEmpty")
{
    World world;

    Entity::Iterator a = world.createEntity();
    Entity::Iterator b = world.createEntity();
    Entity::Iterator c = world.createEntity();
    Entity::Iterator d = world.createEntity();

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

TEST_CASE("World_DestroyEntityWithChildren")
{
    World world;

    Entity::Iterator a = world.createEntity();
    Entity::Iterator b = world.createEntity();
    Entity::Iterator c = world.createEntity();
    Entity::Iterator d = world.createEntity();

    a->addChild(*b);
    a->addChild(*c);
    c->addChild(*d);

    a->activate();

    REQUIRE(world.entityCount() == 4);

    a->destroy();
    REQUIRE(!a);
    REQUIRE(!b);
    REQUIRE(!c);
    REQUIRE(!d);

    REQUIRE(world.entityCount() == 0);
}

TEST_CASE("World_DestroyEntityWithParent")
{
    World world;

    Entity::Iterator a = world.createEntity();
    Entity::Iterator b = world.createEntity();
    Entity::Iterator c = world.createEntity();

    a->addChild(*b);
    a->addChild(*c);

    a->activate();

    REQUIRE(world.entityCount() == 3);

    b->destroy();

    REQUIRE(world.entityCount() == 2);

    std::vector<EntityId> ids;
    for (Entity& child : a->children())
    {
        ids.push_back(child.id());
    }
    REQUIRE(ids.size() == 1);
    REQUIRE(ids[0] == 2);
}

TEST_CASE("World_CloneEntity")
{
    World world;

    Entity::Iterator a = world.createEntity();
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

TEST_CASE("World_CloneEntityWithChildren")
{
    World world;

    Entity::Iterator a = world.createEntity();
    Entity::Iterator b = world.createEntity();
    Entity::Iterator c = world.createEntity();

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

TEST_CASE("World_AddAndRemoveComponent")
{
    World world;

    Entity::Iterator a = world.createEntity();

    Component<String>::Iterator string = a->addComponent<String>("Test");
    REQUIRE(string == a->component<String>());
    REQUIRE(string);
    REQUIRE(string->value == "Test");
    REQUIRE(&string->entity() == &*a);

    a->removeComponent<String>();
    REQUIRE(!string);
    REQUIRE(!a->component<String>());
}

TEST_CASE("World_ReplaceComponent")
{
    World world;

    Entity::Iterator a = world.createEntity();

    Component<String>::Iterator string = a->addComponent<String>("Test");
    Component<String>::Iterator replacedString = a->replaceComponent<String>("Replaced");
    REQUIRE(string == a->component<String>());
    REQUIRE(replacedString == a->component<String>());
    REQUIRE(string == replacedString);
    REQUIRE(replacedString);
    REQUIRE(replacedString->value == "Replaced");
    REQUIRE(&string->entity() == &*a);
}

TEST_CASE("World_AddExistingComponent")
{
    World world;

    Entity::Iterator a = world.createEntity();
    REQUIRE_THROWS_AS(a->removeComponent<String>(), Error);
}

TEST_CASE("World_RemoveNonExistingComponent")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("Test");
    REQUIRE_THROWS_AS(a->addComponent<String>("Test"), Error);
}

TEST_CASE("World_RemoveNonExistingUnregisteredComponent")
{
    World world;

    Entity::Iterator a = world.createEntity();
    REQUIRE_THROWS_AS(a->removeComponent<String>(), Error);
}

TEST_CASE("World_EncodeDecodeSimple")
{
    testEncodeDecode([](World& world)
    {
        Entity::Iterator a = world.createEntity();
        a->addComponent<String>("Test");
        a->activate();
    }, [](World& world)
    {
        REQUIRE(world.entityCount() == 1);

        Entity::Iterator a = world.entities().begin();
        REQUIRE(a);
        REQUIRE(a->component<String>()->value == "Test");
    });
}

TEST_CASE("World_EncodeDecodeWithChildren")
{
    testEncodeDecode([](World& world)
    {
        Entity::Iterator a = world.createEntity();
        Entity::Iterator b = world.createEntity();
        a->addChild(*b);
        a->activate();
    }, [](World& world)
    {
        REQUIRE(world.entityCount() == 2);

        Entity::Iterator a = world.entities().begin();
        REQUIRE(a);

        Entity::Children::Iterator b = a->children().begin();
        REQUIRE(b);
    });
}

TEST_CASE("World_ComponentIterationEmpty")
{
    World world;

    size_t count = 0;
    for (const String& string : world.components<String>())
    {
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("World_ComponentIterationNoneActivated")
{
    World world;

    world.createEntity()->addComponent<String>("Test");

    size_t count = 0;
    for (const String& string : world.components<String>())
    {
        ++count;
    }

    REQUIRE(count == 0);
}


TEST_CASE("World_ComponentIterationSomeActivated")
{
    World world;

    world.createEntity();
    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity();
    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity();

    std::vector<ComponentId> ids;
    for (const String& string : world.components<String>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("World_ComponentIterationFirstActivated")
{
    World world;

    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity();
    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity();

    std::vector<ComponentId> ids;
    for (const String& string : world.components<String>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("World_ComponentIterationLastActivated")
{
    World world;

    world.createEntity();
    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity();
    world.createEntity()->addComponent<String>("Test")->entity().activate();

    std::vector<ComponentId> ids;
    for (const String& string : world.components<String>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("World_ComponentIterationFirstAndLastActivated")
{
    World world;

    String string("Test");

    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity()->addComponent<String>("Test")->entity().activate();
    world.createEntity();
    world.createEntity()->addComponent<String>("Test")->entity().activate();

    std::vector<ComponentId> ids;
    for (const String& string : world.components<String>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("World_ComponentPoolListeners")
{
    World world;

    ComponentPoolListener listener;
    world.components<String>().addListener(listener);

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("A");
    REQUIRE(listener.receivedEvents.size() == 0);
    a->activate();
    REQUIRE(listener.receivedEvents.size() == 1);

    Entity::Iterator b = world.createEntity();
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

TEST_CASE("World_ComponentPoolFindFirstWithMatch")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("Match");
    b->activate();

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("Match");
    c->activate();

    Entity::Iterator d = world.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    ComponentPool<String>& strings = world.components<String>();
    Component<String>::Iterator iterator = strings.findFirst( [](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iterator);
    REQUIRE(iterator->value == "Match");
    REQUIRE(&*iterator == &*b->component<String>());
}

TEST_CASE("World_ComponentPoolFindFirstWithoutMatch")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("NotMatch");
    b->activate();

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("NotMatch");
    c->activate();

    Entity::Iterator d = world.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    ComponentPool<String>& strings = world.components<String>();
    Component<String>::Iterator iterator = strings.findFirst([](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(!iterator);
}

TEST_CASE("World_ComponentPoolFindWithMatches")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("Match");
    b->activate();

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("Match");
    c->activate();

    Entity::Iterator d = world.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    ComponentPool<String>& strings = world.components<String>();
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

TEST_CASE("World_ComponentPoolFindWithoutMatches")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("NotMatch");
    b->activate();

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("NotMatch");
    c->activate();

    Entity::Iterator d = world.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    ComponentPool<String>& strings = world.components<String>();
    Component<String>::Iterator::Vector iters = strings.find([](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("World_EntityPoolFindFirstWithMatch")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("Match");
    b->activate();

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("Match");
    c->activate();

    Entity::Iterator d = world.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    Entity::Iterator iterator = world.entities().findFirst([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(iterator);
    REQUIRE(&*iterator == &*b);
}

TEST_CASE("World_EntityPoolFindFirstWithoutMatch")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("NotMatch");
    b->activate();

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("NotMatch");
    c->activate();

    Entity::Iterator d = world.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    Entity::Iterator iterator = world.entities().findFirst([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(!iterator);
}

TEST_CASE("World_EntityPoolFindWithMatches")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("Match");
    b->activate();

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("Match");
    c->activate();

    Entity::Iterator d = world.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    Entity::Iterator::Vector iters = world.entities().find([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(iters.size() == 2);

    REQUIRE(iters[0]);
    REQUIRE(&*iters[0] == &*b);

    REQUIRE(iters[1]);
    REQUIRE(&*iters[1] == &*c);
}

TEST_CASE("World_EntityPoolFindWithoutMatches")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("NotMatch");
    a->activate();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("NotMatch");
    b->activate();

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("NotMatch");
    c->activate();

    Entity::Iterator d = world.createEntity();
    d->addComponent<String>("NotMatch");
    d->activate();

    Entity::Iterator::Vector iters = world.entities().find([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("World_EntityFindFirstChild")
{
    World world;

    Entity::Iterator a = world.createEntity();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = world.createEntity();
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

TEST_CASE("World_EntityFindChildren")
{
    World world;

    Entity::Iterator a = world.createEntity();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = world.createEntity();
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

TEST_CASE("World_EntityFindFirstDescendant")
{
    World world;

    Entity::Iterator a = world.createEntity();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = world.createEntity();
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

TEST_CASE("World_EntityFindDescendants")
{
    World world;

    Entity::Iterator a = world.createEntity();

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = world.createEntity();
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
        entity;
        return true;
    });
    REQUIRE(iters.size() == 3);
    REQUIRE(iters[0] == b);
    REQUIRE(iters[1] == c);
    REQUIRE(iters[2] == d);
}

TEST_CASE("World_EntityFindFirstAncestor")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("A");

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = world.createEntity();
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

TEST_CASE("World_EntityFindAncestors")
{
    World world;

    Entity::Iterator a = world.createEntity();
    a->addComponent<String>("A");

    Entity::Iterator b = world.createEntity();
    b->addComponent<String>("B");
    a->addChild(*b);

    Entity::Iterator c = world.createEntity();
    c->addComponent<String>("C");
    a->addChild(*c);

    Entity::Iterator d = world.createEntity();
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

TEST_CASE("World_CreateEntityHandle")
{
    World world;

    Entity::Iterator a = world.createEntity();

    Entity::Handle handle = a->createHandle();
    REQUIRE(handle);
    REQUIRE(&*handle == &*a);

    a->destroy();

    REQUIRE(!handle);
    REQUIRE_THROWS_AS(*handle, Error);
}

TEST_CASE("World_CopyEntityHandle")
{
    World world;

    Entity::Iterator a = world.createEntity();

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