//
//  PBRPass.inl
//  PBRLib
//
//  Created by Асиф Мамедов on 19/08/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//


namespace pbrlib
{
    inline PBRPass::Optionals::Optionals(
        DistributionFunction    distrib_func,
        GeometryFunction        geom_func,
        FresnelApproximation    fresnel_approx,
        bool                    use_bent_normal
    ) :
        _distrib_func   (distrib_func),
        _geom_func      (geom_func),
        _fresnel_approx (fresnel_approx)
    {
        _other_options.use_bent_normal = use_bent_normal;
    }

    inline void PBRPass::Optionals::setDistributionFunction(DistributionFunction dist_func) noexcept
    {
        _distrib_func = dist_func;
    }

    inline void PBRPass::Optionals::setGeometryFunction(GeometryFunction geom_func) noexcept
    {
        _geom_func = geom_func;
    }

    inline void PBRPass::Optionals::setFresnelApproximation(FresnelApproximation fresnel_approx) noexcept
    {
        _fresnel_approx = fresnel_approx;
    }

    inline void PBRPass::Optionals::useBentNormal(bool to_use) noexcept
    {
        _other_options.use_bent_normal = to_use;
    }

    inline PBRPass::DistributionFunction PBRPass::Optionals::getDistributionFunction() const noexcept
    {
        return _distrib_func;
    }

    inline PBRPass::GeometryFunction PBRPass::Optionals::getGeometryFunction() const noexcept
    {
        return _geom_func;
    }

    inline PBRPass::FresnelApproximation PBRPass::Optionals::getFresnelApproximation() const noexcept
    {
        return _fresnel_approx;
    }

    inline bool PBRPass::Optionals::useBentNormal() const noexcept
    {
        return _other_options.use_bent_normal;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    inline PtrComputePipeline& PBRPass::getPipeline() noexcept
    {
        return _ptr_pipeline;
    }

    inline const PtrComputePipeline& PBRPass::getPipeline() const noexcept
    {
        return _ptr_pipeline;
    }

    inline PtrPBRPass PBRPass::make(
        const PtrDevice&            ptr_device, 
        const PtrPhysicalDevice&    ptr_physical_deviec,
        uint32_t                    queue_family_index,
        const PtrDescriptorPool&    ptr_descriptor_pool,
        const ImageView&            position_and_metallic_image_view,
        const ImageView&            normal_and_roughness_image_view,
        const ImageView&            albedo_and_baked_AO_image_view,
        const ImageView&            anisotropy_image_view,
        const PtrSampler&           ptr_sampler,
        const Optionals&            optionals
    )
    {
        return make_unique<PBRPass>(
            ptr_device, 
            ptr_physical_deviec,
            queue_family_index,
            ptr_descriptor_pool,
            position_and_metallic_image_view,
            normal_and_roughness_image_view,
            albedo_and_baked_AO_image_view,
            anisotropy_image_view,
            ptr_sampler,
            optionals
        );
    }
}