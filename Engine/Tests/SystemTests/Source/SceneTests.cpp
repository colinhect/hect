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
#include <Hect.h>
using namespace hect;

#include <catch.hpp>

class TestComponentA :
    public Component<TestComponentA>
{
public:
    std::string value;

    TestComponentA()
    {
    }

    TestComponentA(const std::string& value) :
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

class TestComponentB :
    public Component<TestComponentB>
{
public:
    std::string value;

    TestComponentB()
    {
    }

    TestComponentB(const std::string& value) :
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

class TestSystemA :
    public System<TestSystemA>
{
public:
    std::string value;

    TestSystemA(Scene& scene) :
        System(scene)
    {
    }

    void tick(Seconds timeStep)
    {
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
    public EventListener<ComponentEvent<TestComponentA>>
{
public:
    void receiveEvent(const ComponentEvent<TestComponentA>& event)
    {
        receivedEvents.push_back(event);
    }

    std::vector<ComponentEvent<TestComponentA>> receivedEvents;
};

class TestSystemB :
    public System<TestSystemB, Components<TestComponentA, TestComponentB>>
{
public:
    std::string value;

    TestSystemB(Scene& scene) :
        System(scene)
    {
    }

    void tick(Seconds timeStep)
    {
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

    void onComponentAdded(TestComponentA& test) override
    {
        (void)test;
        value = "TestA added";
    }

    void onComponentAdded(TestComponentB& test) override
    {
        (void)test;
        value = "TestB added";
    }
};

class TestScene :
    public Scene
{
public:
    TestScene(Engine& engine) :
        Scene(engine),
        testSystemA(*this),
        testSystemB(*this)
    {
    }

    void tick(Seconds timeStep) override
    {
        (void)timeStep;
    }

    void render(RenderTarget& target) override
    {
        (void)target;
    }

    TestSystemA testSystemA;
    TestSystemB testSystemB;
};

void testEncodeDecode(std::function<void(TestScene& scene)> createScene, std::function<void(TestScene& scene)> verifyScene)
{
    // Json
    DataValue dataValue;

    {
        TestScene scene(Engine::instance());
        createScene(scene);
        scene.refresh();

        DataValueEncoder encoder;
        encoder << encodeValue(scene);
        dataValue = encoder.dataValues()[0];
    }

    {
        TestScene scene(Engine::instance());

        DataValueDecoder decoder(dataValue);
        decoder >> decodeValue(scene);

        verifyScene(scene);
    }

    // Binary
    std::vector<uint8_t> data;

    {
        TestScene scene(Engine::instance());
        createScene(scene);
        scene.refresh();

        MemoryWriteStream writeStream(data);
        BinaryEncoder encoder(writeStream);
        encoder << encodeValue(scene);
    }

    {
        TestScene scene(Engine::instance());

        MemoryReadStream readStream(data);
        BinaryDecoder decoder(readStream);
        decoder >> decodeValue(scene);

        verifyScene(scene);
    }
}

TEST_CASE("Register a component type", "[Scene]")
{
    Type::create<TestComponentA>(Kind::Class, "TestA");
    ComponentRegistry::registerType<TestComponentA>();
    Type::create<TestComponentB>(Kind::Class, "TestB");
    ComponentRegistry::registerType<TestComponentB>();
}

TEST_CASE("Register a system type", "[Scene]")
{
    Type::create<TestSystemA>(Kind::Class, "TestSystemA");
    SystemRegistry::registerType<TestSystemA>();
    Type::create<TestSystemB>(Kind::Class, "TestSystemB");
    SystemRegistry::registerType<TestSystemB>();
}

TEST_CASE("Register a scene type", "[Scene]")
{
    Type::create<TestScene>(Kind::Class, "TestScene");
    SceneRegistry::registerType<TestScene>();
}

TEST_CASE("Create and destroy entities in a scene", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity("A").iterator();
    REQUIRE(a);
    REQUIRE(a->id() == 0);
    REQUIRE(a->name() == "A");

    Entity::Iterator b = scene.createEntity("B").iterator();
    REQUIRE(b);
    REQUIRE(b->id() == 1);
    REQUIRE(b->name() == "B");

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
    TestScene scene(Engine::instance());

    Entity::Iterator a;
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, InvalidOperation);

    Entity::Iterator b = scene.entities().end();
    REQUIRE(!b);
    REQUIRE_THROWS_AS(*b, InvalidOperation);
}

TEST_CASE("Dereference an iterator to a destroyed entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    REQUIRE(a);
    a->destroy();
    REQUIRE(a);
    scene.refresh();
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, InvalidOperation);
}

TEST_CASE("Create and activate entities in a scene", "[Scene]")
{
    TestScene scene(Engine::instance());

    REQUIRE(scene.entityCount() == 0);

    Entity::Iterator a = scene.createEntity().iterator();
    REQUIRE(scene.entityCount() == 0);

    Entity::Iterator b = scene.createEntity().iterator();
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
    TestScene scene(Engine::instance());

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
    TestScene scene(Engine::instance());

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
    TestScene scene(Engine::instance());

    scene.createEntity();
    scene.createEntity().activate();
    scene.createEntity().activate();
    scene.createEntity();
    scene.createEntity().activate();
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
    TestScene scene(Engine::instance());

    scene.createEntity().activate();
    scene.createEntity();
    scene.createEntity().activate();
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
    TestScene scene(Engine::instance());

    scene.createEntity();
    scene.createEntity().activate();
    scene.createEntity();
    scene.createEntity().activate();

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
    TestScene scene(Engine::instance());

    scene.createEntity().activate();
    scene.createEntity();
    scene.createEntity().activate();

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
    TestScene scene(Engine::instance());

    std::vector<Entity::Handle> entityHandles;
    std::vector<Entity*> entityPointers;
    for (EntityId id = 0; id < 256; ++id)
    {
        Entity& entity = scene.createEntity();
        entity.activate();

        entityHandles.push_back(entity.handle());
        entityPointers.push_back(&entity);

        scene.refresh();
    }

    EntityId id = 0;
    for (const Entity::Handle& entity : entityHandles)
    {
        REQUIRE(entity);
        REQUIRE(entity->id() == id);
        REQUIRE(&*entity == entityPointers[id]);
        ++id;
    }
}

TEST_CASE("Add and remove children to an unactivated entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    REQUIRE(!a->parent());

    Entity::Iterator b = scene.createEntity().iterator();
    REQUIRE(!b->parent());

    Entity::Iterator c = scene.createEntity().iterator();
    REQUIRE(!c->parent());

    a->addChild(*b);
    REQUIRE(&*b->parent() == &*a);

    a->removeChild(*b);
    REQUIRE(b);
    REQUIRE(!b->parent());
}

TEST_CASE("Add and remove children to an activated entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->activate();
    scene.refresh();
    REQUIRE(!a->parent());

    Entity::Iterator b = scene.createEntity().iterator();
    b->activate();
    scene.refresh();
    REQUIRE(!b->parent());

    Entity::Iterator c = scene.createEntity().iterator();
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
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    Entity::Iterator b = scene.createEntity().iterator();
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();

    REQUIRE_THROWS_AS(c->addChild(*b), InvalidOperation);
}

TEST_CASE("Add child to an entity pending activation", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();

    REQUIRE_THROWS_AS(a->addChild(*b), InvalidOperation);
}

TEST_CASE("Add an unactivated child to an activated entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->activate();
    scene.refresh();

    Entity::Iterator b = scene.createEntity().iterator();

    REQUIRE_THROWS_AS(a->addChild(*b), InvalidOperation);
}

TEST_CASE("Add a child to an entity in another scene", "[Scene]")
{
    TestScene sceneA(Engine::instance());

    Entity::Iterator a = sceneA.createEntity().iterator();

    TestScene sceneB(Engine::instance());

    Entity::Iterator b = sceneB.createEntity().iterator();

    REQUIRE_THROWS_AS(a->addChild(*b), InvalidOperation);
}

TEST_CASE("Activating an entity activates all of its children", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    Entity::Iterator b = scene.createEntity().iterator();
    Entity::Iterator c = scene.createEntity().iterator();
    Entity::Iterator d = scene.createEntity().iterator();

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
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

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
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    Entity::Iterator b = scene.createEntity().iterator();
    Entity::Iterator c = scene.createEntity().iterator();
    Entity::Iterator d = scene.createEntity().iterator();

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
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    Entity::Iterator b = scene.createEntity().iterator();
    Entity::Iterator c = scene.createEntity().iterator();

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
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    Entity::Iterator b = scene.createEntity().iterator();
    Entity::Iterator c = scene.createEntity().iterator();

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
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    Entity::Iterator b = scene.createEntity().iterator();
    Entity::Iterator c = scene.createEntity().iterator();

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
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    auto& stringA = a->addComponent<TestComponentA>("TestA");
    a->activate();

    scene.refresh();

    Entity::Iterator b = a->clone().iterator();
    auto& stringB = b->component<TestComponentA>();

    REQUIRE(a->isActivated());
    REQUIRE(!b->isActivated());
    REQUIRE(&stringA.entity() == &*a);
    REQUIRE(&stringB.entity() == &*b);
    REQUIRE(&stringA != &stringB);
    REQUIRE(stringA.id() != stringB.id());
}

TEST_CASE("Clone an entity with children", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    Entity::Iterator b = scene.createEntity().iterator();
    Entity::Iterator c = scene.createEntity().iterator();

    a->addChild(*b);
    a->addChild(*c);

    Entity::Iterator cloneA = a->clone().iterator();
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
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    auto& string = a->addComponent<TestComponentA>("TestA");
    REQUIRE(&string == &a->component<TestComponentA>());
    REQUIRE(string.value == "TestA");
    REQUIRE(&string.entity() == &*a);

    a->removeComponent<TestComponentA>();
}

TEST_CASE("Replace a component of an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    auto& string = a->addComponent<TestComponentA>("TestA");
    auto& replacedString = a->replaceComponent<TestComponentA>("Replaced");
    REQUIRE(&string == &a->component<TestComponentA>());
    REQUIRE(&replacedString == &a->component<TestComponentA>());
    REQUIRE(&string == &replacedString);
    REQUIRE(replacedString.value == "Replaced");
    REQUIRE(&string.entity() == &*a);
}

TEST_CASE("Remove a non-existing component from an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    REQUIRE_THROWS_AS(a->removeComponent<TestComponentA>(), InvalidOperation);
}

TEST_CASE("Add a component that already exists for an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("TestA");
    REQUIRE_THROWS_AS(a->addComponent<TestComponentA>("TestA"), InvalidOperation);
}

TEST_CASE("Remove a non-existing unregistered component from an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    REQUIRE_THROWS_AS(a->removeComponent<TestComponentA>(), InvalidOperation);
}

TEST_CASE("Check that an entity does not have a component of a specific type", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    REQUIRE(a->hasComponent<TestComponentA>() == false);
}

TEST_CASE("Check that an entity has a component of a specific type", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("TestA");

    REQUIRE(a->hasComponent<TestComponentA>() == true);
}

TEST_CASE("Iterate over the components in a scene without any components", "[Scene]")
{
    TestScene scene(Engine::instance());

    size_t count = 0;
    for (const TestComponentA& string : scene.components<TestComponentA>())
    {
        (void)string;
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Iterate over the components in a scene without any activated components", "[Scene]")
{
    TestScene scene(Engine::instance());

    scene.createEntity().addComponent<TestComponentA>("TestA");

    size_t count = 0;
    for (const TestComponentA& string : scene.components<TestComponentA>())
    {
        (void)string;
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Iterate over the components in a scene with some activated components", "[Scene]")
{
    TestScene scene(Engine::instance());

    scene.createEntity();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const TestComponentA& string : scene.components<TestComponentA>())
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
    TestScene scene(Engine::instance());

    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const TestComponentA& string : scene.components<TestComponentA>())
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
    TestScene scene(Engine::instance());

    scene.createEntity();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const TestComponentA& string : scene.components<TestComponentA>())
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
    TestScene scene(Engine::instance());

    TestComponentA string("TestA");

    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();
    scene.createEntity();
    scene.createEntity().addComponent<TestComponentA>("TestA").entity().activate();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const TestComponentA& string : scene.components<TestComponentA>())
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
    TestScene scene(Engine::instance());

    TestComponentPoolListener listener;
    scene.components<TestComponentA>().registerListener(listener);

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("A");
    a->activate();

    REQUIRE(listener.receivedEvents.size() == 0);

    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType::Add);
    REQUIRE(&*listener.receivedEvents[0].entity == &*a);
}

TEST_CASE("Dispatch of the component remove event", "[Scene]")
{
    TestScene scene(Engine::instance());

    TestComponentPoolListener listener;

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("A");

    SECTION("For an activated entity")
    {
        a->activate();

        SECTION("With the scene refreshed before")
        {
            scene.refresh();
        }
    }

    scene.components<TestComponentA>().registerListener(listener);
    a->destroy();

    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType::Remove);
}

TEST_CASE("Component pool listeners", "[Scene]")
{
    TestScene scene(Engine::instance());

    TestComponentPoolListener listener;
    scene.components<TestComponentA>().registerListener(listener);

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("A");
    REQUIRE(listener.receivedEvents.size() == 0);
    a->activate();
    REQUIRE(listener.receivedEvents.size() == 0);
    scene.refresh();
    REQUIRE(listener.receivedEvents.size() == 1);

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    REQUIRE(listener.receivedEvents.size() == 1);
    b->activate();
    REQUIRE(listener.receivedEvents.size() == 1);
    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 2);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType::Add);
    REQUIRE(&*listener.receivedEvents[0].entity == &*a);
    REQUIRE(listener.receivedEvents[1].type == ComponentEventType::Add);
    REQUIRE(&*listener.receivedEvents[1].entity == &*b);
    listener.receivedEvents.clear();

    a->destroy();
    REQUIRE(listener.receivedEvents.size() == 0);
    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType::Remove);
    REQUIRE(!listener.receivedEvents[0].entity);
    listener.receivedEvents.clear();

    b->removeComponent<TestComponentA>();
    scene.refresh();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentEventType::Remove);
    REQUIRE(&*listener.receivedEvents[0].entity == &*b);
}

