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
#include "TaskPool.h"

#include <algorithm>

using namespace hect;

TaskPool::TaskPool(bool adaptive) :
    _adaptive(adaptive)
{
    const unsigned hardware_thread_count = std::max(std::thread::hardware_concurrency(), 1u);
    if (hardware_thread_count > 1)
    {
        initialize_threads(hardware_thread_count - 1);
    }
}

TaskPool::TaskPool(size_t thread_count, bool adaptive) :
    _adaptive(adaptive)
{
    initialize_threads(thread_count);
}

TaskPool::~TaskPool()
{
    // Notify all threads that the pool is stopping
    {
        std::unique_lock<std::mutex> lock(_queue_mutex);
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
            std::unique_lock<std::mutex> lock(_queue_mutex);

            // If this task pool is adaptive and there are no threads
            // available then initialize another thread
            if (_adaptive && _available_thread_count.load() == 0)
            {
                initialize_threads(1);
            }

            _task_queue.push_back(handle);
        }

        // Notify one of the threads that a new task is available
        _condition.notify_one();
    }

    return handle;
}

void TaskPool::wait()
{
    while (_available_thread_count < _threads.size())
    {
        std::this_thread::yield();
    }
}

void TaskPool::initialize_threads(size_t thread_count)
{
    for (unsigned i = 0; i < thread_count; ++i)
    {
        _threads.push_back(std::thread([this] { thread_loop(); }));
    }
}

void TaskPool::thread_loop()
{
    ++_available_thread_count;
    for (;;)
    {
        Task::Handle task_handle;

        {
            std::unique_lock<std::mutex> lock(_queue_mutex);

            while (!_stop && _task_queue.empty())
            {
                _condition.wait(lock);
            }

            if (_stop)
            {
                --_available_thread_count;
                return;
            }

            // Get the next tasks
            task_handle = _task_queue.front();
            _task_queue.pop_front();
        }

        // Execute the task
        --_available_thread_count;
        Task* task = task_handle._task.get();
        if (task)
        {
            task->execute();
        }
        ++_available_thread_count;
    }
}
