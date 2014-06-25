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

    void encode(ObjectEncoder& encoder) const
    {
        encoder.encodeString("value", value);
    }

    void decode(ObjectDecoder& decoder, AssetCache& assetCache)
    {
        assetCache;

        if (decoder.hasMember("value"))
        {
            value = decoder.decodeString("value");
        }
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

TEST_CASE("Scene_CreateAndDestroyEntities")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    REQUIRE(a);
    REQUIRE(a->name() == "A");
    REQUIRE(a->id() == 0);

    Entity::Iter b = scene.createEntity("B");
    REQUIRE(b);
    REQUIRE(b->name() == "B");
    REQUIRE(b->id() == 1);

    a->destroy();
    REQUIRE(!a);

    REQUIRE(b);
    REQUIRE(b->name() == "B");
    REQUIRE(b->id() == 1);
}

TEST_CASE("Scene_DereferenceInvalidEntityIter")
{
    Scene scene;

    Entity::Iter a = scene.entities().end();
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, Error);
}

TEST_CASE("Scene_DereferenceDestroyedEntityIter")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    REQUIRE(a);
    a->destroy();
    REQUIRE(!a);
    REQUIRE_THROWS_AS(*a, Error);
}

TEST_CASE("Scene_CreateAndActivateEntities")
{
    Scene scene;

    REQUIRE(scene.entityCount() == 0);

    Entity::Iter a = scene.createEntity();
    REQUIRE(scene.entityCount() == 0);

    Entity::Iter b = scene.createEntity();
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
    Scene scene;

    size_t count = 0;
    for (const Entity& entity : scene.entities())
    {
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Scene_EntityIterationNoneActivated")
{
    Scene scene;

    scene.createEntity();
    scene.createEntity();
    scene.createEntity();

    size_t count = 0;
    for (const Entity& entity : scene.entities())
    {
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Scene_EntityIterationSomeActivated")
{
    Scene scene;

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
    Scene scene;

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
    Scene scene;

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
    Scene scene;

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
    Scene scene;

    std::vector<Entity::Iter> entities;
    for (EntityId id = 0; id < 64; ++id)
    {
        {
            std::string name = format("Entity %d", id);
            Entity::Iter entity = scene.createEntity(name);
            entity->activate();
            entities.push_back(entity);
        }

        {
            EntityId id = 0;
            for (const Entity::Iter& entity : entities)
            {
                std::string name = format("Entity %d", id);

                REQUIRE(entity);
                REQUIRE(entity->id() == id);
                REQUIRE(entity->name() == name);

                ++id;
            }
        }
    }
}

TEST_CASE("Scene_AddAndRemoveComponent")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("A");

    Component<String>::Iter string = a->addComponent(String("Test"));
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
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("A");

    Component<String>::Iter string = a->addComponent(String("Test"));
    Component<String>::Iter replacedString = a->replaceComponent(String("Replaced"));
    REQUIRE(string == a->component<String>());
    REQUIRE(replacedString == a->component<String>());
    REQUIRE(string == replacedString);
    REQUIRE(replacedString);
    REQUIRE(replacedString->value == "Replaced");
    REQUIRE(&string->entity() == &*a);
}

TEST_CASE("Scene_AddUnregisteredComponent")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    REQUIRE_THROWS_AS(a->addComponent(String("Test")), Error);
}

TEST_CASE("Scene_AddExistingComponent")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("A");
    REQUIRE_THROWS_AS(a->removeComponent<String>(), Error);
}

TEST_CASE("Scene_RemoveNonExistingComponent")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("A");
    a->addComponent(String("Test"));
    REQUIRE_THROWS_AS(a->addComponent(String("Test")), Error);
}

TEST_CASE("Scene_RemoveNonExistingUnregisteredComponent")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    REQUIRE_THROWS_AS(a->removeComponent<String>(), Error);
}

