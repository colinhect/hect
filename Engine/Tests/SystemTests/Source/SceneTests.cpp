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
#include <Hect/Logic/Scene.h>
#include <Hect/Logic/ListeningSystem.h>
#include <Hect/IO/BinaryDecoder.h>
#include <Hect/IO/BinaryEncoder.h>
#include <Hect/IO/DataValueDecoder.h>
#include <Hect/IO/DataValueEncoder.h>
#include <Hect/IO/MemoryReadStream.h>
#include <Hect/IO/MemoryWriteStream.h>
#include <Hect/Runtime/Engine.h>

using namespace hect;

#include <catch.hpp>

extern Engine* engine;

class Test :
    public Component<Test>
{
public:
    std::string value;

    Test()
    {
    }

    Test(const std::string& value) :
        value(value)
    {
    }

    void encode(Encoder& encoder) const
    {
        encoder << encodeValue("value", value);
    }

    void decode(Decoder& decoder)
    {
        decoder >> decodeValue("value", value);
    }
};


class TestB :
    public Component<TestB>
{
public:
    std::string value;

    TestB()
    {
    }

    TestB(const std::string& value) :
        value(value)
    {
    }

    void encode(Encoder& encoder) const
    {
        encoder << encodeValue("value", value);
    }

    void decode(Decoder& decoder)
    {
        decoder >> decodeValue("value", value);
    }
};

class TestSystem :
    public BaseSystem
{
public:
    std::string value;

    TestSystem(Engine& engine, Scene& scene) :
        BaseSystem(scene)
    {
        (void)engine;
    }

    void tick(Engine& engine, double timeStep) override
    {
        (void)engine;
        (void)timeStep;
    }

    void encode(Encoder& encoder) const
    {
        encoder << encodeValue("value", value);
    }

    void decode(Decoder& decoder)
    {
        decoder >> decodeValue("value", value);
    }
};

class TestComponentPoolListener :
    public Listener<ComponentEvent<Test>>
{
public:
    void receiveEvent(const ComponentEvent<Test>& event)
    {
        receivedEvents.push_back(event);
    }

    std::vector<ComponentEvent<Test>> receivedEvents;
};

class TestSystemB :
    public System<Test, TestB>
{
public:
    std::string value;

    TestSystemB(Engine& engine, Scene& scene) :
        System(scene)
    {
        (void)engine;
    }

    void tick(Engine& engine, double timeStep) override
    {
        (void)engine;
        (void)timeStep;
    }

    void onComponentAdded(Test::Iterator test) override
    {
        (void)test;
        value = "Test";
    }

    void onComponentAdded(TestB::Iterator test) override
    {
        (void)test;
        value = "TestB";
    }

    void encode(Encoder& encoder) const
    {
        encoder << encodeValue("value", value);
    }

    void decode(Decoder& decoder)
    {
        decoder >> decodeValue("value", value);
    }
};

