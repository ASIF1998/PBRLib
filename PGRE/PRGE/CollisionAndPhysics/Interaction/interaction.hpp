//
//  interaction.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 16/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef interaction_hpp
#define interaction_hpp

#include "../../Core/core.h"

#include "../../Core/Math/vec.h"
#include "../../Core/Math/normal.h"
#include "../../Core/Math/point.h"

namespace PRGE
{
    /**
     * @field p точка, связаная с взаимодействием
     * @field wo отрицательное направление луча
     * @field n нормаль поверхности в точке p 
    */
    struct Interaction
    {
        Interaction() NOEXCEPT_PRGE;

        /**
         * Конструктор.
         * 
         * @param inp точка, связаная с взаимодействием
         * @param inw0 отрицательное направление луча
         * @param inn нормаль поверхности в точке inp 
         * @param intime время связанное с взаимодействием
        */
        Interaction(const Point3<float>& inp, const Vec3<float>& inw0, const Normal3f& inn, float intime) NOEXCEPT_PRGE;

        inline Interaction(const Interaction& interaction) NOEXCEPT_PRGE;
        Interaction(Interaction&& interaction) NOEXCEPT_PRGE;

        inline Interaction& operator = (const Interaction& interaction) NOEXCEPT_PRGE;
        inline Interaction& operator = (Interaction&& interaction) NOEXCEPT_PRGE;

        /**
         * Т. к. для взаимодействя на поверхности необходиима нормаль,
         * данный метод опредеяет является ли она нулевой или нет.
         * 
         * @return true - если нормаль не 0'ая, иначе false
        */
        inline bool isSurfaceInteraction() const noexcept;

        Point3<float> p;
        Vec3<float> w0;
        Normal3f n;
        float time;
    };
}

#endif /* interaction_hpp */
