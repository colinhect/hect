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
#include <functional>
#include <memory>
#include <string>

#include "Hect/Concurrency/TaskError.h"
#include "Hect/Core/Exception.h"
#include "Hect/Core/Export.h"
#include "Hect/Core/Uncopyable.h"

namespace hect
{

///
/// An asynchronous action performed in the context of a TaskPool.
class HECT_EXPORT Task :
    public Uncopyable
{
    friend class TaskPool;
public:

    ///
    /// An action for a Task to execute.
    typedef std::function<void()> Action;

    ///
    /// A handle to a Task that has been enqueued in a TaskPool.
    ///
    /// A task handle behaves as a reference counted shared pointer.
    class HECT_EXPORT Handle
    {
        friend class TaskPool;
    public:

        ///
        /// Creates an invalid handle.
        Handle();

        ///
        /// Dereferences the handle to a reference to the associated task.
        ///
        /// \returns A reference to the associated task.
        ///
        /// \throws InvalidOperation If the handle is invalid.
        Task& operator*() const;

        ///
        /// Dereferences the handle to a pointer to the associated task.
        ///
        /// \returns A pointer to the associated task.
        ///
        /// \throws InvalidOperation If the handle is invalid.
        Task* operator->() const;

        ///
        /// Returns whether the handle is valid.
        operator bool() const;

    private:
        Handle(const std::shared_ptr<Task>& task);

        Task& dereference() const;

        std::shared_ptr<Task> _task;
    };

    ///
    /// Waits until the task has completed.
    ///
    /// \throws TaskError If an error occurred while executing the task.
    void wait();

    ///
    /// Returns whether the task has completed.
    bool has_completed() const;

private:
    Task(Task::Action action);

    void execute();

    Task::Action _action;
    std::atomic<bool> _completed { false };
    bool _exception_occurred { false };
    std::string _exception_message;
};

}
