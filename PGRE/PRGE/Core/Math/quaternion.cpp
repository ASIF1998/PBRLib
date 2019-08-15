#include "quaternion.h"

#include <cmath>

#include <algorithm>

#if DEBUG_PRGE == 1
#include <iomanip>
#endif

using namespace std;

namespace PRGE
{
    Quaternion::Quaternion(float x, float y, float z, float w) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(x, y, z, w);
#endif

        _xyzw[0] = x;
        _xyzw[1] = y;
        _xyzw[2] = z;
        _xyzw[3] = w;
    }

    Quaternion::Quaternion(const Quaternion& quaternion) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

        *reinterpret_cast<__m128*>(_xyzw) = *reinterpret_cast<const __m128*>(quaternion._xyzw);
    }

    Quaternion::Quaternion(Quaternion&& quaternion) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif
        
        auto&& q1 = move(quaternion._xyzw);
        auto&& q2 = move(_xyzw);
        swap(q1, q2);
    }

    Quaternion::Quaternion(__m128 m) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(m[0], m[1], m[2], m[3]);
#endif

        *reinterpret_cast<__m128*>(_xyzw) = m;
    }

    Quaternion& Quaternion::operator = (const Quaternion& quaternion) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
            NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

        *reinterpret_cast<__m128*>(_xyzw) = *reinterpret_cast<const __m128*>(quaternion._xyzw);

        return *this;
    }

    Quaternion& Quaternion::operator = (Quaternion&& quaternion) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

        auto&& q1 = move(quaternion._xyzw);
        auto&& q2 = move(_xyzw);
        swap(q1, q2);

        return *this;
    }

    Quaternion Quaternion::operator + (const Quaternion& quaternion) const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

        return {_mm_add_ps(*reinterpret_cast<const __m128*>(_xyzw), *reinterpret_cast<const __m128*>(quaternion._xyzw))};
    }

    Quaternion Quaternion::operator - (const Quaternion& quaternion) const NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

        return {_mm_sub_ps(*reinterpret_cast<const __m128*>(_xyzw), *reinterpret_cast<const __m128*>(quaternion._xyzw))};
    }

    Quaternion Quaternion::operator - () const noexcept
    {
        return {_mm_mul_ps(*reinterpret_cast<const __m128*>(_xyzw), _mm_set_ps(-1.0f, -1.0f, -1.0f, -1.0f))};
    }

    Quaternion Quaternion::operator * (float s) const noexcept
    {
        return {_mm_mul_ps(*reinterpret_cast<const __m128*>(_xyzw), _mm_set_ps(s, s, s, s))};
    }

    Quaternion Quaternion::operator / (float s) const
    {
        float invS = 1.0f / s;
        return {_mm_mul_ps(*reinterpret_cast<const __m128*>(_xyzw), _mm_set_ps(invS, invS, invS, invS))};
    }

    Quaternion& Quaternion::operator += (const Quaternion& quaternion) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

        *reinterpret_cast<__m128*>(_xyzw) = _mm_add_ps(*reinterpret_cast<const __m128*>(_xyzw), *reinterpret_cast<const __m128*>(quaternion._xyzw));
        return *this;
    }

    Quaternion& Quaternion::operator -= (const Quaternion& quaternion) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_XYZW(quaternion._xyzw[0], quaternion._xyzw[1], quaternion._xyzw[2], quaternion._xyzw[3]);
#endif

        *reinterpret_cast<__m128*>(_xyzw) = _mm_sub_ps(*reinterpret_cast<const __m128*>(_xyzw), *reinterpret_cast<const __m128*>(quaternion._xyzw));
        return *this;
    }

    Quaternion& Quaternion::operator *= (float s) noexcept
    {
        *reinterpret_cast<__m128*>(_xyzw) = _mm_mul_ps(*reinterpret_cast<const __m128*>(_xyzw), _mm_set_ps(s, s, s, s));
        return *this;
    }

    Quaternion& Quaternion::operator /= (float s)
    {
        float invS = 1.0f / s;
        *reinterpret_cast<__m128*>(_xyzw) = _mm_mul_ps(*reinterpret_cast<const __m128*>(_xyzw), _mm_set_ps(invS, invS, invS, invS));
        return *this;
    }

#if DEBUG_PRGE == 1
    ostream& operator << (ostream& os, const Quaternion& quaternion)
    {
        int w = static_cast<int>(os.width());
        w = !w ? 2 : w;
        os << quaternion._xyzw[0] << setw(w) << quaternion._xyzw[1] << setw(w) << quaternion._xyzw[2] << setw(w) << quaternion._xyzw[3];
        return os;
    }
#endif

    float dot(const Quaternion& q1, const Quaternion& q2) NOEXCEPT_PRGE
    {
#if DEBUG_PRGE == 1
        NAN_OR_INF_XYZW(q1._xyzw[0], q1._xyzw[1], q1._xyzw[2], q1._xyzw[3]);
        NAN_OR_INF_XYZW(q2._xyzw[0], q2._xyzw[1], q2._xyzw[2], q2._xyzw[3]);
#endif

        auto res = _mm_mul_ps(*reinterpret_cast<const __m128*>(q1._xyzw), *reinterpret_cast<const __m128*>(q2._xyzw));
        return res[0] + res[1] + res[2] + res[3];
    }

    Quaternion normalize(const Quaternion& quaternion) NOEXCEPT_PRGE
    {
        return quaternion / sqrt(dot(quaternion, quaternion));
    }

    Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) NOEXCEPT_PRGE
    {
        float cosTheta = dot(q1, q2);

        if (cosTheta > 0.9995f) {
            return q1 * (1.0f - t) + q2 * t;
        } 

        float theta = acos(cosTheta);
        float thetap = t * theta;
        auto qperp = normalize(q2 - q1 * cosTheta);
        return q1 * cos(thetap) + qperp * sin(theta);
    }
}
