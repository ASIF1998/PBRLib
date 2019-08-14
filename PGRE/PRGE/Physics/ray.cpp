//
//  ray.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 13/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "ray.hpp"

#include <limits>

using namespace std;

namespace PRGE
{
    Ray::Ray() noexcept:
        _time{0},
        _tMax{numeric_limits<float>::infinity()}
    {}

    Ray::Ray(const Point3<float>& o, Vec3<float>& dir, float time, float tMax) NOEXCEPT_PRGE :
        _o{0},
        _dir{dir},
        _time{time},
        _tMax{tMax}
    {}

    Point3<float> Ray::operator () (float t) const noexcept
    {
        return _o + _dir * t;
    }

#if DEBUG_PRGE == 1
    ostream& operator << (ostream& os, const Ray& ray)
    {
        os << "Start point: " << ray._o << endl 
             << "Directional: " << ray._dir << endl
             << "Time: " << ray._time << endl
             << "Max time: " << ray._tMax << endl;
        
        return os;
    }
#endif

    RayDifferential::RayDifferential() :
        Ray(),
        _hasDifferentials{false}
    {}

    RayDifferential::RayDifferential(Point3<float> o, Vec3<float> dir, float time, float tMax) :
        Ray(o, dir, time, tMax)
    {}

    void RayDifferential::scaleDifferentional(float s) NOEXCEPT_PRGE
    {
        _rxOrigin = _o + (_rxOrigin - _o) * s;
        _ryOrigin = _o + (_ryOrigin - _o) * s;
        _rxDirection = _dir + (_rxDirection - _dir) * s;
        _ryDirection = _dir + (_ryDirection - _dir) * s;
    }

#if DEBUG_PRGE == 1
    ostream& operator << (ostream& os, const RayDifferential& rayDifferential)
    {
        os << "Start point: " << rayDifferential._o << endl 
           << "Directional: " << rayDifferential._dir << endl
           << "Time: " << rayDifferential._time << endl
           << "Max time: " << rayDifferential._tMax << endl
           << "Has differentional: " << rayDifferential._hasDifferentials << endl 
           << "RX origin: " << rayDifferential._rxOrigin << endl
           << "RY origin: " << rayDifferential._ryOrigin << endl
           << "RX Direction: " << rayDifferential._rxDirection << endl
           << "RY Direction: " << rayDifferential._ryDirection << endl;
        
        return os;
    }
#endif
}
