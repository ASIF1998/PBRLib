//
//  matrix2x2.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 30/01/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <memory>

namespace pbrlib::math
{
    template<typename Type>
    Matrix2x2<Type>::Matrix2x2(Type init_value)
    {
        for (size_t i{0}; i < 4; i++) {
            _array4[i] = init_value;
        }
    }

    template<typename Type>
    inline Matrix2x2<Type>::Matrix2x2(Type v11, Type v12, Type v21, Type v22)
    {
        _array2x2[0][0] = v11;
        _array2x2[0][1] = v12;

        _array2x2[1][0] = v21;
        _array2x2[1][1] = v22;
    }

    template<typename Type>
    inline Matrix2x2<Type>::Matrix2x2(const Type* ptr_data)
    {
        assert(ptr_data);
        memcpy(_array4, ptr_data, sizeof(Type) * 4);
    }

    template<typename Type>
    inline bool Matrix2x2<Type>::operator == (const Matrix2x2<Type>& mat) const
    {
        return  _array4[0] == mat._array4[0] &&
                _array4[1] == mat._array4[1] &&
                _array4[2] == mat._array4[2] &&
                _array4[3] == mat._array4[3];
    }

    template<typename Type>
    inline bool Matrix2x2<Type>::operator != (const Matrix2x2<Type>& mat) const
    {
        return  _array4[0] != mat._array4[0] ||
                _array4[1] != mat._array4[1] ||
                _array4[2] != mat._array4[2] ||
                _array4[3] != mat._array4[3];
    }

    template<typename Type>
    inline Matrix2x2<Type> Matrix2x2<Type>::operator + (const Matrix2x2<Type>& mat) const
    {
        return {
            _array4[0] + mat._array4[0],
            _array4[1] + mat._array4[1],
            _array4[2] + mat._array4[2],
            _array4[3] + mat._array4[3]
        };
    }

    template<typename Type>
    inline Matrix2x2<Type> Matrix2x2<Type>::operator - (const Matrix2x2<Type>& mat) const
    {
        return {
            _array4[0] - mat._array4[0],
            _array4[1] - mat._array4[1],
            _array4[2] - mat._array4[2],
            _array4[3] - mat._array4[3]
        };
    }

    template<typename Type>
    inline Matrix2x2<Type> Matrix2x2<Type>::operator * (const Matrix2x2<Type>& mat) const
    {
        return {
            _array2x2[0][0] * mat._array2x2[0][0] + _array2x2[0][1] * mat._array2x2[1][0],
            _array2x2[0][0] * mat._array2x2[0][1] + _array2x2[0][1] * mat._array2x2[1][1],
            _array2x2[1][0] * mat._array2x2[0][0] + _array2x2[1][1] * mat._array2x2[1][0],
            _array2x2[1][0] * mat._array2x2[0][1] + _array2x2[1][1] * mat._array2x2[1][1]
        };
    }

    template<typename Type>
    inline Matrix2x2<Type> Matrix2x2<Type>::operator * (Type scal) const
    {
        return {
            _array4[0] * scal,
            _array4[1] * scal,
            _array4[2] * scal,
            _array4[3] * scal
        };
    }

    template<typename Type>
    inline Vec2<Type> Matrix2x2<Type>::operator * (const Vec2<Type>& v) const
    {
        return {
            _array2x2[0][0] * v[0] + _array2x2[0][1] * v[1],
            _array2x2[1][0] * v[0] + _array2x2[1][1] * v[1]
        };
    }

    template<typename Type>
    Matrix2x2<Type>& Matrix2x2<Type>::operator += (const Matrix2x2<Type>& mat)
    {
        _array4[0] += mat._array4[0];
        _array4[1] += mat._array4[1];
        _array4[2] += mat._array4[2];
        _array4[3] += mat._array4[3];

        return *this;
    }

    template<typename Type>
    Matrix2x2<Type>& Matrix2x2<Type>::operator -= (const Matrix2x2<Type>& mat)
    {
        _array4[0] -= mat._array4[0];
        _array4[1] -= mat._array4[1];
        _array4[2] -= mat._array4[2];
        _array4[3] -= mat._array4[3];

        return *this;
    }

