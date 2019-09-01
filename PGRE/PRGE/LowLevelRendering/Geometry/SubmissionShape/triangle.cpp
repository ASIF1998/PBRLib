//
//  triangle.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 28/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "triangle.hpp"

#include "../../../Core/Math/math.hpp"

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
        float t = tScaled * invDet;

        return true;
    }

    bool Triangle::intersect(const Ray& ray, bool testAlphaTexture) const NOEXCEPT_PRGE
    {
        return true;
    }

    float Triangle::arena() const NOEXCEPT_PRGE
    {
        return 0.f;
    }

    Interaction Triangle::sample(const Point2<float>& uv) const
    {
        return Interaction{};
    }
}
