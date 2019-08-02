#include "math.h"

#include <xmmintrin.h>

#if DEBUG_PRGE == 1
#include <iostream>
#include <iomanip>
using namespace std;
#endif

namespace PRGE
{
    /**
     * Класс описывающий двумерный вектор.
    */
    template<typename Type>
    class Vec2
    {
    public:
        inline Vec2() noexcept
        {
            _xy[0] = static_cast<Type>(0);
            _xy[1] = static_cast<Type>(0);
        }

        inline Vec2(Type x, Type y) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(x, y);
#endif

            _xy[0] = x;
            _xy[1] = y;
        }

        inline Vec2(const Vec2& vec2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[0]);
#endif

            _xy[0] = vec2._xy[0];
            _xy[1] = vec2._xy[1];
        }

        inline Vec2& operator = (const Vec2& vec2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[0]);
#endif

            _xy[0] = vec2._xy[0];
            _xy[1] = vec2._xy[1];

            return *this;
        }

        inline Vec2 operator + (const Vec2& vec2) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[0]);
#endif

            return {_xy[0] + vec2._xy[0], _xy[1] + vec2._xy[1]};
        }

        inline Vec2 operator - (const Vec2& vec2) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[0]);
#endif

            return {_xy[0] - vec2._xy[0], _xy[1] - vec2._xy[1]};
        }

        Vec2 operator * (const Vec2& vec2) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[1]);
#endif

            return {_xy[0] * vec2._xy[0], _xy[1] * vec2._xy[1]};
        }

        Vec2 operator * (Type s) const noexcept
        {
            return {_xy[0] * s, _xy[1] * s};
        }

        Vec2 operator += (const Vec2& vec2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[1]);
#endif

            _xy[0] += vec2._xy[0];
            _xy[1] += vec2._xy[1];

            return *this;
        }

        Vec2 operator -= (const Vec2& vec2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[1]);
#endif

            _xy[0] -= vec2._xy[0];
            _xy[1] -= vec2._xy[1];

            return *this;
        }

        Vec2 operator *= (const Vec2& vec2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[1]);
#endif

            _xy[0] *= vec2._xy[0];
            _xy[1] *= vec2._xy[1];

            return *this;
        }

        Vec2 operator *= (Type s) noexcept
		{
            _xy[0] *= s;
            _xy[1] *= s;
            return *this;
        }

        Type& operator [] (size_t i) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 2) {
                throw length_error("Go beyond the boundaries of the two-dimensional vector");
            }
#endif

            return _xy[i];
        }

        Type operator [] (size_t i) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 2) {
                throw length_error("Go beyond the boundaries of the two-dimensional vector");
            }
#endif

            return _xy[i];
        }

#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const Vec2& vec2)
        {
            int w = static_cast<int>(os.width());
            os << vec2._xy[0] << setw(!w ? 2 : w) << vec2._xy[1];
            return os;
        }
#endif

    private:
        Type _xy[2];
    };

    /**
     * Класс описывающий трехмерный вектор.
    */
    template<typename Type>
    class Vec3
    {
    public:
        inline Vec3() noexcept
        {
            _xyz[0] = static_cast<Type>(0);
            _xyz[1] = static_cast<Type>(0);
            _xyz[3] = static_cast<Type>(0);
        }

        inline Vec3(Type x, Type y, Type z) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(x, y, z);
#endif

            _xyz[0] = x;
            _xyz[1] = y;
            _xyz[2] = z;
        }

        inline Vec3(const Vec3& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            _xyz[0] = vec3._xyz[0];
            _xyz[0] = vec3._xyz[1];
            _xyz[2] = vec3._xyz[2];
        }

        inline Vec3& operator = (const Vec3& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            _xyz[0] = vec3._xyz[0];
            _xyz[0] = vec3._xyz[1];
            _xyz[2] = vec3._xyz[2];

            return *this;
        }

        inline Vec3 operator + (const Vec3& vec3) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            return {_xyz[0] + vec3._xyz[0], _xyz[1] + vec3._xyz[1], _xyz[2] + vec3._xyz[2]};
        }

        inline Vec3 operator - (const Vec3& vec3) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            return {_xyz[0] - vec3._xyz[0], _xyz[1] - vec3._xyz[1], _xyz[2] - vec3._xyz[2]};
        }

        inline Vec3 operator * (Type s) const noexcept
        {
            return {_xyz[0] * s, _xyz[1] * s, _xyz[2] * s};
        }

        inline Vec3& operator += (const Vec3& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            _xyz[0] += vec3._xyz[0];
            _xyz[1] += vec3._xyz[1];
            _xyz[2] += vec3._xyz[2];

            return *this;
        }

        inline Vec3& operator -= (const Vec3& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            _xyz[0] -= vec3._xyz[0];
            _xyz[1] -= vec3._xyz[1];
            _xyz[2] -= vec3._xyz[2];

            return *this;
        }

        inline Vec3& operator *= (Type s) NOEXCEPT_PRGE
        {
            _xyz[0] *= s;
            _xyz[1] *= s;
            _xyz[2] *= s;

            return *this;
        }

        Type& operator [] (size_t i) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 3) {
                throw length_error("Go beyond the boundaries of the two-dimensional vector");
            }
