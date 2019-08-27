#include "matrix.h"

#include <stdexcept>

#include <cmath>

#if DEBUG_PRGE == 1
#   include <iomanip>
#endif

using namespace std;

#define NAN_OR_INF_MATRIX4X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33) \
                            if (isnan(m00) || isnan(m01) || isnan(m02) || isnan(m03) || isinf(m00) || isinf(m01) || isinf(m02) || isinf(m03) || \
                                isnan(m10) || isnan(m11) || isnan(m12) || isnan(m13) || isinf(m10) || isinf(m11) || isinf(m12) || isinf(m13) || \
                                isnan(m20) || isnan(m21) || isnan(m22) || isnan(m23) || isinf(m20) || isinf(m21) || isinf(m22) || isinf(m23) || \
                                isnan(m30) || isnan(m31) || isnan(m32) || isnan(m33) || isinf(m30) || isinf(m31) || isinf(m32) || isinf(m33)) \
                                    throw invalid_argument("The matrix passed are not valid")


namespace PRGE
{
    Matrix4x4f::Matrix4x4f(float m00, float m01, float m02, float m03,
                           float m10, float m11, float m12, float m13,
                           float m20, float m21, float m22, float m23,
                           float m30, float m31, float m32, float m33) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33);
#endif

        _m[0][0] = m00;
        _m[0][1] = m01;
        _m[0][2] = m02;
        _m[0][3] = m03;

        _m[1][0] = m10;
        _m[1][1] = m11;
        _m[1][2] = m12;
        _m[1][3] = m13;

        _m[2][0] = m20;
        _m[2][1] = m21;
        _m[2][2] = m22;
        _m[2][3] = m23;

        _m[3][0] = m30;
        _m[3][1] = m31;
        _m[3][2] = m32;
        _m[3][3] = m33;
    }

    Matrix4x4f::Matrix4x4f(const Matrix4x4f& matrix4x4) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(matrix4x4._m[0][0], matrix4x4._m[0][1], matrix4x4._m[0][2], matrix4x4._m[0][3],
                             matrix4x4._m[1][0], matrix4x4._m[1][1], matrix4x4._m[1][2], matrix4x4._m[1][3],
                             matrix4x4._m[2][0], matrix4x4._m[2][1], matrix4x4._m[2][2], matrix4x4._m[2][3],
                             matrix4x4._m[3][0], matrix4x4._m[3][1], matrix4x4._m[3][2], matrix4x4._m[3][3]);
#endif

        *reinterpret_cast<__m128*>(_m[0]) = *reinterpret_cast<const __m128*>(matrix4x4._m[0]);
        *reinterpret_cast<__m128*>(_m[1]) = *reinterpret_cast<const __m128*>(matrix4x4._m[1]);
        *reinterpret_cast<__m128*>(_m[2]) = *reinterpret_cast<const __m128*>(matrix4x4._m[2]);
        *reinterpret_cast<__m128*>(_m[3]) = *reinterpret_cast<const __m128*>(matrix4x4._m[3]);
    }

    Matrix4x4f::Matrix4x4f(Matrix4x4f&& matrix4x4) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(matrix4x4._m[0][0], matrix4x4._m[0][1], matrix4x4._m[0][2], matrix4x4._m[0][3],
                             matrix4x4._m[1][0], matrix4x4._m[1][1], matrix4x4._m[1][2], matrix4x4._m[1][3],
                             matrix4x4._m[2][0], matrix4x4._m[2][1], matrix4x4._m[2][2], matrix4x4._m[2][3],
                             matrix4x4._m[3][0], matrix4x4._m[3][1], matrix4x4._m[3][2], matrix4x4._m[3][3]);
#endif

        auto&& m1 = move(_m);
        auto&& m2 = move(matrix4x4._m);
        swap(m1, m2);
    }

    Matrix4x4f::Matrix4x4f(const __m128 m[4]) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(m[0][0], m[0][1], m[0][2], m[0][3],
                             m[1][0], m[1][1], m[1][2], m[1][3],
                             m[2][0], m[2][1], m[2][2], m[2][3],
                             m[3][0], m[3][1], m[3][2], m[3][3]);
