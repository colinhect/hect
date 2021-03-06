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
#include <Hect/Graphics/Image.h>
#include <Hect/Noise/Random.h>
using namespace hect;

#include <catch.hpp>

TEST_CASE("Construct a default image", "[Image]")
{
    Image image;

    REQUIRE(image.width() == 0);
    REQUIRE(image.height() == 0);
}

TEST_CASE("Construct a sized image", "[Image]")
{
    Image image(256, 512, PixelFormat::Rgb8);

    REQUIRE(image.width() == 256);
    REQUIRE(image.height() == 512);
    REQUIRE(image.pixel_format() == PixelFormat::Rgb8);
    REQUIRE(!image.has_pixel_data());
}

TEST_CASE("Write and read pixels", "[Image]")
{
    Random random(42);

    SECTION("RGB pixel format")
    {
        SECTION("Single byte pixel type")
        {
            Image image(4, 8, PixelFormat::Rgb8);

            for (unsigned x = 0; x < image.width(); ++x)
            {
                for (unsigned y = 0; y < image.height(); ++y)
                {
                    uint8_t min = 0;
                    uint8_t max = 255;

                    Color color;
                    color.r = static_cast<double>(random.next(min, max)) / 255.0;
                    color.g = static_cast<double>(random.next(min, max)) / 255.0;
                    color.b = static_cast<double>(random.next(min, max)) / 255.0;

                    image.write_pixel(x, y, color);

                    Color read_color = image.read_pixel(x, y);
                    REQUIRE(color == read_color);
                }
            }
        }

        SECTION("32-bit float pixel type")
        {
            Image image(4, 8, PixelFormat::Rgb32);

            for (unsigned x = 0; x < image.width(); ++x)
            {
                for (unsigned y = 0; y < image.height(); ++y)
                {
                    Color color;
                    color.r = random.next(0.0f, 1.0f);
                    color.g = random.next(0.0f, 1.0f);
                    color.b = random.next(0.0f, 1.0f);

                    image.write_pixel(x, y, color);

                    Color read_color = image.read_pixel(x, y);
                    REQUIRE(color == read_color);
                }
            }
        }
    }

    SECTION("RGBA pixel format")
    {
        SECTION("Single byte pixel type")
        {
            Image image(4, 8, PixelFormat::Rgba8);

            for (unsigned x = 0; x < image.width(); ++x)
            {
                for (unsigned y = 0; y < image.height(); ++y)
                {
                    uint8_t min = 0;
                    uint8_t max = 255;

                    Color color;
                    color.r = static_cast<double>(random.next(min, max)) / 255.0;
                    color.g = static_cast<double>(random.next(min, max)) / 255.0;
                    color.b = static_cast<double>(random.next(min, max)) / 255.0;
                    color.a = static_cast<double>(random.next(min, max)) / 255.0;

                    image.write_pixel(x, y, color);

                    Color read_color = image.read_pixel(x, y);
                    REQUIRE(color == read_color);
                }
            }
        }

        SECTION("32-bit float pixel type")
        {
            Image image(4, 8, PixelFormat::Rgba32);

            for (unsigned x = 0; x < image.width(); ++x)
            {
                for (unsigned y = 0; y < image.height(); ++y)
                {
                    Color color;
                    color.r = random.next(0.0f, 1.0f);
                    color.g = random.next(0.0f, 1.0f);
                    color.b = random.next(0.0f, 1.0f);
                    color.a = random.next(0.0f, 1.0f);

                    image.write_pixel(x, y, color);

                    Color read_color = image.read_pixel(x, y);
                    REQUIRE(color == read_color);
                }
            }
        }
    }
}