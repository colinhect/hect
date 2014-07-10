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

    Entity::Iter a = scene.createEntity();
    REQUIRE(a);
    REQUIRE(a->id() == 0);

    Entity::Iter b = scene.createEntity();
    REQUIRE(b);
    REQUIRE(b->id() == 1);

    a->destroy();
    REQUIRE(!a);

    REQUIRE(b);
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

    Entity::Iter a = scene.createEntity();
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
            Entity::Iter entity = scene.createEntity();
            entity->activate();
            entities.push_back(entity);
        }

        {
            EntityId id = 0;
            for (const Entity::Iter& entity : entities)
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
    Scene scene;

    Entity::Iter a = scene.createEntity();
    a->activate();
    REQUIRE(!a->parent());

    Entity::Iter b = scene.createEntity();
    b->activate();
    REQUIRE(!b->parent());

    Entity::Iter c = scene.createEntity();
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

    Entity::Iter a = scene.createEntity();
    a->activate();

    Entity::Iter b = scene.createEntity();
    b->activate();

    a->addChild(*b);

    Entity::Iter c = scene.createEntity();
    c->activate();

    REQUIRE_THROWS_AS(c->addChild(*b), Error);
}

TEST_CASE("Scene_AddEntityChildActivationRestrictions")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();
    a->activate();

    Entity::Iter b = scene.createEntity();

    REQUIRE_THROWS_AS(a->addChild(*b), Error);

    Entity::Iter c = scene.createEntity();
    c->activate();

    REQUIRE_THROWS_AS(c->addChild(*b), Error);
}

TEST_CASE("Scene_AddChildFromAnotherScene")
{
    Scene sceneA;

    Entity::Iter a = sceneA.createEntity();
    a->activate();

    Scene sceneB;

    Entity::Iter b = sceneB.createEntity();
    b->activate();

    REQUIRE_THROWS_AS(a->addChild(*b), Error);
}

TEST_CASE("Scene_ChildEntityActivation")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();
    Entity::Iter b = scene.createEntity();
    Entity::Iter c = scene.createEntity();
    Entity::Iter d = scene.createEntity();

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

    Entity::Iter a = scene.createEntity();

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

    Entity::Iter a = scene.createEntity();
    Entity::Iter b = scene.createEntity();
    Entity::Iter c = scene.createEntity();
    Entity::Iter d = scene.createEntity();

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

    Entity::Iter a = scene.createEntity();
    Entity::Iter b = scene.createEntity();
    Entity::Iter c = scene.createEntity();
    Entity::Iter d = scene.createEntity();

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
    Scene::registerComponent<String>("String");
    Scene scene;

    Entity::Iter a = scene.createEntity();
    Entity::Iter b = scene.createEntity();
    Entity::Iter c = scene.createEntity();

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
    Scene scene;    

    Entity::Iter a = scene.createEntity();
    Component<String>::Iter stringA = a->addComponent(String("Test"));
    a->activate();

    Entity::Iter b = a->clone();
    Component<String>::Iter stringB = b->component<String>();

    REQUIRE(a->isActivated());
    REQUIRE(!b->isActivated());
    REQUIRE(&stringA->entity() == &*a);
    REQUIRE(&stringB->entity() == &*b);
    REQUIRE(&*stringA != &*stringB);
    REQUIRE(stringA->id() != stringB->id());
}

TEST_CASE("Scene_CloneEntityWithChildren")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();
    Entity::Iter b = scene.createEntity();
    Entity::Iter c = scene.createEntity();

    a->addChild(*b);
    a->addChild(*c);

    Entity::Iter cloneA = a->clone();
    Entity::Children::Iter childIter = cloneA->children().begin();
    REQUIRE(&*childIter->parent() == &*cloneA);
    REQUIRE(&*childIter != &*b);
    REQUIRE(++childIter);
    REQUIRE(&*childIter->parent() == &*cloneA);
    REQUIRE(&*childIter != &*c);
    REQUIRE(!++childIter);
}

TEST_CASE("Scene_AddAndRemoveComponent")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();

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

    Entity::Iter a = scene.createEntity();

    Component<String>::Iter string = a->addComponent(String("Test"));
    Component<String>::Iter replacedString = a->replaceComponent(String("Replaced"));
    REQUIRE(string == a->component<String>());
    REQUIRE(replacedString == a->component<String>());
    REQUIRE(string == replacedString);
    REQUIRE(replacedString);
    REQUIRE(replacedString->value == "Replaced");
    REQUIRE(&string->entity() == &*a);
}