void testEncodeDecode(std::function<void(Scene& scene)> createScene, std::function<void(Scene& scene)> verifyScene)
{
    // Json
    {
        DataValue dataValue;

        {
            Scene scene(*engine);

            createScene(scene);

            DataValueEncoder encoder;
            encoder << encodeValue(scene);
            dataValue = encoder.dataValues()[0];
        }

        {
            Scene scene(*engine);

            DataValueDecoder decoder(dataValue);
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

TEST_CASE("Register a component type", "[Scene]")
{
    Type::create<Test>(Kind_Class, "Test");
    ComponentRegistry::registerType<Test>();
    Type::create<TestB>(Kind_Class, "TestB");
    ComponentRegistry::registerType<TestB>();
}

TEST_CASE("Register a system type", "[Scene]")
{
    Type::create<TestSystem>(Kind_Class, "TestSystem");
    SystemRegistry::registerType<TestSystem>();
    Type::create<TestSystemB>(Kind_Class, "TestSystemB");
    SystemRegistry::registerType<TestSystemB>();
}

TEST_CASE("Create and destroy entities in a scene", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    REQUIRE(a);
    REQUIRE(a->id() == 0);

    Entity::Iterator b = scene.createEntity();
    REQUIRE(b);
    REQUIRE(b->id() == 1);

    a->destroy();
    REQUIRE(a);
    REQUIRE(a->isPendingDestruction());

    scene.refresh();

    REQUIRE(!a);

    REQUIRE(b);
    REQUIRE(b->id() == 1);
}

TEST_CASE("Dereference an invalid entity iterator", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a;
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, InvalidOperation);

    Entity::Iterator b = scene.entities().end();
    REQUIRE(!b);
    REQUIRE_THROWS_AS(*b, InvalidOperation);
}

TEST_CASE("Dereference an iterator to a destroyed entity", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    REQUIRE(a);
    a->destroy();
    REQUIRE(a);
    scene.refresh();
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, InvalidOperation);
}

TEST_CASE("Create and activate entities in a scene", "[Scene]")
{
    Scene scene(*engine);

    REQUIRE(scene.entityCount() == 0);

    Entity::Iterator a = scene.createEntity();
    REQUIRE(scene.entityCount() == 0);

    Entity::Iterator b = scene.createEntity();
    REQUIRE(scene.entityCount() == 0);

    a->activate();
    REQUIRE(!a->isActivated());
    REQUIRE(a->isPendingActivation());
    REQUIRE(scene.entityCount() == 0);

    scene.refresh();
    REQUIRE(a->isActivated());
    REQUIRE(!a->isPendingActivation());
    REQUIRE(scene.entityCount() == 1);

    a->destroy();
    REQUIRE(scene.entityCount() == 1);
    scene.refresh();
    REQUIRE(scene.entityCount() == 0);

    b->destroy();
    scene.refresh();
    REQUIRE(scene.entityCount() == 0);
}

TEST_CASE("Iterate over the entities in an empty scene", "[Scene]")
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

TEST_CASE("Iterate over the entities in a non-empty scene when no entities are activated", "[Scene]")
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

TEST_CASE("Iterate over the entities in a non-empty scene when some entities are activated", "[Scene]")
{
    Scene scene(*engine);

    scene.createEntity();
    scene.createEntity()->activate();
    scene.createEntity()->activate();
    scene.createEntity();
    scene.createEntity()->activate();
    scene.createEntity();

    scene.refresh();

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

TEST_CASE("Iterate over the entities in a non-empty scene when the first entity is activated", "[Scene]")
{
    Scene scene(*engine);

    scene.createEntity()->activate();
    scene.createEntity();
    scene.createEntity()->activate();
    scene.createEntity();

    scene.refresh();

    std::vector<EntityId> ids;
    for (const Entity& entity : scene.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 2);
}

TEST_CASE("Iterate over the entities in a non-empty scene when the last entity is activated", "[Scene]")
{
    Scene scene(*engine);

    scene.createEntity();
    scene.createEntity()->activate();
    scene.createEntity();
    scene.createEntity()->activate();

    scene.refresh();

    std::vector<EntityId> ids;
    for (const Entity& entity : scene.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 1);
    REQUIRE(ids[1] == 3);
}

TEST_CASE("Iterate over the entities in a non-empty scene when the first and last entities are activated", "[Scene]")
{
    Scene scene(*engine);

    scene.createEntity()->activate();
    scene.createEntity();
    scene.createEntity()->activate();

    scene.refresh();

    std::vector<EntityId> ids;
    for (const Entity& entity : scene.entities())
    {
        ids.push_back(entity.id());
    }

    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 2);
}

TEST_CASE("Create and activate many entities in a scene", "[Scene]")
{
    Scene scene(*engine);

    std::vector<Entity::Iterator> entities;
    for (EntityId id = 0; id < 128; ++id)
    {
        {
            Entity::Iterator entity = scene.createEntity();
            entity->activate();
            entities.push_back(entity);
        }

        scene.refresh();

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

TEST_CASE("Add and remove children to an unactivated entity", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    REQUIRE(!a->parent());

    Entity::Iterator b = scene.createEntity();
    REQUIRE(!b->parent());

    Entity::Iterator c = scene.createEntity();
    REQUIRE(!c->parent());

    a->addChild(*b);
    REQUIRE(&*b->parent() == &*a);

    a->removeChild(*b);
    REQUIRE(b);
    REQUIRE(!b->parent());
}

TEST_CASE("Add and remove children to an activated entity", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->activate();
    scene.refresh();
    REQUIRE(!a->parent());

    Entity::Iterator b = scene.createEntity();
    b->activate();
    scene.refresh();
    REQUIRE(!b->parent());

    Entity::Iterator c = scene.createEntity();
    c->activate();
    scene.refresh();
    REQUIRE(!c->parent());

    a->addChild(*b);
    REQUIRE(&*b->parent() == &*a);

    a->removeChild(*b);
    REQUIRE(b);
    REQUIRE(!b->parent());
}

TEST_CASE("Add a child entity as a child to another entity", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();

    REQUIRE_THROWS_AS(c->addChild(*b), InvalidOperation);
}

TEST_CASE("Add child to an entity pending activation", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->activate();

    Entity::Iterator b = scene.createEntity();

    REQUIRE_THROWS_AS(a->addChild(*b), InvalidOperation);
}

TEST_CASE("Add an unactivated child to an activated entity", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    a->activate();
    scene.refresh();

    Entity::Iterator b = scene.createEntity();

    REQUIRE_THROWS_AS(a->addChild(*b), InvalidOperation);
}

TEST_CASE("Add a child to an entity in another scene", "[Scene]")
{
    Scene sceneA(*engine);

    Entity::Iterator a = sceneA.createEntity();

    Scene sceneB(*engine);

    Entity::Iterator b = sceneB.createEntity();

    REQUIRE_THROWS_AS(a->addChild(*b), InvalidOperation);
}

TEST_CASE("Activating an entity activates all of its children", "[Scene]")
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

    REQUIRE(a->isPendingActivation());
    REQUIRE(b->isPendingActivation());
    REQUIRE(c->isPendingActivation());
    REQUIRE(d->isPendingActivation());

    scene.refresh();

    REQUIRE(a->isActivated());
    REQUIRE(b->isActivated());
    REQUIRE(c->isActivated());
    REQUIRE(d->isActivated());
}

TEST_CASE("Iterating over the children of an entity without any children", "[Scene]")
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

TEST_CASE("Iterating over the children of an entity with children", "[Scene]")
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

TEST_CASE("Destroy an entity with children", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    Entity::Iterator c = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);

    a->activate();

    scene.refresh();

    REQUIRE(scene.entityCount() == 3);

    a->destroy();
    REQUIRE(a->isPendingDestruction());
    REQUIRE(b->isPendingDestruction());
    REQUIRE(c->isPendingDestruction());

    scene.refresh();

    REQUIRE(!a);
    REQUIRE(!b);
    REQUIRE(!c);

    REQUIRE(scene.entityCount() == 0);
}

TEST_CASE("Destroy all children from an entity", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    Entity::Iterator c = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);

    a->activate();

    scene.refresh();

    REQUIRE(scene.entityCount() == 3);

    a->destroyAllChildren();
    REQUIRE(!a->isPendingDestruction());
    REQUIRE(b->isPendingDestruction());
    REQUIRE(c->isPendingDestruction());

    scene.refresh();

    REQUIRE(a);
    REQUIRE(!b);
    REQUIRE(!c);

    REQUIRE(scene.entityCount() == 1);
}