TEST_CASE("Find first component in a component pool with a match", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<TestComponentA>& strings = scene.components<TestComponentA>();
    Component<TestComponentA>::Iterator iterator = strings.findFirst([](const TestComponentA& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iterator);
    REQUIRE(iterator->value == "Match");
    REQUIRE(&*iterator == &b->component<TestComponentA>());
}

TEST_CASE("Find first component in a component pool without a match", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<TestComponentA>& strings = scene.components<TestComponentA>();
    Component<TestComponentA>::Iterator iterator = strings.findFirst([](const TestComponentA& string)
    {
        return string.value == "Match";
    });

    REQUIRE(!iterator);
}

TEST_CASE("Find components in a component pool with matches", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<TestComponentA>& strings = scene.components<TestComponentA>();
    std::vector<Component<TestComponentA>::Iterator> iters = strings.find([](const TestComponentA& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iters.size() == 2);

    REQUIRE(iters[0]);
    REQUIRE(iters[0]->value == "Match");
    REQUIRE(&*iters[0] == &b->component<TestComponentA>());

    REQUIRE(iters[1]);
    REQUIRE(iters[1]->value == "Match");
    REQUIRE(&*iters[1] == &c->component<TestComponentA>());
}

TEST_CASE("Find components in a component pool without matches", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<TestComponentA>& strings = scene.components<TestComponentA>();
    std::vector<Component<TestComponentA>::Iterator> iters = strings.find([](const TestComponentA& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("Find first entity in a entity pool with a match", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    Entity::Handle handle = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "Match";
    });

    REQUIRE(handle);
    REQUIRE(&*handle == &*b);
}

TEST_CASE("Find first entity in a entity pool without a match", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    Entity::Handle handle = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "Match";
    });

    REQUIRE(!handle);
}

TEST_CASE("Find entities in a entity pool with matches", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("Match");
    b->activate();

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("Match");
    c->activate();

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    std::vector<Entity::Handle> handles = scene.entities().find([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "Match";
    });

    REQUIRE(handles.size() == 2);

    REQUIRE(handles[0]);
    REQUIRE(&*handles[0] == &*b);

    REQUIRE(handles[1]);
    REQUIRE(&*handles[1] == &*c);
}

TEST_CASE("Find entities in a entity pool without matches", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    std::vector<Entity::Handle> handle = scene.entities().find([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "Match";
    });

    REQUIRE(handle.size() == 0);
}

TEST_CASE("Find first child entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    Entity::Handle handle = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "B";
    });
    REQUIRE(handle);
    REQUIRE(handle == b);

    handle = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handle);
    REQUIRE(handle == c);

    handle = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "D";
    });
    REQUIRE(!handle);
}

