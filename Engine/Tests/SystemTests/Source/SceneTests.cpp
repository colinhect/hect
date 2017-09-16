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

    void encode(Encoder& encoder) const override
    {
        encoder << encode_value("value", value);
    }

    void decode(Decoder& decoder) override
    {
        decoder >> decode_value("value", value);
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

    void encode(Encoder& encoder) const override
    {
        encoder << encode_value("value", value);
    }

    void decode(Decoder& decoder) override
    {
        decoder >> decode_value("value", value);
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

    void tick(Seconds time_step)
    {
        (void)time_step;
    }

    void encode(Encoder& encoder) const override
    {
        encoder << encode_value("value", value);
    }

    void decode(Decoder& decoder) override
    {
        decoder >> decode_value("value", value);
    }
};

class TestComponentPoolListener :
    public EventListener<ComponentEvent<TestComponentA>>
{
public:
    void receive_event(const ComponentEvent<TestComponentA>& event) override
    {
        received_events.push_back(event);
    }

    std::vector<ComponentEvent<TestComponentA>> received_events;
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

    void tick(Seconds time_step)
    {
        (void)time_step;
    }

    void encode(Encoder& encoder) const override
    {
        encoder << encode_value("value", value);
    }

    void decode(Decoder& decoder) override
    {
        decoder >> decode_value("value", value);
    }

    void on_component_added(TestComponentA& test) override
    {
        (void)test;
        value = "TestA added";
    }

    void on_component_added(TestComponentB& test) override
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
        test_system_a(*this),
        test_system_b(*this)
    {
    }

    void tick(Seconds time_step) override
    {
        (void)time_step;
    }

    void render(RenderTarget& target) override
    {
        (void)target;
    }

    TestSystemA test_system_a;
    TestSystemB test_system_b;
};

void test_encode_decode(std::function<void(TestScene& scene)> create_scene, std::function<void(TestScene& scene)> verify_scene)
{
    // Json
    DataValue data_value;

    {
        TestScene scene(Engine::instance());
        create_scene(scene);
        scene.refresh();

        DataValueEncoder encoder;
        encoder << encode_value(scene);
        data_value = encoder.data_values()[0];
    }

    {
        TestScene scene(Engine::instance());

        DataValueDecoder decoder(data_value);
        decoder >> decode_value(scene);

        verify_scene(scene);
    }

    // Binary
    std::vector<uint8_t> data;

    {
        TestScene scene(Engine::instance());
        create_scene(scene);
        scene.refresh();

        MemoryWriteStream write_stream(data);
        BinaryEncoder encoder(write_stream);
        encoder << encode_value(scene);
    }

    {
        TestScene scene(Engine::instance());

        MemoryReadStream read_stream(data);
        BinaryDecoder decoder(read_stream);
        decoder >> decode_value(scene);

        verify_scene(scene);
    }
}

TEST_CASE("Register a component type", "[Scene]")
{
    Type::create<TestComponentA>(Kind::Class, "TestA");
    ComponentRegistry::register_type<TestComponentA>();
    Type::create<TestComponentB>(Kind::Class, "TestB");
    ComponentRegistry::register_type<TestComponentB>();
}

TEST_CASE("Register a system type", "[Scene]")
{
    Type::create<TestSystemA>(Kind::Class, "TestSystemA");
    SystemRegistry::register_type<TestSystemA>();
    Type::create<TestSystemB>(Kind::Class, "TestSystemB");
    SystemRegistry::register_type<TestSystemB>();
}

TEST_CASE("Register a scene type", "[Scene]")
{
    Type::create<TestScene>(Kind::Class, "TestScene");
    SceneRegistry::register_type<TestScene>();
}

TEST_CASE("Create and destroy entities in a scene", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity("A").iterator();
    REQUIRE(a);
    REQUIRE(a->id() == 0);
    REQUIRE(a->name() == "A");

    Entity::Iterator b = scene.create_entity("B").iterator();
    REQUIRE(b);
    REQUIRE(b->id() == 1);
    REQUIRE(b->name() == "B");

    a->destroy();
    REQUIRE(a);
    REQUIRE(a->is_pending_destruction());

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

    Entity::Iterator a = scene.create_entity().iterator();
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

    REQUIRE(scene.entity_count() == 0);

    Entity::Iterator a = scene.create_entity().iterator();
    REQUIRE(scene.entity_count() == 0);

    Entity::Iterator b = scene.create_entity().iterator();
    REQUIRE(scene.entity_count() == 0);

    a->activate();
    REQUIRE(!a->is_activated());
    REQUIRE(a->is_pending_activation());
    REQUIRE(scene.entity_count() == 0);

    scene.refresh();
    REQUIRE(a->is_activated());
    REQUIRE(!a->is_pending_activation());
    REQUIRE(scene.entity_count() == 1);

    a->destroy();
    REQUIRE(scene.entity_count() == 1);
    scene.refresh();
    REQUIRE(scene.entity_count() == 0);

    b->destroy();
    scene.refresh();
    REQUIRE(scene.entity_count() == 0);
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

    scene.create_entity();
    scene.create_entity();
    scene.create_entity();

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

    scene.create_entity();
    scene.create_entity().activate();
    scene.create_entity().activate();
    scene.create_entity();
    scene.create_entity().activate();
    scene.create_entity();

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

    scene.create_entity().activate();
    scene.create_entity();
    scene.create_entity().activate();
    scene.create_entity();

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

    scene.create_entity();
    scene.create_entity().activate();
    scene.create_entity();
    scene.create_entity().activate();

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

    scene.create_entity().activate();
    scene.create_entity();
    scene.create_entity().activate();

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

    std::vector<Entity::Handle> entity_handles;
    std::vector<Entity*> entity_pointers;
    for (EntityId id = 0; id < 256; ++id)
    {
        Entity& entity = scene.create_entity();
        entity.activate();

        entity_handles.push_back(entity.handle());
        entity_pointers.push_back(&entity);

        scene.refresh();
    }

    EntityId id = 0;
    for (const Entity::Handle& entity : entity_handles)
    {
        REQUIRE(entity);
        REQUIRE(entity->id() == id);
        REQUIRE(&*entity == entity_pointers[id]);
        ++id;
    }
}

TEST_CASE("Add and remove children to an unactivated entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    REQUIRE(!a->parent());

    Entity::Iterator b = scene.create_entity().iterator();
    REQUIRE(!b->parent());

    Entity::Iterator c = scene.create_entity().iterator();
    REQUIRE(!c->parent());

    a->add_child(*b);
    REQUIRE(&*b->parent() == &*a);

    a->remove_child(*b);
    REQUIRE(b);
    REQUIRE(!b->parent());
}

TEST_CASE("Add and remove children to an activated entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->activate();
    scene.refresh();
    REQUIRE(!a->parent());

    Entity::Iterator b = scene.create_entity().iterator();
    b->activate();
    scene.refresh();
    REQUIRE(!b->parent());

    Entity::Iterator c = scene.create_entity().iterator();
    c->activate();
    scene.refresh();
    REQUIRE(!c->parent());

    a->add_child(*b);
    REQUIRE(&*b->parent() == &*a);

    a->remove_child(*b);
    REQUIRE(b);
    REQUIRE(!b->parent());
}

TEST_CASE("Add a child entity as a child to another entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    Entity::Iterator b = scene.create_entity().iterator();
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();

    REQUIRE_THROWS_AS(c->add_child(*b), InvalidOperation);
}

TEST_CASE("Add child to an entity pending activation", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();

    REQUIRE_THROWS_AS(a->add_child(*b), InvalidOperation);
}

TEST_CASE("Add an unactivated child to an activated entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->activate();
    scene.refresh();

    Entity::Iterator b = scene.create_entity().iterator();

    REQUIRE_THROWS_AS(a->add_child(*b), InvalidOperation);
}

TEST_CASE("Add a child to an entity in another scene", "[Scene]")
{
    TestScene scene_a(Engine::instance());

    Entity::Iterator a = scene_a.create_entity().iterator();

    TestScene scene_b(Engine::instance());

    Entity::Iterator b = scene_b.create_entity().iterator();

    REQUIRE_THROWS_AS(a->add_child(*b), InvalidOperation);
}

TEST_CASE("Activating an entity activates all of its children", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    Entity::Iterator b = scene.create_entity().iterator();
    Entity::Iterator c = scene.create_entity().iterator();
    Entity::Iterator d = scene.create_entity().iterator();

    a->add_child(*b);
    a->add_child(*c);
    c->add_child(*d);

    a->activate();

    REQUIRE(a->is_pending_activation());
    REQUIRE(b->is_pending_activation());
    REQUIRE(c->is_pending_activation());
    REQUIRE(d->is_pending_activation());

    scene.refresh();

    REQUIRE(a->is_activated());
    REQUIRE(b->is_activated());
    REQUIRE(c->is_activated());
    REQUIRE(d->is_activated());
}

TEST_CASE("Iterating over the children of an entity without any children", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();

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

    Entity::Iterator a = scene.create_entity().iterator();
    Entity::Iterator b = scene.create_entity().iterator();
    Entity::Iterator c = scene.create_entity().iterator();
    Entity::Iterator d = scene.create_entity().iterator();

    a->add_child(*b);
    a->add_child(*c);
    c->add_child(*d);

    std::vector<EntityId> ids;
    for (Entity& child : a->children())
    {
        ids.push_back(child.id());
    }
    REQUIRE(ids.size() == 2);
    REQUIRE(ids[0] == 1);
    REQUIRE(ids[1] == 2);
    ids.clear();

    a->remove_child(*b);

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

    Entity::Iterator a = scene.create_entity().iterator();
    Entity::Iterator b = scene.create_entity().iterator();
    Entity::Iterator c = scene.create_entity().iterator();

    a->add_child(*b);
    a->add_child(*c);

    a->activate();

    scene.refresh();

    REQUIRE(scene.entity_count() == 3);

    a->destroy();
    REQUIRE(a->is_pending_destruction());
    REQUIRE(b->is_pending_destruction());
    REQUIRE(c->is_pending_destruction());

    scene.refresh();

    REQUIRE(!a);
    REQUIRE(!b);
    REQUIRE(!c);

    REQUIRE(scene.entity_count() == 0);
}

TEST_CASE("Destroy all children from an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    Entity::Iterator b = scene.create_entity().iterator();
    Entity::Iterator c = scene.create_entity().iterator();

    a->add_child(*b);
    a->add_child(*c);

    a->activate();

    scene.refresh();

    REQUIRE(scene.entity_count() == 3);

    a->destroy_all_children();
    REQUIRE(!a->is_pending_destruction());
    REQUIRE(b->is_pending_destruction());
    REQUIRE(c->is_pending_destruction());

    scene.refresh();

    REQUIRE(a);
    REQUIRE(!b);
    REQUIRE(!c);

    REQUIRE(scene.entity_count() == 1);
}

TEST_CASE("Destroy an entity with a parent", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    Entity::Iterator b = scene.create_entity().iterator();
    Entity::Iterator c = scene.create_entity().iterator();

    a->add_child(*b);
    a->add_child(*c);

    a->activate();

    scene.refresh();

    REQUIRE(scene.entity_count() == 3);

    b->destroy();
    REQUIRE(b);
    REQUIRE(b->is_pending_destruction());

    scene.refresh();

    REQUIRE(scene.entity_count() == 2);

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

    Entity::Iterator a = scene.create_entity().iterator();
    auto& string_a = a->add_component<TestComponentA>("TestA");
    a->activate();

    scene.refresh();

    Entity::Iterator b = a->clone().iterator();
    auto& string_b = b->component<TestComponentA>();

    REQUIRE(a->is_activated());
    REQUIRE(!b->is_activated());
    REQUIRE(&string_a.entity() == &*a);
    REQUIRE(&string_b.entity() == &*b);
    REQUIRE(&string_a != &string_b);
    REQUIRE(string_a.id() != string_b.id());
}

TEST_CASE("Clone an entity with children", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    Entity::Iterator b = scene.create_entity().iterator();
    Entity::Iterator c = scene.create_entity().iterator();

    a->add_child(*b);
    a->add_child(*c);

    Entity::Iterator clone_a = a->clone().iterator();
    Entity::Children::Iterator child_iter = clone_a->children().begin();
    REQUIRE(&*child_iter->parent() == &*clone_a);
    REQUIRE(&*child_iter != &*b);
    REQUIRE(++child_iter);
    REQUIRE(&*child_iter->parent() == &*clone_a);
    REQUIRE(&*child_iter != &*c);
    REQUIRE(!++child_iter);
}

TEST_CASE("Create an entity with components", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity& a = scene.create_entity_with<TestComponentA, TestComponentB>();

    REQUIRE(a.has_component<TestComponentA>());
    REQUIRE(a.has_component<TestComponentB>());
}

TEST_CASE("Add and remove a component to and from an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();

    auto& string = a->add_component<TestComponentA>("TestA");
    REQUIRE(&string == &a->component<TestComponentA>());
    REQUIRE(string.value == "TestA");
    REQUIRE(&string.entity() == &*a);

    a->remove_component<TestComponentA>();
}

TEST_CASE("Replace a component of an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();

    auto& string = a->add_component<TestComponentA>("TestA");
    auto& replaced_string = a->replace_component<TestComponentA>("Replaced");
    REQUIRE(&string == &a->component<TestComponentA>());
    REQUIRE(&replaced_string == &a->component<TestComponentA>());
    REQUIRE(&string == &replaced_string);
    REQUIRE(replaced_string.value == "Replaced");
    REQUIRE(&string.entity() == &*a);
}

TEST_CASE("Remove a non-existing component from an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    REQUIRE_THROWS_AS(a->remove_component<TestComponentA>(), InvalidOperation);
}

TEST_CASE("Add a component that already exists for an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("TestA");
    REQUIRE_THROWS_AS(a->add_component<TestComponentA>("TestA"), InvalidOperation);
}

TEST_CASE("Remove a non-existing unregistered component from an entity", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    REQUIRE_THROWS_AS(a->remove_component<TestComponentA>(), InvalidOperation);
}

TEST_CASE("Check that an entity does not have a component of a specific type", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();

    REQUIRE(a->has_component<TestComponentA>() == false);
}

TEST_CASE("Check that an entity has a component of a specific type", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("TestA");

    REQUIRE(a->has_component<TestComponentA>() == true);
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

    scene.create_entity().add_component<TestComponentA>("TestA");

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

    scene.create_entity();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity();

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

    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity();

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

    scene.create_entity();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();

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

    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();
    scene.create_entity();
    scene.create_entity().add_component<TestComponentA>("TestA").entity().activate();

    scene.refresh();

    std::vector<ComponentId> ids;
    for (const TestComponentA& test_component : scene.components<TestComponentA>())
    {
        ids.push_back(test_component.id());
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
    scene.components<TestComponentA>().register_listener(listener);

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("A");
    a->activate();

    REQUIRE(listener.received_events.size() == 0);

    scene.refresh();

    REQUIRE(listener.received_events.size() == 1);
    REQUIRE(listener.received_events[0].type == ComponentEventType::Add);
    REQUIRE(&*listener.received_events[0].entity == &*a);
}

TEST_CASE("Dispatch of the component remove event", "[Scene]")
{
    TestScene scene(Engine::instance());

    TestComponentPoolListener listener;

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("A");

    SECTION("For an activated entity")
    {
        a->activate();

        SECTION("With the scene refreshed before")
        {
            scene.refresh();
        }
    }

    scene.components<TestComponentA>().register_listener(listener);
    a->destroy();

    scene.refresh();

    REQUIRE(listener.received_events.size() == 1);
    REQUIRE(listener.received_events[0].type == ComponentEventType::Remove);
}

TEST_CASE("Component pool listeners", "[Scene]")
{
    TestScene scene(Engine::instance());

    TestComponentPoolListener listener;
    scene.components<TestComponentA>().register_listener(listener);

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("A");
    REQUIRE(listener.received_events.size() == 0);
    a->activate();
    REQUIRE(listener.received_events.size() == 0);
    scene.refresh();
    REQUIRE(listener.received_events.size() == 1);

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    REQUIRE(listener.received_events.size() == 1);
    b->activate();
    REQUIRE(listener.received_events.size() == 1);
    scene.refresh();

    REQUIRE(listener.received_events.size() == 2);
    REQUIRE(listener.received_events[0].type == ComponentEventType::Add);
    REQUIRE(&*listener.received_events[0].entity == &*a);
    REQUIRE(listener.received_events[1].type == ComponentEventType::Add);
    REQUIRE(&*listener.received_events[1].entity == &*b);
    listener.received_events.clear();

    a->destroy();
    REQUIRE(listener.received_events.size() == 0);
    scene.refresh();

    REQUIRE(listener.received_events.size() == 1);
    REQUIRE(listener.received_events[0].type == ComponentEventType::Remove);
    REQUIRE(!listener.received_events[0].entity);
    listener.received_events.clear();

    b->remove_component<TestComponentA>();
    scene.refresh();

    REQUIRE(listener.received_events.size() == 1);
    REQUIRE(listener.received_events[0].type == ComponentEventType::Remove);
    REQUIRE(&*listener.received_events[0].entity == &*b);
}

TEST_CASE("Find first component in a component pool with a match", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("Match");
    b->activate();

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("Match");
    c->activate();

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<TestComponentA>& strings = scene.components<TestComponentA>();
    Component<TestComponentA>::Handle handle = strings.find_first([](const TestComponentA& string)
    {
        return string.value == "Match";
    });

    REQUIRE(handle);
    REQUIRE(handle->value == "Match");
    REQUIRE(&*handle == &b->component<TestComponentA>());
}

TEST_CASE("Find first component in a component pool without a match", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<TestComponentA>& strings = scene.components<TestComponentA>();
    Component<TestComponentA>::Handle handle = strings.find_first([](const TestComponentA& string)
    {
        return string.value == "Match";
    });

    REQUIRE(!handle);
}

TEST_CASE("Find components in a component pool with matches", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("Match");
    b->activate();

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("Match");
    c->activate();

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<TestComponentA>& strings = scene.components<TestComponentA>();
    std::vector<Component<TestComponentA>::Handle> handles = strings.find([](const TestComponentA& string)
    {
        return string.value == "Match";
    });

    REQUIRE(handles.size() == 2);

    REQUIRE(handles[0]);
    REQUIRE(handles[0]->value == "Match");
    REQUIRE(&*handles[0] == &b->component<TestComponentA>());

    REQUIRE(handles[1]);
    REQUIRE(handles[1]->value == "Match");
    REQUIRE(&*handles[1] == &c->component<TestComponentA>());
}

TEST_CASE("Find components in a component pool without matches", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    ComponentPool<TestComponentA>& strings = scene.components<TestComponentA>();
    std::vector<Component<TestComponentA>::Handle> handles = strings.find([](const TestComponentA& string)
    {
        return string.value == "Match";
    });

    REQUIRE(handles.size() == 0);
}

TEST_CASE("Find first entity in a entity pool with a match", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("Match");
    b->activate();

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("Match");
    c->activate();

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    Entity::Handle handle = scene.entities().find_first([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "Match";
    });

    REQUIRE(handle);
    REQUIRE(&*handle == &*b);
}

TEST_CASE("Find first entity in a entity pool without a match", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("NotMatch");
    d->activate();

    scene.refresh();

    Entity::Handle handle = scene.entities().find_first([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "Match";
    });

    REQUIRE(!handle);
}

TEST_CASE("Find entities in a entity pool with matches", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("Match");
    b->activate();

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("Match");
    c->activate();

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("NotMatch");
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

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("NotMatch");
    a->activate();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("NotMatch");
    b->activate();

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("NotMatch");
    c->activate();

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("NotMatch");
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

    Entity::Iterator a = scene.create_entity().iterator();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    Entity::Handle handle = a->find_first_child([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "B";
    });
    REQUIRE(handle);
    REQUIRE(handle == b);

    handle = a->find_first_child([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handle);
    REQUIRE(handle == c);

    handle = a->find_first_child([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "D";
    });
    REQUIRE(!handle);
}

TEST_CASE("Find children entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Handle> handles = a->find_children([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "B" ||
               entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handles.size() == 2);
    REQUIRE(handles[0] == b);
    REQUIRE(handles[1] == c);

    handles = a->find_children([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handles.size() == 1);
    REQUIRE(handles[0] == c);

    handles = a->find_children([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "D";
    });
    REQUIRE(handles.size() == 0);
}

TEST_CASE("Iterate over children entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters;
    a->for_children([&](Entity& entity)
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

    Entity::Iterator a = scene.create_entity().iterator();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    Entity::Handle handle = a->find_first_descendant([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "B";
    });
    REQUIRE(handle);
    REQUIRE(handle == b);

    handle = a->find_first_descendant([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handle);
    REQUIRE(handle == c);

    handle = a->find_first_descendant([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "D";
    });
    REQUIRE(handle);
    REQUIRE(handle == d);

    handle = a->find_first_descendant([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "E";
    });
    REQUIRE(!handle);
}

TEST_CASE("Find descendant entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Handle> handles = a->find_descendants([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "B" ||
               entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handles.size() == 2);
    REQUIRE(handles[0] == b);
    REQUIRE(handles[1] == c);

    handles = a->find_descendants([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handles.size() == 1);
    REQUIRE(handles[0] == c);

    handles = a->find_descendants([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "D";
    });
    REQUIRE(handles.size() == 1);
    REQUIRE(handles[0] == d);

    handles = a->find_descendants([](const Entity& entity)
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

    Entity::Iterator a = scene.create_entity().iterator();

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters;
    a->for_descendants([&](Entity& entity)
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

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("A");

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    Entity::Handle handle = d->find_first_ancestor([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C";
    });
    REQUIRE(handle);
    REQUIRE(handle == c);

    handle = d->find_first_ancestor([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "A";
    });
    REQUIRE(handle);
    REQUIRE(handle == a);

    handle = d->find_first_ancestor([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "E";
    });
    REQUIRE(!handle);
}

TEST_CASE("Find ancestor entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("A");

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Handle> handles = d->find_ancestors([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "C" ||
               entity.component<TestComponentA>().value == "A";
    });
    REQUIRE(handles.size() == 2);
    REQUIRE(handles[0] == c);
    REQUIRE(handles[1] == a);

    handles = d->find_ancestors([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "A";
    });
    REQUIRE(handles.size() == 1);
    REQUIRE(handles[0] == a);

    handles = d->find_ancestors([](const Entity& entity)
    {
        return entity.component<TestComponentA>().value == "E";
    });
    REQUIRE(handles.size() == 0);
}

TEST_CASE("Iterate over ancestor entities", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator a = scene.create_entity().iterator();
    a->add_component<TestComponentA>("A");

    Entity::Iterator b = scene.create_entity().iterator();
    b->add_component<TestComponentA>("B");
    a->add_child(*b);

    Entity::Iterator c = scene.create_entity().iterator();
    c->add_component<TestComponentA>("C");
    a->add_child(*c);

    Entity::Iterator d = scene.create_entity().iterator();
    d->add_component<TestComponentA>("D");
    c->add_child(*d);

    a->activate();

    scene.refresh();

    std::vector<Entity::Iterator> iters;
    d->for_ancestors([&](Entity& entity)
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

    Entity::Iterator a = scene.create_entity().iterator();

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

    Entity::Iterator a = scene.create_entity().iterator();

    Entity::Handle handle = a->handle();
    Entity::Handle handle_copy = handle;
    REQUIRE(handle == handle_copy);
    REQUIRE(&*handle == &*a);
    REQUIRE(&*handle_copy == &*a);

    a->destroy();

    scene.refresh();

    REQUIRE(!handle);
    REQUIRE_THROWS_AS(*handle, InvalidOperation);
    REQUIRE(!handle_copy);
    REQUIRE_THROWS_AS(*handle_copy, InvalidOperation);
}

TEST_CASE("Encode and decode a simple scene", "[Scene]")
{
    test_encode_decode([](TestScene& scene)
    {
        // Create an entity
        Entity::Iterator a = scene.create_entity().iterator();

        // Add a transform component to the entity
        auto& transform_component = a->add_component<TransformComponent>();
        transform_component.local_position = Vector3(1, 2, 3);

        // Activate the entity and refresh the scene
        a->activate();
        scene.refresh();
    }, [](TestScene& scene)
    {
        scene.refresh();

        REQUIRE(scene.entity_count() == 1);

        Entity::Iterator a = scene.entities().begin();
        REQUIRE(a);
        REQUIRE(a->component<TransformComponent>().local_position == Vector3(1, 2, 3));
    });
}

TEST_CASE("Encode and decode a simple scene with children entities", "[Scene]")
{
    test_encode_decode([](TestScene& scene)
    {
        Entity::Iterator a = scene.create_entity().iterator();
        Entity::Iterator b = scene.create_entity().iterator();
        a->add_child(*b);
        a->activate();
        scene.refresh();
    }, [](TestScene& scene)
    {
        scene.refresh();
        REQUIRE(scene.entity_count() == 2);

        Entity::Iterator a = scene.entities().begin();
        REQUIRE(a);

        Entity::Children::Iterator b = a->children().begin();
        REQUIRE(b);
    });
}

TEST_CASE("Encode and decode a simple scene with systems", "[Scene]")
{
    test_encode_decode([](TestScene& scene)
    {
        scene.test_system_a.value = "System A Value";
        scene.test_system_b.value = "System B Value";
    }, [](TestScene& scene)
    {
        REQUIRE(scene.test_system_a.value == "System A Value");
        REQUIRE(scene.test_system_b.value == "System B Value");
    });
}

TEST_CASE("Systems are notified about the additions and removals of component types specified", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Iterator entity = scene.create_entity().iterator();
    entity->add_component<TestComponentA>();
    entity->activate();
    scene.refresh();

    REQUIRE(scene.test_system_b.value == "TestA added");

    entity->add_component<TestComponentB>();
    REQUIRE(scene.test_system_b.value == "TestB added");
}

TEST_CASE("Entity handle is invalidated when entity is destroyed", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity::Handle entity = scene.create_entity().handle();
    REQUIRE(entity);

    entity->activate();
    scene.refresh();
    REQUIRE(entity);

    entity->destroy();
    REQUIRE(entity);

    scene.refresh();
    REQUIRE(!entity);
}

TEST_CASE("Component handle is invalidated when entity is destroyed", "[Scene]")
{
    TestScene scene(Engine::instance());

    Entity& entity = scene.create_entity();

    TestComponentA::Handle component = entity.add_component<TestComponentA>().handle();
    REQUIRE(component);

    entity.activate();
    REQUIRE(component);

    scene.refresh();
    REQUIRE(component);

    entity.destroy();
    REQUIRE(component);

    scene.refresh();
    REQUIRE(!component);
}
