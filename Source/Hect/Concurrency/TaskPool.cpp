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
#include "TaskPool.h"

using namespace hect;

TaskPool::TaskPool() :
    _stop(false),
    _adaptive(true),
    _availableThreadCount(0)
{
}

TaskPool::TaskPool(size_t threadCount) :
    _stop(false),
    _adaptive(false),
    _availableThreadCount(0)
{
    initializeThreads(threadCount);
}

TaskPool::~TaskPool()
{
    // Notify all threads that the pool is stopping
    {
        std::unique_lock<std::mutex> lock(_queueMutex);
        _stop = true;
        _condition.notify_all();
    }

    // Join all of the threads
    for (std::thread& thread : _threads)
    {
        thread.join();
    }
}

Task::Handle TaskPool::enqueue(Task::Action action)
{
    // Create the task
    std::shared_ptr<Task> task(new Task(action));

    // Create the task
    Task::Handle handle(task);

    if (_threads.empty() && !_adaptive)
    {
        // The task pool has no threads so execute the task synchronously
        task->execute();
    }
    else
    {
        // Add this task to the queue
        {
            std::unique_lock<std::mutex> lock(_queueMutex);

            // If this task pool is adaptive and there are no threads available,
            // then initialize another thread
            if (_adaptive && _availableThreadCount.load() == 0)
            {
                initializeThreads(1);
            }

            _taskQueue.push_back(handle);
        }

        // Notify one of the threads that a new task is available
        _condition.notify_one();
    }

    return handle;
}

void TaskPool::initializeThreads(size_t threadCount)
{
    for (unsigned i = 0; i < threadCount; ++i)
    {
        _threads.push_back(std::thread([this]
            {
                threadLoop();
            }
        ));
    }
}

void TaskPool::threadLoop()
{
    ++_availableThreadCount;
    for (;;)
    {
        Task::Handle taskHandle;

        {
            std::unique_lock<std::mutex> lock(_queueMutex);

            while (!_stop && _taskQueue.empty())
            {
                _condition.wait(lock);
            }

            if (_stop)
            {
                --_availableThreadCount;
                return;
            }

            // Get the next tasks
            taskHandle = _taskQueue.front();
            _taskQueue.pop_front();
        }

        // Execute the task
        --_availableThreadCount;
        Task* task = taskHandle._task.get();
        if (task)
        {
            task->execute();
        }
        ++_availableThreadCount;
    }
}