#endif

            return _xyz[i];
        }

        Type operator [] (size_t i) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 3) {
                throw length_error("Go beyond the boundaries of the two-dimensional vector");
            }
#endif

            return _xyz[i];
        }

#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const Vec3& vec3) 
        {
            int w = static_cast<int>(os.width());
            w = !w ? 2 : w;
            os << vec3._xyz[0] << setw(w) << vec3._xyz[1] << setw(w) << vec3._xyz[2];
            return os;
        }
#endif

        /**
         * Функция осуществляющая скалярное произведение.
         * 
         * @param v1 первый вектор
         * @param v2 второй вектор
         * @return скалярное произведение между векторами v1 и v2
        */
        friend inline Type dot(const Vec3& v1, const Vec3& v2) NOEXCEPT_PRGE 
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(v1._xyz[0], v1._xyz[1], v1._xyz[2]);
            NAN_OR_INF_XYZ(v2._xyz[0], v2._xyz[1], v2._xyz[2]);
#endif

            return v1._xyz[0] * v2._xyz[0] + v1._xyz[1] * v2._xyz[1] + v1._xyz[2] * v2._xyz[2];
        }

        /**
         * Функция осуществляющая векторное произведение.
         * 
         * @param v1 первый вектор
         * @param v2 второй вектор
         * @return векторное произведение между векторами v1 и v2
        */
        friend inline Vec3 cross(const Vec3& v1, const Vec3& v2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(v1._xyz[0], v1._xyz[1], v1._xyz[2]);
            NAN_OR_INF_XYZ(v2._xyz[0], v2._xyz[1], v2._xyz[2]);
#endif

            return {v1._xyz[1] * v2._xyz[2] - v1._xyz[2] * v2._xyz[1], 
                    v1._xyz[2] * v2._xyz[0] - v1._xyz[0] * v2._xyz[2],
                    v1._xyz[0] * v2._xyz[1] - v1._xyz[1] * v2._xyz[0]};
        }

    private:
        Type _xyz[3];
    };

    /**
     * Класс описывающий трехмерный вектор типа float с применением SIMD.
    */
    template<>
    class Vec3<float>
    {
    public:
        inline Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(x, y, z);
#endif

            _xyz[0] = x;
            _xyz[1] = y;
            _xyz[2] = z;
        }

        inline Vec3(const Vec3& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            _xyz[0] = vec3._xyz[0];
            _xyz[0] = vec3._xyz[1];
            _xyz[2] = vec3._xyz[2];
        }

        inline Vec3& operator = (const Vec3& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            _xyz[0] = vec3._xyz[0];
            _xyz[0] = vec3._xyz[1];
            _xyz[2] = vec3._xyz[2];

            return *this;
        }

        inline Vec3 operator + (const Vec3& vec3) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            return {_mm_add_ps(_mm_set_ps(_xyz[0], _xyz[1], _xyz[2], 0.0f), _mm_set_ps(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2], 0.0f))};
        }

        inline Vec3 operator - (const Vec3& vec3) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            return {_mm_sub_ps(_mm_set_ps(_xyz[0], _xyz[1], _xyz[2], 0.0f), _mm_set_ps(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2], 0.0f))};
        }

        inline Vec3 operator * (float s) const noexcept
        {
            return {_mm_mul_ps(_mm_set_ps(_xyz[0], _xyz[1], _xyz[2], 0.0f), _mm_set_ps(s, s, s, s))};
        }

        inline Vec3& operator += (const Vec3& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            auto res = _mm_add_ps(_mm_set_ps(_xyz[0], _xyz[1], _xyz[2], 0.0f), _mm_set_ps(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2], 0.0f));

            _xyz[0] = res[0];
            _xyz[1] = res[1];
            _xyz[2] = res[2];

            return *this;
        }

        inline Vec3& operator -= (const Vec3& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            auto res = _mm_sub_ps(_mm_set_ps(_xyz[0], _xyz[1], _xyz[2], 0.0f), _mm_set_ps(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2], 0.0f));

            _xyz[0] = res[0];
            _xyz[1] = res[1];
            _xyz[2] = res[2];

            return *this;
        }

        inline Vec3& operator *= (float s) noexcept
        {
            auto res = _mm_mul_ps(_mm_set_ps(_xyz[0], _xyz[1], _xyz[2], 0.0f), _mm_set_ps(s, s, s, s));

            _xyz[0] = res[0];
            _xyz[1] = res[1];
            _xyz[2] = res[2];

            return *this;
        }

        float& operator [] (size_t i) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 3) {
                throw length_error("Go beyond the boundaries of the two-dimensional vector");
            }