    template<typename Type>
    Matrix2x2<Type>& Matrix2x2<Type>::operator *= (const Matrix2x2<Type>& mat)
    {
        auto temp (*this);

        _array4[0] = temp._array2x2[0][0] * mat._array2x2[0][0] + temp._array2x2[0][1] * mat._array2x2[1][0];
        _array4[1] = temp._array2x2[0][0] * mat._array2x2[0][1] + temp._array2x2[0][1] * mat._array2x2[1][1];
        _array4[2] = temp._array2x2[1][0] * mat._array2x2[0][0] + temp._array2x2[1][1] * mat._array2x2[1][0];
        _array4[3] = temp._array2x2[1][0] * mat._array2x2[0][1] + temp._array2x2[1][1] * mat._array2x2[1][1];

        return *this;
    }

    template<typename Type>
    Matrix2x2<Type>& Matrix2x2<Type>::operator *= (Type scal)
    {
        _array4[0] *= scal;
        _array4[1] *= scal;
        _array4[2] *= scal;
        _array4[3] *= scal;

        return *this;
    }

    template<typename Type>
    inline Type* Matrix2x2<Type>::operator [] (size_t i)
    {
        return _array2x2[i];
    }

    template<typename Type>
    inline const Type* Matrix2x2<Type>::operator [] (size_t i) const
    {
        return _array2x2[i];
    }

    template<typename Type>
    Type& Matrix2x2<Type>::at(size_t i, size_t j)
    {
        assert(i < 2 && j < 2);
        return _array2x2[i][j];
    }

    template<typename Type>
    Type Matrix2x2<Type>::at(size_t i, size_t j) const
    {
        assert(i < 2 && j < 2);
        return _array2x2[i][j];
    }

    template<typename Type>
    inline Type Matrix2x2<Type>::det() const
    {
        return _array2x2[0][0] * _array2x2[1][1] - _array2x2[1][0] * _array2x2[0][1];
    }

    template<typename Type>
    void Matrix2x2<Type>::transpose()
    {
        auto temp = _array2x2[1][0];
        _array2x2[1][0] = _array2x2[0][1];
        _array2x2[0][1] = temp;
    }

