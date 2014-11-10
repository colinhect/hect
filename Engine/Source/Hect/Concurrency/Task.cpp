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
        throw Error("Invalid task handle");
    }

    return *_task;
}

Task* Task::Handle::operator->() const
{
    if (!_task)
    {
        throw Error("Invalid task handle");
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

    // Re-throw the error if one occurred
    if (_errorOccurred)
    {
        throw _error;
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
    catch (Error& error)
    {
        _errorOccurred = true;
        _error = error;
    }
    catch (std::exception& exception)
    {
        _errorOccurred = true;
        _error = Error(exception.what());
    }
    catch (...)
    {
        _errorOccurred = true;
        _error = Error("Unknown error");
    }

    _done = true;
}