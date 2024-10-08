//
//  RenderPass.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 19/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/VulkanWrapper/RenderPass.hpp>

#include <cassert>

namespace pbrlib
{
    SubpassDescription::SubpassDescription(
        size_t num_input_attachment,
        size_t num_color_attachment,
        size_t num_present_attachment
    ) :
        _use_depth_stencil_attachment(false)
    {
        _input_attachment.reserve(num_input_attachment);
        _color_attachment.reserve(num_color_attachment);
        _present_attachment.reserve(num_present_attachment);
    }

    SubpassDescription::SubpassDescription(SubpassDescription&& subpass_descriptoin) :
        _depth_stencil_attachment       (subpass_descriptoin._depth_stencil_attachment),
        _use_depth_stencil_attachment   (subpass_descriptoin._use_depth_stencil_attachment)
    {
        swap(_color_attachment, subpass_descriptoin._color_attachment);
        swap(_input_attachment, subpass_descriptoin._input_attachment);
        swap(_present_attachment, subpass_descriptoin._present_attachment);
    }

    SubpassDescription::SubpassDescription(const SubpassDescription& subpass_descriptoin) :
        _input_attachment               (subpass_descriptoin._input_attachment),
        _color_attachment               (subpass_descriptoin._color_attachment),
        _present_attachment             (subpass_descriptoin._present_attachment),
        _depth_stencil_attachment       (subpass_descriptoin._depth_stencil_attachment),
        _use_depth_stencil_attachment   (subpass_descriptoin._use_depth_stencil_attachment)
    {}

    void SubpassDescription::addInputAttachment(uint32_t attachment, VkImageLayout layout)
    {
        _input_attachment.push_back({
            attachment,
            layout
        });
    }

    void SubpassDescription::addColorAttachment(uint32_t attachment, VkImageLayout layout)
    {
        _color_attachment.push_back({
            attachment,
            layout
        });
    }

    void SubpassDescription::addPresentAttachment(uint32_t attachment)
    {
        _present_attachment.push_back(attachment);
    }

    void SubpassDescription::setDepthStencilAttachment(uint32_t attachment, VkImageLayout layout)
    {
        _use_depth_stencil_attachment = true;

        _depth_stencil_attachment = {
            attachment,
            layout
        };
    }

    void SubpassDescription::resetDepthStencilAttahment() noexcept
    {
        _use_depth_stencil_attachment = false;
    }

    std::vector<VkAttachmentReference>& SubpassDescription::getInputAttachment() noexcept
    {
        return _input_attachment;
    }

    const std::vector<VkAttachmentReference>& SubpassDescription::getInputAttachment() const noexcept
    {
        return _input_attachment;
    }

    std::vector<VkAttachmentReference>& SubpassDescription::getColorAttachment() noexcept
    {
        return _color_attachment;
    }

    const std::vector<VkAttachmentReference>& SubpassDescription::getColorAttachment() const noexcept
    {
        return _color_attachment;
    }

    std::vector<uint32_t>& SubpassDescription::getPresentAttachment() noexcept
    {
        return _present_attachment;
    }

    const std::vector<uint32_t>& SubpassDescription::getPresentAttachment() const noexcept
    {
        return _present_attachment;
    }

    VkAttachmentReference& SubpassDescription::getDepthStencilAttachment() noexcept
    {
        return _depth_stencil_attachment;
    }

    const VkAttachmentReference& SubpassDescription::getDepthStencilAttachment() const noexcept
    {
        return _depth_stencil_attachment;
    }

