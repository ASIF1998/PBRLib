//
//  vec4.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 29/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <cmath>

namespace pbrlib::math
{
    template<typename Type>
    inline Vec4<Type>::Vec4(Type xyzw) noexcept :
        x{xyzw},
        y{xyzw},
        z{xyzw},
        w{xyzw}
    {}

    template<typename Type>
    inline Vec4<Type>::Vec4(Type x, Type y, Type z, Type w) noexcept :
        x{x},
        y{y},
        z{z},
        w{w}
    {}

    template<typename Type>
    inline bool Vec4<Type>::operator == (const Vec4<Type>& v) const noexcept
    {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }

    template<typename Type>
    inline bool Vec4<Type>::operator != (const Vec4<Type>& v) const noexcept
    {
        return x != v.x || y != v.y || z != v.z || w == v.w;
    }

    template<typename Type>
    inline Vec4<Type> Vec4<Type>::operator + (const Vec4<Type>& v) const noexcept
    {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    template<typename Type>
    inline Vec4<Type> Vec4<Type>::operator - (const Vec4<Type>& v) const noexcept
    {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    template<typename Type>
    inline Vec4<Type> Vec4<Type>::operator * (Type s) const noexcept
    {
        return {x * s, y * s, z * s, w * s};
    }

    template<typename Type>
    inline Vec4<Type>& Vec4<Type>::operator += (const Vec4<Type>& v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;

        return *this;
    }

    template<typename Type>
    inline Vec4<Type>& Vec4<Type>::operator -= (const Vec4<Type>& v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;

        return *this;
    }

    template<typename Type>
    inline Vec4<Type>& Vec4<Type>::operator *= (Type s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;

        return *this;
    }

    template<typename Type>
    inline Type& Vec4<Type>::operator [] (size_t i) noexcept
    {
        return xyzw[i];
    }

    template<typename Type>
    inline Type Vec4<Type>::operator [] (size_t i) const noexcept
    {
        return xyzw[i];
    }

    template<typename Type>
    inline Type Vec4<Type>::lengthSquared() const noexcept
    {
        return x * x + y * y + z * z + w * w;
    }

    template<typename Type>
    inline Type Vec4<Type>::length() const
    {
        return sqrt(lengthSquared());
    }

    template<typename Type>
    void Vec4<Type>::normalize()
    {
        auto l = length();
    
        assert(l);
        
        x /= l;
        y /= l;
        z /= l;
        w /= l;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline Vec4<float>::Vec4(float xyzw) noexcept :
        xyzw_simd(_mm_set1_ps(xyzw))
    {}

    inline Vec4<float>::Vec4(__m128 xyzw) noexcept :
        xyzw_simd(xyzw)
    {}

    inline Vec4<float>::Vec4(float x, float y, float z, float w) noexcept :
        x{x},
        y{y},
        z{z},
        w{w}
    {}

    inline bool Vec4<float>::operator == (const Vec4& v) const noexcept
    {
        return _mm_movemask_ps(_mm_cmpeq_ps(xyzw_simd, v.xyzw_simd)) == 15;
    }

    inline bool Vec4<float>::operator != (const Vec4& v) const noexcept
    {
        return _mm_movemask_ps(_mm_cmpeq_ps(xyzw_simd, v.xyzw_simd)) != 15;
    }

    inline Vec4<float> Vec4<float>::operator + (const Vec4& v) const noexcept
    {
        return _mm_add_ps(xyzw_simd, v.xyzw_simd);
    }

    inline Vec4<float> Vec4<float>::operator - (const Vec4& v) const noexcept
    {
        return _mm_sub_ps(xyzw_simd, v.xyzw_simd);
    }

    inline Vec4<float> Vec4<float>::operator * (float s) const noexcept
    {
        return _mm_mul_ps(xyzw_simd, _mm_set1_ps(s));
    }

    inline Vec4<float>& Vec4<float>::operator += (const Vec4& v) noexcept
    {
        xyzw_simd = _mm_add_ps(xyzw_simd, v.xyzw_simd);
        return *this;
    }

    inline Vec4<float>& Vec4<float>::operator -= (const Vec4& v) noexcept
    {
        xyzw_simd = _mm_sub_ps(xyzw_simd, v.xyzw_simd);
        return *this;
    }

    inline Vec4<float>& Vec4<float>::operator *= (float s) noexcept
    {
        xyzw_simd = _mm_mul_ps(xyzw_simd, _mm_set1_ps(s));
        return *this;
    }

    inline float& Vec4<float>::operator [] (size_t i) noexcept
    {
        return xyzw[i];
    }

    inline float Vec4<float>::operator [] (size_t i) const noexcept
    {
        return xyzw[i];
    }

    inline float Vec4<float>::lengthSquared() const noexcept
    {
        auto r = _mm_mul_ps(xyzw_simd, xyzw_simd);
        return r[0] + r[1] + r[2] + r[3];
    }

    inline float Vec4<float>::length() const
    {
        return sqrt(lengthSquared());
    }

    void Vec4<float>::normalize()
    {
        float l = length();
    
        assert(l != static_cast<float>(0u));
        xyzw_simd = _mm_div_ps(xyzw_simd, _mm_set1_ps(l));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename Type>
    inline Type dot(const Vec4<Type> v1, const Vec4<Type>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w + v2.w;
    }

    template<>
    inline float dot<float>(const Vec4<float> v1, const Vec4<float>& v2)
    {
        auto r = _mm_mul_ps(v1.xyzw_simd, v2.xyzw_simd);
        return r[0] + r[1] + r[2] + r[4];
    }

    template<typename Type>
    Vec4<Type> normalize(const Vec4<Type>& v)
    {
        Vec4<Type> res (v);
        res.normalize();
        return res;
    }

    template<typename Type>
    ostream& operator << (ostream& print, const Vec4<Type>& vec)
    {
        print << vec.x << ' ' << vec.y << ' ' << vec.z << ' ' << vec.w;
        return print;
    }
}