TEST_CASE("Scene_AddExistingComponent")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();
    REQUIRE_THROWS_AS(a->removeComponent<String>(), Error);
}

TEST_CASE("Scene_RemoveNonExistingComponent")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();
    a->addComponent(String("Test"));
    REQUIRE_THROWS_AS(a->addComponent(String("Test")), Error);
}

TEST_CASE("Scene_RemoveNonExistingUnregisteredComponent")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();
    REQUIRE_THROWS_AS(a->removeComponent<String>(), Error);
}

TEST_CASE("Scene_Decode")
{
    Scene scene;

    REQUIRE(scene.entityCount() == 0);

    JsonValue jsonValue;
    jsonValue.decodeFromJson("{ \"entities\" : [ { \"components\" : [ { \"type\" : \"String\", \"value\" : \"Test\" } ] } ] }");

    scene.decodeFromJsonValue(jsonValue);

    REQUIRE(scene.entityCount() == 1);

    Entity::Iter a = scene.entities().begin();
    REQUIRE(a);
    REQUIRE(a->component<String>()->value == "Test");
}

TEST_CASE("Scene_DecodeWithChildren")
{
    Scene scene;

    REQUIRE(scene.entityCount() == 0);

    JsonValue jsonValue;
    jsonValue.decodeFromJson("{ \"entities\" : [ { \"children\" : [ { \"name\" : \"B\" } ] } ] }");

    scene.decodeFromJsonValue(jsonValue);

    REQUIRE(scene.entityCount() == 2);

    Entity::Iter a = scene.entities().begin();
    REQUIRE(a);

    Entity::Children::Iter b = a->children().begin();
    REQUIRE(b);
}

TEST_CASE("Scene_Encode")
{
    JsonValue jsonValue;

    {
        Scene scene;

        Entity::Iter a = scene.createEntity();
        a->addComponent(String("Test"));
        a->activate();

        jsonValue = scene.encodeToJsonValue();
    }

    {
        Scene scene;
        scene.decodeFromJsonValue(jsonValue);

        REQUIRE(scene.entityCount() == 1);

        Entity::Iter a = scene.entities().begin();
        REQUIRE(a);
        REQUIRE(a->component<String>()->value == "Test");
    }
}

TEST_CASE("Scene_EncodeWithChildren")
{
    JsonValue jsonValue;

    {
        Scene scene;

        Entity::Iter a = scene.createEntity();
        Entity::Iter b = scene.createEntity();
        a->addChild(*b);
        a->activate();

        jsonValue = scene.encodeToJsonValue();
    }

    {
        Scene scene;
        scene.decodeFromJsonValue(jsonValue);

        REQUIRE(scene.entityCount() == 2);

        Entity::Iter a = scene.entities().begin();
        REQUIRE(a);

        Entity::Children::Iter b = a->children().begin();
        REQUIRE(b);
    }
}

TEST_CASE("Scene_ComponentIterationEmpty")
{
    Scene scene;

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

    ComponentPoolListener listener;
    scene.components<String>().dispatcher().addListener(listener);

    Entity::Iter a = scene.createEntity();
    a->addComponent(String("A"));
    REQUIRE(listener.receivedEvents.size() == 0);
    a->activate();
    REQUIRE(listener.receivedEvents.size() == 1);

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("B"));
    REQUIRE(listener.receivedEvents.size() == 1);
    b->activate();

    REQUIRE(listener.receivedEvents.size() == 2);
    REQUIRE(listener.receivedEvents[0].type() == ComponentEventType_Add);
    REQUIRE(&listener.receivedEvents[0].entity() == &*a);
    REQUIRE(listener.receivedEvents[1].type() == ComponentEventType_Add);
    REQUIRE(&listener.receivedEvents[1].entity() == &*b);
    listener.receivedEvents.clear();

    a->destroy();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type() == ComponentEventType_Remove);
    REQUIRE(listener.receivedEvents[0].entity().id() == (EntityId)-1);
    listener.receivedEvents.clear();
    
    b->removeComponent<String>();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type() == ComponentEventType_Remove);
    REQUIRE(&listener.receivedEvents[0].entity() == &*b);
}