TEST_CASE("Scene_CloneEntity")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("A");
    Component<String>::Iter stringA = a->addComponent(String("Test"));
    a->activate();

    Entity::Iter b = a->clone("B");
    Component<String>::Iter stringB = b->component<String>();

    REQUIRE(a->name() == "A");
    REQUIRE(a->isActivated());
    REQUIRE(b->name() == "B");
    REQUIRE(!b->isActivated());
    REQUIRE(&stringA->entity() == &*a);
    REQUIRE(&stringB->entity() == &*b);
    REQUIRE(&*stringA != &*stringB);
    REQUIRE(stringA->id() != stringB->id());
}

TEST_CASE("Scene_Decode")
{
    Scene scene;
    scene.registerComponent<String>("String");

    REQUIRE(scene.entityCount() == 0);

    JsonValue jsonValue;
    jsonValue.decodeFromJson("{ \"entities\" : [ { \"name\" : \"A\", \"components\" : [ { \"type\" : \"String\", \"value\" : \"Test\" } ] } ] }");

    scene.decodeFromJsonValue(jsonValue);

    REQUIRE(scene.entityCount() == 1);

    Entity::Iter a = scene.entities().begin();
    REQUIRE(a);
    REQUIRE(a->name() == "A");
    REQUIRE(a->component<String>()->value == "Test");
}

TEST_CASE("Scene_Encode")
{
    JsonValue jsonValue;

    {
        Scene scene;
        scene.registerComponent<String>("String");

        Entity::Iter a = scene.createEntity("A");
        a->addComponent(String("Test"));
        a->activate();

        jsonValue = scene.encodeToJsonValue();
    }

    {
        Scene scene;
        scene.registerComponent<String>("String");
        scene.decodeFromJsonValue(jsonValue);

        REQUIRE(scene.entityCount() == 1);

        Entity::Iter a = scene.entities().begin();
        REQUIRE(a);
        REQUIRE(a->name() == "A");
        REQUIRE(a->component<String>()->value == "Test");
    }
}

TEST_CASE("Scene_ComponentIterationEmpty")
{
    Scene scene;
    scene.registerComponent<String>("String");

    size_t count = 0;
    for (const String& string : scene.components<String>())
    {
        ++count;
    }

    REQUIRE(count == 0);
}

TEST_CASE("Scene_ComponentIterationNoneActivated")
{
    Scene scene;
    scene.registerComponent<String>("String");

    scene.createEntity()->addComponent(String("Test"));

    size_t count = 0;
    for (const String& string : scene.components<String>())
    {
        ++count;
    }

    REQUIRE(count == 0);
}


TEST_CASE("Scene_ComponentIterationSomeActivated")
{
    Scene scene;
    scene.registerComponent<String>("String");

    String string("Test");

    scene.createEntity();
    scene.createEntity()->addComponent(string)->entity().activate();
    scene.createEntity()->addComponent(string)->entity().activate();
    scene.createEntity();
    scene.createEntity()->addComponent(string)->entity().activate();
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
    Scene scene;
    scene.registerComponent<String>("String");

    String string("Test");

    scene.createEntity()->addComponent(string)->entity().activate();
    scene.createEntity()->addComponent(string)->entity().activate();
    scene.createEntity();
    scene.createEntity()->addComponent(string)->entity().activate();
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
    Scene scene;
    scene.registerComponent<String>("String");

    String string("Test");

    scene.createEntity();
    scene.createEntity()->addComponent(string)->entity().activate();
    scene.createEntity()->addComponent(string)->entity().activate();
    scene.createEntity();
    scene.createEntity()->addComponent(string)->entity().activate();

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
    Scene scene;
    scene.registerComponent<String>("String");

    String string("Test");

    scene.createEntity()->addComponent(string)->entity().activate();
    scene.createEntity()->addComponent(string)->entity().activate();
    scene.createEntity();
    scene.createEntity()->addComponent(string)->entity().activate();

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
    Scene scene;
    scene.registerComponent<String>("String");

    ComponentPoolListener listener;
    scene.components<String>().dispatcher().addListener(listener);

    Entity::Iter a = scene.createEntity("A");
    a->addComponent(String("A"));
    REQUIRE(listener.receivedEvents.size() == 0);
    a->activate();
    REQUIRE(listener.receivedEvents.size() == 1);

    Entity::Iter b = scene.createEntity("B");
    b->addComponent(String("B"));
    REQUIRE(listener.receivedEvents.size() == 1);
    b->activate();

    REQUIRE(listener.receivedEvents.size() == 2);
    REQUIRE(listener.receivedEvents[0].type() == ComponentEventType::Add);
    REQUIRE(&listener.receivedEvents[0].entity() == &*a);
    REQUIRE(listener.receivedEvents[1].type() == ComponentEventType::Add);
    REQUIRE(&listener.receivedEvents[1].entity() == &*b);
    listener.receivedEvents.clear();

    a->destroy();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type() == ComponentEventType::Remove);
    REQUIRE(listener.receivedEvents[0].entity().id() == (EntityId)-1);
    listener.receivedEvents.clear();
    
    b->removeComponent<String>();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type() == ComponentEventType::Remove);
    REQUIRE(&listener.receivedEvents[0].entity() == &*b);
}

