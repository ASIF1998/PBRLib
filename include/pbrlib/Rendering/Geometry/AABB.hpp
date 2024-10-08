//
//  AABB.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef AABB_hpp
#define AABB_hpp

#include <pbrlib/math/vec3.hpp>

namespace pbrlib
{
    class AABB
    {
    public:
        AABB();
        AABB(const math::Vec3<float>& p);
        AABB(const math::Vec3<float>& p1, const math::Vec3<float>& p2);

        /**
         * @brief 
         *      Перегруженный оператор [] возвращающий минимальную 
         *      или максимальную точку ограничивающего объёма.
         * 
         * @param i индекс точки (i == 0 => p_min, i == 1 => p_max).
         * @return минимальную или максимальную точку ограничивающего объёма.
        */
        math::Vec3<float>& operator [] (size_t i);

        /**
         * @brief 
         *      Перегруженный оператор [] возвращающий минимальную 
         *      или максимальную точку ограничивающего объёма.
         * 
         * @param i индекс точки (i == 0 => p_min, i == 1 => p_max).
         * @return минимальную или максимальную точку ограничивающего объёма.
        */
        math::Vec3<float> operator [] (size_t i) const;

        bool operator == (const AABB& bbox) const;
        bool operator != (const AABB& bbox) const;

        /**
         * @brief Метод, возвращающий один из 8'и углов ограничивающего объёма.
         * 
         * @param c индекс угла.
         * @return угол ограничивающего объёма.
        */
        math::Vec3<float> corner(size_t c) const;

        /**
         * @brief Метод, вычисляющий диагональ ограничивающего объёма.
         * 
         * @return диагональ ограничивающего объёма.
        */
        math::Vec3<float> diagonal() const;

        float surfaceArea() const;
        float volume()      const;

        uint32_t maximumExtent() const;

        math::Vec3<float> lerp(const math::Vec3<float>& t) const;

        /**
         * @brief 
         *      Статический метод, вычисляющий новый ограничивающий объём,
         *      который охватывает как bbox, так и p.
         * 
         * @param bbox  ограничивающий объём.
         * @param p     координаты точки.
         * @return ограничивающий объём, который охватывает как bbox, так и p.
        */
        static AABB aabbUnion(const AABB& bbox, const math::Vec3<float>& p);

        /**
         * @brief 
         *      Статический метод, вычисляющий новый ограничивающий объём,
         *      который охватывает как bbox1, так и bbox2.
         * 
         * @param bbox1 ограничивающий объём.
         * @param bbox2 ограничивающий объём.
         * @return ограничивающий объём, который охватывает как bbox1, так и bbox2.
        */
        static AABB aabbUnion(const AABB& bbox1, const AABB& bbox2);

        /**
         * @brief 
         *      Статический метод, вычисляющий новый ограничивающий объём,
         *      находящийся на пересечении bbox1 и bbox2.
         * 
         * @param bbox1 ограничивающий объём.
         * @param bbox2 ограничивающий объём.
         * @return ограничивающий объём,находящийся на пересечении bbox1 и bbox2.
        */
        static AABB intersect(const AABB& bbox1, const AABB& bbox2);

        /**
         * @brief 
         *      Статический метод, выявляющий, пересекаются ли 
         *      bbox1 и bbox2.
         * 
         * @param bbox1 ограничивающий объём.
         * @param bbox2 ограничивающий объём.
         * @return true - если bbox1 и bbox2 перекрываются, иначе false.
        */
        static bool overlaps(const AABB& bbox1, const AABB& bbox2);

        /**
         * @brief 
         *      Статический метод, выявляющий, находится ли p 
         *      внутри bbox.
         * 
         * @param bbox  ограничивающий объём.
         * @param p     координаты точки.
         * @return true - если p внутри bbox, иначе false.
        */
        static bool inside(const AABB& bbox, const math::Vec3<float>& p);

        /**
         * @brief 
         *      Статический метод, расширяющий bbox постоянным 
         *      коэффициентом delta.
         * 
         * @param bbox  ограничивающий объём.
         * @param delta постоянный коэффициент.
         * @return bbox, только расширенный delta.
        */
        static AABB expand(const AABB& bbox, float delta);

        /**
         * @brief Метод, вычисляющий ограничивающий объём по набору позиций.
         * @details Тип TPos должен иметь только три поля типа float.
         * 
         * @tparam  TPos            тип, описывающий позицию.
         * @param   ptr_positions   указатель на позиции.
         * @param   num_positions   количество позиций.
         * @return Ограничивающий объём.
        */
        template<typename TPos>
        static AABB computeAABB(const TPos* ptr_positions, size_t num_positions);

        /**
         * @brief Метод, вычисляющий ограничивающий объём по набору позиций.
         * @details Объекты находящие в контейнере типа TPositions должны иметь только три поля типа float.
         * 
         * @tparam  TPositions  тип, описывающий контейнер, который хранит позиции (например std::vector<Vec3<float>>).
         * @param   positions   позиции.
         * @return Ограничивающий объём.
        */
        template<typename TPositions>
        static AABB computeAABB(const TPositions& positions);

	private:
        union
        {
            struct
            {
                math::Vec3<float> _p_min;
                math::Vec3<float> _p_max;
            };

            math::Vec3<float> _min_max[2];
        };
	};
}

#include "AABB.inl"

#endif /* AABB_hpp */
