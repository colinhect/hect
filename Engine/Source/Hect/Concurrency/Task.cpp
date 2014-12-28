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
#include "Task.h"

#include <thread>

using namespace hect;

Task::Handle::Handle()
{
}

Task& Task::Handle::operator*() const
{
    if (!_task)
    {
        throw InvalidOperation("Invalid task handle");
    }

    return *_task;
}

Task* Task::Handle::operator->() const
{
    if (!_task)
    {
        throw InvalidOperation("Invalid task handle");
    }

    return _task.get();
}

Task::Handle::operator bool() const
{
    return _task.get() != nullptr;
}

Task::Handle::Handle(const std::shared_ptr<Task>& task) :
    _task(task)
{
}

void Task::wait()
{
    while (!_done)
    {
        std::this_thread::yield();
    }

    // Re-throw the exception if one occurred
    if (_exceptionOccurred)
    {
        throw TaskError(_exceptionMessage);
    }
}

bool Task::isDone() const
{
    return _done;
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
        _exceptionOccurred = true;
        _exceptionMessage = exception.what();
    }
    catch (...)
    {
        _exceptionOccurred = true;
        _exceptionMessage = "Unknown exception";
    }

    _done = true;
}