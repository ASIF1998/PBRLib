//
//  quaternion.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 15/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef quaternion_hpp
#define quaternion_hpp

#include <iostream>

#include <xmmintrin.h>

#include "vec3.hpp"

using namespace std;

namespace pbrlib::math
{
    struct Quaternion
    {
    public:
        inline constexpr Quaternion();
        inline constexpr Quaternion(float x, float y, float z, float w);
        inline constexpr Quaternion(const Vec3<float>& v, float w);

        bool operator == (const Quaternion& q) const noexcept;
        bool operator != (const Quaternion& q) const noexcept;

        inline Quaternion operator + (const Quaternion& q)  const noexcept;
        inline Quaternion operator - (const Quaternion& q)  const noexcept;
        inline Quaternion operator * (const Quaternion& q)  const noexcept;
        inline Quaternion operator * (float f)              const noexcept;
        inline Quaternion operator / (float f)              const noexcept;

        inline Quaternion& operator += (const Quaternion& q)    noexcept;
        inline Quaternion& operator -= (const Quaternion& q)    noexcept;
        inline Quaternion& operator *= (const Quaternion& q)    noexcept;
        inline Quaternion& operator *= (float f)                noexcept;
        inline Quaternion& operator /= (float f)                noexcept;

        inline float    operator [] (size_t i) const noexcept;
        inline float&   operator [] (size_t i) noexcept;

        friend ostream& operator << (ostream& print, const Quaternion& q);

        /**
         * TODO: 
         *      1) Добавить метод toTransform;
         *      2) Добавить конструктор, инициализирующий кватернион от класса Transform.
        */

        inline float lengthSquared()    const noexcept;
        inline float length()           const noexcept;

        inline void normalize();
        inline void inverse();

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (1, 0, 0, 0).
         * 
         * @return Кватернион следующего вида: (1, 0, 0, 0).
        */
        inline static Quaternion i() noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (0, 1, 0, 0).
         * 
         * @return Кватернион следующего вида: (0, 1, 0, 0).
        */
        inline static Quaternion j() noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (0, 0, 1, 0).
         * 
         * @return Кватернион следующего вида: (0, 0, 1, 0).
        */
        inline static Quaternion k() noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (0, 0, 0, 1).
         * 
         * @return Кватернион следующего вида: (0, 0, 0, 1).
        */
        inline static Quaternion identity() noexcept;

        /**
         * @brief Статический метод, который создаёт следющий кватернион:
         *      (0, 0, 0, 0).
         * 
         * @return Кватернион следующего вида: (0, 0, 0, 0).
        */
        inline static Quaternion zerro() noexcept;

    private:
        inline constexpr Quaternion(const __m128& m) noexcept;

    public:
        union
        {
            struct
            {
                Vec3<float> v;
                float       w;
            };

            __m128  xyzw_simd;
            float   xyzw[4];
        };
    };

    inline float        dot(const Quaternion& q1, const Quaternion& q2);
    inline Quaternion   normalize(const Quaternion& q);
    inline Quaternion   inverse(const Quaternion& q);

    /**
     * @brief Функция возвращающая сопряжённый кватернион.
     * 
     * @param q кватернион.
     * @return Следующий кватернион: (q.v * -1.0f, q.w).
    */
    inline Quaternion conjugate(const Quaternion& q);

    inline Quaternion lerp(float t, const Quaternion& q1, const Quaternion& q2);
    inline Quaternion slerp(float t, const Quaternion& q1, const Quaternion& q2);
}

#include "quaternion.inl"

#endif /* quaternion_hpp */
