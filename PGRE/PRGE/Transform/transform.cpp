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

    BoundingVolume3<float> Transform::operator () (const BoundingVolume3<float>& boundingVolume3) const NOEXCEPT_PRGE
    {
        BoundingVolume3<float> out {this->operator () (boundingVolume3._pMin)};
        out = merge(out, this->operator () (Point3<float>{boundingVolume3._pMax._xyz[0], boundingVolume3._pMin._xyz[1], boundingVolume3._pMin._xyz[2]}));
        out = merge(out, this->operator () (Point3<float>{boundingVolume3._pMin._xyz[0], boundingVolume3._pMax._xyz[1], boundingVolume3._pMin._xyz[2]}));
        out = merge(out, this->operator () (Point3<float>{boundingVolume3._pMax._xyz[0], boundingVolume3._pMax._xyz[1], boundingVolume3._pMin._xyz[2]}));
        out = merge(out, this->operator () (Point3<float>{boundingVolume3._pMin._xyz[0], boundingVolume3._pMin._xyz[1], boundingVolume3._pMax._xyz[2]}));
        out = merge(out, this->operator () (Point3<float>{boundingVolume3._pMax._xyz[0], boundingVolume3._pMin._xyz[1], boundingVolume3._pMax._xyz[2]}));
        out = merge(out, this->operator () (Point3<float>{boundingVolume3._pMin._xyz[0], boundingVolume3._pMax._xyz[1], boundingVolume3._pMax._xyz[2]}));
        out = merge(out, this->operator () (Point3<float>{boundingVolume3._pMax}));

        return out;
    }

    SurfaceInteraction Transform::operator () (const SurfaceInteraction& surfaceInteraction) const NOEXCEPT_PRGE
    {
        SurfaceInteraction out;

        out.p = this->operator () (surfaceInteraction.p);
        out.w0 = normalize(this->operator () (surfaceInteraction.w0));
        out.n = normalize(this->operator () (surfaceInteraction.n));
        out.time = surfaceInteraction.time;
        out.uv = surfaceInteraction.uv;
        out.dpdu = this->operator () (surfaceInteraction.dpdu);
        out.dpdv = this->operator () (surfaceInteraction.dpdv);
        out.dndu = this->operator () (surfaceInteraction.dndu);
        out.dndv = this->operator () (surfaceInteraction.dndv);
        out.dpdx = this->operator () (surfaceInteraction.dpdx);
        out.dpdy = this->operator () (surfaceInteraction.dpdy);
        out.dudx = surfaceInteraction.dudx;
        out.dudy = surfaceInteraction.dudy;
        out.dvdx = surfaceInteraction.dvdx;
        out.dvdy = surfaceInteraction.dvdy;
        out.shading.n = normalize(this->operator () (surfaceInteraction.shading.n));
        out.shading.dpdu = this->operator () (surfaceInteraction.shading.dpdu);
        out.shading.dpdv = this->operator () (surfaceInteraction.shading.dpdv);
        out.shading.dndu = this->operator () (surfaceInteraction.shading.dndu);
        out.shading.dndv = this->operator () (surfaceInteraction.shading.dndv);

        return out;
    }

    Transform translate(const Vec3<float>& t) NOEXCEPT_PRGE
    {
        return {
            {
                0.0f, 0.0f, 0.0f, t[0],
                0.0f, 0.0f, 0.0f, t[1],
                0.0f, 0.0f, 0.0f, t[2],
                0.0f, 0.0f, 0.0f, 1.0f
            }
        };
    }

    Transform scale(float scaleX, float scaleY, float scaleZ) NOEXCEPT_PRGE
    {
        Matrix4x4f m {
            scaleX, 0.0f, 0.0f, 0.0f,
            0.0f, scaleY, 0.0f, 0.0f,
            0.0f, 0.0f, scaleZ, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        Matrix4x4f invM {
            1.0f / scaleX, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f / scaleY, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f / scaleZ, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        return {m, invM};
    }

    Transform rotateX(float angle) NOEXCEPT_PRGE
    {
        float rad = angle * 180.0f * M_1_PI;
        float sinTheta = sin(rad);
        float cosTheta = cos(rad);

        Matrix4x4f m {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, cosTheta, -1.0f * sinTheta, 0.0f,
            0.0f, sinTheta, cosTheta, 0.0f, 
            0.0f, 0.0f, 0.0f, 1.0f
        };

        return {m, transpose(m)};
    }

    Transform rotateY(float angle) NOEXCEPT_PRGE
    {
        float rad = angle * 180.0f * M_1_PI;
        float sinTheta = sin(rad);
        float cosTheta = cos(rad);

        Matrix4x4f m {
            cosTheta, 0.0f, sinTheta, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            (-1.0f) * sinTheta, 0.0f, cosTheta, 0.0f, 
            0.0f, 0.0f, 0.0f, 1.0f
        };

        return {m, transpose(m)};
    }

    Transform rotateZ(float angle) NOEXCEPT_PRGE
    {
        float rad = angle * 180.0f * M_1_PI;
        float sinTheta = sin(rad);
        float cosTheta = cos(rad);

        Matrix4x4f m {
            cosTheta, (-1.0f) * sinTheta, 0.0f, 0.0f,
            sinTheta, cosTheta, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 
            0.0f, 0.0f, 0.0f, 1.0f
        };

        return {m, transpose(m)};
    }

    Transform rotate(float angle, Vec3<float>& axis) NOEXCEPT_PRGE
    {
        auto a = normalize(axis);
        float rad = angle * 180.0f * M_1_PI;
        float sinTheta = sin(rad);
        float cosTheta = cos(rad);

        Matrix4x4f m {
            a[0] * a[0] + (1.0f - a[0] * a[0]) * cosTheta,
            a[0] * a[1] * (1.0f - cosTheta) - a[2] * sinTheta,
            a[0] * a[2] * (1.0f - cosTheta) + a[1] * sinTheta,
            0.0f,
            a[0] * a[1] * (1.0f - cosTheta) + a[2] * sinTheta,
            a[1] * a[1] + (1.0f - a[1] * a[1]) * cosTheta,
            a[1] * a[2] * (1.0f - cosTheta) - a[0] * sinTheta,
            0.0f,
            a[0] * a[2] * (1.0f - cosTheta) - a[1] * sinTheta,
            a[1] * a[2] * (1.0f - cosTheta) + a[0] * sinTheta,
            a[2] * a[2] + (1.0f - a[2] * a[2]) * cosTheta,
            0.0f,
            0.0f,
            0.0f,
            0.0f,
            1.0f
        };

        return {m, transpose(m)};
    }
    
    Transform lookAt(Point3<float>& pos, Point3<float>& look, Vec3<float>& up) NOEXCEPT_PRGE
    {
        auto dir = normalize(look - pos);
        auto r = normalize(cross(up, dir));
        auto n = normalize(cross(dir, r));

        Matrix4x4f viewMatrix {
            r[0], n[0], dir[0], pos[0],
            r[1], n[1], dir[1], pos[1],
            r[2], n[2], dir[2], pos[2],
            0.0f, 0.0f, 0.0f, 1.0f
        };

        return {inverse(viewMatrix), viewMatrix};
    }
}