#endif

        *reinterpret_cast<__m128*>(_m[0]) = m[0];
        *reinterpret_cast<__m128*>(_m[1]) = m[1];
        *reinterpret_cast<__m128*>(_m[2]) = m[2];
        *reinterpret_cast<__m128*>(_m[3]) = m[3];
    }

    Matrix4x4f& Matrix4x4f::operator = (const Matrix4x4f& matrix4x4) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(matrix4x4._m[0][0], matrix4x4._m[0][1], matrix4x4._m[0][2], matrix4x4._m[0][3],
                             matrix4x4._m[1][0], matrix4x4._m[1][1], matrix4x4._m[1][2], matrix4x4._m[1][3],
                             matrix4x4._m[2][0], matrix4x4._m[2][1], matrix4x4._m[2][2], matrix4x4._m[2][3],
                             matrix4x4._m[3][0], matrix4x4._m[3][1], matrix4x4._m[3][2], matrix4x4._m[3][3]);
#endif

        *reinterpret_cast<__m128*>(_m[0]) = *reinterpret_cast<const __m128*>(matrix4x4._m[0]);
        *reinterpret_cast<__m128*>(_m[1]) = *reinterpret_cast<const __m128*>(matrix4x4._m[1]);
        *reinterpret_cast<__m128*>(_m[2]) = *reinterpret_cast<const __m128*>(matrix4x4._m[2]);
        *reinterpret_cast<__m128*>(_m[3]) = *reinterpret_cast<const __m128*>(matrix4x4._m[3]);

        return *this;
    }

    Matrix4x4f& Matrix4x4f::operator = (Matrix4x4f&& matrix4x4) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(matrix4x4._m[0][0], matrix4x4._m[0][1], matrix4x4._m[0][2], matrix4x4._m[0][3],
                             matrix4x4._m[1][0], matrix4x4._m[1][1], matrix4x4._m[1][2], matrix4x4._m[1][3],
                             matrix4x4._m[2][0], matrix4x4._m[2][1], matrix4x4._m[2][2], matrix4x4._m[2][3],
                             matrix4x4._m[3][0], matrix4x4._m[3][1], matrix4x4._m[3][2], matrix4x4._m[3][3]);
#endif

        auto&& m1 = move(_m);
        auto&& m2 = move(matrix4x4._m);
        swap(m1, m2);

        return *this;
    }

    bool Matrix4x4f::operator == (const Matrix4x4f& matrix4x4) const noexcept
    {
        auto r0 = _mm_cmpeq_ps(*reinterpret_cast<const __m128*>(_m[0]), *reinterpret_cast<const __m128*>(matrix4x4._m[0]));
        auto r1 = _mm_cmpeq_ps(*reinterpret_cast<const __m128*>(_m[1]), *reinterpret_cast<const __m128*>(matrix4x4._m[1]));
        auto r2 = _mm_cmpeq_ps(*reinterpret_cast<const __m128*>(_m[2]), *reinterpret_cast<const __m128*>(matrix4x4._m[2]));
        auto r3 = _mm_cmpeq_ps(*reinterpret_cast<const __m128*>(_m[3]), *reinterpret_cast<const __m128*>(matrix4x4._m[3]));
        return r0[0] && r0[1] && r0[2] && r0[3] && r1[0] && r1[1] && r1[2] && r1[3] && r2[0] && r2[1] && r2[2] && r2[3] && r3[0] && r3[1] && r3[2] && r3[3];
    }

    bool Matrix4x4f::operator != (const Matrix4x4f& matrix4x4) const noexcept
    {
        auto r0 = _mm_cmpneq_ss(*reinterpret_cast<const __m128*>(_m[0]), *reinterpret_cast<const __m128*>(matrix4x4._m[0]));
        auto r1 = _mm_cmpneq_ss(*reinterpret_cast<const __m128*>(_m[1]), *reinterpret_cast<const __m128*>(matrix4x4._m[1]));
        auto r2 = _mm_cmpneq_ss(*reinterpret_cast<const __m128*>(_m[2]), *reinterpret_cast<const __m128*>(matrix4x4._m[2]));
        auto r3 = _mm_cmpneq_ss(*reinterpret_cast<const __m128*>(_m[3]), *reinterpret_cast<const __m128*>(matrix4x4._m[3]));
        return r0[0] || r0[1] || r0[2] || r0[3] || r1[0] || r1[1] || r1[2] || r1[3] || r2[0] || r2[1] || r2[2] || r2[3] || r3[0] || r3[1] || r3[2] || r3[3];
    }

    Matrix4x4f Matrix4x4f::operator * (const Matrix4x4f& matrix4x4) const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(matrix4x4._m[0][0], matrix4x4._m[0][1], matrix4x4._m[0][2], matrix4x4._m[0][3],
                             matrix4x4._m[1][0], matrix4x4._m[1][1], matrix4x4._m[1][2], matrix4x4._m[1][3],
                             matrix4x4._m[2][0], matrix4x4._m[2][1], matrix4x4._m[2][2], matrix4x4._m[2][3],
                             matrix4x4._m[3][0], matrix4x4._m[3][1], matrix4x4._m[3][2], matrix4x4._m[3][3]);
