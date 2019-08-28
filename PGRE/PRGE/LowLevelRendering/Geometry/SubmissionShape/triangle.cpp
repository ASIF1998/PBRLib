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
        return {};
    }

    BoundingVolume3<float> Triangle::worldBound() const NOEXCEPT_PRGE
    {
        return {};
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
