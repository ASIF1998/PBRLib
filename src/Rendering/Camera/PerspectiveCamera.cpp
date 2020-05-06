//
//  PerspectiveCamera.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 06/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "PerspectiveCamera.hpp"

namespace pbrlib
{
    PerspectiveCamera::PerspectiveCamera(
        const Vec3<float>&  pos,
        const Vec3<float>&  eye,
        const Vec3<float>&  up,
        float               near_clipp,
        float               far_clipp,
        float               aspect,
        float               fovy
    ) :
        ICamera(
            Transform::lookAt(pos, eye, up),
            Transform::perspective(fovy, aspect, near_clipp, far_clipp),
            pos,
            eye,
            near_clipp,
            far_clipp
        ),
        _aspect (aspect),
        _fovy   (fovy)
    {}

    Transform PerspectiveCamera::_calculateProjection() const
    {
        return Transform::perspective(_fovy, _aspect, _z_near, _z_far);
    }

    float PerspectiveCamera::getAspect() const noexcept
    {
        return _aspect;
    }

    float PerspectiveCamera::getFovy() const noexcept
    {
        return _fovy;
    }

    void PerspectiveCamera::setAspect(float aspect)
    {
        _aspect     = aspect;
        _projection = Transform::perspective(_fovy, _aspect, _z_near, _z_far);
    }

    void PerspectiveCamera::setAspect(float width, float height)
    {
        assert(height != static_cast<float>(0u));
        _aspect     = width / height;
        _projection = Transform::perspective(_fovy, _aspect, _z_near, _z_far);
    }

    void PerspectiveCamera::setFovy(float fovy)
    {
        _fovy       = fovy;
        _projection = Transform::perspective(_fovy, _aspect, _z_near, _z_far);
    }

    void PerspectiveCamera::setAspectAndFovy(float aspect, float fovy)
    {
        _aspect = aspect;
        _fovy   = fovy;
        _projection = Transform::perspective(_fovy, _aspect, _z_near, _z_far);
    }

    void PerspectiveCamera::setAspectAndFovy(float width, float height, float fovy)
    {
        assert(height != static_cast<float>(0u));
        _aspect     = width / height;
        _fovy       = fovy;
        _projection = Transform::perspective(_fovy, _aspect, _z_near, _z_far);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void PerspectiveCamera::Builder::setPosition(const Vec3<float>& pos)
    {
        _pos = pos;
    }

    void PerspectiveCamera::Builder::setEye(const Vec3<float>& eye)
    {
        _eye = eye;
    }

    void PerspectiveCamera::Builder::setUp(const Vec3<float>& up)
    {
        _up = up;
    }

    void PerspectiveCamera::Builder::setNearClipp(float near) noexcept
    {
        _z_near = near;
    }

    void PerspectiveCamera::Builder::setFarClipp(float far) noexcept
    {
        _z_far = far;
    }

    void PerspectiveCamera::Builder::setAspect(float aspect) noexcept
    {
        _aspect = aspect;
    }

    void PerspectiveCamera::Builder::setAspect(float width, float height)
    {
        assert(height != static_cast<float>(0u));
        _aspect = width / height;
    }

    void PerspectiveCamera::Builder::setFovy(float fovy) noexcept
    {
        _fovy = fovy;
    }

    PerspectiveCamera PerspectiveCamera::Builder::build() const
    {
        return PerspectiveCamera(
            _pos,
            _eye,
            _up,
            _z_near,
            _z_far,
            _aspect,
            _fovy
        );
    }

    PtrPerspectiveCamera PerspectiveCamera::Builder::ptrBuild() const
    {
        return make_shared<PerspectiveCamera>(
            _pos,
            _eye,
            _up,
            _z_near,
            _z_far,
            _aspect,
            _fovy
        );
    }
}
