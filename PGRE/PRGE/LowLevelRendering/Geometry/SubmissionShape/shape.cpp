//
//  shape.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 14/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "shape.hpp"

namespace PRGE
{
    IShape::IShape(const Transform& objectToWorld, const Transform& worldToObject, bool reverseOrientation) NOEXCEPT_PRGE :
        objectToWorld{objectToWorld},
        worldToObject{worldToObject},
        reverseOrientation{reverseOrientation},
        transformSwapsHandednes{objectToWorld.swapHandedness()}
    {}

    Interaction IShape::sample(const Interaction& interaction, const Point2<float>& uv) const
    {
        return sample(uv);
    }

    float IShape::pdf(const Interaction& interaction) const
    {
        return 1.0f / arena();
    }
}
