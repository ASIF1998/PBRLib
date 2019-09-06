//
//  boundingVolume.inl
//  PRGE
//
//  Created by Асиф Мамедов on 15/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Math/vec.h"
#include "Math/point.h"

#include "core.h"

#include "../CollisionAndPhysics/ray.hpp"

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

        inline BoundingVolume2(const Point2<Type>& p1, const Point2<Type>& p2) NOEXCEPT_PRGE :
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
            _pMin = forward<Point2<float>>(boundingVolume2._pMin);
            _pMax = forward<Point2<float>>(boundingVolume2._pMax);

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

        inline const Point2<Type>& operator[](size_t i) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 1) {
                throw out_of_range("This operator only processes arguments within [0, 1]");
            }
#endif

            return (i == 0) ? _pMin : _pMax;
        }

        inline Point2<Type>& operator[](size_t i)
        {
#if DEBUG_PRGE == 1
            if (i > 1) {
                throw out_of_range("This operator only processes arguments within [0, 1]");
            }
#endif
            
            return (i == 0) ? _pMin : _pMax;
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
        inline Point2<float> lerp(const Point2<float>& t) const
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
        Point2<Type> _pMin;
        Point2<Type> _pMax;
    };

    template<typename Type>
    class BoundingVolume3
    {
        friend class Transform;
        friend class BVHPrimitiveInfo;
        friend class BVH;

    public:
        inline BoundingVolume3() NOEXCEPT_PRGE
        {
            auto min = numeric_limits<Type>::min();
            auto max = numeric_limits<Type>::max();

            _pMin = Point3<Type>{min, min, min};
            _pMax = Point3<Type>{max, max, max};
        }

        inline BoundingVolume3(const Point3<float>& point3) NOEXCEPT_PRGE :
            _pMin{point3},
            _pMax{point3}
        {}

        inline BoundingVolume3(const Point3<Type>& p1, const Point3<Type>& p2) NOEXCEPT_PRGE :
            _pMin{min(p1[0], p2[0]), min(p1[1], p2[1]), min(p1[2], p2[2])},
            _pMax{max(p1[0], p2[0]), max(p1[1], p2[1]), max(p1[2], p2[2])}
        {}

        inline BoundingVolume3(const Vec3<float>& v1, const Vec3<float>& v2) NOEXCEPT_PRGE :
            _pMin{min(v1[0], v2[0]), min(v1[1], v2[1]), min(v1[2], v2[2])},
            _pMax{max(v1[0], v2[0]), max(v1[1], v2[1]), max(v1[2], v2[2])}
        {}

        inline BoundingVolume3(const BoundingVolume3& boundingVolume3) NOEXCEPT_PRGE :
            _pMin{boundingVolume3._pMin},
            _pMax{boundingVolume3._pMax}
        {}

        inline BoundingVolume3(BoundingVolume3&& boundingVolume3) NOEXCEPT_PRGE :
            _pMin{forward<Point3<Type>>(boundingVolume3._pMin)},
            _pMax{forward<Point3<Type>>(boundingVolume3._pMax)}
        {}

        inline BoundingVolume3& operator = (const BoundingVolume3& boundingVolume3) NOEXCEPT_PRGE
        {
            _pMin = boundingVolume3._pMin;
            _pMax = boundingVolume3._pMax;

            return *this;
        }

        inline BoundingVolume3& operator = (BoundingVolume3&& boundingVolume3) NOEXCEPT_PRGE
        {
            _pMin = forward<Point3<Type>>(boundingVolume3._pMin);
            _pMax = forward<Point3<Type>>(boundingVolume3._pMax);

            return *this;
        }

        inline bool operator == (const BoundingVolume3& boundingVolume3) const noexcept
        {
            return _pMin == boundingVolume3._pMin && _pMax == boundingVolume3._pMax;
        }

        inline bool operator != (const BoundingVolume3& boundingVolume3) const noexcept
        {
            return _pMin != boundingVolume3._pMin || _pMax != boundingVolume3._pMax;
        }

        inline const Point3<Type>& operator[](size_t i) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 1) {
                throw out_of_range("This operator only processes arguments within [0, 1]");
            }
#endif

            return (i == 0) ? _pMin : _pMax;
        }

        inline Point3<Type>& operator[](size_t i) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 1) {
                throw out_of_range("This operator only processes arguments within [0, 1]");
            }
