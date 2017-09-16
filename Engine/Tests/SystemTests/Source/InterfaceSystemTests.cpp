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

TEST_CASE("Create an interface", "[InterfaceSystem]")
{
    Engine& engine = Engine::instance();

    DefaultScene scene(engine);

    InterfaceSystem& interface_system = scene.interface_system();

    Window& main_window = engine.main_window();
    Interface::Handle interface = interface_system.create_interface(main_window);
    REQUIRE(interface);
    REQUIRE(interface->dimensions() == main_window.dimensions());
}

TEST_CASE("Create a button", "[InterfaceSystem]")
{
    Engine& engine = Engine::instance();

    DefaultScene scene(engine);

    InterfaceSystem& interface_system = scene.interface_system();

    Interface::Handle interface = interface_system.create_interface(engine.main_window());
    REQUIRE(interface);

    ButtonWidget::Handle button = interface->create_child<ButtonWidget>();
    REQUIRE(button);

    button->set_press_action([] { HECT_DEBUG("Press"); });
    button->set_dimensions(Vector2(100, 100));
    button->set_horizontal_align(HorizontalAlign::Center);
    button->set_vertical_align(VerticalAlign::Center);

    //engine.play_scene(scene);
}
