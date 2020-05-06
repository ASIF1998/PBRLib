//
//  PerspectiveCamera.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PerspectiveCamera_hpp
#define PerspectiveCamera_hpp

#include "ICamera.hpp"

#include <memory>

using namespace std;

namespace pbrlib
{
    class PerspectiveCamera;
    using PtrPerspectiveCamera = shared_ptr<PerspectiveCamera>;

    class PerspectiveCamera :
        public ICamera
    {
    public:
        class Builder
        {
        public:
            Builder() = default;

            void setPosition(const Vec3<float>& pos);
            void setEye(const Vec3<float>& eye);
            void setUp(const Vec3<float>& up);

            /**
             * @brief 
             *      Метод позволяющий установить расстояние до ближней 
             *      плоскости отсечения по оси Z.
             * 
             * @param near расстояние до ближней плоскости отсечения по оси Z.
            */
            void setNearClipp(float near) noexcept;

            /**
             * @brief 
             *      Метод позволяющий установить расстояние до дальней 
             *      плоскости отсечения по оси Z.
             * 
             * @param far расстояние до дальней плоскости отсечения по оси Z.
            */
            void setFarClipp(float far) noexcept;

            /**
             * @brief Метод позволяющий установить соотношение сторон.
             * 
             * @param aspect соотношение сторон.
            */
            void setAspect(float aspect) noexcept;

            /**
             * @brief Метод позволяющий установить соотношение сторон.
             * 
             * @param width     ширина.
             * @param height    высота.
            */
            void setAspect(float width, float height);

            /**
             * @brief 
             *      Метод позволяющий установить угол между 
             *      верхней и нижней сторонами усечённого вида.
             * 
             * 
             * @param fovy угол между верхней и нижней сторонами усечённого вида.
            */
            void setFovy(float fovy) noexcept;

            PerspectiveCamera       build()     const;
            PtrPerspectiveCamera    ptrBuild()  const;

        private:
            Vec3<float> _pos;
            Vec3<float> _eye;
            Vec3<float> _up;
            float       _z_near;
            float       _z_far;
            float       _aspect;
            float       _fovy;
        };

    public:
        /**
         * @brief Конструктор.
         * 
         * @param pos           позиция.
         * @param eye           вектор направления взгляда.
         * @param up            вектор, указывающий на вверх.
         * @param near_clipp    расстояние до ближней плоскости отсечения по оси Z.
         * @param far_clipp     расстояние до дальней плоскости отсечения по оси Z.
         * @param aspect        соотношение сторон.
         * @param fovy          угол между верхней и нижней сторонами усечённого вида.
        */
        PerspectiveCamera(
            const Vec3<float>&  pos,
            const Vec3<float>&  eye,
            const Vec3<float>&  up,
            float               near_clipp,
            float               far_clipp,
            float               aspect,
            float               fovy
        );

        float getAspect()   const noexcept;
        float getFovy()     const noexcept;

        void setAspect(float aspect);
        void setAspect(float width, float height);
        void setFovy(float fovy);
        void setAspectAndFovy(float aspect, float fovy);
        void setAspectAndFovy(float width, float height, float fovy);

    private:
        virtual Transform _calculateProjection() const override;
    
    private:
        float _aspect;
        float _fovy;
    };
}

#endif /* PerspectiveCamera_hpp */
