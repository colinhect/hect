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
SUITE(Quaternion)
{
    template <typename T>
    void testDefaultConstructor()
    {
        Quaternion<T> a;
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
        Quaternion<T> a((T)1.0, (T)2.0, (T)3.0, (T)4.0);
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
        Quaternion<T> a(Vector3<T>((T)1.0, (T)2.0, (T)3.0), (T)4.0);
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
        Quaternion<T> a(Vector4<T>((T)1.0, (T)2.0, (T)3.0, (T)4.0));
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
        Quaternion<T> a(Quaternion<T>((T)1.0, (T)2.0, (T)3.0, (T)4.0));
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
        Quaternion<T> a = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)180.0));

        Vector3<T> axis;
        Angle<T> angle;
        a.toAxisAngle(axis, angle);

        CHECK_CLOSE((T)0.0, axis.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, axis.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, axis.z, (T)epsilon);
        CHECK_CLOSE((T)180.0, angle.degrees(), (T)epsilon);
    }

    TEST(AxisAngle)
    {
        testAxisAngle<float>();
        testAxisAngle<double>();
    }

    template <typename T>
    void testMultiply()
    {
        Vector3<T> axis;
        Angle<T> angle;
        Quaternion<T> a;
        Quaternion<T> b;

        a = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)90.0));
        b = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)180.0));
        a = b * a;
        a.toAxisAngle(axis, angle);

        CHECK_CLOSE((T)0.0, axis.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, axis.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, axis.z, (T)epsilon);
        CHECK_CLOSE((T)270.0, angle.degrees(), (T)epsilon);

        a = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)90.0));
        b = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)180.0));
        a *= b;
        a.toAxisAngle(axis, angle);

        CHECK_CLOSE((T)0.0, axis.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, axis.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, axis.z, (T)epsilon);
        CHECK_CLOSE((T)270.0, angle.degrees(), (T)epsilon);
    }

    TEST(Multiply)
    {
        testMultiply<float>();
        testMultiply<double>();
    }

    template <typename T>
    void testMultiplyVector()
    {
        Quaternion<T> r;
        Vector3<T> v;

        r = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)180.0));
        v = r * Vector3<T>::unitX();
        CHECK_CLOSE((T)-1.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        r = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)90.0));
        v = r * Vector3<T>::unitX();
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
        Vector3<T> v;

        v = Quaternion<T>() * Vector3<T>::unitX();
        CHECK_CLOSE((T)1.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        v = Quaternion<T>() * Vector3<T>::unitY();
        CHECK_CLOSE((T)0.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        v = Quaternion<T>() * Vector3<T>::unitZ();
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
        Quaternion<T> a((T)1.0, (T)2.0, (T)3.0, (T)4.0);
        Quaternion<U> b = a;

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
        Quaternion<T> a((T)1.0, (T)2.0, (T)3.0, (T)4.0);

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