#endif

        __m128 out[4];
        __m128 c[4];
        __m128 r[2];

        c[0] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[0]), _mm_setr_ps(matrix4x4._m[0][0], matrix4x4._m[1][0], matrix4x4._m[2][0], matrix4x4._m[3][0]));
        c[1] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[0]), _mm_setr_ps(matrix4x4._m[0][1], matrix4x4._m[1][1], matrix4x4._m[2][1], matrix4x4._m[3][1]));
        c[2] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[0]), _mm_setr_ps(matrix4x4._m[0][2], matrix4x4._m[1][2], matrix4x4._m[2][2], matrix4x4._m[3][2]));
        c[3] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[0]), _mm_setr_ps(matrix4x4._m[0][3], matrix4x4._m[1][3], matrix4x4._m[2][3], matrix4x4._m[3][3]));

        r[0] = _mm_add_ps(_mm_setr_ps(c[0][0], c[1][0], c[2][0], c[3][0]), _mm_setr_ps(c[0][1], c[1][1], c[2][1], c[3][1]));
        r[1] = _mm_add_ps(_mm_setr_ps(c[0][2], c[1][2], c[2][2], c[3][2]), _mm_setr_ps(c[0][3], c[1][3], c[2][3], c[3][3]));

        out[0] = _mm_add_ps(r[0], r[1]);

        c[0] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[1]), _mm_setr_ps(matrix4x4._m[0][0], matrix4x4._m[1][0], matrix4x4._m[2][0], matrix4x4._m[3][0]));
        c[1] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[1]), _mm_setr_ps(matrix4x4._m[0][1], matrix4x4._m[1][1], matrix4x4._m[2][1], matrix4x4._m[3][1]));
        c[2] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[1]), _mm_setr_ps(matrix4x4._m[0][2], matrix4x4._m[1][2], matrix4x4._m[2][2], matrix4x4._m[3][2]));
        c[3] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[1]), _mm_setr_ps(matrix4x4._m[0][3], matrix4x4._m[1][3], matrix4x4._m[2][3], matrix4x4._m[3][3]));

        r[0] = _mm_add_ps(_mm_setr_ps(c[0][0], c[1][0], c[2][0], c[3][0]), _mm_setr_ps(c[0][1], c[1][1], c[2][1], c[3][1]));
        r[1] = _mm_add_ps(_mm_setr_ps(c[0][2], c[1][2], c[2][2], c[3][2]), _mm_setr_ps(c[0][3], c[1][3], c[2][3], c[3][3]));

        out[1] = _mm_add_ps(r[0], r[1]);

        c[0] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[2]), _mm_setr_ps(matrix4x4._m[0][0], matrix4x4._m[1][0], matrix4x4._m[2][0], matrix4x4._m[3][0]));
        c[1] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[2]), _mm_setr_ps(matrix4x4._m[0][1], matrix4x4._m[1][1], matrix4x4._m[2][1], matrix4x4._m[3][1]));
        c[2] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[2]), _mm_setr_ps(matrix4x4._m[0][2], matrix4x4._m[1][2], matrix4x4._m[2][2], matrix4x4._m[3][2]));
        c[3] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[2]), _mm_setr_ps(matrix4x4._m[0][3], matrix4x4._m[1][3], matrix4x4._m[2][3], matrix4x4._m[3][3]));

        r[0] = _mm_add_ps(_mm_setr_ps(c[0][0], c[1][0], c[2][0], c[3][0]), _mm_setr_ps(c[0][1], c[1][1], c[2][1], c[3][1]));
        r[1] = _mm_add_ps(_mm_setr_ps(c[0][2], c[1][2], c[2][2], c[3][2]), _mm_setr_ps(c[0][3], c[1][3], c[2][3], c[3][3]));

        out[2] = _mm_add_ps(r[0], r[1]);

        c[0] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[3]), _mm_setr_ps(matrix4x4._m[0][0], matrix4x4._m[1][0], matrix4x4._m[2][0], matrix4x4._m[3][0]));
        c[1] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[3]), _mm_setr_ps(matrix4x4._m[0][1], matrix4x4._m[1][1], matrix4x4._m[2][1], matrix4x4._m[3][1]));
        c[2] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[3]), _mm_setr_ps(matrix4x4._m[0][2], matrix4x4._m[1][2], matrix4x4._m[2][2], matrix4x4._m[3][2]));
        c[3] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[3]), _mm_setr_ps(matrix4x4._m[0][3], matrix4x4._m[1][3], matrix4x4._m[2][3], matrix4x4._m[3][3]));

        r[0] = _mm_add_ps(_mm_setr_ps(c[0][0], c[1][0], c[2][0], c[3][0]), _mm_setr_ps(c[0][1], c[1][1], c[2][1], c[3][1]));
        r[1] = _mm_add_ps(_mm_setr_ps(c[0][2], c[1][2], c[2][2], c[3][2]), _mm_setr_ps(c[0][3], c[1][3], c[2][3], c[3][3]));

        out[3] = _mm_add_ps(r[0], r[1]);

        return {out};
    }

    Matrix4x4f Matrix4x4f::operator * (float s) const noexcept
    {
        __m128 out[4];
        __m128 vs = _mm_set_ps(s, s, s, s);

        out[0] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[0]), vs);
        out[1] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[1]), vs);
        out[2] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[2]), vs);
        out[3] = _mm_mul_ps(*reinterpret_cast<const __m128*>(_m[3]), vs);

        return {out};
    }

    const float* Matrix4x4f::operator [] (size_t i) const noexcept
    {
        return _m[i];
    }

    float* Matrix4x4f::operator [] (size_t i) noexcept
    {
        return _m[i];
    }

    float Matrix4x4f::at(size_t i, size_t j) const
    {
        if (i > 3 || j > 3) {
            throw out_of_range("Go beyond the matrix");
        }

        return _m[i][j];
    }

    float& Matrix4x4f::at(size_t i, size_t j)
    {
        if (i > 3 || j > 3) {
            throw out_of_range("Go beyond the matrix");
        }

        return _m[i][j];
    }

    float Matrix4x4f::det() const noexcept
    {
        auto d1 = _mm_sub_ps(_mm_mul_ps(_mm_setr_ps(_m[2][2], _m[2][1], _m[2][1], _m[2][0]), _mm_setr_ps(_m[3][3], _m[3][3], _m[3][2], _m[3][3])),
                             _mm_mul_ps(_mm_setr_ps(_m[2][3], _m[2][3], _m[2][2], _m[3][0]), _mm_setr_ps(_m[3][2], _m[3][1], _m[3][1], _m[2][3])));

        auto d2 = _mm_sub_ps(_mm_mul_ps(_mm_setr_ps(_m[2][0], _m[2][0], _m[2][1], 0.f), _mm_setr_ps(_m[3][2], _m[3][1], _m[3][2], 0.f)),
                             _mm_mul_ps(_mm_setr_ps(_m[3][0], _m[3][0], _m[3][1], 0.f), _mm_setr_ps(_m[2][2], _m[2][1], _m[2][2], 0.f)));

        auto r1 = _mm_mul_ps(_mm_setr_ps(_m[1][1], _m[1][0], _m[1][0], _m[1][0]), _mm_setr_ps(d1[0], d1[0], d1[1], d2[2]));
        auto r2 = _mm_mul_ps(_mm_setr_ps(_m[1][2], _m[1][2], _m[1][1], _m[1][1]), _mm_setr_ps(d1[1], d1[3], d1[3], d2[0]));

        r1 = _mm_sub_ps(r1, r2);
        r2 = _mm_mul_ps(_mm_setr_ps(_m[1][3], _m[1][3], _m[1][3], _m[1][2]), _mm_setr_ps(d1[2], d2[0], d2[1], d2[1]));
        r1 = _mm_add_ps(r1, r2);
        r1 = _mm_mul_ps(r1, _mm_setr_ps(_m[0][0], _m[0][1], _m[0][2], _m[0][3]));

        return r1[0] - r1[1] + r1[2] - r1[3];
    }

    Matrix4x4f transpose(const Matrix4x4f& matrix4x4) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(matrix4x4._m[0][0], matrix4x4._m[0][1], matrix4x4._m[0][2], matrix4x4._m[0][3],
                             matrix4x4._m[1][0], matrix4x4._m[1][1], matrix4x4._m[1][2], matrix4x4._m[1][3],
                             matrix4x4._m[2][0], matrix4x4._m[2][1], matrix4x4._m[2][2], matrix4x4._m[2][3],
                             matrix4x4._m[3][0], matrix4x4._m[3][1], matrix4x4._m[3][2], matrix4x4._m[3][3]);