    template<typename Type>
    void Matrix2x2<Type>::inverse()
    {
        Type d = det();

        if (d != Type(0)) {
            Matrix2x2<Type> adj = {
                _array2x2[1][1],  
                -_array2x2[0][1],
                -_array2x2[1][0], 
                _array2x2[0][0]
            };

            *this = adj * (Type(1) / d);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Matrix2x2<float>::Matrix2x2(float init_value) noexcept
    {
        for (size_t i{0}; i < 4; i++) {
            _array4[i] = init_value;
        }
    }

    inline Matrix2x2<float>::Matrix2x2(float v11, float v12, float v21, float v22) noexcept
    {
        _array2x2[0][0] = v11;
        _array2x2[0][1] = v12;

        _array2x2[1][0] = v21;
        _array2x2[1][1] = v22;
    }

    inline Matrix2x2<float>::Matrix2x2(const __m128& init_vec) noexcept :
        _m128_simd(init_vec)
    {}

    inline Matrix2x2<float>::Matrix2x2(const float* ptr_data) 
    {
        assert(ptr_data);
        _m128_simd = _mm_load_ps(ptr_data);
    }

    inline bool Matrix2x2<float>::operator == (const Matrix2x2& mat) const
    {
        return _mm_movemask_ps(_mm_cmpeq_ps(_m128_simd, mat._m128_simd)) == 15;
    }

    inline bool Matrix2x2<float>::operator != (const Matrix2x2& mat) const
    {
        return _mm_movemask_ps(_mm_cmpeq_ps(_m128_simd, mat._m128_simd)) != 15;
    }

    inline Matrix2x2<float> Matrix2x2<float>::operator + (const Matrix2x2<float>& mat) const
    {
        return (_mm_add_ps(_m128_simd, mat._m128_simd));
    }

    inline Matrix2x2<float> Matrix2x2<float>::operator - (const Matrix2x2<float>& mat) const
    {
        return (_mm_sub_ps(_m128_simd, mat._m128_simd));
    }

    inline Matrix2x2<float> Matrix2x2<float>::operator * (const Matrix2x2<float>& mat) const
    {
        auto m1 = _mm_mul_ps(
                        _mm_shuffle_ps(
                            _m128_simd, 
                            _m128_simd, 
                            _MM_SHUFFLE(2, 2, 0, 0)), 
                        _mm_shuffle_ps(
                            mat._m128_simd, 
                            mat._m128_simd, 
                            _MM_SHUFFLE(1, 0, 1, 0)));

        
        auto m2 = _mm_mul_ps(
                        _mm_shuffle_ps(
                            _m128_simd, 
                            _m128_simd, 
                            _MM_SHUFFLE(3, 3, 1, 1)), 
                        _mm_shuffle_ps(
                            mat._m128_simd, 
                            mat._m128_simd, 
                            _MM_SHUFFLE(3, 2, 3, 2)));

        return (_mm_add_ps(m1, m2));
    }

    inline Matrix2x2<float> Matrix2x2<float>::operator * (float scal) const
    {
        return (_mm_mul_ps(_m128_simd, _mm_set1_ps(scal)));
    }

    inline Vec2<float> Matrix2x2<float>::operator * (const Vec2<float>& v) const
    {
        return {
            _array2x2[0][0] * v[0] + _array2x2[0][1] * v[1],
            _array2x2[1][0] * v[0] + _array2x2[1][1] * v[1]
        };
    }

    inline Matrix2x2<float>& Matrix2x2<float>::operator += (const Matrix2x2& mat)
    {
        _m128_simd = _mm_add_ps(_m128_simd, mat._m128_simd);
        return *this;
    }

    inline Matrix2x2<float>& Matrix2x2<float>::operator -= (const Matrix2x2& mat)
    {
        _m128_simd = _mm_sub_ps(_m128_simd, mat._m128_simd);
        return *this;
    }

    inline Matrix2x2<float>& Matrix2x2<float>::operator *= (const Matrix2x2& mat)
    {
        auto m1 = _mm_mul_ps(
                        _mm_shuffle_ps(
                            _m128_simd, 
                            _m128_simd, 
                            _MM_SHUFFLE(2, 2, 0, 0)), 
                        _mm_shuffle_ps(
                            mat._m128_simd, 
                            mat._m128_simd, 
                            _MM_SHUFFLE(1, 0, 1, 0)));

        
        auto m2 = _mm_mul_ps(
                        _mm_shuffle_ps(
                            _m128_simd, 
                            _m128_simd, 
                            _MM_SHUFFLE(3, 3, 1, 1)), 
                        _mm_shuffle_ps(
                            mat._m128_simd, 
                            mat._m128_simd, 
                            _MM_SHUFFLE(3, 2, 3, 2)));

        _m128_simd = _mm_add_ps(m1, m2);

        return *this;
    }

    inline Matrix2x2<float>& Matrix2x2<float>::operator *= (float scal)
    {
        _m128_simd = _mm_mul_ps(_m128_simd, _mm_set1_ps(scal));
        return *this;
    }

    inline float* Matrix2x2<float>::operator [] (size_t i)
    {
        return _array2x2[i];
    }
    
    inline const float* Matrix2x2<float>::operator [] (size_t i) const
    {
        return _array2x2[i];
    }

    inline float& Matrix2x2<float>::at(size_t i, size_t j)
    {
        assert(i < 2 && j < 2);
        return _array2x2[i][j];
    }

    inline float Matrix2x2<float>::at(size_t i, size_t j) const
    {
        assert(i < 2 && j < 2);
        return _array2x2[i][j];
    }

    inline float Matrix2x2<float>::det() const
    {
        return _array2x2[0][0] * _array2x2[1][1] - _array2x2[1][0] * _array2x2[0][1];
    }

    void Matrix2x2<float>::transpose()
    {
        float temp = _array2x2[1][0];
        _array2x2[1][0] = _array2x2[0][1];
        _array2x2[0][1] = temp;
    }

    void Matrix2x2<float>::inverse()
    {
        float d = det();

        if (d != float(0u)) {
            _m128_simd = _mm_mul_ps(
                                _mm_mul_ps(
                                    _mm_shuffle_ps(
                                        _m128_simd, 
                                        _m128_simd, 
                                        _MM_SHUFFLE(0, 2, 1, 3)), 
                                    _mm_setr_ps(1.0f, -1.0f, -1.0f, 1.0f)), 
                                    _mm_set1_ps(1.0f / d));
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template<typename Type>
    inline Matrix2x2<Type> transpose(const Matrix2x2<Type>& mat)
    {
        return {
            mat[0][0], mat[1][0],
            mat[0][1], mat[1][1]
        };
    }

    template<typename Type>
    Matrix2x2<Type> inverse(const Matrix2x2<Type>& mat)
    {
        Matrix2x2<Type> res (mat);
        res.inverse();
        return res;
    }

    template<typename Type>
    ostream& operator << (ostream& print, const Matrix2x2<Type>& mat)
    {
        for (size_t i{0}; i < 2; i++) {
            print << mat[i][0] << ' ' << mat[i][1] << endl;
        }

        return print;
    }
}