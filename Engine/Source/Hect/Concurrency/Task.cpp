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
#include "Task.h"

#include <thread>

using namespace hect;

Task::Handle::Handle()
{
}

Task& Task::Handle::operator*() const
{
    Task& task = dereference();
    return task;
}

Task* Task::Handle::operator->() const
{
    Task& task = dereference();
    return &task;
}

Task::Handle::operator bool() const
{
    return _task.get() != nullptr;
}

Task::Handle::Handle(const std::shared_ptr<Task>& task) :
    _task(task)
{
}

Task& Task::Handle::dereference() const
{
    if (!_task)
    {
        throw InvalidOperation("Invalid task handle");
    }

    return *_task;
}

void Task::wait()
{
    while (!_completed)
    {
        std::this_thread::yield();
    }

    // Re-throw the exception if one occurred
    if (_exception_occurred)
    {
        throw TaskError(_exception_message);
    }
}

bool Task::has_completed() const
{
    return _completed;
}

Task::Task(Action action) :
    _action(action)
{
}

void Task::execute()
{
    try
    {
        _action();
    }
    catch (const std::exception& exception)
    {
        _exception_occurred = true;
        _exception_message = exception.what();
    }
    catch (...)
    {
        _exception_occurred = true;
        _exception_message = "Unknown exception";
    }

    _completed = true;
}
