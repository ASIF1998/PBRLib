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
                                           float time,
                                           const IShape* s) NOEXCEPT_PRGE :
        Interaction(p, w0, cross(dpdu, dpdv), time),
        uv{uv},
        dpdu{dpdu},
        dpdv{dpdv},
        dndu{dndu},
        dndv{dndv},
        shading{n, dpdu, dpdv, dndu, dndv}
    {}

    SurfaceInteraction::SurfaceInteraction(const SurfaceInteraction& surfaceInteraction) NOEXCEPT_PRGE :
        uv{surfaceInteraction.uv},
        dpdu{surfaceInteraction.dpdu},
        ptrPrimitive{surfaceInteraction.ptrPrimitive},
        dndu{surfaceInteraction.dndu},
        dndv{surfaceInteraction.dndv},
        dpdx{surfaceInteraction.dpdx},
        dpdy{surfaceInteraction.dpdy},
        dudx{surfaceInteraction.dudx},
        dvdx{surfaceInteraction.dvdx},
        dudy{surfaceInteraction.dudy},
        dvdy{surfaceInteraction.dvdy}
    {
        shading.n = surfaceInteraction.shading.n;
        shading.dpdu = surfaceInteraction.shading.dpdu;
        shading.dpdv = surfaceInteraction.shading.dpdv;
        shading.dndu = surfaceInteraction.shading.dndu;
        shading.dndv = surfaceInteraction.shading.dndv;
    }

    SurfaceInteraction::SurfaceInteraction(SurfaceInteraction&& surfaceInteraction) NOEXCEPT_PRGE
    {
        swap(uv, surfaceInteraction.uv);
        swap(dpdu, surfaceInteraction.dpdu);
        swap(ptrPrimitive, surfaceInteraction.ptrPrimitive);
        swap(dndu, surfaceInteraction.dndu);
        swap(dndv, surfaceInteraction.dndv);

        swap(shading.n, surfaceInteraction.shading.n);
        swap(shading.dpdu, surfaceInteraction.shading.dpdu);
        swap(shading.dpdv, surfaceInteraction.shading.dpdv);
        swap(shading.dndu, surfaceInteraction.shading.dndu);
        swap(shading.dndv, surfaceInteraction.shading.dndv);

        swap(dpdx, surfaceInteraction.dpdx);
        swap(dpdy, surfaceInteraction.dpdy);

        swap(dudx, surfaceInteraction.dudx);
        swap(dvdx, surfaceInteraction.dvdx);
        swap(dudy, surfaceInteraction.dudy);
        swap(dvdy, surfaceInteraction.dvdy);
    }

    SurfaceInteraction& SurfaceInteraction::operator = (const SurfaceInteraction& surfaceInteraction) NOEXCEPT_PRGE
    {
        uv = surfaceInteraction.uv;
        dpdu = surfaceInteraction.dpdu;
        ptrPrimitive = surfaceInteraction.ptrPrimitive;
        dndu = surfaceInteraction.dndu;
        dndv = surfaceInteraction.dndv;

        shading.n = surfaceInteraction.shading.n;
        shading.dpdu = surfaceInteraction.shading.dpdu;
        shading.dpdv = surfaceInteraction.shading.dpdv;
        shading.dndu = surfaceInteraction.shading.dndu;
        shading.dndv = surfaceInteraction.shading.dndv;

        dpdx = surfaceInteraction.dpdx;
        dpdy = surfaceInteraction.dpdy;

        dudx = surfaceInteraction.dudx;
        dvdx = surfaceInteraction.dvdx;
        dudy = surfaceInteraction.dudy;
        dvdy = surfaceInteraction.dvdy;

        return *this;
    }

    SurfaceInteraction& SurfaceInteraction::operator = (SurfaceInteraction&& surfaceInteraction) NOEXCEPT_PRGE
    {
        swap(uv, surfaceInteraction.uv);
        swap(dpdu, surfaceInteraction.dpdu);
        swap(ptrPrimitive, surfaceInteraction.ptrPrimitive);
        swap(dndu, surfaceInteraction.dndu);
        swap(dndv, surfaceInteraction.dndv);

        swap(shading.n, surfaceInteraction.shading.n);
        swap(shading.dpdu, surfaceInteraction.shading.dpdu);
        swap(shading.dpdv, surfaceInteraction.shading.dpdv);
        swap(shading.dndu, surfaceInteraction.shading.dndu);
        swap(shading.dndv, surfaceInteraction.shading.dndv);

        swap(dpdx, surfaceInteraction.dpdx);
        swap(dpdy, surfaceInteraction.dpdy);

        swap(dudx, surfaceInteraction.dudx);
        swap(dvdx, surfaceInteraction.dvdx);
        swap(dudy, surfaceInteraction.dudy);
        swap(dvdy, surfaceInteraction.dvdy);

        return *this;
    }

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
