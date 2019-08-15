//
//  transform.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 14/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "transform.hpp"

namespace PRGE
{
    Transform::Transform() NOEXCEPT_PRGE
    {}

    Transform::Transform(const Matrix4x4f& transformMatrix) NOEXCEPT_PRGE :
        _m{transformMatrix},
        _invM{inverse(transformMatrix)}
    {}

    Transform::Transform(const Matrix4x4f& transformMatrix, const Matrix4x4f& inverseTransformMatrix) NOEXCEPT_PRGE :
        _m{transformMatrix},
        _invM{inverseTransformMatrix}
    {}

    Transform::Transform(const Transform& transform) NOEXCEPT_PRGE :
        _m{transform._m},
        _invM{transform._invM}
    {}

    Transform::Transform(Transform&& transform) NOEXCEPT_PRGE :
        _m{move(transform._m)},
        _invM{move(transform._invM)}
    {}

    Transform& Transform::operator = (const Transform& transform) NOEXCEPT_PRGE
    {
        _m = transform._m;
        _invM = transform._invM;

        return *this;
    }

    Transform& Transform::operator = (Transform&& transform) NOEXCEPT_PRGE
    {
        _m = move(transform._m);
        _invM = move(transform._invM);

        return *this;
    }

    bool Transform::operator == (const Transform& transform) const noexcept
    {
        return _m == transform._m && _invM == transform._invM;
    }

    bool Transform::operator != (const Transform& transform) const noexcept
    {
        return _m != transform._m || _invM != transform._invM;
    }

    bool Transform::operator < (const Transform& transform) const noexcept
    {
        auto r1 = _mm_cmplt_ps(*reinterpret_cast<const __m128*>(_m._m[0]), *reinterpret_cast<const __m128*>(transform._m._m[0]));
        auto r2 = _mm_cmplt_ps(*reinterpret_cast<const __m128*>(_m._m[1]), *reinterpret_cast<const __m128*>(transform._m._m[1]));
        auto r3 = _mm_cmplt_ps(*reinterpret_cast<const __m128*>(_m._m[2]), *reinterpret_cast<const __m128*>(transform._m._m[2]));
        auto r4 = _mm_cmplt_ps(*reinterpret_cast<const __m128*>(_m._m[3]), *reinterpret_cast<const __m128*>(transform._m._m[3]));

        return r1[0] && r1[1] && r1[2] && r1[3] && r2[0] && r2[1] && r2[2] && r2[3] && r3[0] && r3[1] && r3[2] && r3[3] && r4[0] && r4[1] && r4[2] && r4[3];
    }

    Vec3<float> Transform::operator () (const Vec3<float>& vec3) const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

        auto r1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m._m[0]), *reinterpret_cast<const __m128*>(vec3._xyz));
        auto r2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m._m[1]), *reinterpret_cast<const __m128*>(vec3._xyz));
        auto r3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m._m[2]), *reinterpret_cast<const __m128*>(vec3._xyz));

        return {r1[0] + r1[1] + r1[2], r2[0] + r2[1] + r2[2], r3[0] + r3[1] + r3[2]};
    }

    Point3<float> Transform::operator () (const Point3<float>& point3) const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(point3._xyz[0], point3._xyz[1], point3._xyz[2]);
#endif

        auto r1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m._m[0]), *reinterpret_cast<const __m128*>(point3._xyz));
        auto r2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m._m[1]), *reinterpret_cast<const __m128*>(point3._xyz));
        auto r3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m._m[2]), *reinterpret_cast<const __m128*>(point3._xyz));
        auto r4 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m._m[4]), *reinterpret_cast<const __m128*>(point3._xyz));
        float wp = 1.f / (r4[0] + r4[1] + r4[2]);

        return {(r1[0] + r1[1] + r1[2]) * wp , (r2[0] + r2[1] + r2[2]) * wp, (r3[0] + r3[1] + r3[2]) * wp};
    }

    Normal3f Transform::operator () (const Normal3f& normal) const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif

        auto r1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_invM._m[0]), *reinterpret_cast<const __m128*>(normal._xyz));
        auto r2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_invM._m[1]), *reinterpret_cast<const __m128*>(normal._xyz));
        auto r3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(_invM._m[2]), *reinterpret_cast<const __m128*>(normal._xyz));

        return {r1[0] + r1[1] + r1[2], r2[0] + r2[1] + r2[2], r3[0] + r3[1] + r3[2]};
    }

    Ray Transform::operator () (const Ray& ray) const NOEXCEPT_PRGE
    {
        auto o = this->operator () (ray._o);
        auto dir = this->operator () (ray._dir);

        return {o, dir, ray._time, ray._tMax};
    }

    RayDifferential Transform::operator () (const RayDifferential& rayDifferential) NOEXCEPT_PRGE
    {
        auto o = this->operator()(rayDifferential._o);
        auto dir = this->operator()(rayDifferential._dir);
        RayDifferential outRayDifferential{o, dir, rayDifferential._time, rayDifferential._tMax};

        outRayDifferential._hasDifferentials = rayDifferential._hasDifferentials;
        outRayDifferential._rxOrigin = this->operator () (rayDifferential._rxOrigin);
        outRayDifferential._ryOrigin = this->operator () (rayDifferential._ryOrigin);
        outRayDifferential._rxDirection = this->operator () (rayDifferential._rxDirection);
        outRayDifferential._ryDirection = this->operator () (rayDifferential._ryDirection);

        return outRayDifferential;
    }
}
