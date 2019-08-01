#include "math.h"

#if DEBUG_PRGE == 1
#include <iostream>
using namespace std;
#endif

namespace PRGE
{
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

        const Type& operator [] (size_t i) const NOEXCEPT_PRGE
        {
#if DEBUG_PRGE == 1
            if (i > 2) {
                throw length_error("Go beyond the boundaries of the two-dimensional vector");
            }
#endif

            return _xy[i];
        }

#if DEBUG_PRGE == 1
        friend auto& operator << (ostream& os, const Vec2& vec2)
        {
            int w = static_cast<int>(os.width());
            os << vec2._xy[0] << setw(!w ? 2 : w) << vec2._xy[1];
            return os;
        }
#endif

    private:
        Type _xy[2];
    };
}
