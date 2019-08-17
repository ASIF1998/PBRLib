//
//  surfaceInteraction.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 16/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "surfaceInteraction.hpp"

#include "../../Core/Math/vec.h"
#include "../../Core/Math/math.hpp"

namespace PRGE
{
    SurfaceInteraction::SurfaceInteraction() NOEXCEPT_PRGE
    {}

    SurfaceInteraction::SurfaceInteraction(const Point3<float>& p, 
                                           const Point2<float>& uv, 
                                           const Vec3<float>& w0,
                                           const Vec3<float>& dpdu, 
                                           const Vec3<float>& dpdv, 
                                           const Normal3f& dndu, 
                                           const Normal3f& dndv, 
                                           float time) NOEXCEPT_PRGE :
        Interaction(p, w0, cross(dpdu, dpdv), time),
        uv{uv},
        dpdu{dpdu},
        dpdv{dpdv},
        dndu{dndu},
        dndv{dndv},
        shading{n, dpdu, dpdv, dndu, dndv}
    {}

    void SurfaceInteraction::setShadingGeometry(const Vec3<float>& dpdu, 
                                                const Vec3<float>& dpdv, 
                                                const Normal3f& dndu, 
                                                const Normal3f& dndv, 
                                                bool orientationIsAuthoritative) NOEXCEPT_PRGE
    {
        shading.n = normalize(cross(dpdu, dpdv));
        shading.dpdu = dpdu;
        shading.dpdv = dpdv;
        shading.dndu = dndu;
        shading.dndv = dndv;

        if (orientationIsAuthoritative) {
            n = faceforward(n, shading.n);
        } else {
            shading.n = faceforward(n, shading.n);
        }
    }
}