TEST_CASE("Destroy an entity with a parent", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    Entity::Iterator b = scene.createEntity();
    Entity::Iterator c = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);

    a->activate();

    scene.refresh();

    REQUIRE(scene.entityCount() == 3);

    b->destroy();
    REQUIRE(b);
    REQUIRE(b->isPendingDestruction());

    scene.refresh();

    REQUIRE(scene.entityCount() == 2);

    std::vector<EntityId> ids;
    for (Entity& child : a->children())
    {
        ids.push_back(child.id());
    }
    REQUIRE(ids.size() == 1);
    REQUIRE(ids[0] == 2);
}

TEST_CASE("Clone an entity", "[Scene]")
{
    Scene scene(*engine);

    REQUIRE(!scene.hasComponentType<Test>());
    scene.addComponentType<Test>();
    REQUIRE(scene.hasComponentType<Test>());

    Entity::Iterator a = scene.createEntity();
    Component<Test>::Iterator stringA = a->addComponent<Test>("Test");
    a->activate();

    scene.refresh();

    Entity::Iterator b = a->clone();
    Component<Test>::Iterator stringB = b->component<Test>();

    REQUIRE(a->isActivated());
    REQUIRE(!b->isActivated());
    REQUIRE(&*stringA->entity() == &*a);
    REQUIRE(&*stringB->entity() == &*b);
    REQUIRE(&*stringA != &*stringB);
    REQUIRE(stringA->id() != stringB->id());
}

