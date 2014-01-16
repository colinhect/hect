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
SUITE(Vector2)
{
    template <typename T>
    void testDefaultConstructor()
    {
        Vector2<T> a;
        CHECK_CLOSE((T)0.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, a.y, (T)epsilon);
    }

    TEST(DefaultConstructor)
    {
        testDefaultConstructor<float>();
        testDefaultConstructor<double>();
    }

    template <typename T>
    void testScalarConstructor()
    {
        Vector2<T> a((T)1.0);
        CHECK_CLOSE((T)1.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, a.y, (T)epsilon);
    }

    TEST(ScalarConstructor)
    {
        testScalarConstructor<float>();
        testScalarConstructor<double>();
    }

    template <typename T>
    void testComponentConstructor()
    {
        Vector2<T> a((T)1.0, (T)2.0);
        CHECK_CLOSE((T)1.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)2.0, a.y, (T)epsilon);
    }

    TEST(ComponentConstructor)
    {
        testComponentConstructor<float>();
        testComponentConstructor<double>();
    }

    template <typename T>
    void testCopyConstructor()
    {
        Vector2<T> a((T)1.0, (T)2.0);
        Vector2<T> b(a);
        CHECK_CLOSE(a.x, b.x, (T)epsilon);
        CHECK_CLOSE(a.y, b.y, (T)epsilon);
    }

    TEST(CopyConstructor)
    {
        testCopyConstructor<float>();
        testCopyConstructor<double>();
    }

    template <typename T>
    void testLength()
    {
        Vector2<T> a((T)1.0, (T)2.0);
        CHECK_CLOSE((T)std::sqrt(a.x * a.x + a.y * a.y), a.length(), (T)epsilon);
    }

    TEST(Length)
    {
        testLength<float>();
        testLength<double>();
    }

    template <typename T>
    void testLengthSquared()
    {
        Vector2<T> a((T)1.0, (T)2.0);
        CHECK_CLOSE(a.length(), (T)std::sqrt(a.lengthSquared()), (T)epsilon);
    }

    TEST(LengthSquared)
    {
        testLengthSquared<float>();
        testLengthSquared<double>();
    }

    template <typename T>
    void testNormalize()
    {
        Vector2<T> a((T)1.0, (T)2.0);
        a.normalize();
        CHECK_CLOSE((T)1.0, a.length(), (T)epsilon);
    }

    TEST(Normalize)
    {
        testNormalize<float>();
        testNormalize<double>();
    }

    template <typename T>
    void testNormalized()
    {
        Vector2<T> a((T)1.0, (T)2.0);
        CHECK_CLOSE((T)1.0, a.normalized().length(), (T)epsilon);
    }

    TEST(Normalized)
    {
        testNormalized<float>();
        testNormalized<double>();
    }

    template <typename T>
    void testAngleFrom()
    {
        CHECK_CLOSE((T)0.0, Vector2<T>::unitX().angleFrom(Vector2<T>::unitX()).degrees(), (T)epsilon);
        CHECK_CLOSE((T)180.0, Vector2<T>::unitX().angleFrom(-Vector2<T>::unitX()).degrees(), (T)epsilon);

        CHECK_CLOSE((T)0.0, Vector2<T>::unitY().angleFrom(Vector2<T>::unitY()).degrees(), (T)epsilon);
        CHECK_CLOSE((T)180.0, Vector2<T>::unitY().angleFrom(-Vector2<T>::unitY()).degrees(), (T)epsilon);

        CHECK_CLOSE((T)90.0, Vector2<T>::unitX().angleFrom(Vector2<T>::unitY()).degrees(), (T)epsilon);
        CHECK_CLOSE((T)90.0, Vector2<T>::unitY().angleFrom(Vector2<T>::unitX()).degrees(), (T)epsilon);
    }

    TEST(AngleFrom)
    {
        testAngleFrom<float>();
        testAngleFrom<double>();
    }

    template <typename T>
    void testAdd()
    {
        Vector2<T> a = Vector2<T>((T)1.0, (T)2.0) + Vector2<T>((T)2.0, (T)4.0);
        CHECK_CLOSE((T)3.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)6.0, a.y, (T)epsilon);
    }

    TEST(Add)
    {
        testAdd<float>();
        testAdd<double>();
    }

    template <typename T>
    void testAddEquals()
    {
        Vector2<T> a = Vector2<T>((T)1.0, (T)2.0);
        a += Vector2<T>((T)2.0, (T)4.0);
        CHECK_CLOSE((T)3.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)6.0, a.y, (T)epsilon);
    }

    TEST(AddEquals)
    {
        testAddEquals<float>();
        testAddEquals<double>();
    }

    template <typename T>
    void testSubtract()
    {
        Vector2<T> a = Vector2<T>((T)1.0, (T)2.0) - Vector2<T>((T)2.0, (T)4.0);
        CHECK_CLOSE(-(T)1.0, a.x, (T)epsilon);
        CHECK_CLOSE(-(T)2.0, a.y, (T)epsilon);
    }

    TEST(Subtract)
    {
        testSubtract<float>();
        testSubtract<double>();
    }

    template <typename T>
    void testSubtractEquals()
    {
        Vector2<T> a = Vector2<T>((T)1.0, (T)2.0);
        a -= Vector2<T>((T)2.0, (T)4.0);
        CHECK_CLOSE(-(T)1.0, a.x, (T)epsilon);
        CHECK_CLOSE(-(T)2.0, a.y, (T)epsilon);
    }

    TEST(SubtractEquals)
    {
        testSubtractEquals<float>();
        testSubtractEquals<double>();
    }

    template <typename T>
    void testMultiply()
    {
        Vector2<T> a = Vector2<T>((T)1.0, (T)2.0) * Vector2<T>((T)2.0, (T)4.0);
        CHECK_CLOSE((T)2.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)8.0, a.y, (T)epsilon);
    }

    TEST(Multiply)
    {
        testMultiply<float>();
        testMultiply<double>();
    }

    template <typename T>
    void testMultiplyEquals()
    {
        Vector2<T> a = Vector2<T>((T)1.0, (T)2.0);
        a *= Vector2<T>((T)2.0, (T)4.0);
        CHECK_CLOSE((T)2.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)8.0, a.y, (T)epsilon);
    }

    TEST(MultiplyEquals)
    {
        testMultiplyEquals<float>();
        testMultiplyEquals<double>();
    }

    template <typename T>
    void testMultiplyScalar()
    {
        Vector2<T> a = Vector2<T>((T)1.0, (T)2.0) * (T)4.0;
        CHECK_CLOSE((T)4.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)8.0, a.y, (T)epsilon);
    }

    TEST(MultiplyScalar)
    {
        testMultiplyScalar<float>();
        testMultiplyScalar<double>();
    }

    template <typename T>
    void testMultiplyScalarEquals()
    {
        Vector2<T> a = Vector2<T>((T)1.0, (T)2.0);
        a *= (T)4.0;
        CHECK_CLOSE((T)4.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)8.0, a.y, (T)epsilon);
    }

    TEST(MultiplyScalarEquals)
    {
        testMultiplyScalarEquals<float>();
        testMultiplyScalarEquals<double>();
    }

    template <typename T>
    void testDivide()
    {
        Vector2<T> a = Vector2<T>((T)6.0, (T)8.0) / Vector2<T>((T)2.0, (T)4.0);
        CHECK_CLOSE((T)3.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)2.0, a.y, (T)epsilon);
    }

    TEST(Divide)
    {
        testDivide<float>();
        testDivide<double>();
    }

    template <typename T>
    void testDivideEquals()
    {
        Vector2<T> a = Vector2<T>((T)6.0, (T)8.0);
        a /= Vector2<T>((T)2.0, (T)4.0);
        CHECK_CLOSE((T)3.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)2.0, a.y, (T)epsilon);
    }

    TEST(DivideEquals)
    {
        testDivideEquals<float>();
        testDivideEquals<double>();
    }

    template <typename T>
    void testDivideScalar()
    {
        Vector2<T> a = Vector2<T>((T)6.0, (T)8.0) / (T)2.0;
        CHECK_CLOSE((T)3.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)4.0, a.y, (T)epsilon);
    }

    TEST(DivideScalar)
    {
        testDivideScalar<float>();
        testDivideScalar<double>();
    }

    template <typename T>
    void testDivideScalarEquals()
    {
        Vector2<T> a = Vector2<T>((T)6.0, (T)8.0);
        a /= (T)2.0;
        CHECK_CLOSE((T)3.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)4.0, a.y, (T)epsilon);
    }

    TEST(DivideScalarEquals)
    {
        testDivideScalarEquals<float>();
        testDivideScalarEquals<double>();
    }

    template <typename T, typename U>
    void testCast()
    {
        Vector2<T> a((T)1.0, (T)2.0);
        Vector2<U> b = a;

        CHECK_CLOSE(a.x, (T)b.x, (T)epsilon);
        CHECK_CLOSE(a.y, (T)b.y, (T)epsilon);
    }

    TEST(Cast)
    {
        testCast<float, double>();
        testCast<double, float>();
    }

    template <typename T>
    void testIndex()
    {
        Vector2<T> a((T)1.0, (T)2.0);

        CHECK_CLOSE((T)1.0, a[0], (T)epsilon);
        CHECK_CLOSE((T)2.0, a[1], (T)epsilon);
    }

    TEST(Index)
    {
        testIndex<float>();
        testIndex<double>();
    }
}