#endif

            return (i == 0) ? _pMin : _pMax;
        }

        inline Vec3<Type> diagonal() const NOEXCEPT_PRGE
        {
            return _pMax - _pMin;
        }

        /**
         * Метод вычисляющий площадь поверхности ограничивающего пространства.
         * 
         * @return площад поверхности
        */
        inline Type surfaceArea() const NOEXCEPT_PRGE
        {
            auto d = _pMax - _pMin;
            return static_cast<Type>(2) * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
        }

        /**
         * Метод вычисляющий объём поверхности ограничивающего пространства.
         * 
         * @return объём пространства
        */
        inline Type volume() const NOEXCEPT_PRGE
        {
            auto d = _pMax - _pMin;
            return d[0] * d[1] * d[2];
        }

        /**
         * Метод определяющий по какой из осей ограничевающего пространства, 
         * представденный в виде куба, вытянутей всего.
         * 
         * @return 0 - по оси X, 1 - по оси Y, Z - по оси Z
        */
        inline int stretchingInTheCoordinateAxis() const NOEXCEPT_PRGE
        {
            auto  d = _pMax - _pMin;
            return (d[0] > d[1] && d[0] > d[2] ? 0 : (d[1] > d[2] ? 1 : 2));
        }

        /**
         * Функция осуществляющая линейную интерполяцию между крайними углами ограничивающего
         * пространства представленного в виде куба.
         * 
         * @param t параметр интерполяции
         * @return промежуточное значение полученное с помощью линейной интерполяции.
        */
        inline Point3<float> lerp(const Point3<float>& t) const
        {
            return {_pMax[0] * (1.0f - t[0]) + _pMax[0] * t[0], _pMax[1] * (1.0f - t[1]) + _pMax[1] * t[1], _pMax[2] * (1.0f - t[2]) + _pMax[2] * t[2]};
        }

        /**
         * Вычисляет координату точки относительно ограничивающей пространства.
         *
         * @param p точка
         * @return координата точки p  относительно ограничивающей коробки
        */
        Vec3<Type> offset(const Point3<Type>& p) const
        {
            auto o = p - _pMin;
            auto d = _pMax - _pMin;

            if (_pMax[0] > _pMin[0]) {
                o[0] = o[0] / d[0];
            }

            if (_pMax[1] > _pMin[1]) {
                o[1] = o[1] / d[1];
            }

            if (_pMax[2] > _pMin[2]) {
                o[3] = o[2] / d[2];
            }

            return o;
        }

        Point3<Type> corner(int c) const
        {
            const auto* ptr = &_pMin;
            return {
                ptr[(c & 1)][0],
                ptr[(c & 2) ? 1 : 0][1],
                ptr[(c & 4) ? 1 : 0][2]
            };
        }

        inline auto maxExtent() const NOEXCEPT_PRGE
        {
            auto v = _pMax - _pMin;
            return maxDimension(v);
        }

        /**
         * Данная функция вычисляет ограничивающее пространство которое включает в себя b1 и b2.
         * 
         * @param b1 ограничивающее пространство
         * @param b2 ограничивающее пространство
         * @return ограничивающее пространство которое включает в себя b1 и b2
        */
        friend inline BoundingVolume3 merge(const BoundingVolume3& b1, const BoundingVolume3& b2) NOEXCEPT_PRGE
        {
            Point3<Type> pMin {min(b1._pMin[0], b2._pMin[0]), min(b1._pMin[1], b2._pMin[1]), min(b1._pMin[2], b2._pMin[2])};
            Point3<Type> pMax {max(b1._pMax[0], b2._pMax[0]), max(b1._pMax[1], b2._pMax[1]), max(b1._pMax[2], b2._pMax[2])};

            return {pMin, pMax};
        }

        /**
         * Данная функция вычисляет ограничивающее пространство которое включает в себя boundingVolume3 и point3.
         * 
         * @param boundingVolume3 ограничивающее пространство
         * @param point3 точка
         * @return ограничивающее пространство которое включает в себя boundingVolume3 и point3
        */
        friend inline BoundingVolume3 merge(const BoundingVolume3& boundingVolume3, const Point3<Type>& point3) NOEXCEPT_PRGE
        {
            Point3<Type> pMin {min(boundingVolume3._pMin[0], point3[0]), min(boundingVolume3._pMin[1], point3[1]), min(boundingVolume3._pMin[2], point3[2])};
            Point3<Type> pMax {max(boundingVolume3._pMax[0], point3[0]), max(boundingVolume3._pMax[1], point3[1]), max(boundingVolume3._pMax[2], point3[2])};

            return {pMin, pMax};
        }

        /**
         * Функция вычисляющее ограничивающее пространство в которой находятся(пересекаются) b1 и b2.
         * 
         * @param b1 ограничивающее пространство
         * @param b2 ограничивающее пространство
         * @return ограничивающее пространство в которой находятся(пересекаются) b1 и b2
        */
        friend inline BoundingVolume3 intersect(const BoundingVolume3& b1, const BoundingVolume3& b2) NOEXCEPT_PRGE
        {
            Point3<Type> p1 {max(b1._pMin[0], b2._pMin[0]), max(b1._pMin[1], b2._pMin[1]), max(b1._pMin[2], b2._pMin[2])};
            Point3<Type> p2 {min(b1._pMax[0], b2._pMax[0]), min(b1._pMax[1], b2._pMax[1]), min(b1._pMax[2], b2._pMax[2])};

            return {p1, p2};
        }

        /// TODO: написать методы iboxSphere

        friend bool intersect(const BoundingVolume3& boundingVolume3, const Ray& ray, float *t1, float* t2)
        {
            Vec3<float> invDir {1.0f / ray._dir[0], 1.0f / ray._dir[1], 1.0f / ray._dir[2]};
            auto tNear = (boundingVolume3._pMin - ray._o) * invDir;
            auto tFar = (boundingVolume3._pMax - ray._o) * invDir;

            if (tNear[0] > tFar[0]) {
                swap(tNear[0], tFar[0]);
            }

            if (tNear[1] > tFar[1]) {
                swap(tNear[1], tFar[1]);
            }

            if (tNear[2] > tFar[2]) {
                swap(tNear[2], tFar[2]);
            }

            if (t1) {
                *t1 = max(tNear[0], max(tNear[1], tNear[2]));
            }

            if (t2) {
                *t2 = min(tFar[0], min(tFar[1], tFar[2]));
            }

            return *t1 < *t2;
        }

        friend bool intersectP(const BoundingVolume3& boundingVolume3, const Ray& ray, const Vec3<Type>& invDir, const Vec3<bool>& dirIsNeg)
        {
            float tMin = (boundingVolume3[dirIsNeg[0]][0]- ray._o[0]) * invDir[0];
            float tMax = (boundingVolume3[!dirIsNeg[0]][0]- ray._o[0]) * invDir[0];
            float tyMin = (boundingVolume3[dirIsNeg[1]][1]- ray._o[1]) * invDir[1];
            float tyMax = (boundingVolume3[!dirIsNeg[1]][1]- ray._o[1]) * invDir[1];

            if (tMin > tyMax || tyMin > tMax) {
                return false;
            }

            if (tyMin > tMin) {
                tMin = tyMin;
            }

            if (tyMax < tMax) {
                tMax = tyMax;
            }

            float tzMin = (boundingVolume3[dirIsNeg[2]][2]- ray._o[2]) * invDir[2];
            float tzMax = (boundingVolume3[!dirIsNeg[2]][2]- ray._o[2]) * invDir[2];

            if (tMin > tzMax || tzMin > tMax) {
                return false;
            }

            if (tzMin > tMin) {
                tMin = tzMin;
            }

            if (tzMax < tMax) {
                tMax = tzMax;
            }

            return (tMin < ray._tMax) && (tMax > 0);;
        }

        /**
         * Функция определяющая перекрываются ли b1 и b2.
         * 
         * @param b1 ограничивающее пространство
         * @param b2 ограничивающее пространство
         * @return true - если перекрываются, иначе false
        */
        friend bool overlabs(const BoundingVolume3& b1, const BoundingVolume3& b2)
        {
            if constexpr (is_same_v<Type, float>) {
                // b1._pMax[0] >= b2._pMin[0]  |  b1._pMax[1] >= b2._pMin[1]  |  b1._pMax[2] >= b2._pMin[2]
                auto r1 = _mm_cmpge_ps(*reinterpret_cast<const __m128*>(b1._pMax._xyz), *reinterpret_cast<const __m128*>(b2._pMin._xyz));

                // b1._pMin[0] <= b2._pMax[0]  |  b1._pMin[1] <= b2._pMax[1]  |  b1._pMin[2] <= b2._pMax[2]
                auto r2 = _mm_cmple_ps(*reinterpret_cast<const __m128*>(b1._pMin._xyz), *reinterpret_cast<const __m128*>(b2._pMax._xyz));

                return r1[0] && r1[1] && r1[2] && r2[0] && r2[1] && r2[2];
            } else {
                bool x = (b1._pMax[0] >= b2._pMin[0]) && (b1._pMin[0] <= b2._pMax[0]);
                bool y = (b1._pMax[1] >= b2._pMin[1]) && (b1._pMin[1] <= b2._pMax[1]);
                bool z = (b1._pMax[2] >= b2._pMin[2]) && (b1._pMin[2] <= b2._pMax[2]);

                return (x && y && z);
            }
        }

        /**
         * Функция определяющая находится ли точка point3 внутри boundingVolume3.
         * 
         * @param boundingVolume3 ограничивающее пространство
         * @param point3 точка
         * @retrun true - если точка p находится внутри коробки boundingVolume3, инаяе false
        */
        friend bool inside(const BoundingVolume3& boundingVolume3, const Point3<Type>& point3)
        {
            if constexpr (is_same_v<Type, float>) {
                // point3._xyz[0] <= boundingVolume3._pMax[0]  |  point3._xyz[1] <= boundingVolume3._pMax[1]  |  point3._xyz[2] <= boundingVolume3._pMax[2]
                auto r1 = _mm_cmple_ps(*reinterpret_cast<const __m128*>(point3._xyz), *reinterpret_cast<const __m128*>(boundingVolume3._pMax._xyz));

                // point3._xyz[0] >= boundingVolume3._pMin[0]  |  point3._xyz[1] >= boundingVolume3._pMin[1]  |  point3._xyz[2] >= boundingVolume3._pMin[2]
                auto r2 = _mm_cmpge_ps(*reinterpret_cast<const __m128*>(point3._xyz), *reinterpret_cast<const __m128*>(boundingVolume3._pMin._xyz));

                return r1[0] && r1[1] && r1[2] && r2[0] && r2[1] && r2[2];
            } else {
                return point3[0] >= boundingVolume3._pMin[0] && point3[0] <= boundingVolume3._pMax[0] 
                    && point3[1] >= boundingVolume3._pMin[1] && point3[1] <= boundingVolume3._pMax[1] 
                    && point3[2] >= boundingVolume3._pMin[2] && point3[2] <= boundingVolume3._pMax[2];
            }
        }

        friend bool insideWithOutUpper(const BoundingVolume3& boundingVolume3, const Point3<Type>& point3)
        {
            if constexpr (is_same_v<Type, float>) {
                // point3._xyz[0] < boundingVolume3._pMax[0]  |  point3._xyz[1] < boundingVolume3._pMax[1]  |  point3._xyz[2] < boundingVolume3._pMax[2]
                auto r1 = _mm_cmplt_ps(*reinterpret_cast<const __m128*>(point3._xyz), *reinterpret_cast<const __m128*>(boundingVolume3._pMax._xyz));

                // point3._xyz[0] >= boundingVolume3._pMin[0]  |  point3._xyz[1] >= boundingVolume3._pMin[1]  |  point3._xyz[2] >= boundingVolume3._pMin[2]
                auto r2 = _mm_cmpge_ps(*reinterpret_cast<const __m128*>(point3._xyz), *reinterpret_cast<const __m128*>(boundingVolume3._pMin._xyz));

                return r1[0] && r1[1] && r1[2] && r2[0] && r2[1] && r2[2];
            } else {
                return point3[0] >= boundingVolume3._pMin[0] && point3[0] < boundingVolume3._pMax[0] 
                    && point3[1] >= boundingVolume3._pMin[1] && point3[1] < boundingVolume3._pMax[1] 
                    && point3[2] >= boundingVolume3._pMin[2] && point3[2] < boundingVolume3._pMax[2];
            }
        }

        /**
         * Функция возвращающую коробку расширенную на delta по сравнению с коробкой box.
         *
         * @param boundingVolume3 ограничивающее пространство
         * @param delta дельта 
         * @return ограничивающее пространство расширенная на delta по сравнению с boundingVolume3
        */
        friend inline BoundingVolume3 expand(const BoundingVolume3& boundingVolume3, Type delta)
        {
            Vec3<Type> d {delta, delta, delta};
            return {boundingVolume3._pMin - d, boundingVolume3._pMax + d};
        }

    private:
        Point3<Type> _pMin;
        Point3<Type> _pMax;
    };
}