TEST_CASE("Clone an entity with children", "[Scene]")
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

TEST_CASE("Add and remove a component to and from an entity", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();

    Component<Test>::Iterator string = a->addComponent<Test>("Test");
    REQUIRE(string == a->component<Test>());
    REQUIRE(string);
    REQUIRE(string->value == "Test");
    REQUIRE(&*string->entity() == &*a);

    a->removeComponent<Test>();
    REQUIRE(!string);
    REQUIRE(!a->component<Test>());
}

TEST_CASE("Replace a component of an entity", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();

    Component<Test>::Iterator string = a->addComponent<Test>("Test");
    Component<Test>::Iterator replacedString = a->replaceComponent<Test>("Replaced");
    REQUIRE(string == a->component<Test>());
    REQUIRE(replacedString == a->component<Test>());
    REQUIRE(string == replacedString);
    REQUIRE(replacedString);
    REQUIRE(replacedString->value == "Replaced");
    REQUIRE(&*string->entity() == &*a);
}

TEST_CASE("Remove a non-existing component from an entity", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    REQUIRE_THROWS_AS(a->removeComponent<Test>(), InvalidOperation);
}

TEST_CASE("Add a component that already exists for an entity", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("Test");
    REQUIRE_THROWS_AS(a->addComponent<Test>("Test"), InvalidOperation);
}

TEST_CASE("Remove a non-existing unregistered component from an entity", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();
    REQUIRE_THROWS_AS(a->removeComponent<Test>(), InvalidOperation);
}

TEST_CASE("Iterate over the components in a scene without any components", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    size_t count = 0;
    for (const Test& string : scene.components<Test>())
    {
        (void)string;
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Iterate over the components in a scene without any activated components", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    scene.createEntity()->addComponent<Test>("Test");

    size_t count = 0;
    for (const Test& string : scene.components<Test>())
    {
        (void)string;
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Iterate over the components in a scene with some activated components", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    scene.createEntity();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const Test& string : scene.components<Test>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("Iterate over the components in a scene with the first component activated", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const Test& string : scene.components<Test>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("Iterate over the components in a scene with the last component activated", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    scene.createEntity();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const Test& string : scene.components<Test>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("Iterate over the components in a scene with the first and last components activated", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Test string("Test");

    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();
    scene.createEntity();
    scene.createEntity()->addComponent<Test>("Test")->entity()->activate();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const Test& string : scene.components<Test>())
    {
        ids.push_back(string.id());
    }

    REQUIRE(ids.size() == 3);
    REQUIRE(ids[0] == 0);
    REQUIRE(ids[1] == 1);
    REQUIRE(ids[2] == 2);
}

TEST_CASE("Dispatch of the component add event", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    TestComponentPoolListener listener;
    scene.components<Test>().addListener(listener);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("A");
    a->activate();

    REQUIRE(listener.receivedEvents.size() == 0);

    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType_Add);
    REQUIRE(&*listener.receivedEvents[0].entity == &*a);
}

