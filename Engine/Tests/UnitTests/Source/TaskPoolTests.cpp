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
#include <Hect/Concurrency/TaskPool.h>
using namespace hect;

#include <catch.hpp>

namespace
{

const unsigned max_thread_count = 6;
const unsigned max_task_count = 8;

void empty_task() { }

void short_task()
{
    volatile float total = 0.0f;
    for (unsigned i = 0; i < 1024; ++i)
    {
        total += i;
        total *= 1.0f / static_cast<float>(i);
    }
}

void long_task()
{
    volatile float total = 0.0f;
    for (unsigned i = 0; i < 4096 * 4; ++i)
    {
        total += i;
        total *= 1.0f / static_cast<float>(i);
    }
}

void test_tasks(unsigned thread_count, unsigned task_count, bool adaptive, Task::Action action)
{
    TaskPool task_pool(thread_count, adaptive);

    bool task_done[max_task_count];
    std::vector<Task::Handle> tasks;

    for (unsigned i = 0; i < task_count; ++i)
    {
        bool* this_task_done = &task_done[i];
        Task::Handle task = task_pool.enqueue([action, this_task_done]
        {
            action();
            *this_task_done = true;
        });

        tasks.push_back(task);
    }

    unsigned i = 0;
    for (Task::Handle& task : tasks)
    {
        REQUIRE(task);
        task->wait();
        REQUIRE(task_done[i] == true);
        REQUIRE(task_done[i] == task->has_completed());

        ++i;
    }
}

void test_tasks_with_exceptions(unsigned thread_count, unsigned task_count, bool adaptive, Task::Action action)
{
    TaskPool task_pool(thread_count, adaptive);

    std::vector<Task::Handle> tasks;

    for (unsigned i = 0; i < task_count; ++i)
    {
        Task::Handle task = task_pool.enqueue([action]
        {
            action();
            throw Exception("Task exception");
        });

        tasks.push_back(task);
    }

    for (Task::Handle& task : tasks)
    {
        bool exception_thrown = false;
        try
        {
            REQUIRE(task);
            task->wait();
        }
        catch (Exception& exception)
        {
            exception_thrown = true;
            REQUIRE(std::string(exception.what()) == "Task exception");
        }

        REQUIRE(exception_thrown == true);
    }
}

#define TEST_TASKS(action, adaptive)\
    for (unsigned thread_count = 0; thread_count < max_thread_count; ++thread_count) \
    { \
        for (unsigned task_count = 1; task_count < max_task_count; ++task_count) \
        { \
            test_tasks(thread_count, task_count, adaptive, action); \
        } \
    }

#define TEST_TASKS_WITH_EXCEPTIONS(action, adaptive)\
    for (unsigned thread_count = 0; thread_count < max_thread_count; ++thread_count) \
    { \
        for (unsigned task_count = 1; task_count < max_task_count; ++task_count) \
        { \
            test_tasks_with_exceptions(thread_count, task_count, adaptive, action); \
        } \
    }

}

TEST_CASE("Execute empty tasks in a task pool", "[TaskPool]")
{
    TEST_TASKS(empty_task, false);
}

TEST_CASE("Execute empty tasks in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS(empty_task, true);
}

TEST_CASE("Execute short tasks in a task pool", "[TaskPool]")
{
    TEST_TASKS(short_task, false);
}

TEST_CASE("Execute short tasks in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS(short_task, true);
}

TEST_CASE("Execute long tasks in a task pool", "[TaskPool]")
{
    TEST_TASKS(long_task, false);
}

TEST_CASE("Execute long tasks in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS(long_task, true);
}

TEST_CASE("Execute empty tasks with errors in a task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(empty_task, false);
}

TEST_CASE("Execute empty tasks with errors in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(empty_task, true);
}

TEST_CASE("Execute short tasks with errors in a task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(short_task, false);
}

TEST_CASE("Execute short tasks with errors in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(short_task, true);
}

TEST_CASE("Execute long tasks with errors in a task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(long_task, false);
}

TEST_CASE("Execute long tasks with errors in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(long_task, true);
}

TEST_CASE("Dereference an invalid task handle", "[TaskPool]")
{
    Task::Handle task_handle;
    REQUIRE(!task_handle);
    REQUIRE_THROWS_AS(*task_handle, InvalidOperation);
    REQUIRE_THROWS_AS(task_handle->has_completed(), InvalidOperation);
}
