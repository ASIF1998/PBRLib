//
//  CommandPool.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 04/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/CommandPool.hpp>

#include <cassert>

namespace pbrlib
{
    CommandPool::CommandPool(const Device* ptr_device, uint32_t queue_family_index) :
        _ptr_device         (ptr_device),
        _command_pool_handle(VK_NULL_HANDLE),
        _queue_family_index (queue_family_index)
    {
        VkCommandPoolCreateInfo command_pool_info = { };
        command_pool_info.sType             = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        command_pool_info.flags             = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        command_pool_info.queueFamilyIndex  = _queue_family_index;
        
        assert(vkCreateCommandPool(
            _ptr_device->getDeviceHandle(), 
            &command_pool_info, 
            nullptr, 
            &_command_pool_handle
        ) == VK_SUCCESS);
        
        assert(_command_pool_handle != VK_NULL_HANDLE);
    }

    CommandPool::CommandPool(CommandPool&& command_pool) :
        _ptr_device         (command_pool._ptr_device),
        _command_pool_handle(VK_NULL_HANDLE),
        _queue_family_index (command_pool._queue_family_index)
    {
        std::swap(_command_pool_handle, command_pool._command_pool_handle);
    }

    CommandPool::~CommandPool()
    {
        if (_command_pool_handle != VK_NULL_HANDLE) {
            vkDestroyCommandPool(_ptr_device->getDeviceHandle(), _command_pool_handle, nullptr);
        }
    }

    const Device* CommandPool::getDevice() const noexcept
    {
        return _ptr_device;
    }

    uint32_t CommandPool::getFamilyIndex() const noexcept
    {
        return _queue_family_index;
    }

    const VkCommandPool& CommandPool::getCommandPoolHandle() const noexcept
    {
        return _command_pool_handle;
    }

    std::unique_ptr<CommandPool> CommandPool::make(const Device* ptr_device, uint32_t queue_family_index)
    {
        return std::make_unique<CommandPool>(ptr_device, queue_family_index);
    }
}