TEST_CASE("Scene_ComponentPoolFindFirstWithMatch")
{
    Scene scene;

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

    Entity::Iter iter = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(iter);
    REQUIRE(&*iter == &*b);
}

TEST_CASE("Scene_EntityPoolFindFirstWithoutMatch")
{
    Scene scene;

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

    Entity::Iter iter = scene.entities().findFirst([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });
    
    REQUIRE(!iter);
}

TEST_CASE("Scene_EntityPoolFindWithMatches")
{
    Scene scene;

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

    Entity::Iter::Array iters = scene.entities().find([](const Entity& entity)
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
    Scene scene;

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

    Entity::Iter::Array iters = scene.entities().find([](const Entity& entity)
    {
        return entity.component<String>()->value == "Match";
    });

    REQUIRE(iters.size() == 0);
}

TEST_CASE("Scene_EntityFindFirstChild")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("B"));
    a->addChild(*b);

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("C"));
    a->addChild(*c);

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("D"));
    c->addChild(*d);

    a->activate();

    Entity::Iter iter = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<String>()->value == "B";
    });
    REQUIRE(iter);
    REQUIRE(iter == b);

    iter = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<String>()->value == "C";
    });
    REQUIRE(iter);
    REQUIRE(iter == c);

    iter = a->findFirstChild([](const Entity& entity)
    {
        return entity.component<String>()->value == "D";
    });
    REQUIRE(!iter);
}

TEST_CASE("Scene_EntityFindChildren")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("B"));
    a->addChild(*b);

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("C"));
    a->addChild(*c);

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("D"));
    c->addChild(*d);

    a->activate();

    Entity::Iter::Array iters = a->findChildren([](const Entity& entity)
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
    Scene scene;

    Entity::Iter a = scene.createEntity();

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("B"));
    a->addChild(*b);

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("C"));
    a->addChild(*c);

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("D"));
    c->addChild(*d);

    a->activate();

    Entity::Iter iter = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<String>()->value == "B";
    });
    REQUIRE(iter);
    REQUIRE(iter == b);

    iter = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<String>()->value == "C";
    });
    REQUIRE(iter);
    REQUIRE(iter == c);

    iter = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<String>()->value == "D";
    });
    REQUIRE(iter);
    REQUIRE(iter == d);

    iter = a->findFirstDescendant([](const Entity& entity)
    {
        return entity.component<String>()->value == "E";
    });
    REQUIRE(!iter);
}

TEST_CASE("Scene_EntityFindDescendants")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("B"));
    a->addChild(*b);

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("C"));
    a->addChild(*c);

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("D"));
    c->addChild(*d);

    a->activate();

    Entity::Iter::Array iters = a->findDescendants([](const Entity& entity)
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

TEST_CASE("Scene_EntityFindFirstAncestor")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();
    a->addComponent(String("A"));

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("B"));
    a->addChild(*b);

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("C"));
    a->addChild(*c);

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("D"));
    c->addChild(*d);

    a->activate();

    Entity::Iter iter = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<String>()->value == "C";
    });
    REQUIRE(iter);
    REQUIRE(iter == c);

    iter = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<String>()->value == "A";
    });
    REQUIRE(iter);
    REQUIRE(iter == a);

    iter = d->findFirstAncestor([](const Entity& entity)
    {
        return entity.component<String>()->value == "E";
    });
    REQUIRE(!iter);
}

TEST_CASE("Scene_EntityFindAncestors")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();
    a->addComponent(String("A"));

    Entity::Iter b = scene.createEntity();
    b->addComponent(String("B"));
    a->addChild(*b);

    Entity::Iter c = scene.createEntity();
    c->addComponent(String("C"));
    a->addChild(*c);

    Entity::Iter d = scene.createEntity();
    d->addComponent(String("D"));
    c->addChild(*d);

    a->activate();

    Entity::Iter::Array iters = d->findAncestors([](const Entity& entity)
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
    Scene scene;

    Entity::Iter a = scene.createEntity();

    Entity::Handle handle = a->createHandle();
    REQUIRE(handle);
    REQUIRE(&*handle == &*a);

    a->destroy();

    REQUIRE(!handle);
    REQUIRE_THROWS_AS(*handle, Error);
}

TEST_CASE("Scene_CopyEntityHandle")
{
    Scene scene;

    Entity::Iter a = scene.createEntity();

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