#endif

            return _xyz[i];
        }

        float operator [] (size_t i) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 3) {
                throw length_error("Go beyond the boundaries of the two-dimensional vector");
            }
#endif

            return _xyz[i];
        }

#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const Vec3& vec3) 
        {
            int w = static_cast<int>(os.width());
            w = !w ? 2 : w;
            os << vec3._xyz[0] << setw(w) << vec3._xyz[1] << setw(w) << vec3._xyz[2];
            return os;
        }
#endif

        /**
         * Функция осуществляющая скалярное произведение.
         * 
         * @param v1 первый вектор
         * @param v2 второй вектор
         * @return скалярное произведение между векторами v1 и v2
        */
        friend float dot(const Vec3& v1, const Vec3& v2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(v1._xyz[0], v1._xyz[1], v1._xyz[2]);
            NAN_OR_INF_XYZ(v2._xyz[0], v2._xyz[1], v2._xyz[2]);
#endif

            auto r = _mm_mul_ps(_mm_set_ps(v1._xyz[0], v1._xyz[1], v1._xyz[2], 0.0f), _mm_set_ps(v2._xyz[0], v2._xyz[1], v2._xyz[2], 0.0f));

            return r[0] + r[1] + r[2]; 
        }

        /**
         * Функция осуществляющая векторное произведение.
         * 
         * @param v1 первый вектор
         * @param v2 второй вектор
         * @return векторное произведение между векторами v1 и v2
        */
        friend Vec3 cross(const Vec3& v1, const Vec3& v2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(v1._xyz[0], v1._xyz[1], v1._xyz[2]);
            NAN_OR_INF_XYZ(v2._xyz[0], v2._xyz[1], v2._xyz[2]);
#endif

            auto r1 = _mm_mul_ps(_mm_set_ps(v1._xyz[1], v1._xyz[2], v1._xyz[0]), _mm_set_ps(v2._xyz[2], v2._xyz[0], v2._xyz[1]));
            auto r2 = _mm_mul_ps(_mm_set_ps(v1._xyz[2], v1._xyz[0], v1._xyz[1]), _mm_set_ps(v2._xy1[1], v2._xyz[2], v2._xyz[0]));

            return _mm_sub_ps(r1, r2);
        }

    private:
        inline Vec3(const __m128& v) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(v[0], v[1], v[2]);
#endif

            _xyz[0] = v[0];
            _xyz[1] = v[1];
            _xyz[2] = v[2];
        }

    private:
        float _xyz[3];
    };
}
