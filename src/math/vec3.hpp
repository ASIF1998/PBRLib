//
//  vec3.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 22/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef vec3_hpp
#define vec3_hpp

namespace pbrlib::math
{
    template<typename Type>
    struct Vec3
    {
    public:
        inline Vec3(Type xyz = static_cast<Type>(0)) noexcept;
        inline Vec3(Type x, Type y, Type z) noexcept;

        inline bool operator == (const Vec3& v) const noexcept;
        inline bool operator != (const Vec3& v) const noexcept;

        inline Vec3 operator + (const Vec3& v) const noexcept;
        inline Vec3 operator - (const Vec3& v) const noexcept;
        inline Vec3 operator * (Type s) const noexcept;

        inline Vec3& operator += (const Vec3& v) noexcept;
        inline Vec3& operator -= (const Vec3& v) noexcept;
        inline Vec3& operator *= (Type s) noexcept;

        inline Type& operator [] (size_t i) noexcept;
        inline Type operator [] (size_t i) const noexcept;

        /**
         * @brief Метод необходимый для вычисления квадрата длины вектора.
         * 
         * @return квадрат длины вектора.
        */
        inline Type lengthSquared() const noexcept;

        /**
         * @brief Метод необходимый для вычисления длины вектора.
         * 
         * @return длину векора.
        */
        inline Type length() const;

        /**
         * @brief Метод необходимый для нормирования вектора.
        */
        void normalize();

    public:
        union
        {
            struct
            {
                Type x;
                Type y;
                Type z;
            };

            Type xyz[3];
        };
    };

    /**
     * @brief Функция предназначенная для осуществления скалярного произведения.
     * 
     * @param v1 вектор.
     * @param v2 вектор.
     * @return результат скалярного произведения.
    */
    template<typename Type>
    inline Type dot(const Vec3<Type> v1, const Vec3<Type>& v2);

    /**
     * @brief Функция предназначенная для осуществления векторного произведения.
     * 
     * @param v1 вектор.
     * @param v2 вектор.
     * @return результат векторного произведения.
    */
    template<typename Type>
    inline Vec3<Type> cross(const Vec3<Type> v1, const Vec3<Type>& v2);

    /**
     * @brief Функция необходимая для нормирования вектора.
     * 
     * @param v вектор.
     * @return нормированный вектор v.
    */
    template<typename Type>
    Vec3<Type> normalize(const Vec3<Type>& v);
}

#include "vec3.inl"

#endif /* vec3_hpp */