TEST_CASE("Scene_ComponentPoolFindFirstWithMatch")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity();
    a->addComponent(String("NotMatch"));
    a->activate();

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("Match"));
    b->activate();

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("Match"));
    c->activate();

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("NotMatch"));
    d->activate();

    ComponentPool<String>& strings = scene.components<String>();
    Component<String>::Iter iter = strings.findFirst( [](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iter);
    REQUIRE(iter->value == "Match");
    REQUIRE(&*iter == &*b->component<String>());
}

TEST_CASE("Scene_ComponentPoolFindFirstWithoutMatch")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity();
    a->addComponent(String("NotMatch"));
    a->activate();

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("NotMatch"));
    b->activate();

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("NotMatch"));
    c->activate();

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("NotMatch"));
    d->activate();

    ComponentPool<String>& strings = scene.components<String>();
    Component<String>::Iter iter = strings.findFirst([](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(!iter);
}

TEST_CASE("Scene_ComponentPoolFindWithMatches")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity();
    a->addComponent(String("NotMatch"));
    a->activate();

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("Match"));
    b->activate();

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("Match"));
    c->activate();

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("NotMatch"));
    d->activate();

    ComponentPool<String>& strings = scene.components<String>();
    Component<String>::Iter::Array iters = strings.find([](const String& string)
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
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity();
    a->addComponent(String("NotMatch"));
    a->activate();

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("NotMatch"));
    b->activate();

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("NotMatch"));
    c->activate();

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("NotMatch"));
    d->activate();

    ComponentPool<String>& strings = scene.components<String>();
    Component<String>::Iter::Array iters = strings.find([](const String& string)
    {
        return string.value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("Scene_EntityPoolFindFirstWithMatch")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("NotMatch");
    a->activate();

    Entity::Iter b = scene.createEntity("Match");
    b->activate();

    Entity::Iter c = scene.createEntity("Match");
    c->activate();

    Entity::Iter d = scene.createEntity("NotMatch");
    d->activate();

    Entity::Iter iter = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.name() == "Match";
    });

    REQUIRE(iter);
    REQUIRE(iter->name() == "Match");
    REQUIRE(&*iter == &*b);
}

TEST_CASE("Scene_EntityPoolFindFirstWithoutMatch")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("NotMatch");
    a->activate();

    Entity::Iter b = scene.createEntity("NotMatch");
    b->activate();

    Entity::Iter c = scene.createEntity("NotMatch");
    c->activate();

    Entity::Iter d = scene.createEntity("NotMatch");
    d->activate();

    Entity::Iter iter = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.name() == "Match";
    });
    
    REQUIRE(!iter);
}

