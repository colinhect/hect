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

namespace hect
{

///
/// Manages a flow of logic layers.
///
/// \remarks All layers in the flow are updated in the order in which they were
/// added.  Inactive layers are removed from the flow.
class LogicFlow :
    public Uncopyable
{
public:

    ///
    /// Constructs a logic flow given the duration of time between each fixed
    /// update.
    ///
    /// \param timeStep The duration of time between each fixed update.
    LogicFlow(TimeSpan timeStep);

    ///
    /// Adds a layer to the flow.
    ///
    /// \remarks The layer will remain in the flow until it is inactive or
    /// explicitly removed.
    ///
    /// \param layer The layer to add.
    void addLayer(LogicLayer& layer);
    
    ///
    /// Removes a layer from the flow.
    ///
    /// \param layer The layer to remove.
    void removeLayer(LogicLayer& layer);

    ///
    /// Removes all layers from the flow.
    void removeAllLayers();

    ///
    /// Updates all layers in the flow.
    ///
    /// \returns True if there are layers in the flow; false otherwise.
    bool update();

private:
    void _removeInactiveLayers();

    Timer _timer;
    TimeSpan _timeStep;
    TimeSpan _accumulator;
    TimeSpan _delta;

    std::vector<LogicLayer*> _layers;
};

}