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
SUITE(Matrix4)
{
    template <typename T>
    void testMultiplyVector()
    {
        Quaternion<T> r;
        Matrix4<T> m;
        Vector3<T> v;

        r = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)180.0));
        m = Matrix4<T>::fromRotation(r);
        v = m * Vector3<T>::unitX();
        CHECK_CLOSE((T)-1.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        r = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)90.0));
        m = Matrix4<T>::fromRotation(r);
        v = m * Vector3<T>::unitX();
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
        Vector3<T> v = Matrix4<T>() * Vector3<T>::unitX();
        CHECK_CLOSE((T)1.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        v = Matrix4<T>() * Vector3<T>::unitY();
        CHECK_CLOSE((T)0.0, v.x, (T)epsilon);
        CHECK_CLOSE((T)1.0, v.y, (T)epsilon);
        CHECK_CLOSE((T)0.0, v.z, (T)epsilon);

        v = Matrix4<T>() * Vector3<T>::unitZ();
        CHECK_CLOSE((T)0.0, v.x, epsilon);
        CHECK_CLOSE((T)0.0, v.y, epsilon);
        CHECK_CLOSE((T)1.0, v.z, epsilon);
    }

    TEST(MultiplyVectorByIdentity)
    {
        testMultiplyVectorByIdentity<float>();
        testMultiplyVectorByIdentity<double>();
    }

    template <typename T, typename U>
    void testCast()
    {
        Quaternion<T> r = Quaternion<T>::fromAxisAngle(Vector3<T>::unitY(), Angle<T>::fromDegrees((T)180.0));
        Matrix4<T> a = Matrix4<T>::fromRotation(r);
        Matrix4<U> b = a;

        for (size_t i = 0; i < 16; ++i)
        {
            CHECK_CLOSE((T)a[i], (T)b[i], (T)epsilon);
        }
    }

    TEST(Cast)
    {
        testCast<float, double>();
        testCast<double, float>();
    }
}