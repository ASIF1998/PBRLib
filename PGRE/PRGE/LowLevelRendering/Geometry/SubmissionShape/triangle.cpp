//
//  triangle.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 28/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "triangle.hpp"

#include "../../../Core/Math/math.hpp"

#include "../../../CollisionAndPhysics/Interaction/surfaceInteraction.hpp"

namespace PRGE
{
    BoundingVolume3<float> Triangle::objectBound() const NOEXCEPT_PRGE
    {
        auto p0 = worldToObject(_ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray]]);
        auto p1 = worldToObject(_ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 1]]);
        auto p2 = worldToObject(_ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 2]]);

        return merge(BoundingVolume3<float>(p0, p1), p2);
    }

    BoundingVolume3<float> Triangle::worldBound() const NOEXCEPT_PRGE
    {
        auto p0 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray]];
        auto p1 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 1]];
        auto p2 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 2]];

        return merge(BoundingVolume3<float>(p0, p1), p2);
    }

    bool Triangle::intersect(const Ray& ray, float* tHit, SurfaceInteraction* surfaceInteraction, bool testAlphaTexture) const NOEXCEPT_PRGE
    {
        auto p0 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray]];
        auto p1 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 1]];
        auto p2 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 2]];

        Vec3<float> rayo {ray._o[0], ray._o[1], ray._o[2]};

        auto p0t = p0 - rayo;
        auto p1t = p1 - rayo;
        auto p2t = p2 - rayo;

        auto indxZ = maxDimension(PRGE::abs(ray._dir));
        auto indxX = indxZ + 1;

        indxX = (indxX == 3 ? 0 : indxX);

        auto indxY = indxX + 1;

        indxY = (indxY == 3 ? 0 : indxY);

        Vec3<float> d {ray._dir[indxX], ray._dir[indxY], ray._dir[indxZ]};

        p0t = Point3<float> (p0t[indxX], p0t[indxY], p0t[indxZ]);
        p1t = Point3<float> (p1t[indxX], p1t[indxY], p1t[indxZ]);
        p2t = Point3<float> (p2t[indxX], p2t[indxY], p2t[indxZ]);

        float sx = -d[0] / d[2];
        float sy = -d[1] / d[2];
        float sz = 1.0f / d[2];

        p0t[0] += sx * p0t[2];
        p0t[1] += sy * p0t[2];
        p1t[0] += sx * p1t[2];
        p1t[1] += sy * p1t[2];
        p2t[0] += sx * p2t[2];
        p2t[1] += sy * p2t[2];

        float e0 = p1t[0] * p2t[1] - p1t[1] * p2t[0];
        float e1 = p2t[0] * p0t[1] - p2t[1] * p0t[0];
        float e2 = p0t[0] * p1t[1] - p0t[1] * p1t[0];
        float det = e0 + e1 + e2;

        // После нахождения значений трёх граничных функций (e_i)у нас есть две 
        // возможности выявления того что нет пересечения. Во-первых, если знаки 
        // трёх граничных функций различны, то это говорит о том, что пересечения
        // нет. Во-вторых, если сумма трёх граничных функций равно нулю, то это 
        // говорит о том что луч приближается к границе треугольника, и мы
        // будем возвращать false.
        if (((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0)) || det == 0) {
            return false;
        }

        // Т. к. луч начинается в начале координат, имеет единичную длинну и расположен 
        // вдоль положительной оси Z, то значение координаты z точки пересечнея равно 
        // параметрическому значению t пересечения.

        p0t[2] *= sz;
        p1t[2] *= sz;
        p2t[2] *= sz;

        float tScaled = e0 * p0t[2] + e1 * p1t[2] + e2 * p2t[2];

        if ((det < 0 && (tScaled >= 0 || tScaled < ray._tMax * det)) || (det > 0 && (tScaled <= 0 || tScaled > ray._tMax * det))) {
            return false;
        }

        float invDet = 1.0f / det;
        float b0 = e0 * invDet;
        float b1 = e1 * invDet;
        float b2 = e2 * invDet;
        *tHit = tScaled * invDet;

        Vec3<float> dpdu, dpdv;
        Point2<float> uv[3];

        auto duv02 = uv[0] - uv[2];
        auto duv12 = uv[1] - uv[2];
        auto v02 = p0 - p2;
        auto v12 = p1 - p2;
        float deter = duv02[0] * duv12[1] - duv02[1] * duv12[0];

        /// В случае если матрица не может быть обращена 
        /// мы будем выбирать произвольную систему координат относительно
        /// нормали к поверхности треугольника (полученные вектора будут
        /// ортогональны).
        if (deter == 0.0f) {
            coordinateSystem(normalize(cross(p2 - p0, p2 - p1)), &dpdu, &dpdv);
        } else {
            float invDeter = 1.0f / deter;
            
            dpdu = (v02 * duv12[1] - v12 * duv02[1]) * invDeter;
            dpdv = (v02 * duv02[0] - v12 * duv12[0]) * invDeter;
        }

        Point3<float> pHit = p0 * b0 + p1 * b1 + p2 * b2;
        Point2<float> uvHit = uv[0] * b0 + uv[1] * b1 + uv[2] * b2;

        /// Если присутстыует альфа-маска и она установлена как fasle
        /// то пересечение будет игнорироваться.
        
        /**
         * TODO: добавить проверка на наличие альфа-маски в меше.
        */
    //    if (testAlphaTexture) {
    //        SurfaceInteraction isectLocal {
    //            pHit,
    //            uvHit,
    //            Vec3<float>{0.0f, 0.0f, 0.0f},
    //            dpdu,
    //            dpdv,
    //            Normal3f{0.0f, 0.0f, 0.0f},
    //            Normal3f{0.0f, 0.0f, 0.0f},
    //            ray._time,
    //            this
    //        };
        
    //        /// Данная чать не дописана, т. к. не реализована
    //        /// текстура.
    //    }

        *surfaceInteraction = SurfaceInteraction {
            pHit,
            uvHit,
            ray._dir * (-1.0f),
            dpdu,
            dpdv,
            Normal3f{0.0f, 0.0f, 0.0f},
            Normal3f{0.0f, 0.0f, 0.0f},
            ray._time,
            this
        };

        surfaceInteraction->n = surfaceInteraction->shading.n = Normal3f(normalize(cross(v02, v12)));

        if (!_ptrMesh->normals.empty() || !_ptrMesh->tangets.empty()) {
            Normal3f ns;

            if (!_ptrMesh->normals.empty()) {
                ns = normalize(_ptrMesh->normals[_ptrMesh->indices[_indexInIndecesArray]] * b0 + 
                               _ptrMesh->normals[_ptrMesh->indices[_indexInIndecesArray + 1]] * b1 +
                               _ptrMesh->normals[_ptrMesh->indices[_indexInIndecesArray + 2]] * b2);
            } else {
                ns = surfaceInteraction->n;
            }

            Vec3<float> ss;

            if (!_ptrMesh->tangets.empty()) {
                ss = normalize(_ptrMesh->tangets[_ptrMesh->indices[_indexInIndecesArray]] * b0 + 
                               _ptrMesh->tangets[_ptrMesh->indices[_indexInIndecesArray + 1]] * b1 +
                               _ptrMesh->tangets[_ptrMesh->indices[_indexInIndecesArray + 2]] * b2);
            }

            auto ts = cross(static_cast<Vec3<float>>(ns), ss);

            if (ts.lengthSquared() > 0.f) {
                ts = normalize(ts);
                ss = cross(ns, ts);
            } else {
                coordinateSystem(static_cast<Vec3<float>>(ns), &ss, &ts);
            }

            Normal3f dndu, dndv;

            if (!_ptrMesh->normals.empty()) {
                Vec2<float> duv02 = uv[0] - uv[2];
                Vec2<float> duv12 = uv[1] - uv[2];
                Normal3f dn1 = _ptrMesh->normals[_ptrMesh->indices[_indexInIndecesArray]] - _ptrMesh->normals[_ptrMesh->indices[_indexInIndecesArray + 2]];
                Normal3f dn2 = _ptrMesh->normals[_ptrMesh->indices[_indexInIndecesArray + 1]] - _ptrMesh->normals[_ptrMesh->indices[_indexInIndecesArray + 2]];

                float determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];

                if (determinant == 0.f) {
                    dndu = dndv = Normal3f{0.0f, 0.0f, 0.0f};
                } else {
                    float invDeterminant = 1.0f / determinant;
                    dndu = (dn1 * duv12[1] - dn2 * duv02[1]) * invDeterminant;
                    dndv = (dn1 * -duv12[0]  + dn2 * duv02[0]) * invDeterminant;
                }
            } else {
                dndu = dndv = Normal3f{0.0f, 0.0f, 0.0f};
            }

            surfaceInteraction->setShadingGeometry(ss, ts, dndu, dndv, true);
        }

        if (!_ptrMesh->normals.empty()) {
            surfaceInteraction->n = faceforward(surfaceInteraction->n, surfaceInteraction->shading.n);
        } else if (reverseOrientation ^ transformSwapsHandednes) {
            surfaceInteraction->n = surfaceInteraction->shading.n = -surfaceInteraction->n;
        }

        return true;
    }

    bool Triangle::intersect(const Ray& ray, bool testAlphaTexture) const NOEXCEPT_PRGE
    {
        auto p0 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray]];
        auto p1 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 1]];
        auto p2 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 2]];

        Vec3<float> rayo {ray._o[0], ray._o[1], ray._o[2]};

        auto p0t = p0 - rayo;
        auto p1t = p1 - rayo;
        auto p2t = p2 - rayo;

        auto indxZ = maxDimension(PRGE::abs(ray._dir));
        auto indxX = indxZ + 1;

        indxX = (indxX == 3 ? 0 : indxX);

        auto indxY = indxX + 1;

        indxY = (indxY == 3 ? 0 : indxY);

        Vec3<float> d {ray._dir[indxX], ray._dir[indxY], ray._dir[indxZ]};

        p0t = Point3<float> (p0t[indxX], p0t[indxY], p0t[indxZ]);
        p1t = Point3<float> (p1t[indxX], p1t[indxY], p1t[indxZ]);
        p2t = Point3<float> (p2t[indxX], p2t[indxY], p2t[indxZ]);

        float sx = -d[0] / d[2];
        float sy = -d[1] / d[2];
        float sz = 1.0f / d[2];

        p0t[0] += sx * p0t[2];
        p0t[1] += sy * p0t[2];
        p1t[0] += sx * p1t[2];
        p1t[1] += sy * p1t[2];
        p2t[0] += sx * p2t[2];
        p2t[1] += sy * p2t[2];

        float e0 = p1t[0] * p2t[1] - p1t[1] * p2t[0];
        float e1 = p2t[0] * p0t[1] - p2t[1] * p0t[0];
        float e2 = p0t[0] * p1t[1] - p0t[1] * p1t[0];
        float det = e0 + e1 + e2;

        // После нахождения значений трёх граничных функций (e_i)у нас есть две 
        // возможности выявления того что нет пересечения. Во-первых, если знаки 
        // трёх граничных функций различны, то это говорит о том, что пересечения
        // нет. Во-вторых, если сумма трёх граничных функций равно нулю, то это 
        // говорит о том что луч приближается к границе треугольника, и мы
        // будем возвращать false.
        if (((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0)) || det == 0) {
            return false;
        }

        // Т. к. луч начинается в начале координат, имеет единичную длинну и расположен 
        // вдоль положительной оси Z, то значение координаты z точки пересечнея равно 
        // параметрическому значению t пересечения.

        p0t[2] *= sz;
        p1t[2] *= sz;
        p2t[2] *= sz;

        float tScaled = e0 * p0t[2] + e1 * p1t[2] + e2 * p2t[2];

        if ((det < 0 && (tScaled >= 0 || tScaled < ray._tMax * det)) || (det > 0 && (tScaled <= 0 || tScaled > ray._tMax * det))) {
            return false;
        }

        return true;
    }

    float Triangle::area() const NOEXCEPT_PRGE
    {
        auto p0 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray]];
        auto p1 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 1]];
        auto p2 = _ptrMesh->vertices[_ptrMesh->indices[_indexInIndecesArray + 2]];
        return 0.5f * cross(p1 - p0, p2 - p0).length();
    }

    Interaction Triangle::sample(const Point2<float>& uv) const
    {
        return Interaction{};
    }

    void Triangle::getUV(Point2<float> uv[3]) const
    {
        if (_ptrMesh->uvs.empty()) {
            uv[0][0] = 0.0f;
            uv[0][1] = 0.0f;

            uv[1][0] = 1.0f;
            uv[1][1] = 0.0f;

            uv[2][0] = 1.0f;
            uv[2][1] = 1.0f;
        } else {
            uv[0] = _ptrMesh->uvs[_ptrMesh->indices[_indexInIndecesArray]];
            uv[1] = _ptrMesh->uvs[_ptrMesh->indices[_indexInIndecesArray + 1]];
            uv[2] = _ptrMesh->uvs[_ptrMesh->indices[_indexInIndecesArray + 2]];
        }
    }
}
