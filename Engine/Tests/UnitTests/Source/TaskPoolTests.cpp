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

const unsigned maxThreadCount = 6;
const unsigned maxTaskCount = 8;

void emptyTask() { }

void shortTask()
{
    volatile float total = 0.0f;
    for (unsigned i = 0; i < 1024; ++i)
    {
        total += i;
        total *= 1.0f / static_cast<float>(i);
    }
}

void longTask()
{
    volatile float total = 0.0f;
    for (unsigned i = 0; i < 4096 * 4; ++i)
    {
        total += i;
        total *= 1.0f / static_cast<float>(i);
    }
}

void testTasks(unsigned threadCount, unsigned taskCount, bool adaptive, Task::Action action)
{
    TaskPool taskPool(threadCount, adaptive);

    bool taskDone[maxTaskCount];
    std::vector<Task::Handle> tasks;

    for (unsigned i = 0; i < taskCount; ++i)
    {
        bool* thisTaskDone = &taskDone[i];
        Task::Handle task = taskPool.enqueue([action, thisTaskDone]
        {
            action();
            *thisTaskDone = true;
        });

        tasks.push_back(task);
    }

    unsigned i = 0;
    for (Task::Handle& task : tasks)
    {
        REQUIRE(task);
        task->wait();
        REQUIRE(taskDone[i] == true);
        REQUIRE(taskDone[i] == task->hasCompleted());

        ++i;
    }
}

void testTasksWithExceptions(unsigned threadCount, unsigned taskCount, bool adaptive, Task::Action action)
{
    TaskPool taskPool(threadCount, adaptive);

    std::vector<Task::Handle> tasks;

    for (unsigned i = 0; i < taskCount; ++i)
    {
        Task::Handle task = taskPool.enqueue([action]
        {
            action();
            throw Exception("Task exception");
        });

        tasks.push_back(task);
    }

    for (Task::Handle& task : tasks)
    {
        bool exceptionThrown = false;
        try
        {
            REQUIRE(task);
            task->wait();
        }
        catch (Exception& exception)
        {
            exceptionThrown = true;
            REQUIRE(std::string(exception.what()) == "Task exception");
        }

        REQUIRE(exceptionThrown == true);
    }
}

#define TEST_TASKS(action, adaptive)\
    for (unsigned threadCount = 0; threadCount < maxThreadCount; ++threadCount) \
    { \
        for (unsigned taskCount = 1; taskCount < maxTaskCount; ++taskCount) \
        { \
            testTasks(threadCount, taskCount, adaptive, action); \
        } \
    }

#define TEST_TASKS_WITH_EXCEPTIONS(action, adaptive)\
    for (unsigned threadCount = 0; threadCount < maxThreadCount; ++threadCount) \
    { \
        for (unsigned taskCount = 1; taskCount < maxTaskCount; ++taskCount) \
        { \
            testTasksWithExceptions(threadCount, taskCount, adaptive, action); \
        } \
    }

}

TEST_CASE("Execute empty tasks in a task pool", "[TaskPool]")
{
    TEST_TASKS(emptyTask, false);
}

TEST_CASE("Execute empty tasks in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS(emptyTask, true);
}

TEST_CASE("Execute short tasks in a task pool", "[TaskPool]")
{
    TEST_TASKS(shortTask, false);
}

TEST_CASE("Execute short tasks in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS(shortTask, true);
}

TEST_CASE("Execute long tasks in a task pool", "[TaskPool]")
{
    TEST_TASKS(longTask, false);
}

TEST_CASE("Execute long tasks in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS(longTask, true);
}

TEST_CASE("Execute empty tasks with errors in a task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(emptyTask, false);
}

TEST_CASE("Execute empty tasks with errors in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(emptyTask, true);
}

TEST_CASE("Execute short tasks with errors in a task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(shortTask, false);
}

TEST_CASE("Execute short tasks with errors in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(shortTask, true);
}

TEST_CASE("Execute long tasks with errors in a task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(longTask, false);
}

TEST_CASE("Execute long tasks with errors in an adaptive task pool", "[TaskPool]")
{
    TEST_TASKS_WITH_EXCEPTIONS(longTask, true);
}

TEST_CASE("Dereference an invalid task handle", "[TaskPool]")
{
    Task::Handle taskHandle;
    REQUIRE(!taskHandle);
    REQUIRE_THROWS_AS(*taskHandle, InvalidOperation);
    REQUIRE_THROWS_AS(taskHandle->hasCompleted(), InvalidOperation);
}
