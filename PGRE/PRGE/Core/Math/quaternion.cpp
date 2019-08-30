#include "quaternion.h"


using namespace std;

namespace PRGE
{
    Quaternion::Quaternion(const Matrix4x4f& matrix4x4)
    {
        auto t = matrix4x4[0][0] + matrix4x4[1][1] + matrix4x4[2][2] + 1.0f;

        if (t > 0) {
            auto s = sqrt(t + 1.0f);

            _xyzw[3] = s * 0.5f;
            s = 2.0f * s;

            _xyzw[0] = (matrix4x4[2][1] - matrix4x4[1][2]) * s;
            _xyzw[1] = (matrix4x4[0][2] - matrix4x4[2][0]) * s;
            _xyzw[2] = (matrix4x4[1][0] - matrix4x4[0][1]) * s;
        } else {
            const int nxt[3] = {1, 2, 0};
            float q[3];
            int i = 0;

            if (matrix4x4[1][1] > matrix4x4[0][0]) {
                i = 1;
            }

            if (matrix4x4[2][2] > matrix4x4[i][i]) {
                i = 2;
            }

            int j = nxt[i];
            int k = nxt[j];

            float s = sqrt((matrix4x4[i][i] - matrix4x4[j][j] + matrix4x4[k][k]) + 1.0f);

            q[i] = s * 0.5f;

            if (s != 0.f) {
                s *= 2.0f;
            }

            _xyzw[3] = (matrix4x4[k][j] - matrix4x4[j][k]) * s;
            q[j] = (matrix4x4[j][i] + matrix4x4[i][j]) * s;
            q[k] = (matrix4x4[k][i] + matrix4x4[i][k]) * s;

            _xyzw[0] = q[0];
            _xyzw[1] = q[1];
            _xyzw[2] = q[2];
        }
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

    Transform Quaternion::toTransform() const
    {
        float xx = _xyzw[0] * _xyzw[0], yy = _xyzw[1] * _xyzw[1], zz = _xyzw[2] * _xyzw[2];
        float xy = _xyzw[0] * _xyzw[1], xz = _xyzw[0] * _xyzw[2], yz = _xyzw[1] * _xyzw[2];
        float wx = _xyzw[0] * _xyzw[3], wy = _xyzw[1] * _xyzw[3], wz = _xyzw[2] * _xyzw[3];

        Matrix4x4f m {
            1 - 2 * (yy + zz), 2 * (xy + wz), 2 * (xz - wy), 0.f,
            2 * (xy - wz), 1 - 2 * (xx + zz), 2 * (yz + wx), 0.f,
            2 * (xz + wy), 2 * (yz - wx), 1 - 2 * (xx + yy), 0.f
        };

        return {transpose(m), m};
    }

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

    Quaternion slerp(float t, const Quaternion& q1, const Quaternion& q2) NOEXCEPT_PRGE
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
