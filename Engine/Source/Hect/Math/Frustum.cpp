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
#include "Frustum.h"

namespace hect
{

Frustum::Frustum()
{
}

Frustum::Frustum(
    Vector3 position,
    Vector3 front,
    Vector3 up,
    Radians field_of_view,
    double aspect_ratio,
    double near_clip,
    double far_clip) :
    _position(position)
{
    double tangent = static_cast<double>(std::tan(field_of_view.value * 0.5));

    double near_height = near_clip * tangent;
    double near_width = near_height * aspect_ratio;

    double far_height = far_clip * tangent;
    double far_width = far_height * aspect_ratio;

    Vector3 z = -front;
    z.normalize();

    Vector3 x = up.cross(z);
    x.normalize();

    Vector3 y = z.cross(x);

    // Compute center of near/far planes
    Vector3 near_center = position - z * near_clip;
    Vector3 far_center = position - z * far_clip;

    // Compute corners of near plane
    Vector3 near_top_left = near_center + y * near_height - x * near_width;
    Vector3 near_top_right = near_center + y * near_height + x * near_width;
    Vector3 near_bottom_left = near_center - y * near_height - x * near_width;
    Vector3 near_bottom_right = near_center - y * near_height + x * near_width;

    // Compute corners of far plane
    Vector3 far_top_left = far_center + y * far_height - x * far_width;
    Vector3 far_top_right = far_center + y * far_height + x * far_width;
    Vector3 far_bottom_left = far_center - y * far_height - x * far_width;
    Vector3 far_bottom_right = far_center - y * far_height + x * far_width;

    // Build planes from points
    _planes[0] = Plane::from_points(near_top_right, near_top_left, far_top_left); // Top
    _planes[1] = Plane::from_points(near_bottom_left, near_bottom_right, far_bottom_right); // Bottom
    _planes[2] = Plane::from_points(near_top_left, near_bottom_left, far_bottom_left); // Left
    _planes[3] = Plane::from_points(near_bottom_right, near_top_right, far_bottom_right); // Right
    _planes[4] = Plane::from_points(near_top_left, near_top_right, near_bottom_right); // Near
    _planes[5] = Plane::from_points(far_top_right, far_top_left, far_bottom_left); // Far
}

FrustumTestResult Frustum::test_axis_aligned_box(const AxisAlignedBox& box) const
{
    FrustumTestResult result = FrustumTestResult::Inside;

    if (!box.has_size())
    {
        return result;
    }

    for (int i = 0; i < 6; i++)
    {
        Vector3 normal(_planes[i].normal());

        Vector3 minimum(box.minimum());
        Vector3 maximum(box.maximum());

        Vector3 p(minimum);
        Vector3 n(maximum);

        if (normal.x >= 0)
        {
            p.x = maximum.x;
            n.x = minimum.x;
        }

        if (normal.y >= 0)
        {
            p.y = maximum.y;
            n.y = minimum.y;
        }

        if (normal.z >= 0)
        {
            p.z = maximum.z;
            n.z = minimum.z;
        }

        double distance_p = _planes[i].distance() + normal.dot(p);
        double distance_n = _planes[i].distance() + normal.dot(n);

        if (distance_p < 0)
        {
            return FrustumTestResult::Outside;
        }
        else if (distance_n < 0)
        {
            result = FrustumTestResult::Intersect;
        }
    }

    return result;
}

bool Frustum::contains_sphere(Sphere sphere, Vector3 position) const
{
    double radius = sphere.radius();

    for (int i = 0; i < 6; ++i)
    {
        double distance = _planes[i].distance() + _planes[i].normal().dot(position);

        if (distance < -radius)
        {
            return false;
        }
    }

    return true;
}

}
