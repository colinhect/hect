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
#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "Hect/Concurrency/Task.h"
#include "Hect/Core/Exception.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"

namespace hect
{

///
/// Provides the functionality for executing asynchronous Task%s.
class HECT_EXPORT TaskPool :
    public Uncopyable
{
public:

    ///
    /// Constructs a task pool with a worker thread for each concurrent thread
    /// available in hardware on the system.
    ///
    /// \param adaptive Whether to spawn additional worker threads as needed.
    TaskPool(bool adaptive = false);

    ///
    /// Constructs a task pool with a specific number of worker threads.
    ///
    /// \param thread_count The number of worker threads.
    /// \param adaptive Whether to spawn additional worker threads as needed.
    TaskPool(size_t thread_count, bool adaptive = false);

    ///
    /// Waits until all running tasks complete (ignores any enqueued tasks
    /// remaining).
    ~TaskPool();

    ///
    /// Enqueues a task to be executed asynchronously.
    ///
    /// \param action The action for the task to perform.
    ///
    /// \returns The handle to the enqueued task.
    ///
    /// \b Example
    /// \code{.cpp}
    /// TaskPool task_pool;
    /// Task::Handle task_a = task_pool.enqueue([] { do_something(); });
    /// Task::Handle task_b = task_pool.enqueue([] { do_something_else(); });
    /// \endcode
    Task::Handle enqueue(Task::Action action);

    ///
    /// Waits until all enqueued tasks complete.
    void wait();

private:
    void initialize_threads(size_t thread_count);
    void thread_loop();

    std::deque<Task::Handle> _task_queue;

    std::mutex _threads_mutex;
    std::vector<std::thread> _threads;

    std::mutex _queue_mutex;
    std::condition_variable _condition;

    bool _stop { false };
    bool _adaptive { true };

    std::atomic<size_t> _available_thread_count { 0 };
};

}