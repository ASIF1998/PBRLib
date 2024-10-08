//
//  ShaderModule.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef ShaderModule_hpp
#define ShaderModule_hpp

#include "Device.hpp"

namespace pbrlib
{
    class SpecializationInfo
    {
    public:
        SpecializationInfo(
            size_t size_data,
            size_t num_map_entires
        );

        SpecializationInfo(SpecializationInfo&& specialization_info);
        SpecializationInfo(const SpecializationInfo& specialization_info);

        ~SpecializationInfo();

        SpecializationInfo& operator = (SpecializationInfo&&)       = delete;
        SpecializationInfo& operator = (const SpecializationInfo&)  = delete;
        
        void addMapEntry(const void* ptr_data, size_t data_size, uint32_t constant_id);

        const VkSpecializationInfo&         getSpecializationInfo()             const noexcept;
        VkSpecializationInfo&               getSpecializationInfo()             noexcept;
        uint8_t*                            getData()                           noexcept;
        const uint8_t*                      getData()                           const noexcept;
        VkSpecializationMapEntry*           getSpecializationMapEntries()       noexcept;
        const VkSpecializationMapEntry*     getSpecializationMapEntries()       const noexcept;
        size_t                              getDataSize()                       const noexcept;
        size_t                              getSpecializationMapEntriesNum()    const noexcept;
        size_t                              capacityData()                      const noexcept;
        size_t                              capacitySpecializationMapEntries()  const noexcept;

    private:
        uint8_t*                    _ptr_data;
        VkSpecializationMapEntry*   _ptr_map_enties;
        size_t                      _current_byte_in_data;
        size_t                      _current_map_entry;
        VkSpecializationInfo        _specialization_info;
    };

    class ShaderModule
    {
    public:
        class Builder
        {
        public:
            Builder();

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)      = delete;
            Builder& operator = (const Builder&) = delete;

            void setDevice(const Device* ptr_device);
            void setShaderType(VkShaderStageFlagBits shader_type)                               noexcept;
            void setShaderCode(const uint32_t* ptr_shader_code, size_t shader_code_size)        noexcept;
            void setSpecializationInfoSizeData(size_t specialization_info_size_data)            noexcept;
            void setSpecializationInfoNumMapEntries(size_t specialization_info_num_map_entires) noexcept;

            ShaderModule                    build()     const;
            std::unique_ptr<ShaderModule>   buildPtr()  const;

        private:
            const Device*           _ptr_device;
            VkShaderStageFlagBits   _shader_type;
            const uint32_t*         _ptr_shader_code;
            size_t                  _shader_code_size;
            size_t                  _specialization_info_size_data;
            size_t                  _specialization_info_num_map_entires;
        };

    public:
        ShaderModule(
            const Device*           ptr_device,
            VkShaderStageFlagBits   shader_type,
            const uint32_t*         ptr_shader_code,
            size_t                  shader_code_size,
            size_t                  specialization_info_size_data       = 0,
            size_t                  specialization_info_num_map_entires = 0
        );

        ShaderModule(ShaderModule&& shader_module);
        ShaderModule(const ShaderModule&) = delete;
        
        ~ShaderModule();

        ShaderModule& operator = (ShaderModule&&)       = delete;
        ShaderModule& operator = (const ShaderModule&)  = delete;

        const Device*   getDevice()             const noexcept;
        VkShaderStageFlagBits           getShaderType()         const noexcept;
        const VkShaderModule&           getShaderHandle()       const noexcept;
        SpecializationInfo&             getSpecializationInfo() noexcept;
        const SpecializationInfo&       getSpecializationInfo() const noexcept;

        static std::unique_ptr<ShaderModule> make(
            const Device*           ptr_device,
            VkShaderStageFlagBits   shader_type,
            const uint32_t*         ptr_shader_code,
            size_t                  shader_code_size,
            size_t                  specialization_info_size_data       = 0,
            size_t                  specialization_info_num_map_entires = 0
        );

    private:
        const Device*           _ptr_device;
        VkShaderStageFlagBits   _shader_type;
        SpecializationInfo      _specialization_info;
        VkShaderModule          _shader_handle;
    };
}

#endif /* ShaderModule_hpp */
