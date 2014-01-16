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
SUITE(Angle)
{
    template <typename T>
    void testDefaultConstructor()
    {
        Angle<T> a;
        CHECK_CLOSE((T)0.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)0.0, a.radians(), (T)epsilon);
    }

    TEST(DefaultConstructor)
    {
        testDefaultConstructor<float>();
        testDefaultConstructor<double>();
    }

    template <typename T>
    void testFromAndToDegrees()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)180.0);
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(FromAndToDegrees)
    {
        testFromAndToDegrees<float>();
        testFromAndToDegrees<double>();
    }

    template <typename T>
    void testFromAndToRadians()
    {
        Angle<T> a = Angle<T>::fromRadians((T)pi);
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(FromAndToRadians)
    {
        testFromAndToRadians<float>();
        testFromAndToRadians<double>();
    }

    template <typename T>
    void testNegate()
    {
        Angle<T> a = Angle<T>::fromRadians((T)pi);
        a = -a;
        CHECK_CLOSE((T)-180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)-pi, a.radians(), (T)epsilon);
    }

    TEST(Negate)
    {
        testNegate<float>();
        testNegate<double>();
    }

    template <typename T>
    void testAdd()
    {
        Angle<T> a = Angle<T>::fromDegrees(45.0) + Angle<T>::fromDegrees(135.0);
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(Add)
    {
        testAdd<float>();
        testAdd<double>();
    }

    template <typename T>
    void testAddEquals()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)45.0);
        a += Angle<T>::fromDegrees((T)135.0);
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(AddEquals)
    {
        testAddEquals<float>();
        testAddEquals<double>();
    }

    template <typename T>
    void testSubtract()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)270.0) - Angle<T>::fromDegrees((T)90.0);
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(Subtract)
    {
        testSubtract<float>();
        testSubtract<double>();
    }

    template <typename T>
    void testSubtractEquals()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)270.0);
        a -= Angle<T>::fromDegrees((T)90.0);
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(SubtractEquals)
    {
        testSubtractEquals<float>();
        testSubtractEquals<double>();
    }

    template <typename T>
    void testMultiply()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)90.0) * (T)2.0;
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(Multiply)
    {
        testMultiply<float>();
        testMultiply<double>();
    }

    template <typename T>
    void testMultiplyEquals()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)90.0);
        a *= (T)2.0;
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(MultiplyEquals)
    {
        testMultiplyEquals<float>();
        testMultiplyEquals<double>();
    }

    template <typename T>
    void testDivide()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)360.0) / (T)2.0;
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(Divide)
    {
        testDivide<float>();
        testDivide<double>();
    }

    template <typename T>
    void testDivideEquals()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)360.0);
        a /= (T)2.0;
        CHECK_CLOSE((T)180.0, a.degrees(), (T)epsilon);
        CHECK_CLOSE((T)pi, a.radians(), (T)epsilon);
    }

    TEST(DivideEquals)
    {
        testDivideEquals<float>();
        testDivideEquals<double>();
    }

    template <typename T, typename U>
    void testCast()
    {
        Angle<T> a = Angle<T>::fromDegrees((T)45.0);
        Angle<U> b = a;

        CHECK_CLOSE(a.degrees(), b.degrees(), (T)epsilon);
        CHECK_CLOSE(a.radians(), b.radians(), (T)epsilon);
    }

    TEST(Cast)
    {
        testCast<float, double>();
        testCast<double, float>();
    }
}