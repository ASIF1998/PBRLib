#ifndef QUATERNION_H
#define QUATERNION_H

#include "../core.h"

#include "math.hpp"

#include "matrix.h"

#include "../../Transform/transform.hpp"

#include <xmmintrin.h>

#include <cmath>

#include <algorithm>

using namespace std;


namespace PRGE
{
    struct Quaternion
    {
    public:
        inline Quaternion(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(x, y, z, w);
#endif

            _xyzw[0] = x;
            _xyzw[1] = y;
            _xyzw[2] = z;
            _xyzw[3] = w;
        }
        
        inline Quaternion(const Quaternion& quaternion) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

            *reinterpret_cast<__m128*>(_xyzw) = *reinterpret_cast<const __m128*>(quaternion._xyzw);
        }

        inline Quaternion(Quaternion&& quaternion) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

            auto&& q1 = move(quaternion._xyzw);
            auto&& q2 = move(_xyzw);
            swap(q1, q2);
        }

        /**
         * Конструктор.
         * 
         * @param matrix4x4 матрица
        */
        Quaternion(const Matrix4x4f& matrix4x4);

        Quaternion& operator = (const Quaternion& quaternion) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

            *reinterpret_cast<__m128*>(_xyzw) = *reinterpret_cast<const __m128*>(quaternion._xyzw);
            
            return *this;
        }

        Quaternion& operator = (Quaternion&& quaternion) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

            auto&& q1 = move(quaternion._xyzw);
            auto&& q2 = move(_xyzw);
            swap(q1, q2);

            return *this;
        }

        inline Quaternion operator + (const Quaternion& quaternion) const NOEXCEPT_PRGE;
        Quaternion operator - (const Quaternion& quaternion) const NOEXCEPT_PRGE;
        inline Quaternion operator - () const noexcept;
        Quaternion operator * (float s) const noexcept;
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

       Transform toTransform() const;

    private:
        inline Quaternion(__m128 m) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(m[0], m[1], m[2], m[3]);
#endif

            *reinterpret_cast<__m128*>(_xyzw) = m;
        }

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
    Quaternion normalize(const Quaternion& quaternion) NOEXCEPT_PRGE;

    /**
     * Функция осущеставляющая сферическое линейное интерполирование.
     * 
     * @param q1 кватернион
     * @param q2 кватернион
     * @param t параметр интерполяции (в диапазоне [0.0, 1.0])
     * @return промежуточный кватернион находящийся между q1 и q2
    */
    Quaternion slerp(float t, const Quaternion& q1, const Quaternion& q2) NOEXCEPT_PRGE;
}

#endif // QUATERNION_H