TEST_CASE("Dispatch of the component remove event", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    TestComponentPoolListener listener;

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("A");

    SECTION("For an activated entity")
    {
        a->activate();

        SECTION("With the scene refreshed before")
        {
            scene.refresh();
        }
    }

    scene.components<Test>().addListener(listener);
    a->destroy();

    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType_Remove);
}

TEST_CASE("Component pool listeners", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    TestComponentPoolListener listener;
    scene.components<Test>().addListener(listener);

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("A");
    REQUIRE(listener.receivedEvents.size() == 0);
    a->activate();
    REQUIRE(listener.receivedEvents.size() == 0);
    scene.refresh();
    REQUIRE(listener.receivedEvents.size() == 1);

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("B");
    REQUIRE(listener.receivedEvents.size() == 1);
    b->activate();
    REQUIRE(listener.receivedEvents.size() == 1);
    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 2);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType_Add);
    REQUIRE(&*listener.receivedEvents[0].entity == &*a);
    REQUIRE(listener.receivedEvents[1].type == ComponentEventType_Add);
    REQUIRE(&*listener.receivedEvents[1].entity == &*b);
    listener.receivedEvents.clear();

    a->destroy();
    REQUIRE(listener.receivedEvents.size() == 0);
    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType_Remove);
    REQUIRE(!listener.receivedEvents[0].entity);
    listener.receivedEvents.clear();

    b->removeComponent<Test>();
    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType_Remove);
    REQUIRE(&*listener.receivedEvents[0].entity == &*b);
}

TEST_CASE("Find first component in a component pool with a match", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<Test>& strings = scene.components<Test>();
    Component<Test>::Iterator iterator = strings.findFirst([](const Test& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iterator);
    REQUIRE(iterator->value == "Match");
    REQUIRE(&*iterator == &*b->component<Test>());
}

TEST_CASE("Find first component in a component pool without a match", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<Test>& strings = scene.components<Test>();
    Component<Test>::Iterator iterator = strings.findFirst([](const Test& string)
    {
        return string.value == "Match";
    });

    REQUIRE(!iterator);
}

TEST_CASE("Find components in a component pool with matches", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<Test>& strings = scene.components<Test>();
    std::vector<Component<Test>::Iterator> iters = strings.find([](const Test& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iters.size() == 2);

    REQUIRE(iters[0]);
    REQUIRE(iters[0]->value == "Match");
    REQUIRE(&*iters[0] == &*b->component<Test>());

    REQUIRE(iters[1]);
    REQUIRE(iters[1]->value == "Match");
    REQUIRE(&*iters[1] == &*c->component<Test>());
}

TEST_CASE("Find components in a component pool without matches", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<Test>& strings = scene.components<Test>();
    std::vector<Component<Test>::Iterator> iters = strings.find([](const Test& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("Find first entity in a entity pool with a match", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("NotMatch");
    d->activate();

    scene.refresh();

    Entity::Iterator iterator = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.component<Test>()->value == "Match";
    });

    REQUIRE(iterator);
    REQUIRE(&*iterator == &*b);
}

TEST_CASE("Find first entity in a entity pool without a match", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("NotMatch");
    d->activate();

    scene.refresh();

    Entity::Iterator iterator = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.component<Test>()->value == "Match";
    });

    REQUIRE(!iterator);
}

TEST_CASE("Find entities in a entity pool with matches", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("NotMatch");
    d->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters = scene.entities().find([](const Entity& entity)
    {
        return entity.component<Test>()->value == "Match";
    });

    REQUIRE(iters.size() == 2);

    REQUIRE(iters[0]);
    REQUIRE(&*iters[0] == &*b);

    REQUIRE(iters[1]);
    REQUIRE(&*iters[1] == &*c);
}

TEST_CASE("Find entities in a entity pool without matches", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("NotMatch");
    d->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters = scene.entities().find([](const Entity& entity)
    {
        return entity.component<Test>()->value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("Find first child entity", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    Entity::Iterator iterator = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<Test>()->value == "B";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == b);

    iterator = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<Test>()->value == "C";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == c);

    iterator = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<Test>()->value == "D";
    });
    REQUIRE(!iterator);
}

