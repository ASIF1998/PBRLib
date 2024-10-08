//
//  PBR.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Renderers/PBR.hpp>

#include <pbrlib/Rendering/VulkanWrapper/PhysicalDevice.hpp>

#include <pbrlib/Rendering/VulkanWrapper/DescriptorPool.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Framebuffer.hpp>
#include <pbrlib/Rendering/VulkanWrapper/Image.hpp>

#include <pbrlib/Rendering/VulkanWrapper/CommandBuffer.hpp>

namespace pbrlib
{
    enum class NumDescriptors :
        uint32_t
    {
        UniformBuffer           = 7,
        CombinedImageSampler    = 9,
        StorageImage            = 1
    };

    std::shared_ptr<DescriptorPool> descriptorPoolCreate(const Device* ptr_device)
    {
        std::vector<VkDescriptorPoolSize> descriptor_pool_size;
        
        descriptor_pool_size.push_back({
            VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            utils::enumCast(NumDescriptors::UniformBuffer)
        });
        
        descriptor_pool_size.push_back({
            VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            utils::enumCast(NumDescriptors::CombinedImageSampler)
        });
        
        descriptor_pool_size.push_back({
            VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
            utils::enumCast(NumDescriptors::StorageImage)
        });
        
        uint32_t max_allocate_sets_count = 2;

        return DescriptorPool::make(ptr_device, descriptor_pool_size, max_allocate_sets_count);
    }

