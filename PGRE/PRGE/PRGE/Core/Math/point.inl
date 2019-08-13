#include "math.h"

#include "vec.h"

namespace PRGE
{
    /**
     * Класс описывающий двумерную точку.
    */
    template<typename Type>
    class Point2
    {
    public:
        inline Point2() noexcept
        {
            _xy[0] = static_cast<Type>(0);
            _xy[1] = static_cast<Type>(0);
        }

        inline Point2(Type x, Type y) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(x, y);
#endif

            _xy[0] = x;
            _xy[1] = y;
        }

        inline Point2(const Point2& point2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(point2._xy[0], point2._xy[0]);
#endif

            _xy[0] = point2._xy[0];
            _xy[1] = point2._xy[1];
        }

        inline Point2& operator = (const Point2& point2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(point2._xy[0], point2._xy[1]);
#endif

            _xy[0] = point2._xy[0];
            _xy[1] = point2._xy[1];

            return *this;
        }

        template<typename U>
        inline explicit operator Point2<U> () const
        {
        return {static_cast<U>(_xy[0]), static_cast<U>(_xy[1])};
        }

        inline explicit operator Vec2<Type> () const noexcept
        {
            return {_xy[0], _xy[1]};
        }

        inline Point2 operator + (const Vec2<Type>& vec2) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[1]);
#endif

            return {_xy[0] + vec2._xy[0], _xy[1] + vec2._xy[1]};
        }

        template<typename T, template<typename> class U>
        inline Vec2<T> operator - (const U<T>& p) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(p._xy[0], p._xy[1]);
#endif

            return {_xy[0] - p._xy[0], _xy[1] - p._xy[1]};
        }

        inline Point2 operator * (Type s) const noexcept
        {
            return {_xy[0] * s, _xy[1] * s};
        }

        Point2 operator / (Type s) const
        {
            float invS = 1.0f / s;
            return {_xy[0] * invS, _xy[1] * invS};
        }

        Point2& operator += (const Vec2<Type>& vec2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(vec2._xy[0], vec2._xy[1]);
#endif

            _xy[0] += vec2._xy[0];
            _xy[1] += vec2._xy[1];

            return *this;
        }

        template<typename T, template<typename> class U>
        Point2& operator -= (const U<T>& p) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XY(p._xy[0], p._xy[1]);
#endif

            _xy[0] -= p._xy[0];
            _xy[1] -= p._xy[1];

            return *this;
        }

        Point2& operator *= (Type s)
        {
            _xy[0] *= s;
            _xy[1] *= s;

            return *this;
        }

        Point2& operator /= (Type s)
        {
            float invS = 1.0f / s;

            _xy[0] *= invS;
            _xy[1] *= invS;

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
        friend ostream& operator << (ostream& os, const Point2& point2)
        {
            int w = static_cast<int>(os.width());
            os << point2._xy[0] << setw(!w ? 2 : w) << point2._xy[1];
            return os;
        }
#endif

    private:
        Type _xy[2];
    };

    /**
     * Класс описывающий трехмерную точку.
    */
    template<typename Type>
    class Point3
    {
    public:
        inline Point3() noexcept
        {
            _xyz[0] = static_cast<Type>(0);
            _xyz[1] = static_cast<Type>(0);
            _xyz[2] = static_cast<Type>(0);
        }

        inline Point3(Type x, Type y, Type z) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(x, y, z);
#endif

            _xyz[0] = x;
            _xyz[1] = y;
            _xyz[2] = z;
        }

        inline Point3(const Point3& point3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(point3._xyz[0], point3._xyz[1], point3._xyz[2]);
#endif

            _xyz[0] = point3._xyz[0];
            _xyz[1] = point3._xyz[1];
            _xyz[2] = point3._xyz[2];
        }

        inline Point3& operator = (const Point3& point3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(point3._xyz[0], point3._xyz[1], point3._xyz[2]);
#endif

            _xyz[0] = point3._xyz[0];
            _xyz[1] = point3._xyz[1];
            _xyz[2] = point3._xyz[2];
        }

        template<typename U>
        inline explicit operator Point3<U> () const
        {
            return {static_cast<U>(_xyz[0]), static_cast<U>(_xyz[1]), static_cast<U>(_xyz[2])};
        }

        inline explicit operator Vec3<Type> () const noexcept
        {
            return {_xyz[0], _xyz[1], _xyz[2]};
        }

        inline Point3 operator + (const Vec3<Type>& vec3) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            return {_xyz[0] + vec3._xyz[0], _xyz[1] + vec3._xyz[1], _xyz[2] + vec3._xyz[2]};
        }

        template<template<typename> class U>
        inline auto operator - (const U<Type>& p) const NOEXCEPT_PRGE
            -> decltype(is_same<U<Type>, Vec3<Type>>::value ? Point3<Type>{} : Vec3<Type>{})
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(p._xyz[0], p._xyz[1], p._xyz[2]);
#endif

            return {_xyz[0] - p._xyz[0], _xyz[1] - p._xyz[1], _xyz[2] - p._xyz[2]};
        }

        inline Point3 operator * (Type s) const noexcept
        {
            return {_xyz[0] * s, _xyz[1] * s, _xyz[2] * s};
        }

        inline Point3 operator / (Type s) const
        {
            float invS = 1.0f / s;
            return {_xyz[0] * invS, _xyz[1] * invS, _xyz[2] * invS};
        }

        Point3& operator += (const Vec3<Type>& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            _xyz[0] += vec3._xyz[0];
            _xyz[1] += vec3._xyz[1];
            _xyz[2] += vec3._xyz[2];

            return *this;
        }

        template<typename T, template<typename> class U>
        Point3& operator -= (const U<T>& p) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(p._xyz[0], p._xyz[1], p._xyz[2]);
