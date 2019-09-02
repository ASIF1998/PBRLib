#include "../core.h"

#include "math.h"

#include "vec.h"

#include <cmath>

using namespace std;

namespace PRGE
{
    class Normal3f
    {
        friend class Transform;

    public:
        inline Normal3f(float x = 0.f, float y = 0.f, float z = 0.f) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(x, y, z);
#endif
            _xyz[0] = x;
            _xyz[1] = y;
            _xyz[2] = z;
            _xyz[3] = 0.f;
        }

        inline Normal3f(const Vec3<float>& vec3) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(vec3._xyz[0], vec3._xyz[1], vec3._xyz[2]);
#endif

            *reinterpret_cast<__m128*>(_xyz) = *reinterpret_cast<const __m128*>(vec3._xyz);
        }

        inline Normal3f(const Normal3f& normal) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif

            *reinterpret_cast<__m128*>(_xyz) = *reinterpret_cast<const __m128*>(normal._xyz);
        }
        
        operator Vec3<float> () const
        {
            return {_xyz[0], _xyz[1], _xyz[2]};
        }

        Normal3f(Normal3f&& normal) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif

            auto&& n1 = move(normal._xyz);
            auto&& n2 = move(_xyz);
            swap(n1, n2);
        }


        inline Normal3f& operator = (const Normal3f& normal) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif

            *reinterpret_cast<__m128*>(_xyz) = *reinterpret_cast<const __m128*>(normal._xyz);

            return *this;
        }

        Normal3f& operator = (Normal3f&& normal) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif
            
            auto&& n1 = move(normal._xyz);
            auto&& n2 = move(_xyz);
            swap(n1, n2);

            return *this;
        }

        inline Normal3f operator + (const Normal3f& normal) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif

            return {_mm_add_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(normal._xyz))};
        }

        inline Normal3f operator - (const Normal3f& normal) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif

            return {_mm_sub_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(normal._xyz))};
        }

        inline Normal3f operator - () const noexcept
        {
            return {-_xyz[0], -_xyz[1], -_xyz[2]};
        }

        inline Normal3f operator * (float s) const noexcept
        {
            return {_mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), _mm_set_ps(s, s, s, s))};
        }

        inline Normal3f operator / (float s) const
        {
            float invS = 1.0f / s;
            return {_mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), _mm_set_ps(invS, invS, invS, invS))};
        }

        inline Normal3f& operator += (const Normal3f& normal) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif

            *reinterpret_cast<__m128*>(_xyz) = _mm_add_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(normal._xyz));

            return *this;
        }

        inline Normal3f& operator -= (const Normal3f& normal) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(normal._xyz[0], normal._xyz[1], normal._xyz[2]);
#endif

            *reinterpret_cast<__m128*>(_xyz) = _mm_sub_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(normal._xyz));

            return *this;
        }

        inline Normal3f& operator *= (float s) noexcept
        {
            *reinterpret_cast<__m128*>(_xyz) = _mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), _mm_set_ps(s, s, s, s));

            return *this;
        }

        inline Normal3f& operator /= (float s) noexcept
        {
            float invS = 1.0f / s;
            *reinterpret_cast<__m128*>(_xyz) = _mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), _mm_set_ps(invS, invS, invS, invS));

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

        float lengthSquared() const noexcept
        {
            auto res = _mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(_xyz));
            return res[0] + res[1] + res[2];
        }

        float length() const noexcept
        {
            auto res = _mm_mul_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(_xyz));
            return sqrt(res[0] + res[1] + res[2]);
        }

        inline bool operator == (const Normal3f& normal) const noexcept
        {
            auto res = _mm_cmpeq_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(normal._xyz));
            return res[0] && res[1] && res[2];
        }

        inline bool operator != (const Normal3f& normal) const noexcept
        {
            auto res = _mm_cmpneq_ps(*reinterpret_cast<const __m128*>(_xyz), *reinterpret_cast<const __m128*>(normal._xyz));
            return res[0] || res[1] || res[2];
        }

#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const Normal3f& point3)
        {
            int w = static_cast<int>(os.width());
            w = !w ? 2 : w;
            os << point3._xyz[0] << setw(w) << point3._xyz[1] << setw(w) << point3._xyz[2];
            return os;
        }
#endif

        friend Normal3f normalize(const Normal3f& normal)
        {
            auto res = _mm_mul_ps(*reinterpret_cast<const __m128*>(normal._xyz), *reinterpret_cast<const __m128*>(normal._xyz));
            auto invLength = 1.0f / sqrt(res[0] + res[1] + res[2]);
            return {_mm_mul_ps(*reinterpret_cast<const __m128*>(normal._xyz), _mm_set_ps(invLength, invLength, invLength, invLength))};
        }

    private:
        inline Normal3f(__m128 m) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(m[0], m[1], m[2]);
#endif

            *reinterpret_cast<__m128*>(_xyz) = m;
        }

        /**
         * Функция осуществляющая скалярное произведение.
         *
         * @param n1 первый вектор
         * @param n2 второй вектор
         * @return скалярное произведение между векторами v1 и v2
         */
        friend float dot(const Normal3f& n1, const Normal3f& n2) NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZ(n1._xyz[0], n1._xyz[1], n1._xyz[2]);
            NAN_OR_INF_XYZ(n2._xyz[0], n2._xyz[1], n2._xyz[2]);
#endif

            auto r = _mm_mul_ps(*reinterpret_cast<const __m128*>(n1._xyz), *reinterpret_cast<const __m128*>(n2._xyz));

            return r[0] + r[1] + r[2];
        }

        float _xyz[4];
    };
}
