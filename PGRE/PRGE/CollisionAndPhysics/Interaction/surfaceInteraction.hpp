//
//  surfaceInteraction.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 16/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef surfaceInteraction_hpp
#define surfaceInteraction_hpp

#include "interaction.hpp"

namespace PRGE
{
    /**
     * Данная структура хранит экземпляр нормали поверхности и различные частные
     * производные для представления возможных возмущённых значений этих величин,
     * которые могут быть получены путём рельефного отображения или интерполированных
     * нормалей на каждую вершину с треугольниками.
    */
    struct Shading
    {
        Normal3f n;
        Vec3<float> dpdu;
        Vec3<float> dpdv;
        Normal3f dndu;
        Normal3f dndv;
    };

    /**
     * Данный класс представляет геометрия на конткретной поверхности.
     * 
     * Наличие такой абстакции позваляет большей части системы работать
     * с точками на поверхностях без необходимости учитывать конкретный
     * тип геометрии формы, на которой лежит точка.
     * 
     * Данная абстакция предоставляет достаточно информации о поверзности
     * для вычесления света и выполения каких либо геометрических преобразований.
     * 
     * @firld uv координата параметризации поверхности
     * @field dpdu параметрическая частная производная точки
     * @field dpdv параметрическая частная производная точки
     * @field dndu параметрическая частная производная нормали
     * @field dndv параметрическая частная производная нормали
     * @field shape указатель на поверхность, на котрой находится точка
    */
    struct SurfaceInteraction :
        public Interaction
    {
        SurfaceInteraction() NOEXCEPT_PRGE;

        /**
         * Конструктор.
         * 
         * @param p точка, связаная с взаимодействием
         * @param uv координата параметризации поверхности
         * @param wo отрицательное направление луча
         * @param dpdu параметрическая частная производная точки
         * @param dpdv параметрическая частная производная точки
         * @param dndu параметрическая частная производная нормали
         * @param dndv параметрическая частная производная нормали
         * @param time время связанное с взаимодействием
        */
        inline SurfaceInteraction(const Point3<float>& p, 
                                  const Point2<float>& uv, 
                                  const Vec3<float>& wo, 
                                  const Vec3<float>& dpdu, 
                                  const Vec3<float>& dpdv, 
                                  const Normal3f& dndu, 
                                  const Normal3f& dndv, 
                                  float time) NOEXCEPT_PRGE;

        /**
         * Метод, позволяющий установить значения для шейдинга.
         * 
         * @param dpdu параметрическая частная производная точки
         * @param dpdv параметрическая частная производная точки
         * @param dndu параметрическая частная производная нормали
         * @param dndv параметрическая частная производная нормали
         * @param orientationIsAuthoritative true - геометрическая нормаль будет в одном полушарии с нормалью затенения, false - все наоборот
        */
        void setShadingGeometry(const Vec3<float>& dpdu, 
                                const Vec3<float>& dpdv, 
                                const Normal3f& dndu, 
                                const Normal3f& dndv, 
                                bool orientationIsAuthoritative) NOEXCEPT_PRGE;

        Point2<float> uv;
        Vec3<float> dpdu;
        Vec3<float> dpdv;
        // const Shape* shape
        Normal3f dndu;
        Normal3f dndv;
        Shading shading;
        mutable Vec3<float> dpdx;
        mutable Vec3<float> dpdy;
        mutable float dudx;
        mutable float dvdx;
        mutable float dudy;
        mutable float dvdy;
    };
}

#endif /* surfaceInteraction_hpp */