#endif

            _xyz[0] -= p._xyz[0];
            _xyz[1] -= p._xyz[1];
            _xyz[2] -= p._xyz[2];

            return *this;
        }

        Point3& operator *= (Type s)
        {
            _xyz[0] *= s;
            _xyz[1] *= s;
            _xyz[2] *= s;

            return *this;
        }

        Point3& operator /= (Type s)
        {
            float invS = 1.0f / s;

            _xyz[0] *= invS;
            _xyz[1] *= invS;
            _xyz[2] *= invS;

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
        friend ostream& operator << (ostream& os, const Point3& point3) 
        {
            int w = static_cast<int>(os.width());
            w = !w ? 2 : w;
            os << point3._xyz[0] << setw(w) << point3._xyz[1] << setw(w) << point3._xyz[2];
            return os;
        }
#endif

        inline bool operator == (const Point3& p) const noexcept
        {
            return _xyz[0] == p._xyz[0] && _xyz[1] == p._xyz[1] && _xyz[2] == p._xyz[2];
        }

        inline bool operator != (const Point3& p) const noexcept
        {
            return _xyz[0] != p._xyz[0] || _xyz[1] != p._xyz[1] || _xyz[2] != p._xyz[2];
        }

    private:
        Type _xyz[3];
    };

    template<>
    class Point3<float>
    {
    public:
        Point3(float x = 0.0f, float y = 0.0f, float z = 0.0f) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(x, y, z);
#endif

            _xyz[0] = x;
            _xyz[1] = y;
            _xyz[2] = z;
            _xyz[3] = 0.f;
        }

        inline Point3(const Point3& point3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(point3._xyz[0], point3._xyz[1], point3._xyz[2]);
#endif

            _xyz[0] = point3._xyz[0];
            _xyz[1] = point3._xyz[1];
            _xyz[2] = point3._xyz[2];
        }

        inline Point3& operator = (const Point3& point3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(point3._xyz[0], point3._xyz[1], point3._xyz[2]);
#endif

            _xyz[0] = point3._xyz[0];
            _xyz[1] = point3._xyz[1];
            _xyz[2] = point3._xyz[2];

            return *this;
        }

        template<typename U>
        inline explicit operator Point3<U> () const 
        {
            return {static_cast<U>(_xyz[0]), static_cast<U>(_xyz[1]), static_cast<U>(_xyz[2])};
        }

        inline explicit operator Vec3<float> () const 
        {
            return {_xyz[0], _xyz[1], _xyz[2]};
        }

        inline Point3 operator + (const Vec3<float>& vec3) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            return {_mm_add_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(vec3._xyz))};
        }

        inline Vec3<float> operator - (const Point3& point3) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(point3._xyz[0], point3._xyz[1], point3._xyz[2]);
#endif

            return {_mm_sub_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(point3._xyz))};
        }

        inline Point3 operator - (const Vec3<float>& vec3) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            return {_mm_sub_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(vec3._xyz))};
        }

        inline Point3 operator * (float s) const noexcept
        {
            return {_mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), _mm_set_ps(s, s, s, s))};
        }

        inline Point3 operator / (float s) const
        {
            float invS = 1.0f / s;
            return {_mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), _mm_set_ps(invS, invS, invS, invS))};
        }

        inline Point3& operator += (const Vec3<float>& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            auto res = _mm_add_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(vec3._xyz));

            _xyz[0] = res[0];
            _xyz[1] = res[1];
            _xyz[2] = res[2];

            return *this;
        }

        inline Point3& operator -= (const Point3& point3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(point3._xyz[0], point3._xyz[1], point3._xyz[2]);
#endif

            auto res = _mm_sub_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(point3._xyz));

            _xyz[0] = res[0];
            _xyz[1] = res[1];
            _xyz[2] = res[2];

            return *this;
        }

        inline Point3& operator -= (const Vec3<float>& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            auto res = _mm_sub_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(vec3._xyz));

            _xyz[0] = res[0];
            _xyz[1] = res[1];
            _xyz[2] = res[2];

            return *this;
        }

        inline Point3& operator *= (float s) noexcept
        {
            auto res = _mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), _mm_set_ps(s, s, s, s));

            _xyz[0] = res[0];
            _xyz[1] = res[1];
            _xyz[2] = res[2];

            return *this;
        }

        inline Point3& operator /= (float s) noexcept
        {
            float invS = 1.0f / s;
            auto res = _mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), _mm_set_ps(invS, invS, invS, invS));

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
        friend ostream& operator << (ostream& os, const Point3& point3) 
        {
            int w = static_cast<int>(os.width());
            w = !w ? 2 : w;
            os << point3._xyz[0] << setw(w) << point3._xyz[1] << setw(w) << point3._xyz[2];
            return os;
        }
#endif

        inline bool operator == (const Point3& point3) const noexcept
        {
            auto res = _mm_cmpeq_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(point3._xyz));
            return res[0] && res[1] && res[2];
        }

        inline bool operator != (const Point3& point3) const noexcept
        {
            auto res = _mm_cmpneq_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(point3._xyz));
            return res[0] || res[1] || res[2];
        }

    private:
        inline Point3(__m128 m) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(m[0], m[1], m[2]);
#endif

            _xyz[0] = m[0];
            _xyz[1] = m[1];
            _xyz[2] = m[2];
        }

    private:
        float _xyz[4];
    };
}
