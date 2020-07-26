//
//  PBR.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef PBR_hpp
#define PBR_hpp

#include "IRenderer.h"

#include "SubPasses/GBufferPass.hpp"
#include "SubPasses/PBRPass.hpp"

#include <vector>
#include <memory>

using namespace std;

namespace pbrlib
{
    class ImageView;
    class Framebuffer;
    class PrimaryCommandBuffer;

    using PtrAttachments            = shared_ptr<vector<ImageView>>;
    using PtrFramebuffer            = shared_ptr<Framebuffer>; 
    using PtrPrimaryCommandBuffer   = shared_ptr<PrimaryCommandBuffer>;

    class PBR :
        public IRenderer
    {
    public:
        /**
         * @brief Метод, предназначенный для инициализации.
         * 
         * @param ptr_window            указатель на окно.
         * @param ptr_device            указатель на логическое устройство.
         * @param ptr_physical_device   указатель на физическое устройство.
        */
        virtual void init(
            const PtrWindow&            ptr_window, 
            const PtrDevice&            ptr_device, 
            const PtrPhysicalDevice&    ptr_physical_device
        ) override;

        /**
         * @brief Метод, отвечающий за отрисовку.
         * 
         * @param camera            камера.
         * @param visible_list      видимые узлы.
         * @param point_lights      точечные источники света.
         * @param spot_lights       прожекторные источники света.
         * @param direction_lights  направленные источники света.
         * @param delta_time        количество пройденного времени с момента завершения последнего кадра.
        */
        virtual void draw(
            const CameraBase&               camera,
            const Scene::VisibleList&       visible_list, 
            const vector<Scene::PtrNode>    point_lights,
            const vector<Scene::PtrNode>    spot_lights,
            const vector<Scene::PtrNode>    direction_lights,
            float                           delta_time
        ) override;

        PtrGBufferPass&          getGBUfferPass()    noexcept;
        const PtrGBufferPass&    getGBUfferPass()    const noexcept;
        PtrPBRPass&              getPBRPass()        noexcept;
        const PtrPBRPass&        getPBRPass()        const noexcept;

    private:
        PtrGBufferPass  _ptr_gbuffer_pass;
        PtrPBRPass      _ptr_pbr_pass;

        PtrDevice _ptr_device;

        PtrDescriptorPool _ptr_descriptor_pool;

        PtrRenderPass           _ptr_render_pass;
        PtrAttachments          _ptr_framebuffer_attachments;
        vector<PtrFramebuffer>  _ptr_framebuffers;
        PtrSwapchain            _ptr_swapchain;
        PtrSampler              _ptr_sampler_linear;
        PtrSampler              _ptr_sampler_nearest;

        PtrPrimaryCommandBuffer _ptr_command_buffer;
        PtrDeviceQueue          _ptr_device_queue;
    };
}

#endif /* PBR_hpp */
