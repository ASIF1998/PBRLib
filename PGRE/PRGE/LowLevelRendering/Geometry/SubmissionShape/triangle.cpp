//
//  triangle.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 28/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "triangle.hpp"

namespace PRGE
{
    BoundingVolume3<float> Triangle::objectBound() const NOEXCEPT_PRGE
    {
        auto p0 = worldToObject(_ptrMesh->ptrVertices[_ptrMesh->indices[_indexInIndecesArray]]);
        auto p1 = worldToObject(_ptrMesh->ptrVertices[_ptrMesh->indices[_indexInIndecesArray + 1]]);
        auto p2 = worldToObject(_ptrMesh->ptrVertices[_ptrMesh->indices[_indexInIndecesArray + 2]]);

        return merge(BoundingVolume3<float>(p0, p1), p2);
    }

    BoundingVolume3<float> Triangle::worldBound() const NOEXCEPT_PRGE
    {
        auto p0 = _ptrMesh->ptrVertices[_ptrMesh->indices[_indexInIndecesArray]];
        auto p1 = _ptrMesh->ptrVertices[_ptrMesh->indices[_indexInIndecesArray + 1]];
        auto p2 = _ptrMesh->ptrVertices[_ptrMesh->indices[_indexInIndecesArray + 2]];

        return merge(BoundingVolume3<float>(p0, p1), p2);
    }

    bool Triangle::intersect(const Ray& ray, float* tHit, SurfaceInteraction* surfaceInteraction, bool testAlphaTexture) const NOEXCEPT_PRGE
    {
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
