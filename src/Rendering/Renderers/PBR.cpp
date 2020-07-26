//
//  PBR.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/07/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "PBR.hpp"

#include "../VulkanWrapper/PhysicalDevice.hpp"

#include "../VulkanWrapper/DescriptorPool.hpp"
#include "../VulkanWrapper/Framebuffer.hpp"
#include "../VulkanWrapper/Image.hpp"

#include "../VulkanWrapper/CommandBuffer.hpp"

namespace pbrlib
{
    PtrDescriptorPool descriptorPoolCreate(const PtrDevice& ptr_device)
    {
        vector<VkDescriptorPoolSize> descriptor_pool_size;
        
        descriptor_pool_size.push_back({
            .type               = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount    = 1
        });
        
        descriptor_pool_size.push_back({
            .type               = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount    = 8
        });
        
        descriptor_pool_size.push_back({
            .type               = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
            .descriptorCount    = 1
        });
        
        uint32_t max_allocate_sets_count = 2;

        return DescriptorPool::make(ptr_device, descriptor_pool_size, max_allocate_sets_count);
    }

    PtrRenderPass createRenderPass(const PtrSwapchain& ptr_swapchain, const PtrAttachments& image_view)
    {   
        RenderPass::Builder build_render_pass       (5, 2, 1);
        SubpassDescription  subpass_description1    (0, 3, 0);

        auto& image_view_reference = ptr_swapchain->getImagesView()[0];

        subpass_description1.addColorAttachment(1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        subpass_description1.addColorAttachment(2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        subpass_description1.addColorAttachment(3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        subpass_description1.setDepthStencilAttachment(4, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);

        build_render_pass.setDevice(ptr_swapchain->getDevice());
        
        build_render_pass.addAttachmentDescription(
            image_view_reference.getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_GENERAL
        );
        
        build_render_pass.addAttachmentDescription(
            image_view->at(0).getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_CLEAR,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        build_render_pass.addAttachmentDescription(
            image_view->at(1).getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_CLEAR,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        build_render_pass.addAttachmentDescription(
            image_view->at(2).getFormat(),
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_CLEAR,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        );

        //!< Глубина.
        build_render_pass.addAttachmentDescription(
            VK_FORMAT_D32_SFLOAT,
            VK_SAMPLE_COUNT_1_BIT,
            VK_ATTACHMENT_LOAD_OP_CLEAR,
            VK_ATTACHMENT_STORE_OP_STORE,
            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            VK_ATTACHMENT_STORE_OP_DONT_CARE,
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL
        );
    
        build_render_pass.addSubpassDescription(move(subpass_description1));

        return build_render_pass.buildPtr();
    }

    PtrAttachments createFramebufferAttachments(
        const PtrDevice&            ptr_device,
        const PtrPhysicalDevice&    ptr_physical_device,
        uint32_t                    gpu_queue_family_index,
        uint32_t                    width, 
        uint32_t                    height
    )
    {
        PtrAttachments ptr_framebuffer_attachments (new vector<ImageView>());

        Image::Builder<Image::TexelType::RGBA, float, Image::NumBits::NB32>     builder_image;
        ImageView::Builder                                                      builder_image_view;
        Image::Builder<Image::TexelType::Depth, float, Image::NumBits::NB32>    builder_depth_image;

        builder_image.setDevice(ptr_device);
        builder_image.setExtend(width, height, 1);
        builder_image.setImageType(VK_IMAGE_TYPE_2D);
        builder_image.setMemoryTypeIndex(ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
        
        builder_image.setNumArrayLayers(1);
        builder_image.setNumMipLevels(1);
        builder_image.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
        builder_image.setTiling(VK_IMAGE_TILING_OPTIMAL);
        builder_image.setUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT); /// TODO: Убрать всё ненужное
        builder_image.addQueueFamilyIndex(gpu_queue_family_index);

        builder_image_view.setAspectMask(VK_IMAGE_ASPECT_COLOR_BIT);
        builder_image_view.setBaseArrayLayer(0);
        builder_image_view.setBaseMipLevel(0);
        builder_image_view.setFormat(VK_FORMAT_R32G32B32A32_SFLOAT);
        builder_image_view.setImage(builder_image.buildPtr());
        builder_image_view.setLayerCount(1);
        builder_image_view.setLevelCount(1);
        builder_image_view.setType(VK_IMAGE_VIEW_TYPE_2D);

        builder_depth_image.setDevice(ptr_device);
        builder_depth_image.setExtend(width, height, 1);
        builder_depth_image.setImageType(VK_IMAGE_TYPE_2D);
        builder_depth_image.setMemoryTypeIndex(ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
        builder_depth_image.setNumArrayLayers(1);
        builder_depth_image.setNumMipLevels(1);
        builder_depth_image.setNumSamples(VK_SAMPLE_COUNT_1_BIT);
        builder_depth_image.setTiling(VK_IMAGE_TILING_OPTIMAL);
        builder_depth_image.setUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
        builder_depth_image.addQueueFamilyIndex(gpu_queue_family_index);

        ptr_framebuffer_attachments->push_back(builder_image_view.build());
        
        builder_image_view.setImage(builder_image.buildPtr());
        ptr_framebuffer_attachments->push_back(builder_image_view.build());

        builder_image_view.setImage(builder_image.buildPtr());
        ptr_framebuffer_attachments->push_back(builder_image_view.build());
        
        builder_image_view.setImage(builder_depth_image.buildPtr());
        builder_image_view.setFormat(VK_FORMAT_D32_SFLOAT);
        builder_image_view.setAspectMask(VK_IMAGE_ASPECT_DEPTH_BIT);

        ptr_framebuffer_attachments->push_back(builder_image_view.build());
        
        return ptr_framebuffer_attachments;
    }

    vector<PtrFramebuffer> createFramebuffers(
        const PtrSwapchain&     ptr_swapchain,
        const PtrAttachments&   ptr_framebuffer_attachments,
        const PtrRenderPass&    ptr_render_pass,
        uint32_t                width,
        uint32_t                height
    )
    {
        Framebuffer::Builder    build_framebuffer;
        vector<PtrFramebuffer>  framebuffers (ptr_swapchain->getImagesView().size());

        build_framebuffer.setSwapchain(ptr_swapchain);
        build_framebuffer.setAttachments(ptr_framebuffer_attachments);
        build_framebuffer.setRenderPass(ptr_render_pass);
        build_framebuffer.setWidth(width);
        build_framebuffer.setHeight(height);
        build_framebuffer.setNumLayers(1);

        for (uint32_t i{0}, size{static_cast<uint32_t>(ptr_swapchain->getImagesView().size())}; i < size; i++) {
            build_framebuffer.setSwapchainAttachmentIndex(i);
            framebuffers[i] = build_framebuffer.buildPtr();
        }

        return framebuffers;
    }

    PtrSampler createSampler(const PtrDevice& ptr_device)
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
    void PBR::init(
        const PtrWindow&            ptr_window, 
        const PtrDevice&            ptr_device, 
        const PtrPhysicalDevice&    ptr_physical_device
    )
    {
        _ptr_device = ptr_device;

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
        
        uint32_t    gpu_queue_family_index  = _ptr_device->getDeviceQueueInfo()[0].queueFamilyIndex;
        auto        gpu_memory_index        = ptr_physical_device->memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        auto        [width, height]         = ptr_window->getExtent();

        _ptr_swapchain                  = ptr_window->getSwapchain();
        _ptr_device_queue               = DeviceQueue::make(_ptr_device, gpu_queue_family_index, 0); 
        _ptr_descriptor_pool            = descriptorPoolCreate(_ptr_device);
        _ptr_framebuffer_attachments    = createFramebufferAttachments(_ptr_device, ptr_physical_device, gpu_queue_family_index, width, height);
        _ptr_render_pass                = createRenderPass(_ptr_swapchain, _ptr_framebuffer_attachments);
        _ptr_framebuffers               = createFramebuffers(_ptr_swapchain, _ptr_framebuffer_attachments, _ptr_render_pass, width, height);
        _ptr_command_buffer             = PrimaryCommandBuffer::make(CommandPool::make(_ptr_device, gpu_queue_family_index));
        _ptr_sampler_linear             = build_sampler.buildPtr();

        build_sampler.setMagFilter(VK_FILTER_NEAREST);
        build_sampler.setMinFilter(VK_FILTER_NEAREST);      

        _ptr_sampler_nearest = build_sampler.buildPtr();  

        _ptr_gbuffer_pass = GBufferPass::make(
            _ptr_device,
            _ptr_descriptor_pool,
            _ptr_render_pass,
            0,
            width, height,
            gpu_memory_index,
            gpu_queue_family_index
        );

        _ptr_pbr_pass = PBRPass::make(_ptr_device, _ptr_descriptor_pool);
    }

    void PBR::draw(
        const CameraBase&               camera,
        const Scene::VisibleList&       visible_list, 
        const vector<Scene::PtrNode>    point_lights,
        const vector<Scene::PtrNode>    spot_lights,
        const vector<Scene::PtrNode>    direction_lights,
        float                           delta_time
    )
    {
        uint32_t image_index = 0;
        
        VkSemaphore             semaphore_handle    = VK_NULL_HANDLE;
        VkSemaphoreCreateInfo   semaphore_info      = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
        
        assert(vkCreateSemaphore(_ptr_device->getDeviceHandle(), &semaphore_info, nullptr, &semaphore_handle) == VK_SUCCESS);
        
        _ptr_swapchain->getNextPresentImageIndex(image_index, semaphore_handle);
        
        vkDestroySemaphore(_ptr_device->getDeviceHandle(), semaphore_handle, nullptr);

        vector<VkClearValue> clear_values (5);
        
        VkClearValue color_attachment_clear_value   {1.0f, 1.0f, 1.0f, 1.0f};
        VkClearValue depth_clear_value              {1.0f, 1.0f, 1.0f, 1.0f};
        
        clear_values[0] = color_attachment_clear_value;
        clear_values[1] = color_attachment_clear_value;
        clear_values[2] = color_attachment_clear_value;
        clear_values[3] = color_attachment_clear_value;
        clear_values[4] = depth_clear_value;

        _ptr_command_buffer->reset();
        _ptr_command_buffer->begin();

        _ptr_command_buffer->begineRenderPass(_ptr_framebuffers[image_index], clear_values);
        _ptr_command_buffer->bindToPipeline(_ptr_gbuffer_pass->getPipeline());
        _ptr_gbuffer_pass->draw(camera.getProjection(), camera.getView(), visible_list, _ptr_command_buffer, _ptr_sampler_linear);
        _ptr_command_buffer->endRenderPass();

        _ptr_command_buffer->bindToPipeline(_ptr_pbr_pass->getPipeline());

        _ptr_pbr_pass->draw(
            _ptr_command_buffer, 
            _ptr_framebuffers[image_index]->getAttachments()->at(0),
            _ptr_framebuffers[image_index]->getAttachments()->at(1),
            _ptr_framebuffers[image_index]->getAttachments()->at(2),
            _ptr_swapchain->getImagesView()[image_index],
            _ptr_sampler_nearest
        );

        static VkImageSubresourceRange image_subresource_range {
            .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel   = 0,
            .levelCount     = 1,
            .baseArrayLayer = 0,
            .layerCount     = 1
        };

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

    PtrGBufferPass& PBR::getGBUfferPass() noexcept
    {
        return _ptr_gbuffer_pass;
    }

    const PtrGBufferPass& PBR::getGBUfferPass() const noexcept
    {
        return _ptr_gbuffer_pass;
    }

    PtrPBRPass& PBR::getPBRPass() noexcept
    {
        return _ptr_pbr_pass;
    }

    const PtrPBRPass& PBR::getPBRPass() const noexcept
    {
        return _ptr_pbr_pass;
    }
}
