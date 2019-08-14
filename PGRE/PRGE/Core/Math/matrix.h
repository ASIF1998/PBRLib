#ifndef MATRIX_H
#define MATRIX_H

#include "math.h"

#include <xmmintrin.h>

#if DEBUG_PRGE == 1
#include <iostream>
using namespace std;
#endif

namespace PRGE
{
    class Matrix4x4f
    {
        friend class Transform;

    public:
        Matrix4x4f(float m00 = 0.f, float m01 = 0.f, float m02 = 0.f, float m03 = 0.f,
                   float m10 = 0.f, float m11 = 0.f, float m12 = 0.f, float m13 = 0.f,
                   float m20 = 0.f, float m21 = 0.f, float m22 = 0.f, float m23 = 0.f,
                   float m30 = 0.f, float m31 = 0.f, float m32 = 0.f, float m33 = 0.f) NOEXCEPT_PRGE;

        Matrix4x4f(const Matrix4x4f& matrix4x4) NOEXCEPT_PRGE;
        Matrix4x4f(Matrix4x4f&& matrix4x4) NOEXCEPT_PRGE;

        Matrix4x4f& operator = (const Matrix4x4f& matrix4x4) NOEXCEPT_PRGE;
        Matrix4x4f& operator = (Matrix4x4f&& matrix4x4) NOEXCEPT_PRGE;

        bool operator == (const Matrix4x4f& matrix4x4) const noexcept;
        bool operator != (const Matrix4x4f& matrix4x4) const noexcept;
        Matrix4x4f operator * (const Matrix4x4f& matrix4x4) const NOEXCEPT_PRGE;
        Matrix4x4f operator * (float s) const noexcept;
        const float* operator [] (size_t i) const noexcept;
        float* operator [] (size_t i) noexcept ;

#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const Matrix4x4f& matrix4x4);
#endif

        float at(size_t i, size_t j) const;
        float& at(size_t i, size_t j);

        float det() const noexcept;

        /**
         * Функция транспонирующая матрицу.
         * 
         * @param matrix4x4 матрица 4x4
         * @return транспонированная матрица 4x4
        */
        friend Matrix4x4f transpose(const Matrix4x4f& matrix4x4) NOEXCEPT_PRGE;

        /**
         * Функция вычисляющая обратную матрицу.
         * 
         * @param matrix4x4 матрица 4x4
         * @return обратную матрицу, в случае если определитель отличен от нуля (иначе возвращается нулевая матрица)
        */
        friend Matrix4x4f inverse(const Matrix4x4f& matrix4x4) NOEXCEPT_PRGE;

    private:
        Matrix4x4f(const __m128 m[4]) NOEXCEPT_PRGE;

    private:
        float _m[4][4];
    };
}

#endif // MATRIX_H
