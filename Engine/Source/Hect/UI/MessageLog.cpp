///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "MessageLog.h"

#include "Hect/Timing/Timer.h"

using namespace hect;

MessageLog::MessageLog(const Vector2& position, const Vector2& dimensions, Font::Handle font, double size) :
    Widget(position, dimensions),
    _font(font),
    _size(size)
{
}

void MessageLog::addMessage(const std::string& text)
{
    Message message;
    message.added = Timer::totalElapsed();
    message.text = text;

    _messages.push_front(message);
}

Font::Handle MessageLog::font() const
{
    return _font;
}

void MessageLog::setFont(Font::Handle font, double size)
{
    _font = font;
    _size = size;
}

void MessageLog::tick(double timeStep)
{
    removeExpiredMessages();
}

void MessageLog::render(VectorRenderer::Frame& frame)
{
    if (_font)
    {
        const Vector2 origin = position() + Vector2::UnitY * dimensions().y;

        frame.setFont(*_font, _size);

        // For each message
        size_t index = 0;
        for (const Message& message : _messages)
        {
            // Compute the position/dimensions of this text message
            const Vector2 textPosition(origin - Vector2::UnitY * _lineHeight * static_cast<double>(++index));
            const Vector2 textDimensions(dimensions().x, _lineHeight);

            // Set the color of the text
            const TimeSpan lifeTime = Timer::totalElapsed() - message.added;
            double alpha = 1.0 - lifeTime.seconds() / messageExpiration.seconds();
            frame.setColor(Color(1.0, 1.0, 1.0, alpha));

            // Render the text
            frame.renderText(message.text, textPosition, textDimensions);
        }
    }
}

void MessageLog::removeExpiredMessages()
{
    _messages.erase(std::remove_if(_messages.begin(), _messages.end(),
                                   [=](const Message& message)
    {
        return Timer::totalElapsed() - message.added > messageExpiration;
    }),
    _messages.end());
}