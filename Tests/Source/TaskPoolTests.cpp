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
#include <Hect/Concurrency/TaskPool.h>
using namespace hect;

#include <catch.hpp>

const unsigned maxThreadCount = 6;
const unsigned maxTaskCount = 8;

void emptyTask() { }

void shortTask()
{
    volatile float total = 0.0f;
    for (int i = 0; i < 1024; ++i)
    {
        total += i;
        total *= 1.0f / (float)i;
    }
}

void longTask()
{
    volatile float total = 0.0f;
    for (int i = 0; i < 4096 * 4; ++i)
    {
        total += i;
        total *= 1.0f / (float)i;
    }
}

void testTasks(unsigned threadCount, unsigned taskCount, Task::Action action)
{
    TaskPool taskPool(threadCount);

    bool taskDone[maxTaskCount];
    std::vector<Task::Handle> tasks;

    for (unsigned i = 0; i < taskCount; ++i)
    {
        bool* thisTaskDone = &taskDone[i];
        tasks.push_back(taskPool.enqueue([action, thisTaskDone]
        {
            action();
            *thisTaskDone = true;
        }));
    }

    unsigned i = 0;
    for (auto& taskHandle : tasks)
    {
        REQUIRE(taskHandle);
        taskHandle->wait();

        REQUIRE(taskDone[i] == true);
        REQUIRE(taskDone[i++] == taskHandle->isDone());
    }
}

void testTasksWithErrors(unsigned threadCount, unsigned taskCount, Task::Action action)
{
    TaskPool taskPool(threadCount);

    std::vector<Task::Handle> taskHandles;

    for (unsigned i = 0; i < taskCount; ++i)
    {
        taskHandles.push_back(taskPool.enqueue([action]
        {
            action();
            throw Error("Task error");
        }));
    }

    for (auto& taskHandle : taskHandles)
    {
        bool errorThrown = false;
        try
        {
            REQUIRE(taskHandle);
            taskHandle->wait();
        }
        catch (Error&)
        {
            errorThrown = true;
        }

        REQUIRE(errorThrown == true);
    }
}

#define TEST_TASKS(action)\
for (unsigned threadCount = 0; threadCount < maxThreadCount; ++threadCount) {\
    for (unsigned taskCount = 1; taskCount < maxTaskCount; ++taskCount) {\
        testTasks(threadCount, taskCount, action); \
    }\
}

#define TEST_TASKS_WITH_ERRORS(action)\
for (unsigned threadCount = 0; threadCount < maxThreadCount; ++threadCount) {\
    for (unsigned taskCount = 1; taskCount < maxTaskCount; ++taskCount) {\
        testTasksWithErrors(threadCount, taskCount, action); \
    }\
}

TEST_CASE("TaskPool_EmptyTasks")
{
    TEST_TASKS(emptyTask);
}

TEST_CASE("TaskPool_ShortTasks")
{
    TEST_TASKS(shortTask);
}

TEST_CASE("TaskPool_LongTasks")
{
    TEST_TASKS(longTask);
}

TEST_CASE("TaskPool_EmptyTasksWithErrors")
{
    TEST_TASKS_WITH_ERRORS(emptyTask);
}

TEST_CASE("TaskPool_ShortTasksWithErrors")
{
    TEST_TASKS_WITH_ERRORS(shortTask);
}

TEST_CASE("TaskPool_LongTasksWithErrors")
{
    TEST_TASKS_WITH_ERRORS(longTask);
}

TEST_CASE("TaskPool_DereferenceInvalidTaskHandle")
{
    Task::Handle taskHandle;
    REQUIRE(!taskHandle);
    REQUIRE_THROWS_AS(*taskHandle, Error);
    REQUIRE_THROWS_AS(taskHandle->isDone(), Error);
}