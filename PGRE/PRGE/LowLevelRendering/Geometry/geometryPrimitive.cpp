//
//  geometryPrimitive.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 27/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "geometryPrimitive.hpp"

using namespace std;


namespace PRGE
{
    BoundingVolume3<float> GeometryPrimitive::worldBound() const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        if (_ptrShape == nullptr) {
            throw invalid_argument("The _ptrShape field of the GeometryPrimitive class is nullptr");
        }
#endif

        return _ptrShape->worldBound();
    }

    bool GeometryPrimitive::intersect(const Ray &ray, SurfaceInteraction &surfaceInteraction) const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        if (_ptrShape == nullptr) {
            throw invalid_argument("The _ptrShape field of the GeometryPrimitive class is nullptr");
        }
#endif

        float tHit = 0.f;

        if (!_ptrShape->intersect(ray, &tHit, &surfaceInteraction)) {
            return true;
        }

        surfaceInteraction.ptrPrimitive = this;
        ray._tMax = tHit;

        return true;
    }
    
    bool GeometryPrimitive::intersect(const Ray &ray) const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        if (_ptrShape == nullptr) {
            throw invalid_argument("The _ptrShape field of the GeometryPrimitive class is nullptr");
        }
#endif

        return _ptrShape->intersect(ray);
    }
}