TEST_CASE("Scene_EntityPoolFindWithMatches")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("NotMatch");
    a->activate();

    Entity::Iter b = scene.createEntity("Match");
    b->activate();

    Entity::Iter c = scene.createEntity("Match");
    c->activate();

    Entity::Iter d = scene.createEntity("NotMatch");
    d->activate();

    Entity::Iter::Array iters = scene.entities().find([](const Entity& entity)
    {
        return entity.name() == "Match";
    });

    REQUIRE(iters.size() == 2);

    REQUIRE(iters[0]);
    REQUIRE(iters[0]->name() == "Match");
    REQUIRE(&*iters[0] == &*b);

    REQUIRE(iters[1]);
    REQUIRE(iters[1]->name() == "Match");
    REQUIRE(&*iters[1] == &*c);
}

TEST_CASE("Scene_EntityPoolFindWithoutMatches")
{
    Scene scene;
    scene.registerComponent<String>("String");

    Entity::Iter a = scene.createEntity("NotMatch");
    a->activate();

    Entity::Iter b = scene.createEntity("NotMatch");
    b->activate();

    Entity::Iter c = scene.createEntity("NotMatch");
    c->activate();

    Entity::Iter d = scene.createEntity("NotMatch");
    d->activate();

    Entity::Iter::Array iters = scene.entities().find([](const Entity& entity)
    {
        return entity.name() == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("Scene_AddRemoveEntityChildren")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    a->activate();
    REQUIRE(!a->parent());

    Entity::Iter b = scene.createEntity("B");
    b->activate();
    REQUIRE(!b->parent());

    Entity::Iter c = scene.createEntity("C");
    c->activate();
    REQUIRE(!c->parent());

    a->addChild(*b);
    REQUIRE(&*b->parent() == &*a);

    a->removeChild(*b);
    REQUIRE(!b->parent());
}

TEST_CASE("Scene_AddChildEntityAsChild")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    a->activate();

    Entity::Iter b = scene.createEntity("B");
    b->activate();

    a->addChild(*b);

    Entity::Iter c = scene.createEntity("C");
    c->activate();

    REQUIRE_THROWS_AS(c->addChild(*b), Error);
}

TEST_CASE("Scene_AddEntityChildActivationRestrictions")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    a->activate();

    Entity::Iter b = scene.createEntity("B");

    REQUIRE_THROWS_AS(a->addChild(*b), Error);

    Entity::Iter c = scene.createEntity("C");
    c->activate();

    REQUIRE_THROWS_AS(c->addChild(*b), Error);
}

TEST_CASE("Scene_AddChildFromAnotherScene")
{
    Scene sceneA;

    Entity::Iter a = sceneA.createEntity("A");
    a->activate();

    Scene sceneB;

    Entity::Iter b = sceneB.createEntity("B");
    b->activate();

    REQUIRE_THROWS_AS(a->addChild(*b), Error);
}

TEST_CASE("Scene_ChildEntityActivation")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    Entity::Iter b = scene.createEntity("B");
    Entity::Iter c = scene.createEntity("C");
    Entity::Iter d = scene.createEntity("D");

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
    Scene scene;

    Entity::Iter a = scene.createEntity("A");

    size_t count = 0;
    for (Entity& child : a->children())
    {
        ++count;
    }
    REQUIRE(count == 0);
}

TEST_CASE("Scene_ChildEntityIterationNonEmpty")
{
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    Entity::Iter b = scene.createEntity("B");
    Entity::Iter c = scene.createEntity("C");
    Entity::Iter d = scene.createEntity("D");

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
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    Entity::Iter b = scene.createEntity("B");
    Entity::Iter c = scene.createEntity("C");
    Entity::Iter d = scene.createEntity("D");

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
    Scene scene;

    Entity::Iter a = scene.createEntity("A");
    Entity::Iter b = scene.createEntity("B");
    Entity::Iter c = scene.createEntity("C");

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