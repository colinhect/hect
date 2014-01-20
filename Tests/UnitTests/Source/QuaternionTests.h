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

SUITE(Quaternion)
{
    template <typename T>
    void testDefaultConstructor()
    {
        QuaternionT<T> a;
        CHECK_CLOSE((T)0.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, a.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, a.z, (T)epsilon);
        CHECK_CLOSE((T)1.0, a.w, (T)epsilon);
    }

    TEST(DefaultConstructor)
    {
        testDefaultConstructor<float>();
        testDefaultConstructor<double>();
    }

    template <typename T>
    void testComponentConstructor()
    {
        QuaternionT<T> a((T)1.0, (T)2.0, (T)3.0, (T)4.0);
        CHECK_CLOSE((T)1.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)2.0, a.y, (T)epsilon);
        CHECK_CLOSE((T)3.0, a.z, (T)epsilon);
        CHECK_CLOSE((T)4.0, a.w, (T)epsilon);
    }

    TEST(ComponentConstructor)
    {
        testComponentConstructor<float>();
        testComponentConstructor<double>();
    }

    template <typename T>
    void testVector3Constructor()
    {
        QuaternionT<T> a(Vector3T<T>((T)1.0, (T)2.0, (T)3.0), (T)4.0);
        CHECK_CLOSE((T)1.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)2.0, a.y, (T)epsilon);
        CHECK_CLOSE((T)3.0, a.z, (T)epsilon);
        CHECK_CLOSE((T)4.0, a.w, (T)epsilon);
    }

    TEST(Vector3Constructor)
    {
        testVector3Constructor<float>();
        testVector3Constructor<double>();
    }

    template <typename T>
    void testVector4Constructor()
    {
        QuaternionT<T> a(Vector4T<T>((T)1.0, (T)2.0, (T)3.0, (T)4.0));
        CHECK_CLOSE((T)1.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)2.0, a.y, (T)epsilon);
        CHECK_CLOSE((T)3.0, a.z, (T)epsilon);
        CHECK_CLOSE((T)4.0, a.w, (T)epsilon);
    }

    TEST(Vector4Constructor)
    {
        testVector4Constructor<float>();
        testVector4Constructor<double>();
    }

    template <typename T>
    void testCopyConstructor()
    {
        QuaternionT<T> a(QuaternionT<T>((T)1.0, (T)2.0, (T)3.0, (T)4.0));
        CHECK_CLOSE((T)1.0, a.x, (T)epsilon);
        CHECK_CLOSE((T)2.0, a.y, (T)epsilon);
        CHECK_CLOSE((T)3.0, a.z, (T)epsilon);
        CHECK_CLOSE((T)4.0, a.w, (T)epsilon);
    }

    TEST(CopyConstructor)
    {
        testCopyConstructor<float>();
        testCopyConstructor<double>();
    }

    template <typename T>
    void testAxisAngle()
    {
        QuaternionT<T> a = QuaternionT<T>::fromAxisAngle(Vector3T<T>::unitY(), Angle::fromDegrees(180));

        Vector3T<T> axis;
        Angle angle;
        a.toAxisAngle(axis, angle);

        CHECK_CLOSE((T)0.0, axis.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, axis.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, axis.z, (T)epsilon);
        CHECK_CLOSE(180, angle.degrees(), epsilon);
    }

    TEST(AxisAngle)
    {
        testAxisAngle<float>();
        testAxisAngle<double>();
    }

    template <typename T>
    void testMultiply()
    {
        Vector3T<T> axis;
        Angle angle;
        QuaternionT<T> a;
        QuaternionT<T> b;

        a = QuaternionT<T>::fromAxisAngle(Vector3T<T>::unitY(), Angle::fromDegrees(90));
        b = QuaternionT<T>::fromAxisAngle(Vector3T<T>::unitY(), Angle::fromDegrees(180));
        a = b * a;
        a.toAxisAngle(axis, angle);

        CHECK_CLOSE((T)0.0, axis.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, axis.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, axis.z, (T)epsilon);
        CHECK_CLOSE(270, angle.degrees(), epsilon);

        a = QuaternionT<T>::fromAxisAngle(Vector3T<T>::unitY(), Angle::fromDegrees(90));
        b = QuaternionT<T>::fromAxisAngle(Vector3T<T>::unitY(), Angle::fromDegrees(180));
        a *= b;
        a.toAxisAngle(axis, angle);

        CHECK_CLOSE((T)0.0, axis.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, axis.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, axis.z, (T)epsilon);
        CHECK_CLOSE(270, angle.degrees(), epsilon);
    }

    TEST(Multiply)
    {
        testMultiply<float>();
        testMultiply<double>();
    }

    template <typename T>
    void testMultiplyVector()
    {
        QuaternionT<T> r;
        Vector3T<T> v;

        r = QuaternionT<T>::fromAxisAngle(Vector3T<T>::unitY(), Angle::fromDegrees(180));
        v = r * Vector3T<T>::unitX();
        CHECK_CLOSE((T)-1.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        r = QuaternionT<T>::fromAxisAngle(Vector3T<T>::unitY(), Angle::fromDegrees(90));
        v = r * Vector3T<T>::unitX();
        CHECK_CLOSE((T)0.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)1.0, v.z, (T)epsilon);
    }

    TEST(MultiplyVector)
    {
        testMultiplyVector<float>();
        testMultiplyVector<double>();
    }

    template <typename T>
    void testMultiplyVectorByIdentity()
    {
        Vector3T<T> v;

        v = QuaternionT<T>() * Vector3T<T>::unitX();
        CHECK_CLOSE((T)1.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        v = QuaternionT<T>() * Vector3T<T>::unitY();
        CHECK_CLOSE((T)0.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        v = QuaternionT<T>() * Vector3T<T>::unitZ();
        CHECK_CLOSE((T)0.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)1.0, v.z, (T)epsilon);
    }

    TEST(MultiplyVectorByIdentity)
    {
        testMultiplyVectorByIdentity<float>();
        testMultiplyVectorByIdentity<double>();
    }

    template <typename T, typename U>
    void testCast()
    {
        QuaternionT<T> a((T)1.0, (T)2.0, (T)3.0, (T)4.0);
        QuaternionT<U> b = a;

        CHECK_CLOSE(a.x, (T)b.x, (T)epsilon);
        CHECK_CLOSE(a.y, (T)b.y, (T)epsilon);
        CHECK_CLOSE(a.z, (T)b.z, (T)epsilon);
        CHECK_CLOSE(a.w, (T)b.w, (T)epsilon);
    }

    TEST(Cast)
    {
        testCast<float, double>();
        testCast<double, float>();
    }

    template <typename T>
    void testIndex()
    {
        QuaternionT<T> a((T)1.0, (T)2.0, (T)3.0, (T)4.0);

        CHECK_CLOSE((T)1.0, a[0], (T)epsilon);
        CHECK_CLOSE((T)2.0, a[1], (T)epsilon);
        CHECK_CLOSE((T)3.0, a[2], (T)epsilon);
        CHECK_CLOSE((T)4.0, a[3], (T)epsilon);
    }

    TEST(Index)
    {
        testIndex<float>();
        testIndex<double>();
    }
}