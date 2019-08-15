//
//  boundingVolume.inl
//  PRGE
//
//  Created by Асиф Мамедов on 15/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Math/vec.h"

#include "core.h"

#include <limits>

using namespace std;

namespace PRGE
{
    template<typename Type>
    class BoundingVolume2
    {
    public:
        inline BoundingVolume2() NOEXCEPT_PRGE
        {
            auto min = numeric_limits<Type>::min();
            auto max = numeric_limits<Type>::max();

            _pMin = Vec2<Type>{min, min, min};
            _pMax = Vec2<Type>{max, max, max};
        }

        inline BoundingVolume2(const Vec2<Type>& p1, const Vec2<Type>& p2) NOEXCEPT_PRGE :
            _pMin{min(p1[0], p2[0]), min(p1[1], p2[1])},
            _pMax{max(p1[0], p2[0]), max(p1[1], p2[1])}
        {}

        inline BoundingVolume2(const BoundingVolume2& boundingVolume2) NOEXCEPT_PRGE :
            _pMin{boundingVolume2._pMin},
            _pMax{boundingVolume2._pMax}
        {}

        inline BoundingVolume2(BoundingVolume2&& boundingVolume2) NOEXCEPT_PRGE :
            _pMin{forward<Vec3<float>>(boundingVolume2._pMin)},
            _pMax{forward<Vec3<float>>(boundingVolume2._pMax)}
        {}

        inline BoundingVolume2& operator = (const BoundingVolume2& boundingVolume2) NOEXCEPT_PRGE
        {
            _pMin = boundingVolume2._pMin;
            _pMax = boundingVolume2._pMax;

            return *this;
        }

        inline BoundingVolume2& operator = (BoundingVolume2&& boundingVolume2) NOEXCEPT_PRGE
        {
            _pMin = forward<Vec3<float>>(boundingVolume2._pMin);
            _pMax = forward<Vec3<float>>(boundingVolume2._pMax);

            return *this;
        }

        inline bool operator == (const BoundingVolume2& boundingVolume2) NOEXCEPT_PRGE
        {
            return _pMin == boundingVolume2._pMax && _pMax == boundingVolume2._pMax;
        }

        inline bool operator != (const BoundingVolume2& boundingVolume2) NOEXCEPT_PRGE
        {
            return _pMin != boundingVolume2._pMax || _pMax != boundingVolume2._pMax;
        }


        template<typename U>
        inline explicit operator BoundingVolume2<U> () const
        {
            return {static_cast<Vec2<U>>(_pMin), static_cast<Vec2<U>>(_pMax)};
        }

        inline Vec2<Type> diagonal() const NOEXCEPT_PRGE
        {
            return _pMax - _pMin;
        }

        inline Type arena() const NOEXCEPT_PRGE
        {
            auto ar = _pMax - _pMin;
            return ar[0] * ar[1];
        }

        /**
         * Метод определяющий по какой из осей коробка вытянутей всего.
         * 
         * @return 0 - по оси X, 1 - по оси Y
        */
        int stretchingInTheCoordinateAxis() const NOEXCEPT_PRGE
        {
            auto d = _pMax - _pMin;
            return d[0] > d[1];
        }

        /**
         * Функция осуществляющая линейную интерполяцию между крайними углами ограничивающего
         * пространства представленного в виде квадрата.
         * 
         * @param t параметр интерполяции
         * @return промежуточное значение полученное с помощью линейной интерполяции.
        */
        inline Vec2<float> lerp(const Vec2<float>& t) const
        {
            return {_pMax[0] * (1.0f - t[0]) + _pMax[0] * t[0], _pMax[1] * (1.0f - t[1]) + _pMax[1] * t[1]};
        }
        
        /**
         * Возвращает координату точки относительно ограничивающего квадрата.
         *
         * @param p точка
         * @return координата точки p  относительно ограничивающей коробки
        */
        Vec2<Type> offset(const Point2<Type>& p) const NOEXCEPT_PRGE
        {
            auto o = p - _pMin;
            auto d = _pMax - _pMin;

            if (_pMax[0] > _pMin[0]) {
                o[0] /= d[0];
            }

            if (_pMax[1] > _pMin[1]) {
                o[1] /= d[1];
            }

            return o;
        }

    private:
        Vec2<Type> _pMin;
        Vec2<Type> _pMax;
    };
}
