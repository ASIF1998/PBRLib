//
//  DirectionLight.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 07/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Rendering/Lights/DirectionLight.hpp>

using namespace pbrlib::math;

namespace pbrlib
{
    DirectionLight::DirectionLight(
        const Vec3<float>&  direction_to_light,
        const Vec3<float>&  color,
        float               intensity
    ) :
        Component       ("Direction Light"),
        _dir_to_lihght  (direction_to_light),
        _color          (color),
        _intensity      (intensity)
    {}

    DirectionLight::DirectionLight(
        const std::string_view  name,
        const Vec3<float>&      direction_to_light,
        const Vec3<float>&      color,
        float                   intensity
    ) :
        Component       (name),
        _dir_to_lihght  (direction_to_light),
        _color          (color),
        _intensity      (intensity)
    {}

    void DirectionLight::setIntensity(float intensity) noexcept
    {
        _intensity = intensity;
    }

    void DirectionLight::setColor(const Vec3<float>& color)
    {
        _color = color;
    }

    void DirectionLight::setDirectionToLight(const Vec3<float>& direction)
    {
        _dir_to_lihght = direction;
    }

    float DirectionLight::getIntensity() const noexcept
    {
        return _intensity;
    }

    Vec3<float>& DirectionLight::getColor() noexcept
    {
        return _color;
    }

    const Vec3<float>& DirectionLight::getColor() const noexcept
    {
        return _color;
    }

    Vec3<float>& DirectionLight::getDirectionToLight() noexcept
    {
        return _dir_to_lihght;
    }

    const Vec3<float>& DirectionLight::getDirectionToLight() const noexcept
    {
        return _dir_to_lihght;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
    DirectionLight::Builder::Builder(const std::string_view name) :
        _name(name)
    {}

    void DirectionLight::Builder::setIntensity(float intensity) noexcept
    {
        _intensity = intensity;
    }

    void DirectionLight::Builder::setColor(const Vec3<float>& color)
    {
        _color = color;
    }

    void DirectionLight::Builder::setDirectionToLight(const Vec3<float>& direction)
    {
        _dir_to_light = direction;
    }

    void DirectionLight::Builder::setName(const std::string_view name)
    {
        _name = name;
    }

    DirectionLight DirectionLight::Builder::build() const
    {
        return DirectionLight(_name, _dir_to_light, _color, _intensity);
    }

    std::unique_ptr<DirectionLight> DirectionLight::Builder::buildPtr() const
    {
        return make_unique<DirectionLight>(_name, _dir_to_light, _color, _intensity);
    }
}
