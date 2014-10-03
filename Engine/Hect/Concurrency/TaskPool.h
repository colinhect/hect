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
#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Hect/Concurrency/Task.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Uncopyable.h"

namespace hect
{

///
/// Provides the functionality for executing asynchronous tasks.
class TaskPool :
    public Uncopyable
{
public:

    ///
    /// Constructs a task pool which spawns new threads as they are needed.
    TaskPool();

    ///
    /// Constructs a task pool with a specific number of worker threads.
    ///
    /// \param threadCount The number of worker threads.
    TaskPool(size_t threadCount);

    ///
    /// Waits until all running tasks complete (ignores any enqueued tasks
    /// remaining).
    ~TaskPool();

    ///
    /// Enqueues a task to be executed asynchronously.
    ///
    /// \param action The action for the task to perform.
    ///
    /// \returns The handle to the queued task.
    Task::Handle enqueue(Task::Action action);

private:
    void initializeThreads(size_t threadCount);
    void threadLoop();

    std::deque<Task::Handle> _taskQueue;

    std::mutex _threadsMutex;
    std::vector<std::thread> _threads;

    std::mutex _queueMutex;
    std::condition_variable _condition;

    bool _stop{ false };
    bool _adaptive{ true };

    std::atomic<size_t> _availableThreadCount{ 0 };
};

}