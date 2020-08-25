//
//  PBRPass.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PBRPass_hpp
#define PBRPass_hpp

#include <memory>

#include "../../../SceneGraph/Scene.hpp"

using namespace std;

namespace pbrlib
{
    class ComputePipeline;
    class Device;
    class DescriptorSet;
    class CommandBuffer;
    class ImageView;
    class PBRPass;
    class Sampler;
    class DescriptorPool;
    class Buffer;
    class PhysicalDevice;
    class CameraBase;
    class Scene;

    using PtrComputePipeline    = shared_ptr<ComputePipeline>;
    using PtrDevice             = shared_ptr<Device>;
    using PtrDescriptorSet      = shared_ptr<DescriptorSet>;
    using PtrCommandBuffer      = shared_ptr<CommandBuffer>;
    using PtrImageView          = shared_ptr<ImageView>;
    using PtrPBRPass            = unique_ptr<PBRPass>;
    using PtrSampler            = shared_ptr<Sampler>;
    using PtrDescriptorPool     = shared_ptr<DescriptorPool>;
    using PtrBuffer             = shared_ptr<Buffer>;
    using PtrPhysicalDevice     = shared_ptr<PhysicalDevice>;

    class PBRPass
    {
    public:
        enum class DistributionFunction :
            uint32_t
        {
            Beckmann = 0,
            GGX,
            GGX_Anisotropy
        };

        enum class GeometryFunction :
            uint32_t 
        {
            Beckmann = 0,
            GGX,
            SchlickBeckmann,
            SchlickGGX,
            Implicit,
            Neumann,
            CookTorrance,
            Kelemen,
            Smith
        };

        enum class FresnelApproximation
        {
            None,
            Schlick,
            CookTorrance
        };

        class Optionals
        {
            friend class PBRPass;
            
        public:
            inline Optionals(
                DistributionFunction    distrib_func    = DistributionFunction::GGX,
                GeometryFunction        geom_func       = GeometryFunction::Smith,
                FresnelApproximation    fresnel_approx  = FresnelApproximation::Schlick,
                bool                    use_bent_normal = false
            );

            inline void setDistributionFunction(DistributionFunction dist_func)        noexcept;
            inline void setGeometryFunction(GeometryFunction geom_func)                noexcept;
            inline void setFresnelApproximation(FresnelApproximation fresnel_approx)   noexcept;
            inline void useBentNormal(bool to_use)                                     noexcept;

            inline DistributionFunction     getDistributionFunction()   const noexcept;
            inline GeometryFunction         getGeometryFunction()       const noexcept;
            inline FresnelApproximation     getFresnelApproximation()   const noexcept;
            inline bool                     useBentNormal()             const noexcept;

        private:
            DistributionFunction    _distrib_func;
            GeometryFunction        _geom_func;
            FresnelApproximation    _fresnel_approx;

            union
            {
                struct
                {
                    bool        use_bent_normal :  1;
                    uint32_t    other           :  31;
                };

                uint32_t mask;

            } _other_options;
        };

    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device                        указатель на логическое устройтсво.
         * @param ptr_physical_deviec               укаазетль на физическое устройство.
         * @param queue_family_index                индекс семейства очередей.
         * @param ptr_descriptor_pool               указатель на пул дескрипторов.
         * @param position_and_metallic_image_view  указатель на вид изображения с позициями и металичностью.
         * @param normal_and_roughness_image_view   указатель на вид изображения с нормалями и шероховатостью.
         * @param albedo_and_baked_AO_image_view    указатель на вид изображения с альбедо и запечённым AO.
         * @param anisotropy_image_view             указатель на вид изображения с анизотропностью.
         * @param ptr_sampler                       указатель на сэмплер.
         * @param optionals                         опции рендера.
        */
        PBRPass(
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_deviec,
            uint32_t                    queue_family_index,
            const PtrDescriptorPool&    ptr_descriptor_pool,
            const ImageView&            position_and_metallic_image_view,
            const ImageView&            normal_and_roughness_image_view,
            const ImageView&            albedo_and_baked_AO_image_view,
            const ImageView&            anisotropy_image_view,
            const PtrSampler&           ptr_sampler,
            const Optionals&            optionals = Optionals()
        );

        /**
         * @brief Метод, позволяющий запустить проход физически-корректного рендеринга.
         * 
         * @param camera                 камера.
         * @param ptr_command_buffer     указатель на командный буфер.
         * @param out_image_view         указатель на вид изображения в который будет записан резкльтат прохода.
         * @param point_lights           точечные источники света.
         * @param spot_lights            прожекторные источники света.
         * @param direction_lights       направденные источники света.
        */
        void draw(
            const CameraBase&               camera,
            const PtrCommandBuffer&         ptr_command_buffer,
            const ImageView&                out_image_view,
            const vector<Scene::PtrNode>    point_lights,
            const vector<Scene::PtrNode>    spot_lights,
            const vector<Scene::PtrNode>    direction_lights
        );

        inline PtrComputePipeline&         getPipeline() noexcept;
        inline const PtrComputePipeline&   getPipeline() const noexcept;

        inline static PtrPBRPass make(
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_deviec,
            uint32_t                    queue_family_index,
            const PtrDescriptorPool&    ptr_descriptor_pool,
            const ImageView&            position_and_metallic_image_view,
            const ImageView&            normal_and_roughness_image_view,
            const ImageView&            albedo_and_baked_AO_image_view,
            const ImageView&            anisotropy_image_view,
            const PtrSampler&           ptr_sampler,
            const Optionals&            optionals = Optionals()
        );

    private:
        PtrComputePipeline  _ptr_pipeline;
        PtrDescriptorSet    _ptr_descriptor_set;
        PtrSampler          _ptr_sampler;

        PtrBuffer _ptr_uniform_point_lights_data_buffer;
        PtrBuffer _ptr_uniform_spot_lights_data_buffer;
        PtrBuffer _ptr_uniform_direction_lights_data_buffer;
        PtrBuffer _ptr_uniform_num_lights_buffer;

        PtrBuffer _ptr_uniform_camera_data_buffer;

        union
        {
            struct 
            {
                const ImageView* _ptr_position_and_metallic_image_view;
                const ImageView* _ptr_normal_and_roughness_image_view;
                const ImageView* _ptr_albedo_and_baked_AO_image_view;
                const ImageView* _ptr_anisotropy_image_view;
            };

            const ImageView* _ptr_images_views[4];
        };
    };
}

#include "PBRPass.inl"

#endif /* PBRPass_hpp */