    bool SubpassDescription::useDepthStencilAttachment() const noexcept
    {
        return _use_depth_stencil_attachment;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RenderPassInfo::RenderPassInfo(
        size_t num_attribute_descriptions,
        size_t num_subpass_descriptions,
        size_t num_subpass_dependencies
    )
    {
        _subpass_descriptions.reserve(num_subpass_descriptions);
        _subpass_dependencies.reserve(num_subpass_dependencies);
        _attribute_descriptions.reserve(num_attribute_descriptions);
    }

    RenderPassInfo::RenderPassInfo(RenderPassInfo&& render_pass_info)
    {
        swap(_attribute_descriptions, render_pass_info._attribute_descriptions);
        swap(_subpass_descriptions, render_pass_info._subpass_descriptions);
        swap(_subpass_dependencies, render_pass_info._subpass_dependencies);
    }

    void RenderPassInfo::setNumSubpassDescription(size_t num_subpass_description)
    {
        _subpass_descriptions.resize(num_subpass_description);
    }
    SubpassDescription& RenderPassInfo::getSubpassDescription(size_t i) noexcept
    {
        return _subpass_descriptions[i];
    }

    const SubpassDescription& RenderPassInfo::getSubpassDescription(size_t i) const noexcept
    {
        return _subpass_descriptions[i];
    }

    void RenderPassInfo::addAttachmentDescription(
        VkFormat                format,
        VkSampleCountFlagBits   samples,
        VkAttachmentLoadOp      load_op,
        VkAttachmentStoreOp     store_op,
        VkAttachmentLoadOp      stencil_load_op,
        VkAttachmentStoreOp     stencil_store_op,
        VkImageLayout           initial_layout,
        VkImageLayout           final_layout
    )
    {
        _attribute_descriptions.push_back({
            0,
            format,
            samples,
            load_op,
            store_op,
            stencil_load_op,
            stencil_store_op,
            initial_layout,
            final_layout
        });
    }

    void RenderPassInfo::addSubpassDescription(SubpassDescription&& subpass_description)
    {
        _subpass_descriptions.push_back(std::move(subpass_description));
    }

    void RenderPassInfo::addSubpassDescription(const SubpassDescription& subpass_description)
    {
        _subpass_descriptions.push_back(subpass_description);
    }

    void RenderPassInfo::addSubpassDependency(
        uint32_t                src_subpass,
        uint32_t                dst_subpass,
        VkPipelineStageFlags    src_stage_mask,
        VkPipelineStageFlags    dst_stage_mask,
        VkAccessFlags           src_access_mask,
        VkAccessFlags           dst_access_mask
    )
    {
        _subpass_dependencies.push_back({
            src_subpass,
            dst_subpass,
            src_stage_mask,
            dst_stage_mask,
            src_access_mask,
            dst_access_mask,
            0
        });
    }

    std::vector<VkAttachmentDescription>& RenderPassInfo::getAttachmentDescriptions() noexcept
    {
        return _attribute_descriptions;
    }

    const std::vector<VkAttachmentDescription>& RenderPassInfo::getAttachmentDescriptions() const noexcept
    {
        return _attribute_descriptions;
    }

    std::vector<SubpassDescription>& RenderPassInfo::getSubpassDescriptions() noexcept
    {
        return _subpass_descriptions;
    }

    const std::vector<SubpassDescription>& RenderPassInfo::getSubpassDescriptions() const noexcept
    {
        return _subpass_descriptions;
    }

    std::vector<VkSubpassDependency>& RenderPassInfo::getSubpassDependencies() noexcept
    {
        return _subpass_dependencies;
    }

    const std::vector<VkSubpassDependency>& RenderPassInfo::getSubpassDependencies() const noexcept
    {
        return _subpass_dependencies;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RenderPass::RenderPass(const Device* ptr_device, const RenderPassInfo& render_pass_info) :
        _render_pass_info   (render_pass_info),
        _render_pass_handle (VK_NULL_HANDLE),
        _ptr_device         (ptr_device)
    {
        _create();
    }

    RenderPass::RenderPass(const Device* ptr_device, RenderPassInfo&& render_pass_info) :
        _render_pass_info   (std::move(render_pass_info)),
        _render_pass_handle (VK_NULL_HANDLE),
        _ptr_device         (ptr_device)
    {
        _create();
    }

    RenderPass::RenderPass(RenderPass&& render_pass) :
        _render_pass_info   (std::move(render_pass._render_pass_info)),
        _render_pass_handle (VK_NULL_HANDLE),
        _ptr_device         (render_pass._ptr_device)
    {
        std::swap(_render_pass_handle, render_pass._render_pass_handle);
    }

    RenderPass::~RenderPass() noexcept
    {
        if (_render_pass_handle != VK_NULL_HANDLE) {
            vkDestroyRenderPass(_ptr_device->getDeviceHandle(), _render_pass_handle, nullptr);
        }
    }

    void RenderPass::_create()
    {
        auto& psubpass_descritions = _render_pass_info.getSubpassDescriptions();
        std::vector<VkSubpassDescription> subpass_description (psubpass_descritions.size());

        for (size_t i{0}; i < subpass_description.size(); i++) {
            subpass_description[i] = {
                0,
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                static_cast<uint32_t>(psubpass_descritions[i].getInputAttachment().size()),
                psubpass_descritions[i].getInputAttachment().data(),
                static_cast<uint32_t>(psubpass_descritions[i].getColorAttachment().size()),
                psubpass_descritions[i].getColorAttachment().data(),
                nullptr,
                (psubpass_descritions[i].useDepthStencilAttachment() ?
                    &psubpass_descritions[i].getDepthStencilAttachment() : nullptr),
                static_cast<uint32_t>(psubpass_descritions[i].getPresentAttachment().size()),
                psubpass_descritions[i].getPresentAttachment().data()
            };
        }

        VkRenderPassCreateInfo render_pass_create_info = { };
        render_pass_create_info.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        render_pass_create_info.attachmentCount = static_cast<uint32_t>(_render_pass_info.getAttachmentDescriptions().size());
        render_pass_create_info.pAttachments    = _render_pass_info.getAttachmentDescriptions().data();
        render_pass_create_info.subpassCount    = static_cast<uint32_t>(subpass_description.size());
        render_pass_create_info.pSubpasses      = subpass_description.data();
        render_pass_create_info.dependencyCount = static_cast<uint32_t>(_render_pass_info.getSubpassDependencies().size());
        render_pass_create_info.pDependencies   = _render_pass_info.getSubpassDependencies().data();

       assert(vkCreateRenderPass(
           _ptr_device->getDeviceHandle(), 
           &render_pass_create_info, 
           nullptr, 
           &_render_pass_handle
       ) == VK_SUCCESS);

       assert(_render_pass_handle != VK_NULL_HANDLE);
    }

    const RenderPassInfo& RenderPass::getRenderPassInfo() const noexcept
    {
        return _render_pass_info;
    }

    const VkRenderPass& RenderPass::getRenderPassHandle() const noexcept
    {
        return _render_pass_handle;
    }

    const Device* RenderPass::getDevice() const noexcept
    {
         return _ptr_device;
    }

    std::unique_ptr<RenderPass> RenderPass::make(
        const Device*   ptr_device,
        const RenderPassInfo&           render_pass_info
    )
    {
        return std::make_unique<RenderPass>(ptr_device, render_pass_info);
    }

    std::unique_ptr<RenderPass> RenderPass::make(
        const Device*       ptr_device,
        RenderPassInfo&&    render_pass_info
    )
    {
        return std::make_unique<RenderPass>(ptr_device, std::move(render_pass_info));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    RenderPass::Builder::Builder(
        size_t num_attribute_descriptions,
        size_t num_subpass_descriptions,
        size_t num_subpass_dependencies
    ) :
        RenderPassInfo(
            num_attribute_descriptions,
            num_subpass_descriptions,
            num_subpass_dependencies
        )
    {}

    void RenderPass::Builder::setDevice(const Device* ptr_device)
    {
        _ptr_device = ptr_device;
    }

    RenderPass RenderPass::Builder::build() const
    {
        return RenderPass(
            _ptr_device,
            *this
        );
    }
    
    std::unique_ptr<RenderPass> RenderPass::Builder::buildPtr() const
    {
        return std::make_unique<RenderPass>(
            _ptr_device,
            *this
        );
    }
}