TEST_CASE("Find children entity", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters = a->findChildren([](const Entity& entity)
    {
        return entity.component<Test>()->value == "B" ||
               entity.component<Test>()->value == "C";
    });
    REQUIRE(iters.size() == 2);
    REQUIRE(iters[0] == b);
    REQUIRE(iters[1] == c);

    iters = a->findChildren([](const Entity& entity)
    {
        return entity.component<Test>()->value == "C";
    });
    REQUIRE(iters.size() == 1);
    REQUIRE(iters[0] == c);

    iters = a->findChildren([](const Entity& entity)
    {
        return entity.component<Test>()->value == "D";
    });
    REQUIRE(iters.size() == 0);
}

TEST_CASE("Find first descendant entity", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    Entity::Iterator iterator = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<Test>()->value == "B";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == b);

    iterator = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<Test>()->value == "C";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == c);

    iterator = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<Test>()->value == "D";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == d);

    iterator = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<Test>()->value == "E";
    });
    REQUIRE(!iterator);
}

TEST_CASE("Find descendant entities", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters = a->findDescendants([](const Entity& entity)
    {
        return entity.component<Test>()->value == "B" ||
               entity.component<Test>()->value == "C";
    });
    REQUIRE(iters.size() == 2);
    REQUIRE(iters[0] == b);
    REQUIRE(iters[1] == c);

    iters = a->findDescendants([](const Entity& entity)
    {
        return entity.component<Test>()->value == "C";
    });
    REQUIRE(iters.size() == 1);
    REQUIRE(iters[0] == c);

    iters = a->findDescendants([](const Entity& entity)
    {
        return entity.component<Test>()->value == "D";
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

TEST_CASE("Find first ancestor entity", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("A");

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    Entity::Iterator iterator = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<Test>()->value == "C";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == c);

    iterator = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<Test>()->value == "A";
    });
    REQUIRE(iterator);
    REQUIRE(iterator == a);

    iterator = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<Test>()->value == "E";
    });
    REQUIRE(!iterator);
}

TEST_CASE("Find ancestor entities", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();

    Entity::Iterator a = scene.createEntity();
    a->addComponent<Test>("A");

    Entity::Iterator b = scene.createEntity();
    b->addComponent<Test>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity();
    c->addComponent<Test>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity();
    d->addComponent<Test>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters = d->findAncestors([](const Entity& entity)
    {
        return entity.component<Test>()->value == "C" ||
               entity.component<Test>()->value == "A";
    });
    REQUIRE(iters.size() == 2);
    REQUIRE(iters[0] == c);
    REQUIRE(iters[1] == a);

    iters = d->findAncestors([](const Entity& entity)
    {
        return entity.component<Test>()->value == "A";
    });
    REQUIRE(iters.size() == 1);
    REQUIRE(iters[0] == a);

    iters = d->findAncestors([](const Entity& entity)
    {
        return entity.component<Test>()->value == "E";
    });
    REQUIRE(iters.size() == 0);
}

TEST_CASE("Create an entity handle", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Entity::Handle handle = a->createHandle();
    REQUIRE(handle);
    REQUIRE(&*handle == &*a);

    a->destroy();

    scene.refresh();

    REQUIRE(!handle);
    REQUIRE_THROWS_AS(*handle, InvalidOperation);
}