#endif

        return {
            matrix4x4._m[0][0], matrix4x4._m[1][0], matrix4x4._m[2][0], matrix4x4._m[3][0],
            matrix4x4._m[0][1], matrix4x4._m[1][1], matrix4x4._m[2][1], matrix4x4._m[3][1],
            matrix4x4._m[0][2], matrix4x4._m[1][2], matrix4x4._m[2][2], matrix4x4._m[3][2],
            matrix4x4._m[0][3], matrix4x4._m[1][3], matrix4x4._m[2][3], matrix4x4._m[3][3]
        };
    }

    Matrix4x4f inverse(const Matrix4x4f& matrix4x4) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_MATRIX4X4(matrix4x4._m[0][0], matrix4x4._m[0][1], matrix4x4._m[0][2], matrix4x4._m[0][3],
                             matrix4x4._m[1][0], matrix4x4._m[1][1], matrix4x4._m[1][2], matrix4x4._m[1][3],
                             matrix4x4._m[2][0], matrix4x4._m[2][1], matrix4x4._m[2][2], matrix4x4._m[2][3],
                             matrix4x4._m[3][0], matrix4x4._m[3][1], matrix4x4._m[3][2], matrix4x4._m[3][3]);
#endif

        float det = matrix4x4.det();

        if (det != 0.f) {
            auto tmatrix4x4 = transpose(matrix4x4);

            auto c11 = _mm_setr_ps(tmatrix4x4._m[2][2], tmatrix4x4._m[2][1], tmatrix4x4._m[2][0], tmatrix4x4._m[2][0]);
            auto c12 = _mm_setr_ps(tmatrix4x4._m[3][3], tmatrix4x4._m[3][3], tmatrix4x4._m[3][3], tmatrix4x4._m[3][2]);
            auto c13 = _mm_setr_ps(tmatrix4x4._m[3][2], tmatrix4x4._m[3][1], tmatrix4x4._m[3][0], tmatrix4x4._m[3][0]);
            auto c14 = _mm_setr_ps(tmatrix4x4._m[2][3], tmatrix4x4._m[2][3], tmatrix4x4._m[2][3], tmatrix4x4._m[2][2]);

            auto c21 = _mm_setr_ps(tmatrix4x4._m[2][0], tmatrix4x4._m[2][1], tmatrix4x4._m[1][2], tmatrix4x4._m[1][1]);
            auto c22 = _mm_setr_ps(tmatrix4x4._m[3][1], tmatrix4x4._m[3][2], tmatrix4x4._m[3][3], tmatrix4x4._m[3][3]);
            auto c23 = _mm_setr_ps(tmatrix4x4._m[2][1], tmatrix4x4._m[3][1], tmatrix4x4._m[3][2], tmatrix4x4._m[1][3]);
            auto c24 = _mm_setr_ps(tmatrix4x4._m[3][0], tmatrix4x4._m[2][2], tmatrix4x4._m[1][3], tmatrix4x4._m[3][1]);

            auto c31 = _mm_setr_ps(tmatrix4x4._m[1][1], tmatrix4x4._m[1][0], tmatrix4x4._m[1][0], tmatrix4x4._m[1][0]);
            auto c32 = _mm_setr_ps(tmatrix4x4._m[3][2], tmatrix4x4._m[3][3], tmatrix4x4._m[3][2], tmatrix4x4._m[3][1]);
            auto c33 = _mm_setr_ps(tmatrix4x4._m[3][1], tmatrix4x4._m[1][3], tmatrix4x4._m[1][2], tmatrix4x4._m[1][1]);
            auto c34 = _mm_setr_ps(tmatrix4x4._m[1][2], tmatrix4x4._m[3][0], tmatrix4x4._m[3][0], tmatrix4x4._m[3][0]);

            auto c41 = _mm_setr_ps(tmatrix4x4._m[1][2], tmatrix4x4._m[1][1], tmatrix4x4._m[1][1], tmatrix4x4._m[1][0]);
            auto c42 = _mm_setr_ps(tmatrix4x4._m[2][3], tmatrix4x4._m[2][3], tmatrix4x4._m[2][2], tmatrix4x4._m[2][3]);
            auto c43 = _mm_setr_ps(tmatrix4x4._m[2][2], tmatrix4x4._m[1][3], tmatrix4x4._m[1][2], tmatrix4x4._m[1][3]);
            auto c44 = _mm_setr_ps(tmatrix4x4._m[1][3], tmatrix4x4._m[2][1], tmatrix4x4._m[2][1], tmatrix4x4._m[2][0]);

            auto c51 = _mm_setr_ps(tmatrix4x4._m[1][0], tmatrix4x4._m[1][0], 0.f, 0.f);
            auto c52 = _mm_setr_ps(tmatrix4x4._m[2][2], tmatrix4x4._m[2][1], 0.f, 0.f);
            auto c53 = _mm_setr_ps(tmatrix4x4._m[1][2], tmatrix4x4._m[1][1], 0.f, 0.f);
            auto c54 = _mm_setr_ps(tmatrix4x4._m[2][0], tmatrix4x4._m[2][0], 0.f, 0.f);

            auto e1 = _mm_setr_ps(tmatrix4x4._m[1][1], tmatrix4x4._m[1][0], tmatrix4x4._m[1][0], tmatrix4x4._m[1][0]);
            auto e2 = _mm_setr_ps(tmatrix4x4._m[1][2], tmatrix4x4._m[1][2], tmatrix4x4._m[1][1], tmatrix4x4._m[1][1]);
            auto e3 = _mm_setr_ps(tmatrix4x4._m[1][3], tmatrix4x4._m[1][3], tmatrix4x4._m[1][3], tmatrix4x4._m[1][2]);
            auto e4 = _mm_setr_ps(tmatrix4x4._m[0][1], tmatrix4x4._m[0][0], tmatrix4x4._m[0][0], tmatrix4x4._m[0][0]);
            auto e5 = _mm_setr_ps(tmatrix4x4._m[0][2], tmatrix4x4._m[0][2], tmatrix4x4._m[0][1], tmatrix4x4._m[0][1]);
            auto e6 = _mm_setr_ps(tmatrix4x4._m[0][3], tmatrix4x4._m[0][3], tmatrix4x4._m[0][3], tmatrix4x4._m[0][2]);

            auto d1 = _mm_sub_ps(_mm_mul_ps(c11, c12), _mm_mul_ps(c13, c14));
            auto d2 = _mm_sub_ps(_mm_mul_ps(c21, c22), _mm_mul_ps(c23, c24));
            auto d3 = _mm_sub_ps(_mm_mul_ps(c31, c32), _mm_mul_ps(c33, c34));
            auto d4 = _mm_sub_ps(_mm_mul_ps(c41, c42), _mm_mul_ps(c43, c44));
            auto d5 = _mm_sub_ps(_mm_mul_ps(c51, c52), _mm_mul_ps(c53, c54));

            auto m11 = _mm_mul_ps(e1, _mm_setr_ps(d1[0], d1[0], d1[1], d1[2]));
            auto m12 = _mm_mul_ps(e2, _mm_setr_ps(d1[1], d1[2], d1[2], d1[3]));
            auto m13 = _mm_mul_ps(e3, _mm_setr_ps(d2[1], d1[3], d2[0], d2[0]));

            auto m21 = _mm_mul_ps(e4, _mm_setr_ps(d1[0], d1[0], d1[1], d2[1]));
            auto m22 = _mm_mul_ps(e5, _mm_setr_ps(d1[1], d1[2], d1[2], d1[3]));
            auto m23 = _mm_mul_ps(e6, _mm_setr_ps(d2[1], d1[3], d2[0], d2[0]));

            auto m31 = _mm_mul_ps(e4, _mm_setr_ps(d2[2], d2[2], d2[3], d3[0]));
            auto m32 = _mm_mul_ps(e5, _mm_setr_ps(d2[3], d3[1], d3[1], d3[2]));
            auto m33 = _mm_mul_ps(e6, _mm_setr_ps(d3[0], d3[2], d3[3], d3[3]));

            auto m41 = _mm_mul_ps(e4, _mm_setr_ps(d4[0], d4[0], d4[1], d4[2]));
            auto m42 = _mm_mul_ps(e5, _mm_setr_ps(d4[1], d4[3], d4[3], d5[0]));
            auto m43 = _mm_mul_ps(e6, _mm_setr_ps(d4[2], d5[0], d5[1], d5[1]));

            auto h1 = _mm_setr_ps(1.0f, -1.0f, 1.0f, -1.0f);
            auto h2 = _mm_setr_ps(-1.0f, 1.0f, -1.0f, 1.0f);

            __m128 out[4] {
                _mm_mul_ps(_mm_add_ps(_mm_sub_ps(m11, m12), m13), h1),
                _mm_mul_ps(_mm_add_ps(_mm_sub_ps(m21, m22), m23), h2),
                _mm_mul_ps(_mm_add_ps(_mm_sub_ps(m31, m32), m33), h1),
                _mm_mul_ps(_mm_add_ps(_mm_sub_ps(m41, m42), m43), h2)
            };

            return {out};
        }

        return {};
    }

#if DEBUG_PRGE == 1
        ostream& operator << (ostream& os, const Matrix4x4f& matrix4x4)
        {
            int w = static_cast<int>(os.width());
            w = !w ? 2 : w;
            cout << matrix4x4._m[0][0] << setw(w) << matrix4x4._m[0][1] << setw(w) << matrix4x4._m[0][2] <<  setw(w) << matrix4x4._m[0][3] << endl
                 << matrix4x4._m[1][0] << setw(w) << matrix4x4._m[1][1] << setw(w) << matrix4x4._m[1][2] <<  setw(w) << matrix4x4._m[1][3] << endl
                 << matrix4x4._m[2][0] << setw(w) << matrix4x4._m[2][1] << setw(w) << matrix4x4._m[2][2] <<  setw(w) << matrix4x4._m[2][3] << endl
                 << matrix4x4._m[3][0] << setw(w) << matrix4x4._m[3][1] << setw(w) << matrix4x4._m[3][2] <<  setw(w) << matrix4x4._m[3][3] << endl;

            return os;
        }
#endif
}
