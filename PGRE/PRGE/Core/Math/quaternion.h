#ifndef QUATERNION_H
#define QUATERNION_H

#include "math.h"

#include <xmmintrin.h>

#if DEBUG_PRGE == 1
#include <iostream>
using namespace std;
#endif

namespace PRGE
{
    struct Quaternion
    {
    public:
        inline Quaternion(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f) NOEXCEPT_PRGE;
        inline Quaternion(const Quaternion& quaternion) NOEXCEPT_PRGE;
        inline Quaternion(Quaternion&& quaternion) NOEXCEPT_PRGE;

        inline Quaternion& operator = (const Quaternion& quaternion) NOEXCEPT_PRGE;
        inline Quaternion& operator = (Quaternion&& quaternion) NOEXCEPT_PRGE;
        inline Quaternion operator + (const Quaternion& quaternion) const NOEXCEPT_PRGE;
        inline Quaternion operator - (const Quaternion& quaternion) const NOEXCEPT_PRGE;
        inline Quaternion operator - () const noexcept;
        inline Quaternion operator * (float s) const noexcept;
        inline Quaternion operator / (float s) const;
        inline Quaternion& operator += (const Quaternion& quaternion) NOEXCEPT_PRGE;
        inline Quaternion& operator -= (const Quaternion& quaternion) NOEXCEPT_PRGE;
        inline Quaternion& operator *= (float s) noexcept;
        inline Quaternion& operator /= (float s);
#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const Quaternion& quaternion);
#endif

        /**
         * TODO:
         * 1) Добавить конструктор с классом Transform
         * 2) Добавить метод toTransform 
        */

    private:
        inline Quaternion(__m128 m) NOEXCEPT_PRGE;

    public:
        float _xyzw[4];
    };

    /**
     * Функция вычисляющая скалярное произведение кватернионов.
     * 
     * @param q1 кватернион
     * @param q2 кватернион
     * @return скалярное произведение кватернионов
    */
    float dot(const Quaternion& q1, const Quaternion& q2) NOEXCEPT_PRGE;

    /**
     * Функция нормирующая кватернио н.
     * 
     * @param quaternion кватернион
     * @return нормированный кватернион
    */
    inline Quaternion normalize(const Quaternion& quaternion) NOEXCEPT_PRGE;

    /**
     * Функция осущеставляющая сферическое линейное интерполирование.
     * 
     * @param q1 кватернион
     * @param q2 кватернион
     * @param t параметр интерполяции (в диапазоне [0.0, 1.0])
     * @return промежуточный кватернион находящийся между q1 и q2
    */
    Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) NOEXCEPT_PRGE;
}

#endif // QUATERNION_H