    std::unique_ptr<Sampler> createSampler(const Device* ptr_device)
    {
        Sampler::Builder build_sampler;

        build_sampler.setDevice(ptr_device);
        build_sampler.anisotropyEnable(VK_FALSE);
        build_sampler.compareEnable(VK_FALSE);
        build_sampler.setAdressMode();
        build_sampler.setBorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK);
        build_sampler.setLodRange(0.0f, 0.0f);
        build_sampler.setMagFilter(VK_FILTER_LINEAR);
        build_sampler.setMinFilter(VK_FILTER_LINEAR);
        build_sampler.setMipLodBias(0.0f);
        build_sampler.setMipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR);
        build_sampler.unnormalizedCoordinates(VK_FALSE);

        return build_sampler.buildPtr();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PBR::PBR(const PBRPass::Optionals& optionals) :
        _optionals(optionals)
    {}

    void PBR::init(
        std::shared_ptr<const Window>   ptr_window,
        const Device*                   ptr_device,
        const PhysicalDevice*           ptr_physical_device
    )
    {
        _ptr_device = ptr_device;

        Sampler::Builder build_sampler;

        GBufferPass::Builder    gbuffer_builder;
        PBRPass::Builder        pbr_pass_builder;

        build_sampler.setDevice(ptr_device);
        build_sampler.anisotropyEnable(VK_FALSE);
        build_sampler.compareEnable(VK_FALSE);
        build_sampler.setAdressMode();
        build_sampler.setBorderColor(VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK);
        build_sampler.setLodRange(0.0f, 0.0f);
        build_sampler.setMagFilter(VK_FILTER_LINEAR);
        build_sampler.setMinFilter(VK_FILTER_LINEAR);
        build_sampler.setMipLodBias(0.0f);
        build_sampler.setMipmapMode(VK_SAMPLER_MIPMAP_MODE_LINEAR);
        build_sampler.unnormalizedCoordinates(VK_FALSE);
        
        uint32_t    gpu_queue_family_index  = _ptr_device->getDeviceQueueInfo()[0].queueFamilyIndex;
        auto        gpu_memory_index        = ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        auto        [width, height]         = ptr_window->getExtent();

        _ptr_swapchain          = ptr_window->getSwapchain();
        _ptr_device_queue       = DeviceQueue::make(_ptr_device, gpu_queue_family_index, 0);
        _ptr_descriptor_pool    = descriptorPoolCreate(_ptr_device);
        _ptr_command_pool       = CommandPool::make(_ptr_device, gpu_queue_family_index);
        _ptr_command_buffer     = PrimaryCommandBuffer::make(_ptr_command_pool.get());
        _ptr_sampler_linear     = build_sampler.buildPtr();

        build_sampler.setMagFilter(VK_FILTER_NEAREST);
        build_sampler.setMinFilter(VK_FILTER_NEAREST);

        _ptr_sampler_nearest = build_sampler.buildPtr();

        gbuffer_builder.setDevice(_ptr_device);
        gbuffer_builder.setPhysicalDevice(ptr_physical_device);
        gbuffer_builder.setDescriptorPool(_ptr_descriptor_pool);
        gbuffer_builder.setDeviceMemoryIndex(gpu_memory_index);
        gbuffer_builder.windowSize(width, height);
        gbuffer_builder.setQueue(_ptr_device_queue);
        
        _ptr_gbuffer_pass = gbuffer_builder.buildPtr();

        pbr_pass_builder.setDevice(_ptr_device);
        pbr_pass_builder.setPhysicalDevice(ptr_physical_device);
        pbr_pass_builder.setDescriptorPool(_ptr_descriptor_pool);
        pbr_pass_builder.setQueue(_ptr_device_queue);
        pbr_pass_builder.setPositionAndMetallicImageView(&_ptr_gbuffer_pass->output(GBufferPass::OutputImagesViewsIDs::PositionAndMetallic));
        pbr_pass_builder.setNormalAndRoughnessImageView(&_ptr_gbuffer_pass->output(GBufferPass::OutputImagesViewsIDs::NormalAndRoughness));
        pbr_pass_builder.setAlbedoAndBakedAOImageView(&_ptr_gbuffer_pass->output(GBufferPass::OutputImagesViewsIDs::AlbedoAndBakedAO));
        pbr_pass_builder.setAnisotropyImageView(&_ptr_gbuffer_pass->output(GBufferPass::OutputImagesViewsIDs::TangentAndAnisotropy));
        pbr_pass_builder.setQueue(_ptr_device_queue);
        pbr_pass_builder.setSampler(_ptr_sampler_nearest);
        pbr_pass_builder.setOptionals(_optionals);

        _ptr_pbr_pass = pbr_pass_builder.buildPtr();
    }

    void PBR::draw(
        std::shared_ptr<const SceneItem>            ptr_camera,
        const VisibleList&                          visible_list, 
        std::span<std::shared_ptr<SceneItem>>       point_lights,
        std::span<std::shared_ptr<SceneItem>>       spot_lights,
        std::span<std::shared_ptr<SceneItem>>       direction_lights,
        float                                       delta_time
    )
    {
        if (!ptr_camera->hasComponent<CameraBase>())
            throw std::runtime_error("Нет камеры.");

        const auto& camera = ptr_camera->getComponent<CameraBase>();

        uint32_t image_index = 0;
        
        VkSemaphore             semaphore_handle    = VK_NULL_HANDLE;
        VkSemaphoreCreateInfo   semaphore_info      = { };
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        
        assert(vkCreateSemaphore(_ptr_device->getDeviceHandle(), &semaphore_info, nullptr, &semaphore_handle) == VK_SUCCESS);
        
        _ptr_swapchain->getNextPresentImageIndex(image_index, semaphore_handle);
        
        vkDestroySemaphore(_ptr_device->getDeviceHandle(), semaphore_handle, nullptr);

        _ptr_gbuffer_pass->draw(camera.getProjection(), camera.getView(), visible_list, _ptr_command_buffer, _ptr_sampler_linear);

        _ptr_command_buffer->reset();
        _ptr_command_buffer->begin();

        static VkImageSubresourceRange image_subresource_range {
            VK_IMAGE_ASPECT_COLOR_BIT,
            0,
            1,
            0,
            1
        };

        _ptr_command_buffer->imageMemoryBarrier(
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_GENERAL,
            _ptr_device_queue->getFamilyIndex(),
            _ptr_device_queue->getFamilyIndex(),
            *_ptr_swapchain->getImagesView()[image_index].getImage(),
            image_subresource_range
        );
        
        _ptr_command_buffer->end();
        
        _ptr_device_queue->submit(*_ptr_command_buffer);
        _ptr_device_queue->waitIdle();
        
        _ptr_pbr_pass->output(_ptr_swapchain->getImagesView()[image_index], PBRPass::OutputImagesViewsIDs::Result);

        _ptr_pbr_pass->draw(
            ptr_camera,
            _ptr_command_buffer,
            point_lights,
            spot_lights,
            direction_lights
        );

        _ptr_command_buffer->reset();
        _ptr_command_buffer->begin();

        _ptr_command_buffer->imageMemoryBarrier(
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_ACCESS_SHADER_WRITE_BIT,
            VK_IMAGE_LAYOUT_GENERAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            _ptr_device_queue->getFamilyIndex(),
            _ptr_device_queue->getFamilyIndex(),
            *_ptr_swapchain->getImagesView()[image_index].getImage(),
            image_subresource_range
        );

        _ptr_command_buffer->end();

        _ptr_device_queue->submit(*_ptr_command_buffer);
        _ptr_device_queue->setPresent(_ptr_swapchain, image_index);
        
        _ptr_device_queue->waitIdle();
    }

    const GBufferPass* PBR::getGBUfferPass() const noexcept
    {
        return _ptr_gbuffer_pass.get();
    }

    const PBRPass* PBR::getPBRPass() const noexcept
    {
        return _ptr_pbr_pass.get();
    }

    std::unique_ptr<PBR> PBR::make(const PBRPass::Optionals& optionals)
    {
        return std::make_unique<PBR>(optionals);
    }
}