TEST_CASE("Copy an entity handle", "[Scene]")
{
    Scene scene(*engine);

    Entity::Iterator a = scene.createEntity();

    Entity::Handle handle = a->createHandle();
    Entity::Handle handleCopy = handle;
    REQUIRE(handle == handleCopy);
    REQUIRE(&*handle == &*a);
    REQUIRE(&*handleCopy == &*a);

    a->destroy();

    scene.refresh();

    REQUIRE(!handle);
    REQUIRE_THROWS_AS(*handle, InvalidOperation);
    REQUIRE(!handleCopy);
    REQUIRE_THROWS_AS(*handleCopy, InvalidOperation);
}

TEST_CASE("Add a system type to a scene", "[Scene]")
{
    Scene scene(*engine);

    REQUIRE(!scene.hasSystemType<TestSystem>());
    scene.addSystemType<TestSystem>();
    REQUIRE(scene.hasSystemType<TestSystem>());

    TestSystem& testSystemA = scene.system<TestSystem>();
    TestSystem& testSystemB = scene.system<TestSystem>();

    REQUIRE(&testSystemA == &testSystemB);
}

TEST_CASE("Add an existing system type to a scene", "[Scene]")
{
    Scene scene(*engine);

    scene.addSystemType<TestSystem>();
    REQUIRE_THROWS_AS(scene.addSystemType<TestSystem>(), InvalidOperation);
}

TEST_CASE("Get a non-existing system type", "[Scene]")
{
    Scene scene(*engine);

    REQUIRE_THROWS_AS(scene.system<TestSystem>(), InvalidOperation);
}

TEST_CASE("Remove an existing system type from a scene", "[Scene]")
{
    Scene scene(*engine);

    scene.addSystemType<TestSystem>();
    scene.removeSystemType<TestSystem>();
    REQUIRE(!scene.hasSystemType<TestSystem>());
}

TEST_CASE("Remove a non-existing system type from a scene", "[Scene]")
{
    Scene scene(*engine);

    REQUIRE_THROWS_AS(scene.removeSystemType<TestSystem>(), InvalidOperation);
}

TEST_CASE("Encode and decode a simple scene", "[Scene]")
{
    testEncodeDecode([](Scene& scene)
    {
        scene.addComponentType<Test>();

        Entity::Iterator a = scene.createEntity();
        a->addComponent<Test>("Test");
        a->activate();
        scene.refresh();
    }, [](Scene& scene)
    {
        scene.refresh();

        REQUIRE(scene.entityCount() == 1);

        Entity::Iterator a = scene.entities().begin();
        REQUIRE(a);
        REQUIRE(a->component<Test>()->value == "Test");
    });
}

TEST_CASE("Encode and decode a simple scene with children entities", "[Scene]")
{
    testEncodeDecode([](Scene& scene)
    {
        Entity::Iterator a = scene.createEntity();
        Entity::Iterator b = scene.createEntity();
        a->addChild(*b);
        a->activate();
        scene.refresh();
    }, [](Scene& scene)
    {
        scene.refresh();
        REQUIRE(scene.entityCount() == 2);

        Entity::Iterator a = scene.entities().begin();
        REQUIRE(a);

        Entity::Children::Iterator b = a->children().begin();
        REQUIRE(b);
    });
}

TEST_CASE("Encode and decode a simple scene with systems", "[Scene]")
{
    testEncodeDecode([](Scene& scene)
    {
        scene.addSystemType<TestSystem>();
        scene.system<TestSystem>().value = "Test";
    }, [](Scene& scene)
    {
        REQUIRE(scene.system<TestSystem>().value == "Test");
    });
}

TEST_CASE("Temp...", "[Scene]")
{
    Scene scene(*engine);
    scene.addComponentType<Test>();
    scene.addComponentType<TestB>();

    scene.addSystemType<TestSystem>();
    scene.addSystemType<TestSystemB>();

    Entity::Iterator entity = scene.createEntity();
    entity->addComponent<Test>();
    entity->activate();
    scene.refresh();

    REQUIRE(scene.system<TestSystemB>().value == "Test");

    entity->addComponent<TestB>();
    REQUIRE(scene.system<TestSystemB>().value == "TestB");
}