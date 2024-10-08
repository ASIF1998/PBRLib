//
//  Swapchain.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 09/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/Swapchain.hpp>

#include <pbrlib/Rendering/VulkanWrapper/DeviceQueue.hpp>

#include <pbrlib/Rendering/VulkanWrapper/Image.hpp>

namespace pbrlib
{
    Swapchain::Swapchain(
        const Device*                   ptr_device,
        std::span<const uint32_t>       queue_family_indices,
        std::shared_ptr<const Surface>  surface
    ) :
        _swapchain_handle(VK_NULL_HANDLE),
        _ptr_surface(surface)
    {
        _create(ptr_device, queue_family_indices, VK_SHARING_MODE_CONCURRENT);
    }

    Swapchain::Swapchain(
        const Device*                   ptr_device,
        uint32_t                        queue_family_index,
        std::shared_ptr<const Surface>  surface
    ) :
        _swapchain_handle   (VK_NULL_HANDLE),
        _ptr_surface        (surface)
    {
        std::vector<uint32_t> queue_family_indicies {queue_family_index};
        _create(ptr_device, queue_family_indicies, VK_SHARING_MODE_EXCLUSIVE);
    }

    Swapchain::Swapchain(Swapchain&& swapchain) :
        _swapchain_handle   (VK_NULL_HANDLE),
        _ptr_surface        (move(swapchain._ptr_surface)),
        _images_view        (move(swapchain._images_view))
    {
        std::swap(_swapchain_handle, swapchain._swapchain_handle);
    }

    Swapchain::~Swapchain()
    {
        if (_swapchain_handle != VK_NULL_HANDLE) 
        {
            for (size_t i{0}; i < _images_view.size(); i++) 
                vkDestroyImageView(_images_view[i].getImage()->getDevice()->getDeviceHandle(), _images_view[i]._image_view_handle, nullptr);
            
            vkDestroySwapchainKHR(_images_view[0]._ptr_image->getDevice()->getDeviceHandle(), _swapchain_handle, nullptr);
        }
    }

    void Swapchain::_create(
        const Device*               ptr_device, 
        std::span<const uint32_t>   queue_family_indices,
        VkSharingMode               sharing_mode
    )
    {
        VkSwapchainCreateInfoKHR swapchain_info = { };
        swapchain_info.sType                    = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_info.surface                  = _ptr_surface->getSurfaceHandle();
        swapchain_info.minImageCount            = _ptr_surface->getSurfaceCapabilities().maxImageCount;
        swapchain_info.imageFormat              = _ptr_surface->getSurfaceFormat().format;
        swapchain_info.imageColorSpace          = _ptr_surface->getSurfaceFormat().colorSpace;
        swapchain_info.imageExtent              = _ptr_surface->getSurfaceCapabilities().currentExtent;
        swapchain_info.imageArrayLayers         = 1;
        swapchain_info.imageUsage               = _ptr_surface->getSurfaceCapabilities().supportedUsageFlags;
        swapchain_info.imageSharingMode         = sharing_mode;
        swapchain_info.queueFamilyIndexCount    = static_cast<uint32_t>(queue_family_indices.size());
        swapchain_info.pQueueFamilyIndices      = queue_family_indices.data();
        swapchain_info.preTransform             = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        swapchain_info.compositeAlpha           = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_info.presentMode              = VK_PRESENT_MODE_FIFO_KHR;
        swapchain_info.clipped                  = VK_TRUE;
        swapchain_info.oldSwapchain             = VK_NULL_HANDLE;

        ImageInfo image_info = { };
        image_info.image_type       = VK_IMAGE_TYPE_2D;
        image_info.format           = swapchain_info.imageFormat;
        image_info.usage            = swapchain_info.imageUsage;
        image_info.num_samples      = VK_SAMPLE_COUNT_1_BIT;
        image_info.tiling           = VK_IMAGE_TILING_LINEAR;
        image_info.mip_levels       = 1;
        image_info.image_extend     = {swapchain_info.imageExtent.width, swapchain_info.imageExtent.height, 1};

        VkImageSubresourceRange image_subresource_range = { };
        image_subresource_range.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
        image_subresource_range.baseMipLevel    = 0;
        image_subresource_range.levelCount      = 1;
        image_subresource_range.baseArrayLayer  = 0;
        image_subresource_range.layerCount      = 1;

        assert(vkCreateSwapchainKHR(
            ptr_device->getDeviceHandle(), 
            &swapchain_info, 
            nullptr, 
            &_swapchain_handle
        ) == VK_SUCCESS);

        assert(_swapchain_handle != VK_NULL_HANDLE);

        uint32_t num_images = 0;

        vkGetSwapchainImagesKHR(ptr_device->getDeviceHandle(), _swapchain_handle, &num_images, nullptr);
        assert(num_images);

        std::vector<VkImage>                vk_images (num_images);
        std::vector<std::shared_ptr<Image>> ptr_images;

        vkGetSwapchainImagesKHR(ptr_device->getDeviceHandle(), _swapchain_handle, &num_images, vk_images.data());
        
        ptr_images.reserve(num_images);
        _images_view.reserve(num_images);

        for (size_t i{0}; i < vk_images.size(); i++) {
            ptr_images.push_back(Image::make(ptr_device, vk_images[i], image_info, queue_family_indices));
        }

        for (size_t i{0}; i < ptr_images.size(); i++) {
            _images_view.push_back(ImageView(ptr_images[i], swapchain_info.imageFormat, image_subresource_range, VK_IMAGE_VIEW_TYPE_2D));
        }
    }

    std::vector<ImageView>& Swapchain::getImagesView() noexcept
    {
        return _images_view;
    }


    const std::vector<ImageView>& Swapchain::getImagesView() const noexcept
    {
        return _images_view;
    }

    void Swapchain::getNextPresentImageIndex(uint32_t& image_index, VkSemaphore semaphore, VkFence fence) const
    {
        VkDevice device_handle = _images_view[0].getImage()->getDevice()->getDeviceHandle();

        vkAcquireNextImageKHR(device_handle, _swapchain_handle, std::numeric_limits<uint64_t>::max(), semaphore, fence, &image_index);
    }

    const VkSwapchainKHR& Swapchain::getSwapchainHandle() const noexcept
    {
        return _swapchain_handle;
    }

    std::shared_ptr<const Surface> Swapchain::getSurface() const noexcept
    {
        return _ptr_surface;
    }

    const Device* Swapchain::getDevice() const noexcept
    {
        return _images_view[0].getImage()->getDevice();
    }

    std::unique_ptr<Swapchain> Swapchain::make(
        const Device*                   ptr_device,
        std::span<const uint32_t>       queue_family_indices,
        std::shared_ptr<const Surface>  surface
    )
    {
        return make_unique<Swapchain>(ptr_device, queue_family_indices, surface);
    }

    std::unique_ptr<Swapchain> Swapchain::make(
        const Device*                   ptr_device,
        uint32_t                        queue_family_index,
        std::shared_ptr<const Surface>  surface
    )
    {
        return make_unique<Swapchain>(ptr_device, queue_family_index, surface);
    }
}
