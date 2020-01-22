//
//  matrix4x4.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 22/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef matrix4x4_hpp
#define matrix4x4_hpp

#include <iostream>

#include <immintrin.h>

using namespace std;

namespace pbrlib::math
{
    template<typename Type>
    class Matrix4x4
    {
    public:
        Matrix4x4(Type init_value = static_cast<Type>(0));
        Matrix4x4(Type v11, Type v12, Type v13, Type v14,
                  Type v21, Type v22, Type v23, Type v24,
                  Type v31, Type v32, Type v33, Type v34,
                  Type v41, Type v42, Type v43, Type v44);

        /**
         * @brief Констуктор.
         * 
         * @details 
         *      Указатель ptr_data должен указывать на массив, размер 
         *      которого больше или равняется 16 * sizeof(Type).
         * 
         * @param ptr_data указатель на массив.
        */
        Matrix4x4(const Type* ptr_data);

        bool operator == (const Matrix4x4& mat) const;
        bool operator != (const Matrix4x4& mat) const;

        Matrix4x4 operator + (const Matrix4x4& mat) const;
        Matrix4x4 operator - (const Matrix4x4& mat) const;
        Matrix4x4 operator * (const Matrix4x4& mat) const;
        Matrix4x4 operator * (Type scal) const;

        Matrix4x4& operator += (const Matrix4x4& mat);
        Matrix4x4& operator -= (const Matrix4x4& mat);
        Matrix4x4& operator *= (const Matrix4x4& mat);
        Matrix4x4& operator *= (Type scal);

        inline Type* operator [] (size_t i);
        inline const Type* operator [] (size_t i) const;

        Type& at(size_t i, size_t j);
        Type at(size_t i, size_t j) const;

        /**
         * @brief Метод необходимый для вычисления определителя.
         * 
         * @return определитель матрицы.
        */
        Type det() const;

        /**
         * @brief Метод необходимый для транспонирования матрицы.
        */
        void transpose();

        void inverse();

    private:
        union
        {
            Type _two_dimensional_array[4][4];;
            Type _linear_array[16];
        };
    };

    /**
     * @brief Функция необходимая для транспонирования матрицы.
     * 
     * @tparam Type тип элементов матрицы.
     * @param mat матрица.
     * @return транспонированная матрица mat.
    */
    template<typename Type>
    Matrix4x4<Type> transpose(const Matrix4x4<Type>& mat);

    /**
     * @brief Функция необходимая для вычисления обратной матрицы.
     * 
     * @tparam Type тип элементов матрицы.
     * @param mat матрица.
     * @return обратная матрица mat.
    */
    template<typename Type>
    Matrix4x4<Type> inverse(const Matrix4x4<Type>& mat);

    template<typename Type>
    ostream& operator << (ostream& print, const Matrix4x4<Type>& mat);
}

#include "matrix4x4.inl"

#endif /* matrix4x4_hpp */
