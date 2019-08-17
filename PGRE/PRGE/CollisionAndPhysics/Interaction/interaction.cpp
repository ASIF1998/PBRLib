//
//  interaction.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 16/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "interaction.hpp"

namespace PRGE
{
    Interaction::Interaction() NOEXCEPT_PRGE
    {}

    Interaction::Interaction(const Point3<float>& inp, const Vec3<float>& inw0, const Normal3f& inn, float intime) NOEXCEPT_PRGE :
        p{inp},
        w0{inw0},
        n{inn},
        time{intime}
    {}

    Interaction::Interaction(const Interaction& interaction) NOEXCEPT_PRGE :
        p{interaction.p},
        w0{interaction.w0},
        n{interaction.n},
        time{interaction.time}
    {}

    Interaction::Interaction(Interaction&& interaction) NOEXCEPT_PRGE :
        p{move(interaction.p)},
        w0{move(interaction.w0)},
        n{move(interaction.n)},
        time{interaction.time}
    {}

    Interaction& Interaction::operator = (const Interaction& interaction) NOEXCEPT_PRGE
    {
        p = interaction.p;
        w0 = interaction.w0;
        n = interaction.n;
        time = interaction.time;

        return *this;
    }

    Interaction& Interaction::operator = (Interaction&& interaction) NOEXCEPT_PRGE
    {
        p = move(interaction.p);
        w0 = move(interaction.w0);
        n = move(interaction.n);
        time = interaction.time;

        return *this;
    }

    bool Interaction::isSurfaceInteraction() const noexcept
    {
        const static Normal3f zn;
        return zn != n;
    }
}