TEST_CASE("Find children entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Handle> handles = a->findChildren([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "B" ||
               entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handles.size() == 2);
    REQUIRE(handles[0] == b);
    REQUIRE(handles[1] == c);

    handles = a->findChildren([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handles.size() == 1);
    REQUIRE(handles[0] == c);

    handles = a->findChildren([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "D";
    });
    REQUIRE(handles.size() == 0);
}

TEST_CASE("Iterate over children entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters;
    a->forChildren([&](Entity& entity)
    {
        iters.emplace_back(entity.iterator());
    });
    REQUIRE(iters.size() == 2);
    REQUIRE(iters[0] == b);
    REQUIRE(iters[1] == c);
}

TEST_CASE("Find first descendant entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    Entity::Handle handle = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "B";
    });
    REQUIRE(handle);
    REQUIRE(handle == b);

    handle = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handle);
    REQUIRE(handle == c);

    handle = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "D";
    });
    REQUIRE(handle);
    REQUIRE(handle == d);

    handle = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "E";
    });
    REQUIRE(!handle);
}

TEST_CASE("Find descendant entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Handle> handles = a->findDescendants([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "B" ||
               entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handles.size() == 2);
    REQUIRE(handles[0] == b);
    REQUIRE(handles[1] == c);

    handles = a->findDescendants([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handles.size() == 1);
    REQUIRE(handles[0] == c);

    handles = a->findDescendants([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "D";
    });
    REQUIRE(handles.size() == 1);
    REQUIRE(handles[0] == d);

    handles = a->findDescendants([](const Entity& entity)
    {
        (void)entity;
        return true;
    });
    REQUIRE(handles.size() == 3);
    REQUIRE(handles[0] == b);
    REQUIRE(handles[1] == c);
    REQUIRE(handles[2] == d);
}

TEST_CASE("Iterate over descendant entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters;
    a->forDescendants([&](Entity& entity)
    {
        iters.emplace_back(entity.iterator());
    });
    REQUIRE(iters.size() == 3);
    REQUIRE(iters[0] == b);
    REQUIRE(iters[1] == c);
    REQUIRE(iters[2] == d);
}

TEST_CASE("Find first ancestor entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("A");

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    Entity::Handle handle = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handle);
    REQUIRE(handle == c);

    handle = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "A";
    });
    REQUIRE(handle);
    REQUIRE(handle == a);

    handle = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "E";
    });
    REQUIRE(!handle);
}

TEST_CASE("Find ancestor entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("A");

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Handle> handles = d->findAncestors([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C" ||
               entity.component<TestComponentA>().value == "A";
    });
    REQUIRE(handles.size() == 2);
    REQUIRE(handles[0] == c);
    REQUIRE(handles[1] == a);

    handles = d->findAncestors([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "A";
    });
    REQUIRE(handles.size() == 1);
    REQUIRE(handles[0] == a);

    handles = d->findAncestors([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "E";
    });
    REQUIRE(handles.size() == 0);
}

TEST_CASE("Iterate over ancestor entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();
    a->addComponent<TestComponentA>("A");

    Entity::Iterator b = scene.createEntity().iterator();
    b->addComponent<TestComponentA>("B");
    a->addChild(*b);

    Entity::Iterator c = scene.createEntity().iterator();
    c->addComponent<TestComponentA>("C");
    a->addChild(*c);

    Entity::Iterator d = scene.createEntity().iterator();
    d->addComponent<TestComponentA>("D");
    c->addChild(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters;
    d->forAncestors([&](Entity& entity)
    {
        iters.emplace_back(entity.iterator());
    });
    REQUIRE(iters.size() == 2);
    REQUIRE(iters[0] == c);
    REQUIRE(iters[1] == a);
}

TEST_CASE("Create an entity handle", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    Entity::Handle handle = a->handle();
    REQUIRE(handle);
    REQUIRE(&*handle == &*a);

    a->destroy();

    scene.refresh();

    REQUIRE(!handle);
    REQUIRE_THROWS_AS(*handle, InvalidOperation);
}

TEST_CASE("Copy an entity handle", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.createEntity().iterator();

    Entity::Handle handle = a->handle();
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

TEST_CASE("Encode and decode a simple scene", "[Scene]")
{
    testEncodeDecode([](TestScene& scene)
    {
        // Create an entity
        Entity::Iterator a = scene.createEntity().iterator();

        // Add a transform component to the entity
        auto& transformComponent = a->addComponent<TransformComponent>();
        transformComponent.localPosition = Vector3(1, 2, 3);

        // Activate the entity and refresh the scene
        a->activate();
        scene.refresh();
    }, [](TestScene& scene)
    {
        scene.refresh();

        REQUIRE(scene.entityCount() == 1);

        Entity::Iterator a = scene.entities().begin();
        REQUIRE(a);
        REQUIRE(a->component<TransformComponent>().localPosition == Vector3(1, 2, 3));
    });
}

TEST_CASE("Encode and decode a simple scene with children entities", "[Scene]")
{
    testEncodeDecode([](TestScene& scene)
    {
        Entity::Iterator a = scene.createEntity().iterator();
        Entity::Iterator b = scene.createEntity().iterator();
        a->addChild(*b);
        a->activate();
        scene.refresh();
    }, [](TestScene& scene)
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
    testEncodeDecode([](TestScene& scene)
    {
        scene.testSystemA.value = "System A Value";
        scene.testSystemB.value = "System B Value";
    }, [](TestScene& scene)
    {
        REQUIRE(scene.testSystemA.value == "System A Value");
        REQUIRE(scene.testSystemB.value == "System B Value");
    });
}

TEST_CASE("Systems are notified about the additions and removals of component types specified", "[Scene]")
{
    Engine& engine = Engine::instance();
    TestScene scene(Engine::instance());

    Entity::Iterator entity = scene.createEntity().iterator();
    entity->addComponent<TestComponentA>();
    entity->activate();
    scene.refresh();

    REQUIRE(scene.testSystemB.value == "TestA added");

    entity->addComponent<TestComponentB>();
    REQUIRE(scene.testSystemB.value == "TestB added");
}
