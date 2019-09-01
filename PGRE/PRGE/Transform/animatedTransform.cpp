//
//  animated_translatesransform.cpp
//  P_rotatesGE
//
//  Created by Асиф Мамедов on 30/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "animatedTransform.hpp"

#include <cmath>

using namespace std;

namespace PRGE
{
    AnimatedTransform::AnimatedTransform(const Transform* ptrStartTransform, const Transform* ptrEndTransform, float startTime, float endTime) :
        _ptrStartTransform{ptrStartTransform},
        _ptrEndTransform{ptrEndTransform},
        _startTime{startTime},
        _endTime{endTime},
        _actualAnimated{_ptrStartTransform != _ptrEndTransform}
    {
       AnimatedTransform::decompose(ptrStartTransform->_m, _translates[0], _rotates[0], _scales[0]);
       AnimatedTransform::decompose(ptrEndTransform->_m, _translates[1], _rotates[1], _scales[1]);

       _hasRotate = dot(_rotates[0], _rotates[1]) < 0.9995f;

       float cosTheta = dot(_rotates[0], _rotates[1]);
       float theta = acos(clamp(cosTheta, -1.0f, 1.0f));
       Quaternion qperp = normalize(_rotates[1] - _rotates[0] * cosTheta);
       float t0x = _translates[0][0];
       float t0z = _translates[0][2];
       float t1x = _translates[1][0];
       float t1z = _translates[1][2];
       float q1x = _rotates[0]._xyzw[0];
       float q1y = _rotates[0]._xyzw[1];
       float q1z = _rotates[0]._xyzw[2];
       float q1w = _rotates[0]._xyzw[3];
       float qperpx = qperp._xyzw[0];
       float qperpy = qperp._xyzw[1];
       float qperpz = qperp._xyzw[2];
       float qperpw = qperp._xyzw[3];
       float s000 = _scales[0][0][0];
       float s001 = _scales[0][0][1];
       float s002 = _scales[0][0][2];
       float s010 = _scales[0][1][0];
       float s011 = _scales[0][1][1];
       float s012 = _scales[0][1][2];
       float s020 = _scales[0][2][0];
       float s021 = _scales[0][2][1];
       float s022 = _scales[0][2][2];
       float s100 = _scales[1][0][0];
       float s101 = _scales[1][0][1];
       float s102 = _scales[1][0][2];
       float s110 = _scales[1][1][0];
       float s111 = _scales[1][1][1];
       float s112 = _scales[1][1][2];
       float s120 = _scales[1][2][0];
       float s121 = _scales[1][2][1];
       float s122 = _scales[1][2][2];

       /// q1y * q1y, q1z * q1z, qperpy * qperpy, qperpz * qperpz
       auto multipliedNumbers1 = _mm_mul_ps(_mm_setr_ps(q1y, q1z, qperpy, qperpz), _mm_setr_ps(q1y, q1z, qperpy, qperpz));

       /// q1x * q1x, qperpx * qperpx, q1w * q1z, qperpx * qperpy
       auto multipliedNumbers2 = _mm_mul_ps(_mm_setr_ps(q1x, qperpx, q1w, qperpx), _mm_setr_ps(q1x, qperpx, q1z, qperpy));

       /// qperpw * qperpz, q1w * q1y, qperpw * qperpy, qperpx * qperpz
       auto multipliedNumbers3 = _mm_mul_ps(_mm_setr_ps(qperpw, q1w, qperpw, qperpx), _mm_setr_ps(qperpz, q1y, qperpy, qperpz));

       /// q1z * s020, q1z * s120, q1z * s021, q1z * s121
       auto multipliedNumbers4 = _mm_mul_ps(_mm_setr_ps(q1z, q1z, q1z, q1z), _mm_setr_ps(s020, s120, s021, s121));

       /// q1z * s022, q1z * s122, q1z * qperpy, s010 * theta
       auto multipliedNumbers5 = _mm_mul_ps(_mm_setr_ps(q1z, q1z, q1x, s010), _mm_setr_ps(s022, s122, qperpy, theta));

       /// q1w * qperpz, q1w * qperpy, s020 * theta, q1x * qperpz  
       auto multipliedNumbers6 = _mm_mul_ps(_mm_setr_ps(q1w, q1w, s020, q1x), _mm_setr_ps(qperpz, qperpy, theta, qperpz));

       /// qperpy * s000, qperpx * s010, qperpw * s020, qperpz * s000
       auto multipliedNumbers7 = _mm_mul_ps(_mm_setr_ps(qperpy, qperpx, qperpw, qperpz), _mm_setr_ps(s000, s010, s020, s000));

       /// qperpw * s010, qperpx * s020, qperpy * s001, qperpx * s011
       auto multipliedNumbers8 = _mm_mul_ps(_mm_setr_ps(qperpw, qperpx, qperpy, qperpx), _mm_setr_ps(s010, s020, s001, s011));

       /// qperpw * s021, qperpz * s001, qperpw * s011, qperpx * s021
       auto multipliedNumbers9 = _mm_mul_ps(_mm_setr_ps(qperpw, qperpz, qperpw, qperpx), _mm_setr_ps(s021, s001, s011, s021));

       /// s021 * theta, s022 * theta, qperpy * s002, qperpx * s012
       auto multipliedNumbers10 = _mm_mul_ps(_mm_setr_ps(s021, s022, qperpy, qperpx), _mm_setr_ps(theta, theta, s002, s012));

       /// qperpw * s022, qperpz * s002, qperpw * s012, qperpx * s022
       auto multipliedNumbers11 = _mm_mul_ps(_mm_setr_ps(qperpw, qperpz, qperpw, qperpx), _mm_setr_ps(s022, s002, s012, s022));

       /// qperpx * s110, qperpx * s120, qperpz * s100, qperpw * s110
       auto multipliedNumbers12 = _mm_mul_ps(_mm_setr_ps(qperpx, qperpx, qperpz, qperpw), _mm_setr_ps(s110, s120, s100, s110));

       /// qperpw * s111, qperpx * s111, qperpw * s121, qperpz * s101
       auto multipliedNumbers13 = _mm_mul_ps(_mm_setr_ps(qperpw, qperpx, qperpw, qperpz), _mm_setr_ps(s111, s111, s121, s101));

       /// qperpx * s121, qperpx * s112, qperpw * s122, qperpz * s102
       auto multipliedNumbers14 = _mm_mul_ps(_mm_setr_ps(qperpx, qperpx, qperpw, qperpz), _mm_setr_ps(s121, s112, s122, s102));

       /// qperpw * s112, qperpx * s122, s001 * theta, s011 * theta
       auto multipliedNumbers15 = _mm_mul_ps(_mm_setr_ps(qperpw, qperpx, s001, s011), _mm_setr_ps(s112, s122, theta, theta));

       /// qperpy * qperpz, s002 * theta, q1z * qperpw, s000 * theta
       auto multipliedNumbers16 = _mm_mul_ps(_mm_setr_ps(qperpy, s002, q1z, s000), _mm_setr_ps(qperpz, theta, qperpw, theta));

       /// q1y * qperpx, q1z * qperpz, q1y * qperpz, q1z * s000
       auto multipliedNumbers17 = _mm_mul_ps(_mm_setr_ps(q1y, q1z, q1y, q1z), _mm_setr_ps(qperpx, qperpz, qperpz, s000));

       /// q1z * s100, q1z * s001, q1z * s101, q1z * s002
       auto multipliedNumbers18 = _mm_mul_ps(_mm_setr_ps(q1z, q1z, q1z, q1z), _mm_setr_ps(s100, s001, s101, s002));

       /// qperpz * s010, qperpy * s020, qperpz * s110, qperpy * s120
       auto multipliedNumbers19 = _mm_mul_ps(_mm_setr_ps(qperpz, qperpy, qperpz, qperpy), _mm_setr_ps(s010, s020, s110, s120));

       /// qperpx * s000, qperpz * s020, qperpx * s100, qperpz * s120
       auto multipliedNumbers20 = _mm_mul_ps(_mm_setr_ps(qperpx, qperpz, qperpx, qperpz), _mm_setr_ps(s000, s020, s100, s120));

       /// qperpz * s011, qperpy * s021, qperpz * s111, qperpy * s121
       auto multipliedNumbers21 = _mm_mul_ps(_mm_setr_ps(qperpz, qperpy, qperpz, qperpy), _mm_setr_ps(s011, s021, s111, s121));

       /// qperpx * s001, qperpz * s021, qperpx * s101, qperpz * s121
       auto multipliedNumbers22 = _mm_mul_ps(_mm_setr_ps(qperpx, qperpz, qperpx, qperpz), _mm_setr_ps(s001, s021, s101, s121));

       /// qperpz * s012, qperpy * s022, qperpz * s112, qperpy * s122
       auto multipliedNumbers23 = _mm_mul_ps(_mm_setr_ps(qperpz, qperpy, qperpz, qperpy), _mm_setr_ps(s012, s022, s112, s122));

       /// qperpx * s002, qperpz * s022, qperpx * s102, qperpz * s122
       auto multipliedNumbers24 = _mm_mul_ps(_mm_setr_ps(qperpx, qperpz, qperpx, qperpz), _mm_setr_ps(s002, s022, s102, s122));

       /// q1w * q1x, qperpw * qperpx, q1y * q1z, q1x * q1z
       auto multipliedNumbers25 = _mm_mul_ps(_mm_setr_ps(q1w, qperpw, q1y, q1x), _mm_setr_ps(q1x, qperpx, q1z, q1z));

       /// q1y * qperpw, q1z * qperpx, q1x * qperpw, q1x * qperpx
       auto multipliedNumbers26 = _mm_mul_ps(_mm_setr_ps(q1y, q1z, q1x, q1x), _mm_setr_ps(qperpw, qperpx, qperpw, qperpx));

       /// q1y * qperpy, qperpy * s010, qperpy * s110, qperpy * s011
       auto multipliedNumbers27 = _mm_mul_ps(_mm_setr_ps(q1y, qperpy, qperpy, qperpy),_mm_setr_ps(qperpy, s010, s110, s011));

       /// qperpy * s111, qperpy * s012, qperpy * s112, 2.0f * q1x
       auto multipliedNumbers28 = _mm_mul_ps(_mm_setr_ps(qperpy, qperpy, qperpy, 2.0f), _mm_setr_ps(s111, s012, s112, q1x));

       /// 2 * q1w, 2 * qperpy, 2 * qperpx, 2 * qperpw
       auto multipliedNumbers29 = _mm_mul_ps(_mm_setr_ps(2.0f, 2.0f, 2.0f, 2.0f), _mm_setr_ps(q1w, qperpy, qperpx, qperpw));

       /// 2 * q1y, 2 * q1z, 0, 0
       auto multipliedNumbers30 = _mm_mul_ps(_mm_setr_ps(2.0f, 2.0f, 0.0f, 0.0f), _mm_setr_ps(q1y, q1z, 0.0f, 0.0f));

       // _c1[0]

       _c1[0].kx = (-1 + multipliedNumbers1[0] + multipliedNumbers1[1] + multipliedNumbers1[2] + multipliedNumbers1[3]) *
                     s000 +
              multipliedNumbers2[2] * s010 - multipliedNumbers2[3] * s010 +
              multipliedNumbers3[0] * s010 - multipliedNumbers3[1] * s020 -
              multipliedNumbers3[2] * s020 - multipliedNumbers3[3] * s020 + s100 -
              multipliedNumbers1[0] * s100 - multipliedNumbers1[1] * s100 - multipliedNumbers1[2] * s100 -
              multipliedNumbers1[3] * s100 - multipliedNumbers2[2] * s110 +
              multipliedNumbers2[3] * s110 - multipliedNumbers3[0] * s110 +
              multipliedNumbers3[1] * s120 + multipliedNumbers3[2] * s120 +
              multipliedNumbers3[3] * s120 +
              q1x * (-(q1y * s010) - multipliedNumbers4[0] + q1y * s110 + multipliedNumbers4[1]);

       _c1[0].ky = (-1 + multipliedNumbers1[0] + multipliedNumbers1[1] + multipliedNumbers1[2] + multipliedNumbers1[3]) *
                     s001 +
              multipliedNumbers2[2] * s011 - multipliedNumbers2[3] * s011 +
              multipliedNumbers3[0] * s011 - multipliedNumbers3[1] * s021 -
              multipliedNumbers3[2] * s021 - multipliedNumbers3[3] * s021 + s101 -
              multipliedNumbers1[0] * s101 - multipliedNumbers1[1] * s101 - multipliedNumbers1[2] * s101 -
              multipliedNumbers1[3] * s101 - multipliedNumbers2[2] * s111 +
              multipliedNumbers2[3] * s111 - multipliedNumbers3[0] * s111 +
              multipliedNumbers3[1] * s121 + multipliedNumbers3[2] * s121 +
              multipliedNumbers3[3] * s121 +
              q1x * (-(q1y * s011) - multipliedNumbers4[2] + q1y * s111 + multipliedNumbers4[3]);

       _c1[0].kz = (-1 + multipliedNumbers1[0] + multipliedNumbers1[1] + multipliedNumbers1[2] + multipliedNumbers1[3]) *
              s002 +
              multipliedNumbers2[2] * s012 - multipliedNumbers2[3] * s012 +
              multipliedNumbers3[0] * s012 - multipliedNumbers3[1] * s022 -
              multipliedNumbers3[2] * s022 - multipliedNumbers3[3] * s022 + s102 -
              multipliedNumbers1[0] * s102 - multipliedNumbers1[1] * s102 - multipliedNumbers1[2] * s102 -
              multipliedNumbers1[3] * s102 - multipliedNumbers2[2] * s112 +
              multipliedNumbers2[3] * s112 - multipliedNumbers3[0] * s112 +
              multipliedNumbers3[1] * s122 + multipliedNumbers3[2] * s122 +
              multipliedNumbers3[3] * s122 +
              q1x * (-(q1y * s012) - multipliedNumbers5[0] + q1y * s112 + multipliedNumbers5[1]);

       _c1[0].kc = -t0x + t1x;

       // _c1[1]

       _c1[1].kx = -(multipliedNumbers2[3] * s000) - multipliedNumbers3[0] * s000 - s010 +
              multipliedNumbers1[1] * s010 + multipliedNumbers2[1] * s010 +
              multipliedNumbers1[3] * s010 - q1y * multipliedNumbers4[0] +
              qperpw * multipliedNumbers8[1] - multipliedNumbers16[0] * s020 +
              multipliedNumbers2[3] * s100 + multipliedNumbers3[0] * s100 +
              multipliedNumbers2[2] * (-s000 + s100) + multipliedNumbers2[0] * (s010 - s110) + s110 -
              multipliedNumbers1[1] * s110 - multipliedNumbers2[1] * s110 -
              multipliedNumbers1[3] * s110 +
              q1x * (q1y * (-s000 + s100) + q1w * (s020 - s120)) +
              q1y * multipliedNumbers4[1] - qperpw * multipliedNumbers12[1] +
              multipliedNumbers16[0] * s120;

       _c1[1].ky = -(multipliedNumbers2[3] * s001) - multipliedNumbers3[0] * s001 - s011 +
              multipliedNumbers1[1] * s011 + multipliedNumbers2[1] * s011 +
              multipliedNumbers1[3] * s011 - q1y * multipliedNumbers4[2] +
              qperpw * multipliedNumbers9[3] - multipliedNumbers16[0] * s021 +
              multipliedNumbers2[3] * s101 + multipliedNumbers3[0] * s101 +
              multipliedNumbers2[2] * (-s001 + s101) + multipliedNumbers2[0] * (s011 - s111) + s111 -
              multipliedNumbers1[1] * s111 - multipliedNumbers2[1] * s111 -
              multipliedNumbers1[3] * s111 +
              q1x * (q1y * (-s001 + s101) + q1w * (s021 - s121)) +
              q1y * multipliedNumbers4[3] - qperpw * multipliedNumbers14[0] +
              multipliedNumbers16[0] * s121;

_c1[1].kz = -(multipliedNumbers2[3] * s002) - multipliedNumbers3[0] * s002 - s012 +
              multipliedNumbers1[1] * s012 + multipliedNumbers2[1] * s012 +
              multipliedNumbers1[3] * s012 - q1y * multipliedNumbers5[0] +
              qperpw * multipliedNumbers11[3] - multipliedNumbers16[0] * s022 +
              multipliedNumbers2[3] * s102 + multipliedNumbers3[0] * s102 +
              multipliedNumbers2[2] * (-s002 + s102) + multipliedNumbers2[0] * (s012 - s112) + s112 -
              multipliedNumbers1[1] * s112 - multipliedNumbers2[1] * s112 -
              multipliedNumbers1[3] * s112 +
              q1x * (q1y * (-s002 + s102) + q1w * (s022 - s122)) +
              q1y * multipliedNumbers5[1] - qperpw * multipliedNumbers15[1] +
              multipliedNumbers16[0] * s122;

       _c1[1].kc = 0.f;

       // _c1[2]

       _c1[2].kx = (multipliedNumbers3[2] * s000 - multipliedNumbers3[3] * s000 -
                     multipliedNumbers25[2] * s010 - qperpw * multipliedNumbers7[1] -
                     multipliedNumbers16[0] * s010 - s020 + multipliedNumbers1[0] * s020 +
                     multipliedNumbers2[1] * s020 + multipliedNumbers1[2] * s020 -
                     multipliedNumbers3[2] * s100 + multipliedNumbers3[3] * s100 +
                     multipliedNumbers25[3] * (-s000 + s100) + multipliedNumbers25[2] * s110 +
                     qperpw * multipliedNumbers12[0] + multipliedNumbers16[0] * s110 +
                     q1w * (q1y * (s000 - s100) + q1x * (-s010 + s110)) +
                     multipliedNumbers2[0] * (s020 - s120) + s120 - multipliedNumbers1[0] * s120 -
                     multipliedNumbers2[1] * s120 - multipliedNumbers1[2] * s120);

       _c1[2].ky = (multipliedNumbers3[2] * s001 - multipliedNumbers3[3] * s001 -
       multipliedNumbers25[2] * s011 - qperpw * multipliedNumbers8[3] -
       multipliedNumbers16[0] * s011 - s021 + multipliedNumbers1[0] * s021 +
       multipliedNumbers2[1] * s021 + multipliedNumbers1[2] * s021 -
       multipliedNumbers3[2] * s101 + multipliedNumbers3[3] * s101 +
       multipliedNumbers25[3] * (-s001 + s101) + multipliedNumbers25[2] * s111 +
       qperpw * multipliedNumbers13[1] + multipliedNumbers16[0] * s111 +
       q1w * (q1y * (s001 - s101) + q1x * (-s011 + s111)) +
       multipliedNumbers2[0] * (s021 - s121) + s121 - multipliedNumbers1[0] * s121 -
       multipliedNumbers2[1] * s121 - multipliedNumbers1[2] * s121);

       _c1[2].kz = (multipliedNumbers3[2] * s002 - multipliedNumbers3[3] * s002 -
       multipliedNumbers25[2] * s012 - qperpw * multipliedNumbers10[3] -
       multipliedNumbers16[0] * s012 - s022 + multipliedNumbers1[0] * s022 +
       multipliedNumbers2[1] * s022 + multipliedNumbers1[2] * s022 -
       multipliedNumbers3[2] * s102 + multipliedNumbers3[3] * s102 +
       multipliedNumbers25[3] * (-s002 + s102) + multipliedNumbers25[2] * s112 +
       qperpw * multipliedNumbers14[1] + multipliedNumbers16[0] * s112 +
       q1w * (q1y * (s002 - s102) + q1x * (-s012 + s112)) +
       multipliedNumbers2[0] * (s022 - s122) + s122 - multipliedNumbers1[0] * s122 -
       multipliedNumbers2[1] * s122 - multipliedNumbers1[2] * s122);

       _c1[2].kc = -t0z + t1z;

       // _c2[0]

       _c2[0].kx = -(multipliedNumbers1[2] * s000) - multipliedNumbers1[3] * s000 +
              multipliedNumbers2[3] * s010 - multipliedNumbers3[0] * s010 +
              multipliedNumbers3[2] * s020 + multipliedNumbers3[3] * s020 +
              multipliedNumbers1[0] * (s000 - s100) + multipliedNumbers1[1] * (s000 - s100) +
              multipliedNumbers1[2] * s100 + multipliedNumbers1[3] * s100 -
              multipliedNumbers2[3] * s110 + multipliedNumbers3[0] * s110 -
              multipliedNumbers3[2] * s120 - multipliedNumbers3[3] * s120 +
              multipliedNumbers28[3] * qperpy * multipliedNumbers5[3] -
              2 * multipliedNumbers6[0] * multipliedNumbers5[3] +
              2 * multipliedNumbers6[1] * multipliedNumbers6[2] +
              2 * multipliedNumbers6[3] * multipliedNumbers6[2] +
              q1y *
              (q1x * (-s010 + s110) + q1w * (-s020 + s120) +
              2 * (-2 * multipliedNumbers7[0] + multipliedNumbers7[1] + multipliedNumbers7[2]) *
                     theta) +
              q1z * (q1w * (s010 - s110) + q1x * (-s020 + s120) -
                     2 * (2 * multipliedNumbers7[3] + multipliedNumbers8[0] - multipliedNumbers8[1]) *
                     theta);

       _c2[0].ky = -(multipliedNumbers1[2] * s001) - multipliedNumbers1[3] * s001 +
              multipliedNumbers2[3] * s011 - multipliedNumbers3[0] * s011 +
              multipliedNumbers3[2] * s021 + multipliedNumbers3[3] * s021 +
              multipliedNumbers1[0] * (s001 - s101) + multipliedNumbers1[1] * (s001 - s101) +
              multipliedNumbers1[2] * s101 + multipliedNumbers1[3] * s101 -
              multipliedNumbers2[3] * s111 + multipliedNumbers3[0] * s111 -
              multipliedNumbers3[2] * s121 - multipliedNumbers3[3] * s121 +
              multipliedNumbers28[3] * qperpy * multipliedNumbers15[3] -
              2 * multipliedNumbers6[0] * multipliedNumbers15[3] +
              2 * multipliedNumbers6[1] * multipliedNumbers10[0] +
              2 * multipliedNumbers6[3] * multipliedNumbers10[0] +
              q1y *
              (q1x * (-s011 + s111) + q1w * (-s021 + s121) +
              2 * (-2 * multipliedNumbers8[2] + multipliedNumbers8[3] + multipliedNumbers9[0]) *
                     theta) +
              q1z * (q1w * (s011 - s111) + q1x * (-s021 + s121) -
                     2 * (2 * multipliedNumbers9[1] + multipliedNumbers9[2] - multipliedNumbers9[3]) *
                     theta);

       _c2[0].kz = -(multipliedNumbers1[2] * s002) - multipliedNumbers1[3] * s002 +
              multipliedNumbers2[3] * s012 - multipliedNumbers3[0] * s012 +
              multipliedNumbers3[2] * s022 + multipliedNumbers3[3] * s022 +
              multipliedNumbers1[0] * (s002 - s102) + multipliedNumbers1[1] * (s002 - s102) +
              multipliedNumbers1[2] * s102 + multipliedNumbers1[3] * s102 -
              multipliedNumbers2[3] * s112 + multipliedNumbers3[0] * s112 -
              multipliedNumbers3[2] * s122 - multipliedNumbers3[3] * s122 +
              multipliedNumbers28[3] * multipliedNumbers28[1] * theta -
              2 * multipliedNumbers6[0] * s012 * theta +
              2 * multipliedNumbers6[1] * multipliedNumbers10[1] +
              2 * multipliedNumbers6[3] * multipliedNumbers10[1] +
              q1y *
              (q1x * (-s012 + s112) + q1w * (-s022 + s122) +
              2 * (-2 * multipliedNumbers10[2] + multipliedNumbers10[3] + multipliedNumbers11[0]) *
                     theta) +
              q1z * (q1w * (s012 - s112) + q1x * (-s022 + s122) -
                     2 * (2 * multipliedNumbers11[1] + multipliedNumbers11[2] - multipliedNumbers11[3]) *
                     theta);

       _c2[0].kc = 0.f;

       // _c2[1]

       _c2[1].kx = multipliedNumbers2[3] * s000 + multipliedNumbers3[0] * s000 + multipliedNumbers1[1] * s010 -
              multipliedNumbers2[1] * s010 - multipliedNumbers1[3] * s010 -
              q1y * multipliedNumbers4[0] - qperpw * multipliedNumbers8[1] +
              multipliedNumbers16[0] * s020 - multipliedNumbers2[3] * s100 -
              multipliedNumbers3[0] * s100 + multipliedNumbers2[0] * (s010 - s110) -
              multipliedNumbers1[1] * s110 + multipliedNumbers2[1] * s110 +
              multipliedNumbers1[3] * s110 + q1y * multipliedNumbers4[1] +
              qperpw * multipliedNumbers12[1] - multipliedNumbers16[0] * s120 +
              2 * multipliedNumbers16[2] * multipliedNumbers16[3] +
              2 * multipliedNumbers17[0] * multipliedNumbers16[3] -
              4 * multipliedNumbers17[1] * multipliedNumbers5[3] +
              2 * multipliedNumbers5[2] * multipliedNumbers6[2] +
              2 * multipliedNumbers17[2] * multipliedNumbers6[2] +
              q1x * (q1w * s020 + q1y * (-s000 + s100) - q1w * s120 +
                     2 * multipliedNumbers7[0] * theta - 4 * qperpx * multipliedNumbers5[3] -
                     multipliedNumbers29[3] * multipliedNumbers6[2]) +
              q1w * (-(multipliedNumbers17[3]) + multipliedNumbers18[0] + 2 * multipliedNumbers7[3] * theta -
                     multipliedNumbers29[2] * multipliedNumbers6[2]);

       _c2[1].ky = multipliedNumbers2[3] * s001 + multipliedNumbers3[0] * s001 + multipliedNumbers1[1] * s011 -
              multipliedNumbers2[1] * s011 - multipliedNumbers1[3] * s011 -
              q1y * multipliedNumbers4[2] - qperpw * multipliedNumbers9[3] +
              multipliedNumbers16[0] * s021 - multipliedNumbers2[3] * s101 -
              multipliedNumbers3[0] * s101 + multipliedNumbers2[0] * (s011 - s111) -
              multipliedNumbers1[1] * s111 + multipliedNumbers2[1] * s111 +
              multipliedNumbers1[3] * s111 + q1y * multipliedNumbers4[3] +
              qperpw * multipliedNumbers14[0] - multipliedNumbers16[0] * s121 +
              2 * multipliedNumbers16[2] * multipliedNumbers15[2] +
              2 * multipliedNumbers17[0] * multipliedNumbers15[2] -
              4 * multipliedNumbers17[1] * multipliedNumbers15[3] +
              2 * multipliedNumbers5[2] * multipliedNumbers10[0] +
              2 * multipliedNumbers17[2] * multipliedNumbers10[0] +
              q1x * (q1w * s021 + q1y * (-s001 + s101) - q1w * s121 +
                     2 * multipliedNumbers8[2] * theta - 4 * multipliedNumbers8[3] * theta -
                     2 * multipliedNumbers9[0] * theta) +
              q1w * (-(multipliedNumbers18[1]) + multipliedNumbers18[2] + 2 * multipliedNumbers9[1] * theta -
                     2 * multipliedNumbers9[3] * theta);

       _c2[1].kz = multipliedNumbers2[3] * s002 + multipliedNumbers3[0] * s002 + multipliedNumbers1[1] * s012 -
              multipliedNumbers2[1] * s012 - multipliedNumbers1[3] * s012 -
              q1y * multipliedNumbers5[0] - qperpw * multipliedNumbers11[3] +
              multipliedNumbers16[0] * s022 - multipliedNumbers2[3] * s102 -
              multipliedNumbers3[0] * s102 + multipliedNumbers2[0] * (s012 - s112) -
              multipliedNumbers1[1] * s112 + multipliedNumbers2[1] * s112 +
              multipliedNumbers1[3] * s112 + q1y * multipliedNumbers5[1] +
              qperpw * multipliedNumbers15[1] - multipliedNumbers16[0] * s122 +
              2 * multipliedNumbers16[2] * multipliedNumbers16[1] +
              2 * multipliedNumbers17[0] * multipliedNumbers16[1] -
              4 * multipliedNumbers17[1] * s012 * theta +
              2 * multipliedNumbers5[2] * multipliedNumbers10[1] +
              2 * multipliedNumbers17[2] * multipliedNumbers10[1] +
              q1x * (q1w * s022 + q1y * (-s002 + s102) - q1w * s122 +
                     2 * multipliedNumbers10[2] * theta - 4 * multipliedNumbers10[3] * theta -
                     multipliedNumbers29[3] * multipliedNumbers10[1]) +
              q1w * (-(multipliedNumbers18[3]) + q1z * s102 + 2 * multipliedNumbers11[1] * theta -
                     multipliedNumbers29[2] * multipliedNumbers10[1]);

       _c2[1].kc = 0.f;

       // _c2[2]

       _c2[2].kx = (multipliedNumbers3[1] * s000 - q1x * multipliedNumbers17[3] - multipliedNumbers3[2] * s000 +
       multipliedNumbers3[3] * s000 - multipliedNumbers25[0] * s010 - multipliedNumbers25[2] * s010 +
       qperpw * multipliedNumbers7[1] + multipliedNumbers16[0] * s010 +
       multipliedNumbers2[0] * s020 + multipliedNumbers1[0] * s020 - multipliedNumbers2[1] * s020 -
       multipliedNumbers1[2] * s020 - multipliedNumbers3[1] * s100 + q1x * multipliedNumbers18[0] +
       multipliedNumbers3[2] * s100 - multipliedNumbers3[3] * s100 +
       multipliedNumbers25[0] * s110 + multipliedNumbers25[2] * s110 - qperpw * multipliedNumbers12[0] -
       multipliedNumbers16[0] * s110 - multipliedNumbers2[0] * s120 - multipliedNumbers1[0] * s120 +
       multipliedNumbers2[1] * s120 + multipliedNumbers1[2] * s120 -
       2 * multipliedNumbers26[0] * multipliedNumbers16[3] + 2 * multipliedNumbers26[1] * multipliedNumbers16[3] -
       2 * multipliedNumbers6[1] * multipliedNumbers16[3] + 2 * multipliedNumbers6[3] * multipliedNumbers16[3] +
       2 * multipliedNumbers26[2] * multipliedNumbers5[3] + multipliedNumbers29[0] * qperpx * multipliedNumbers5[3] +
       2 * multipliedNumbers5[2] * multipliedNumbers5[3] + 2 * multipliedNumbers17[2] * multipliedNumbers5[3] -
       4 * multipliedNumbers26[3] * multipliedNumbers6[2] - 4 * multipliedNumbers27[0] * multipliedNumbers6[2]);

       _c2[2].ky = (multipliedNumbers3[1] * s001 - q1x * multipliedNumbers18[1] - multipliedNumbers3[2] * s001 +
       multipliedNumbers3[3] * s001 - multipliedNumbers25[0] * s011 - multipliedNumbers25[2] * s011 +
       qperpw * multipliedNumbers8[3] + multipliedNumbers16[0] * s011 +
       multipliedNumbers2[0] * s021 + multipliedNumbers1[0] * s021 - multipliedNumbers2[1] * s021 -
       multipliedNumbers1[2] * s021 - multipliedNumbers3[1] * s101 + q1x * multipliedNumbers18[2] +
       multipliedNumbers3[2] * s101 - multipliedNumbers3[3] * s101 +
       multipliedNumbers25[0] * s111 + multipliedNumbers25[2] * s111 - qperpw * multipliedNumbers13[1] -
       multipliedNumbers16[0] * s111 - multipliedNumbers2[0] * s121 - multipliedNumbers1[0] * s121 +
       multipliedNumbers2[1] * s121 + multipliedNumbers1[2] * s121 -
       2 * multipliedNumbers26[0] * multipliedNumbers15[2] + 2 * multipliedNumbers26[1] * multipliedNumbers15[2] -
       2 * multipliedNumbers6[1] * multipliedNumbers15[2] + 2 * multipliedNumbers6[3] * multipliedNumbers15[2] +
       multipliedNumbers28[3] * multipliedNumbers9[2] * theta + multipliedNumbers29[0] * multipliedNumbers8[3] * theta +
       2 * multipliedNumbers5[2] * multipliedNumbers15[3] + 2 * multipliedNumbers17[2] * multipliedNumbers15[3] -
       4 * q1x * multipliedNumbers9[3] * theta - 4 * multipliedNumbers27[0] * multipliedNumbers10[0]);

       _c2[2].kz = (multipliedNumbers3[1] * s002 - q1x * multipliedNumbers18[3] - multipliedNumbers3[2] * s002 +
       multipliedNumbers3[3] * s002 - multipliedNumbers25[0] * s012 - multipliedNumbers25[2] * s012 +
       qperpw * multipliedNumbers10[3] + multipliedNumbers16[0] * s012 +
       multipliedNumbers2[0] * s022 + multipliedNumbers1[0] * s022 - multipliedNumbers2[1] * s022 -
       multipliedNumbers1[2] * s022 - multipliedNumbers3[1] * s102 + multipliedNumbers25[3] * s102 +
       multipliedNumbers3[2] * s102 - multipliedNumbers3[3] * s102 +
       multipliedNumbers25[0] * s112 + multipliedNumbers25[2] * s112 - qperpw * multipliedNumbers14[1] -
       multipliedNumbers16[0] * s112 - multipliedNumbers2[0] * s122 - multipliedNumbers1[0] * s122 +
       multipliedNumbers2[1] * s122 + multipliedNumbers1[2] * s122 -
       2 * multipliedNumbers26[0] * multipliedNumbers16[1] + 2 * multipliedNumbers26[1] * multipliedNumbers16[1] -
       2 * multipliedNumbers6[1] * multipliedNumbers16[1] + 2 * multipliedNumbers6[3] * multipliedNumbers16[1] +
       multipliedNumbers28[3] * multipliedNumbers11[2] * theta + multipliedNumbers29[0] * multipliedNumbers10[3] * theta +
       2 * multipliedNumbers5[2] * s012 * theta + 2 * multipliedNumbers17[2] * s012 * theta -
       4 * multipliedNumbers26[3] * multipliedNumbers10[1] -
       4 * multipliedNumbers27[0] * multipliedNumbers10[1]);

       _c2[2].kc = 0.f;

       // _c3[0]

       _c3[0].kx = -2 * (q1x * multipliedNumbers27[1] - multipliedNumbers6[0] * s010 +
              multipliedNumbers6[1] * s020 + multipliedNumbers6[3] * s020 -
              q1x * multipliedNumbers27[2] + multipliedNumbers6[0] * s110 -
              multipliedNumbers6[1] * s120 - multipliedNumbers6[3] * s120 +
              q1y * (-2 * multipliedNumbers7[0] + multipliedNumbers7[1] + multipliedNumbers7[2] +
                     multipliedNumbers29[1] * s100 - multipliedNumbers12[0] - qperpw * s120) +
              q1z * (-2 * multipliedNumbers7[3] - multipliedNumbers8[0] + multipliedNumbers8[1] +
                     2 * multipliedNumbers12[2] + multipliedNumbers12[3] - multipliedNumbers12[1])) *
              theta;

       _c3[0].ky = -2 * (q1x * multipliedNumbers27[3] - multipliedNumbers6[0] * s011 +
              multipliedNumbers6[1] * s021 + multipliedNumbers6[3] * s021 -
              q1x * multipliedNumbers28[0] + multipliedNumbers6[0] * s111 -
              multipliedNumbers6[1] * s121 - multipliedNumbers6[3] * s121 +
              q1y * (-2 * multipliedNumbers8[2] + multipliedNumbers8[3] + multipliedNumbers9[0] +
                     multipliedNumbers29[1] * s101 - multipliedNumbers13[1] - multipliedNumbers13[2]) +
              q1z * (-2 * multipliedNumbers9[1] - multipliedNumbers9[2] + multipliedNumbers9[3] +
                     2 * multipliedNumbers13[3] + multipliedNumbers13[0] - multipliedNumbers14[0])) *
              theta;

       _c3[0].kz = -2 * (q1x * multipliedNumbers28[1] - multipliedNumbers6[0] * s012 +
              multipliedNumbers6[1] * s022 + multipliedNumbers6[3] * s022 -
              q1x * multipliedNumbers28[2] + multipliedNumbers6[0] * s112 -
              multipliedNumbers6[1] * s122 - multipliedNumbers6[3] * s122 +
              q1y * (-2 * multipliedNumbers10[2] + multipliedNumbers10[3] + multipliedNumbers11[0] +
                     multipliedNumbers29[1] * s102 - multipliedNumbers14[1] - multipliedNumbers14[2]) +
              q1z * (-2 * multipliedNumbers11[1] - multipliedNumbers11[2] + multipliedNumbers11[3] +
                     2 * multipliedNumbers14[3] + multipliedNumbers15[0] - multipliedNumbers15[1])) *
              theta;

       _c3[0].kc = 0.f;

       // _c3[1]

       _c3[1].kx = 2 * (-(q1x * multipliedNumbers7[0]) - multipliedNumbers6[0] * s000 +
              multipliedNumbers28[3] * multipliedNumbers7[1] + q1x * multipliedNumbers7[2] +
              q1w * multipliedNumbers8[1] + q1x * qperpy * s100 +
              multipliedNumbers6[0] * s100 - multipliedNumbers28[3] * multipliedNumbers12[0] -
              multipliedNumbers26[2] * s120 - q1w * multipliedNumbers12[1] +
              q1z * (2 * multipliedNumbers19[0] - multipliedNumbers19[1] +
                     qperpw * (-s000 + s100) - 2 * multipliedNumbers19[2] +
                     multipliedNumbers19[3]) +
              q1y * (-(multipliedNumbers20[0]) - multipliedNumbers20[1] + multipliedNumbers20[2] +
                     multipliedNumbers20[3])) *
              theta;

       _c3[1].ky = 2 * (-(q1x * multipliedNumbers8[2]) - multipliedNumbers6[0] * s001 +
              multipliedNumbers28[3] * multipliedNumbers8[3] + q1x * multipliedNumbers9[0] +
              q1w * multipliedNumbers9[3] + q1x * qperpy * s101 +
              multipliedNumbers6[0] * s101 - multipliedNumbers28[3] * multipliedNumbers13[1] -
              q1x * multipliedNumbers13[2] - q1w * multipliedNumbers14[0] +
              q1z * (2 * multipliedNumbers21[0] - multipliedNumbers21[1] +
                     qperpw * (-s001 + s101) - 2 * multipliedNumbers21[2] +
                     multipliedNumbers21[3]) +
              q1y * (-(multipliedNumbers22[0]) - multipliedNumbers22[1] + multipliedNumbers22[2] +
                     multipliedNumbers22[3])) *
              theta;

       _c3[1].kz = 2 * (-(q1x * multipliedNumbers10[2]) - multipliedNumbers6[0] * s002 +
              multipliedNumbers28[3] * multipliedNumbers10[3] + q1x * multipliedNumbers11[0] +
              q1w * multipliedNumbers11[3] + q1x * qperpy * s102 +
              multipliedNumbers6[0] * s102 - multipliedNumbers28[3] * multipliedNumbers14[1] -
              q1x * multipliedNumbers14[2] - q1w * multipliedNumbers15[1] +
              q1z * (2 * multipliedNumbers23[0] - multipliedNumbers23[1] +
                     qperpw * (-s002 + s102) - 2 * multipliedNumbers23[2] +
                     multipliedNumbers23[3]) +
              q1y * (-(multipliedNumbers24[0]) - multipliedNumbers24[1] + multipliedNumbers24[2] +
                     multipliedNumbers24[3])) *
              theta;

       _c3[1].kc = 0.f;

       // _c3[2]

       _c3[2].kx = -2 * (-(multipliedNumbers6[1] * s000) + multipliedNumbers6[3] * s000 +
                     q1x * multipliedNumbers8[0] + q1w * multipliedNumbers7[1] -
                     multipliedNumbers28[3] * multipliedNumbers8[1] + multipliedNumbers6[1] * s100 -
                     multipliedNumbers6[3] * s100 - q1x * multipliedNumbers12[3] -
                     q1w * multipliedNumbers12[0] +
                     q1z * (multipliedNumbers20[0] + multipliedNumbers27[1] - multipliedNumbers20[2] -
                            multipliedNumbers27[2]) +
                     multipliedNumbers28[3] * multipliedNumbers12[1] +
                     q1y * (multipliedNumbers19[0] - 2 * multipliedNumbers19[1] +
                            qperpw * (-s000 + s100) - multipliedNumbers19[2] +
                            2 * multipliedNumbers19[3])) *
              theta;


       _c3[2].ky = -2 * (-(multipliedNumbers6[1] * s001) + multipliedNumbers6[3] * s001 +
              q1x * multipliedNumbers9[2] + q1w * multipliedNumbers8[3] -
              multipliedNumbers28[3] * multipliedNumbers9[3] + multipliedNumbers6[1] * s101 -
              multipliedNumbers6[3] * s101 - q1x * multipliedNumbers13[0] -
              q1w * multipliedNumbers13[1] +
              q1z * (multipliedNumbers22[0] + multipliedNumbers27[3] - multipliedNumbers22[2] -
                     multipliedNumbers28[0]) +
              multipliedNumbers28[3] * multipliedNumbers14[0] +
              q1y * (multipliedNumbers21[0] - 2 * multipliedNumbers21[1] +
                     qperpw * (-s001 + s101) - multipliedNumbers21[2] +
                     2 * multipliedNumbers21[3])) *
              theta;

       _c3[2].kz = -2 * (-(multipliedNumbers6[1] * s002) + multipliedNumbers6[3] * s002 +
              q1x * multipliedNumbers11[2] + q1w * multipliedNumbers10[3] -
              multipliedNumbers28[3] * multipliedNumbers11[3] + multipliedNumbers6[1] * s102 -
              multipliedNumbers6[3] * s102 - q1x * multipliedNumbers15[0] -
              q1w * multipliedNumbers14[1] +
              q1z * (multipliedNumbers24[0] + multipliedNumbers28[1] - multipliedNumbers24[2] -
                     multipliedNumbers28[2]) +
              multipliedNumbers28[3] * multipliedNumbers15[1] +
              q1y * (multipliedNumbers23[0] - 2 * multipliedNumbers23[1] +
                     qperpw * (-s002 + s102) - multipliedNumbers23[2] +
                     2 * multipliedNumbers23[3])) *
              theta;

       _c3[2].kc = 0.f;

       // _c4[0]

       _c4[0].kx = -(q1x * multipliedNumbers27[1]) + multipliedNumbers6[0] * s010 - multipliedNumbers6[1] * s020 -
              multipliedNumbers6[3] * s020 + q1x * multipliedNumbers27[2] -
              multipliedNumbers6[0] * s110 + multipliedNumbers6[1] * s120 +
              multipliedNumbers6[3] * s120 + 2 * multipliedNumbers1[0] * multipliedNumbers16[3] +
              2 * multipliedNumbers1[1] * multipliedNumbers16[3] -
              2 * multipliedNumbers1[2] * multipliedNumbers16[3] -
              2 * multipliedNumbers1[3] * multipliedNumbers16[3] +
              2 * multipliedNumbers2[3] * multipliedNumbers5[3] -
              2 * multipliedNumbers3[0] * multipliedNumbers5[3] +
              2 * multipliedNumbers3[2] * multipliedNumbers6[2] +
              2 * multipliedNumbers3[3] * multipliedNumbers6[2] +
              q1y * (-(multipliedNumbers7[1]) - multipliedNumbers7[2] +
                     multipliedNumbers29[1] * (s000 - s100) + multipliedNumbers12[0] +
                     qperpw * s120 - multipliedNumbers28[3] * multipliedNumbers5[3] -
                     multipliedNumbers29[0] * multipliedNumbers6[2]) +
              q1z * (2 * multipliedNumbers7[3] + multipliedNumbers8[0] - multipliedNumbers8[1] -
                     2 * multipliedNumbers12[2] - multipliedNumbers12[3] + multipliedNumbers12[1] +
                     multipliedNumbers29[0] * multipliedNumbers5[3] - multipliedNumbers28[3] * multipliedNumbers6[2]);

       _c4[0].ky = -(q1x * multipliedNumbers27[3]) + multipliedNumbers6[0] * s011 - multipliedNumbers6[1] * s021 -
              multipliedNumbers6[3] * s021 + q1x * multipliedNumbers28[0] -
              multipliedNumbers6[0] * s111 + multipliedNumbers6[1] * s121 +
              multipliedNumbers6[3] * s121 + 2 * multipliedNumbers1[0] * multipliedNumbers15[2] +
              2 * multipliedNumbers1[1] * multipliedNumbers15[2] -
              2 * multipliedNumbers1[2] * multipliedNumbers15[2] -
              2 * multipliedNumbers1[3] * multipliedNumbers15[2] +
              2 * multipliedNumbers2[3] * multipliedNumbers15[3] -
              2 * multipliedNumbers3[0] * multipliedNumbers15[3] +
              2 * multipliedNumbers3[2] * multipliedNumbers10[0] +
              2 * multipliedNumbers3[3] * multipliedNumbers10[0] +
              q1y * (-(multipliedNumbers8[3]) - multipliedNumbers9[0] +
                     multipliedNumbers29[1] * (s001 - s101) + multipliedNumbers13[1] +
                     multipliedNumbers13[2] - multipliedNumbers28[3] * multipliedNumbers15[3] -
                     multipliedNumbers29[0] * multipliedNumbers10[0]) +
              q1z * (2 * multipliedNumbers9[1] + multipliedNumbers9[2] - multipliedNumbers9[3] -
                     2 * multipliedNumbers13[3] - multipliedNumbers13[0] + multipliedNumbers14[0] +
                     multipliedNumbers29[0] * multipliedNumbers15[3] - multipliedNumbers28[3] * multipliedNumbers10[0]);

       _c4[0].kz = -(q1x * multipliedNumbers28[1]) + multipliedNumbers6[0] * s012 - multipliedNumbers6[1] * s022 -
              multipliedNumbers6[3] * s022 + q1x * multipliedNumbers28[2] -
              multipliedNumbers6[0] * s112 + multipliedNumbers6[1] * s122 +
              multipliedNumbers6[3] * s122 + 2 * multipliedNumbers1[0] * multipliedNumbers16[1] +
              2 * multipliedNumbers1[1] * multipliedNumbers16[1] -
              2 * multipliedNumbers1[2] * multipliedNumbers16[1] -
              2 * multipliedNumbers1[3] * multipliedNumbers16[1] +
              2 * multipliedNumbers2[3] * s012 * theta -
              2 * multipliedNumbers3[0] * s012 * theta +
              2 * multipliedNumbers3[2] * multipliedNumbers10[1] +
              2 * multipliedNumbers3[3] * multipliedNumbers10[1] +
              q1y * (-(multipliedNumbers10[3]) - multipliedNumbers11[0] +
                     multipliedNumbers29[1] * (s002 - s102) + multipliedNumbers14[1] +
                     multipliedNumbers14[2] - multipliedNumbers28[3] * s012 * theta -
                     multipliedNumbers29[0] * multipliedNumbers10[1]) +
              q1z * (2 * multipliedNumbers11[1] + multipliedNumbers11[2] - multipliedNumbers11[3] -
                     2 * multipliedNumbers14[3] - multipliedNumbers15[0] + multipliedNumbers15[1] +
                     multipliedNumbers29[0] * s012 * theta - multipliedNumbers28[3] * multipliedNumbers10[1]);

       _c4[0].kc = 0.f;

       // _c4[1]

       _c4[1].kx = -(q1x * multipliedNumbers7[0]) - multipliedNumbers6[0] * s000 +
              multipliedNumbers28[3] * multipliedNumbers7[1] + q1x * multipliedNumbers7[2] +
              q1w * multipliedNumbers8[1] + q1x * qperpy * s100 +
              multipliedNumbers6[0] * s100 - multipliedNumbers28[3] * multipliedNumbers12[0] -
              multipliedNumbers26[2] * s120 - q1w * multipliedNumbers12[1] +
              2 * multipliedNumbers2[3] * multipliedNumbers16[3] +
              2 * multipliedNumbers3[0] * multipliedNumbers16[3] +
              2 * multipliedNumbers2[0] * multipliedNumbers5[3] + 2 * multipliedNumbers1[1] * multipliedNumbers5[3] -
              2 * multipliedNumbers2[1] * multipliedNumbers5[3] -
              2 * multipliedNumbers1[3] * multipliedNumbers5[3] +
              2 * multipliedNumbers25[0] * multipliedNumbers6[2] -
              2 * multipliedNumbers25[1] * multipliedNumbers6[2] +
              2 * multipliedNumbers16[0] * multipliedNumbers6[2] +
              q1y * (-(multipliedNumbers20[0]) - multipliedNumbers20[1] + multipliedNumbers20[2] +
                     multipliedNumbers20[3] - multipliedNumbers28[3] * multipliedNumbers16[3]) +
              q1z * (2 * multipliedNumbers19[0] - multipliedNumbers19[1] +
                     qperpw * (-s000 + s100) - 2 * multipliedNumbers19[2] +
                     multipliedNumbers19[3] - multipliedNumbers29[0] * multipliedNumbers16[3] -
                     multipliedNumbers30[0] * multipliedNumbers6[2]);

       _c4[1].ky = -(q1x * multipliedNumbers8[2]) - multipliedNumbers6[0] * s001 +
              multipliedNumbers28[3] * multipliedNumbers8[3] + q1x * multipliedNumbers9[0] +
              q1w * multipliedNumbers9[3] + q1x * qperpy * s101 +
              multipliedNumbers6[0] * s101 - multipliedNumbers28[3] * multipliedNumbers13[1] -
              q1x * multipliedNumbers13[2] - q1w * multipliedNumbers14[0] +
              2 * multipliedNumbers2[3] * multipliedNumbers15[2] +
              2 * multipliedNumbers3[0] * multipliedNumbers15[2] +
              2 * multipliedNumbers2[0] * multipliedNumbers15[3] + 2 * multipliedNumbers1[1] * multipliedNumbers15[3] -
              2 * multipliedNumbers2[1] * multipliedNumbers15[3] -
              2 * multipliedNumbers1[3] * multipliedNumbers15[3] +
              2 * multipliedNumbers25[0] * multipliedNumbers10[0] -
              multipliedNumbers29[3] * multipliedNumbers9[3] * theta +
              2 * multipliedNumbers16[0] * multipliedNumbers10[0] +
              q1y * (-(multipliedNumbers22[0]) - multipliedNumbers22[1] + multipliedNumbers22[2] +
                     multipliedNumbers22[3] - multipliedNumbers28[3] * multipliedNumbers15[2]) +
              q1z * (2 * multipliedNumbers21[0] - multipliedNumbers21[1] +
                     qperpw * (-s001 + s101) - 2 * multipliedNumbers21[2] +
                     multipliedNumbers21[3] - multipliedNumbers29[0] * multipliedNumbers15[2] -
                     multipliedNumbers30[0] * multipliedNumbers10[0]);

       _c4[1].kz = -(q1x * multipliedNumbers10[2]) - multipliedNumbers6[0] * s002 +
              multipliedNumbers28[3] * multipliedNumbers10[3] + q1x * multipliedNumbers11[0] +
              q1w * multipliedNumbers11[3] + q1x * qperpy * s102 +
              multipliedNumbers6[0] * s102 - multipliedNumbers28[3] * multipliedNumbers14[1] -
              q1x * multipliedNumbers14[2] - q1w * multipliedNumbers15[1] +
              2 * multipliedNumbers2[3] * multipliedNumbers16[1] +
              2 * multipliedNumbers3[0] * multipliedNumbers16[1] +
              2 * multipliedNumbers2[0] * s012 * theta + 2 * multipliedNumbers1[1] * s012 * theta -
              2 * multipliedNumbers2[1] * s012 * theta -
              2 * multipliedNumbers1[3] * s012 * theta +
              2 * multipliedNumbers25[0] * multipliedNumbers10[1] -
              2 * multipliedNumbers25[1] * multipliedNumbers10[1] +
              2 * multipliedNumbers16[0] * multipliedNumbers10[1] +
              q1y * (-(multipliedNumbers24[0]) - multipliedNumbers24[1] + multipliedNumbers24[2] +
                     multipliedNumbers24[3] - multipliedNumbers28[3] * multipliedNumbers16[1]) +
              q1z * (2 * multipliedNumbers23[0] - multipliedNumbers23[1] +
                     qperpw * (-s002 + s102) - 2 * multipliedNumbers23[2] +
                     multipliedNumbers23[3] - multipliedNumbers29[0] * multipliedNumbers16[1] -
                     multipliedNumbers30[0] * multipliedNumbers10[1]);

       _c4[1].kc = 0.f;

       // _c4[2]

       _c4[2].kx = multipliedNumbers6[1] * s000 - multipliedNumbers6[3] * s000 - q1x * multipliedNumbers8[0] -
              q1w * multipliedNumbers7[1] + multipliedNumbers28[3] * multipliedNumbers8[1] -
              multipliedNumbers6[1] * s100 + multipliedNumbers6[3] * s100 +
              q1x * multipliedNumbers12[3] + q1w * multipliedNumbers12[0] -
              multipliedNumbers28[3] * multipliedNumbers12[1] - 2 * multipliedNumbers3[2] * multipliedNumbers16[3] +
              2 * multipliedNumbers3[3] * multipliedNumbers16[3] -
              2 * multipliedNumbers25[0] * multipliedNumbers5[3] +
              2 * multipliedNumbers25[1] * multipliedNumbers5[3] +
              2 * multipliedNumbers16[0] * multipliedNumbers5[3] +
              2 * multipliedNumbers2[0] * multipliedNumbers6[2] + 2 * multipliedNumbers1[0] * multipliedNumbers6[2] -
              2 * multipliedNumbers2[1] * multipliedNumbers6[2] -
              2 * multipliedNumbers1[2] * multipliedNumbers6[2] +
              q1z * (-(multipliedNumbers20[0]) - multipliedNumbers27[1] + multipliedNumbers20[2] +
                     multipliedNumbers27[2] - multipliedNumbers28[3] * multipliedNumbers16[3]) +
              q1y * (-(multipliedNumbers19[0]) + 2 * multipliedNumbers19[1] +
                     qperpw * (s000 - s100) + multipliedNumbers19[2] -
                     2 * multipliedNumbers19[3] + multipliedNumbers29[0] * multipliedNumbers16[3] -
                     multipliedNumbers30[1] * multipliedNumbers5[3]);

       _c4[2].ky = multipliedNumbers6[1] * s001 - multipliedNumbers6[3] * s001 - q1x * multipliedNumbers9[2] -
              q1w * multipliedNumbers8[3] + multipliedNumbers28[3] * multipliedNumbers9[3] -
              multipliedNumbers6[1] * s101 + multipliedNumbers6[3] * s101 +
              q1x * multipliedNumbers13[0] + q1w * multipliedNumbers13[1] -
              multipliedNumbers28[3] * multipliedNumbers14[0] - 2 * multipliedNumbers3[2] * multipliedNumbers15[2] +
              2 * multipliedNumbers3[3] * multipliedNumbers15[2] -
              2 * multipliedNumbers25[0] * multipliedNumbers15[3] +
              multipliedNumbers29[3] * multipliedNumbers8[3] * theta +
              2 * multipliedNumbers16[0] * multipliedNumbers15[3] +
              2 * multipliedNumbers2[0] * multipliedNumbers10[0] + 2 * multipliedNumbers1[0] * multipliedNumbers10[0] -
              2 * multipliedNumbers2[1] * multipliedNumbers10[0] -
              2 * multipliedNumbers1[2] * multipliedNumbers10[0] +
              q1z * (-(multipliedNumbers22[0]) - multipliedNumbers27[3] + multipliedNumbers22[2] +
                     multipliedNumbers28[0] - multipliedNumbers28[3] * multipliedNumbers15[2]) +
              q1y * (-(multipliedNumbers21[0]) + 2 * multipliedNumbers21[1] +
                     qperpw * (s001 - s101) + multipliedNumbers21[2] -
                     2 * multipliedNumbers21[3] + multipliedNumbers29[0] * multipliedNumbers15[2] -
                     multipliedNumbers30[1] * multipliedNumbers15[3]);

       _c4[2].kz = multipliedNumbers6[1] * s002 - multipliedNumbers6[3] * s002 - q1x * multipliedNumbers11[2] -
              q1w * multipliedNumbers10[3] + multipliedNumbers28[3] * multipliedNumbers11[3] -
              multipliedNumbers6[1] * s102 + multipliedNumbers6[3] * s102 +
              q1x * multipliedNumbers15[0] + q1w * multipliedNumbers14[1] -
              multipliedNumbers28[3] * multipliedNumbers15[1] - 2 * multipliedNumbers3[2] * multipliedNumbers16[1] +
              2 * multipliedNumbers3[3] * multipliedNumbers16[1] -
              2 * multipliedNumbers25[0] * s012 * theta +
              multipliedNumbers29[3] * multipliedNumbers10[3] * theta +
              2 * multipliedNumbers16[0] * s012 * theta +
              2 * multipliedNumbers2[0] * multipliedNumbers10[1] + 2 * multipliedNumbers1[0] * multipliedNumbers10[1] -
              2 * multipliedNumbers2[1] * multipliedNumbers10[1] -
              2 * multipliedNumbers1[2] * multipliedNumbers10[1] +
              q1z * (-(multipliedNumbers24[0]) - multipliedNumbers28[1] + multipliedNumbers24[2] +
                     multipliedNumbers28[2] - multipliedNumbers28[3] * multipliedNumbers16[1]) +
              q1y * (-(multipliedNumbers23[0]) + 2 * multipliedNumbers23[1] +
                     qperpw * (s002 - s102) + multipliedNumbers23[2] -
                     2 * multipliedNumbers23[3] + multipliedNumbers29[0] * multipliedNumbers16[1] -
                     multipliedNumbers30[1] * s012 * theta);

       _c4[2].kc = 0.f;

       // _c5[0]

       _c5[0].kx = 2 * (multipliedNumbers1[2] * s000 + multipliedNumbers1[3] * s000 -
              multipliedNumbers2[3] * s010 + multipliedNumbers3[0] * s010 -
              multipliedNumbers3[2] * s020 - multipliedNumbers3[3] * s020 -
              multipliedNumbers1[2] * s100 - multipliedNumbers1[3] * s100 +
              multipliedNumbers1[0] * (-s000 + s100) + multipliedNumbers1[1] * (-s000 + s100) +
              multipliedNumbers2[3] * s110 - multipliedNumbers3[0] * s110 +
              q1y * (q1x * (s010 - s110) + q1w * (s020 - s120)) +
              multipliedNumbers3[2] * s120 + multipliedNumbers3[3] * s120 +
              q1z * (-(q1w * s010) + q1x * s020 + q1w * s110 - q1x * s120)) *
              theta;

       _c5[0].ky = 2 * (multipliedNumbers1[2] * s001 + multipliedNumbers1[3] * s001 -
              multipliedNumbers2[3] * s011 + multipliedNumbers3[0] * s011 -
              multipliedNumbers3[2] * s021 - multipliedNumbers3[3] * s021 -
              multipliedNumbers1[2] * s101 - multipliedNumbers1[3] * s101 +
              multipliedNumbers1[0] * (-s001 + s101) + multipliedNumbers1[1] * (-s001 + s101) +
              multipliedNumbers2[3] * s111 - multipliedNumbers3[0] * s111 +
              q1y * (q1x * (s011 - s111) + q1w * (s021 - s121)) +
              multipliedNumbers3[2] * s121 + multipliedNumbers3[3] * s121 +
              q1z * (-(q1w * s011) + q1x * s021 + q1w * s111 - q1x * s121)) *
              theta;

       _c5[0].kz = 2 * (multipliedNumbers1[2] * s002 + multipliedNumbers1[3] * s002 -
              multipliedNumbers2[3] * s012 + multipliedNumbers3[0] * s012 -
              multipliedNumbers3[2] * s022 - multipliedNumbers3[3] * s022 -
              multipliedNumbers1[2] * s102 - multipliedNumbers1[3] * s102 +
              multipliedNumbers1[0] * (-s002 + s102) + multipliedNumbers1[1] * (-s002 + s102) +
              multipliedNumbers2[3] * s112 - multipliedNumbers3[0] * s112 +
              q1y * (q1x * (s012 - s112) + q1w * (s022 - s122)) +
              multipliedNumbers3[2] * s122 + multipliedNumbers3[3] * s122 +
              q1z * (-(q1w * s012) + q1x * s022 + q1w * s112 - q1x * s122)) *
              theta;

       _c5[0].kc = 0.f;

       // _c5[1]

       _c5[1].kx = -2 * (multipliedNumbers2[3] * s000 + multipliedNumbers3[0] * s000 +
                     multipliedNumbers1[1] * s010 - multipliedNumbers2[1] * s010 -
                     multipliedNumbers1[3] * s010 - q1y * multipliedNumbers4[0] -
                     qperpw * multipliedNumbers8[1] + multipliedNumbers16[0] * s020 -
                     multipliedNumbers2[3] * s100 - multipliedNumbers3[0] * s100 +
                     multipliedNumbers2[2] * (-s000 + s100) + multipliedNumbers2[0] * (s010 - s110) -
                     multipliedNumbers1[1] * s110 + multipliedNumbers2[1] * s110 +
                     multipliedNumbers1[3] * s110 +
                     q1x * (q1y * (-s000 + s100) + q1w * (s020 - s120)) +
                     q1y * multipliedNumbers4[1] + qperpw * multipliedNumbers12[1] -
                     multipliedNumbers16[0] * s120) *
              theta;

       _c5[1].ky = -2 * (multipliedNumbers2[3] * s001 + multipliedNumbers3[0] * s001 +
              multipliedNumbers1[1] * s011 - multipliedNumbers2[1] * s011 -
              multipliedNumbers1[3] * s011 - q1y * multipliedNumbers4[2] -
              qperpw * multipliedNumbers9[3] + multipliedNumbers16[0] * s021 -
              multipliedNumbers2[3] * s101 - multipliedNumbers3[0] * s101 +
              multipliedNumbers2[2] * (-s001 + s101) + multipliedNumbers2[0] * (s011 - s111) -
              multipliedNumbers1[1] * s111 + multipliedNumbers2[1] * s111 +
              multipliedNumbers1[3] * s111 +
              q1x * (q1y * (-s001 + s101) + q1w * (s021 - s121)) +
              q1y * multipliedNumbers4[3] + qperpw * multipliedNumbers14[0] -
              multipliedNumbers16[0] * s121) *
              theta;

       _c5[1].kz = -2 * (multipliedNumbers2[3] * s002 + multipliedNumbers3[0] * s002 +
              multipliedNumbers1[1] * s012 - multipliedNumbers2[1] * s012 -
              multipliedNumbers1[3] * s012 - q1y * multipliedNumbers5[0] -
              qperpw * multipliedNumbers11[3] + multipliedNumbers16[0] * s022 -
              multipliedNumbers2[3] * s102 - multipliedNumbers3[0] * s102 +
              multipliedNumbers2[2] * (-s002 + s102) + multipliedNumbers2[0] * (s012 - s112) -
              multipliedNumbers1[1] * s112 + multipliedNumbers2[1] * s112 +
              multipliedNumbers1[3] * s112 +
              q1x * (q1y * (-s002 + s102) + q1w * (s022 - s122)) +
              q1y * multipliedNumbers5[1] + qperpw * multipliedNumbers15[1] -
              multipliedNumbers16[0] * s122) *
              theta;

       _c5[1].kc = 0.f;

       // _c5[2]

       _c5[2].kx = 2 * (multipliedNumbers3[2] * s000 - multipliedNumbers3[3] * s000 +
              multipliedNumbers25[2] * s010 - qperpw * multipliedNumbers7[1] -
              multipliedNumbers16[0] * s010 - multipliedNumbers1[0] * s020 +
              multipliedNumbers2[1] * s020 + multipliedNumbers1[2] * s020 +
              multipliedNumbers25[3] * (s000 - s100) - multipliedNumbers3[2] * s100 +
              multipliedNumbers3[3] * s100 +
              q1w * (q1y * (-s000 + s100) + q1x * (s010 - s110)) -
              multipliedNumbers25[2] * s110 + qperpw * multipliedNumbers12[0] +
              multipliedNumbers16[0] * s110 + multipliedNumbers1[0] * s120 -
              multipliedNumbers2[1] * s120 - multipliedNumbers1[2] * s120 +
              multipliedNumbers2[0] * (-s020 + s120)) *
              theta;

       _c5[2].ky = 2 * (multipliedNumbers3[2] * s001 - multipliedNumbers3[3] * s001 +
              multipliedNumbers25[2] * s011 - qperpw * multipliedNumbers8[3] -
              multipliedNumbers16[0] * s011 - multipliedNumbers1[0] * s021 +
              multipliedNumbers2[1] * s021 + multipliedNumbers1[2] * s021 +
              multipliedNumbers25[3] * (s001 - s101) - multipliedNumbers3[2] * s101 +
              multipliedNumbers3[3] * s101 +
              q1w * (q1y * (-s001 + s101) + q1x * (s011 - s111)) -
              multipliedNumbers25[2] * s111 + qperpw * multipliedNumbers13[1] +
              multipliedNumbers16[0] * s111 + multipliedNumbers1[0] * s121 -
              multipliedNumbers2[1] * s121 - multipliedNumbers1[2] * s121 +
              multipliedNumbers2[0] * (-s021 + s121)) *
              theta;

       _c5[2].kz = 2 * (multipliedNumbers3[2] * s002 - multipliedNumbers3[3] * s002 +
              multipliedNumbers25[2] * s012 - qperpw * multipliedNumbers10[3] -
              multipliedNumbers16[0] * s012 - multipliedNumbers1[0] * s022 +
              multipliedNumbers2[1] * s022 + multipliedNumbers1[2] * s022 +
              multipliedNumbers25[3] * (s002 - s102) - multipliedNumbers3[2] * s102 +
              multipliedNumbers3[3] * s102 +
              q1w * (q1y * (-s002 + s102) + q1x * (s012 - s112)) -
              multipliedNumbers25[2] * s112 + qperpw * multipliedNumbers14[1] +
              multipliedNumbers16[0] * s112 + multipliedNumbers1[0] * s122 -
              multipliedNumbers2[1] * s122 - multipliedNumbers1[2] * s122 +
              multipliedNumbers2[0] * (-s022 + s122)) *
              theta;

       _c5[2].kc = 0.f;
    }

    void AnimatedTransform::decompose(const Matrix4x4f& m, Vec3<float>& translate, Quaternion& rotate, Matrix4x4f& scale)
    {
       translate[0] = m[0][3];
       translate[1] = m[1][3];
       translate[2] = m[2][3];

       auto cm = m;
       float norm;
       int count = 0;

       cm[0][3] = cm[1][3] = cm[2][3] = 0.f;
       cm[3][0] = cm[3][1] = cm[2][3] = 0.f;
       cm[3][3] = 1.f;

       auto r = cm;
       __m128 vec_0_5 = _mm_set_ps(0.5f, 0.5f, 0.5f, 0.5f);
       __m128 n0, n1, n2;

       do {
              Matrix4x4f rNext;
              Matrix4x4f rIt = inverse(transpose(r));
              norm = 0.0f;

              *reinterpret_cast<__m128*>(rNext[0]) = _mm_mul_ps(vec_0_5, _mm_add_ps(*reinterpret_cast<const __m128*>(r[0]), *reinterpret_cast<const __m128*>(rIt[0])));
              *reinterpret_cast<__m128*>(rNext[1]) = _mm_mul_ps(vec_0_5, _mm_add_ps(*reinterpret_cast<const __m128*>(r[1]), *reinterpret_cast<const __m128*>(rIt[1])));
              *reinterpret_cast<__m128*>(rNext[2]) = _mm_mul_ps(vec_0_5, _mm_add_ps(*reinterpret_cast<const __m128*>(r[2]), *reinterpret_cast<const __m128*>(rIt[2])));
              *reinterpret_cast<__m128*>(rNext[3]) = _mm_mul_ps(vec_0_5, _mm_add_ps(*reinterpret_cast<const __m128*>(r[3]), *reinterpret_cast<const __m128*>(rIt[3])));

              n0 = _mm_sub_ps(*reinterpret_cast<const __m128*>(r[0]), *reinterpret_cast<const __m128*>(rNext[0]));
              n1 = _mm_sub_ps(*reinterpret_cast<const __m128*>(r[1]), *reinterpret_cast<const __m128*>(rNext[1]));
              n2 = _mm_sub_ps(*reinterpret_cast<const __m128*>(r[2]), *reinterpret_cast<const __m128*>(rNext[2]));

           norm = max(norm, max(std::abs(n0[0]) + std::abs(n0[1]) + std::abs(n0[2]), max(std::abs(n1[0]) + std::abs(n1[1]) + std::abs(n1[2]), std::abs(n2[0]) + std::abs(n2[1]) + std::abs(n2[2]))));
              r = rNext;

       } while (++count < 100 && norm > 0.001f);

       rotate = Quaternion(r);
       scale = inverse(r) * m;
    }

    Transform AnimatedTransform::interpolate(float time) const
    {
       float dt = (time - _startTime) / (_endTime - _startTime);
       auto trans = lerp(dt, _translates[0], _translates[1]);
       auto rotate = slerp(dt, _rotates[0], _rotates[1]);
       Matrix4x4f scale;

       for (size_t i{0}; i < 3; i++) {
              for (size_t j{0}; j < 3; j++) {
                     scale[i][j] = lerp(dt, _scales[0][i][j], _scales[1][j][i]);
              }
       }
    
       return {translate(trans)._m * rotate.toTransform()._m * scale};
    }

    BoundingVolume3<float> AnimatedTransform::moutionBounds(const BoundingVolume3<float>& boundingVolume) const
    {
       if (!_actualAnimated) {
              return (*_ptrStartTransform)(boundingVolume);
       } else if (!_hasRotate) {
              return merge((*_ptrStartTransform)(boundingVolume), (*_ptrEndTransform)(boundingVolume));
       }

       BoundingVolume3<float> b = boundingVolume.corner(0);
       b = merge(b, boundingVolume.corner(1));
       b = merge(b, boundingVolume.corner(2));
       b = merge(b, boundingVolume.corner(3));
       b = merge(b, boundingVolume.corner(4));
       b = merge(b, boundingVolume.corner(5));
       b = merge(b, boundingVolume.corner(6));
       b = merge(b, boundingVolume.corner(7));

       return b;
    }

    /**
     * Функция осуществляющая поиск нулей в интервале.
     * В этой функции решается следующее вырожени 
     *           c1 + (c2 + c3 * t) * cos(2 * theta * t) + (c4 + c5 * t) * sin(2 * theta * t).
     * 
     * Эта функция работает итеративно. Пока depth не равен нулю будет осуществлятся деление интервала на
     * две части и рекурсивно вызываться эта же функция. Когда depth будет равен нулю будет применяться 
     * метод Ньютона.
     * 
     * ВНИМЕНИЕ, данная функция будет находить только один ноль, даже если есть возможность найти еще нули !!!
     * 
     * @param c1 коэффициент производной функции движения
     * @param c2 коэффициент производной функции движения
     * @param c3 коэффициент производной функции движения
     * @param c4 коэффициент производной функции движения
     * @param c5 коэффициент производной функции движения
     * @param theta угол между двумя кватернионами
     * @param tInterval временной интервал
     * @param depth количство деление интервала
     * @return zeros указатель на массив, в который нужно решение при которм получаются нули (размером depth)
     * @return nZeros указатель на число содержащее количество найденных нулей
    */
    void findZeroInInterval(float c1, 
                             float c2, 
                             float c3, 
                             float c4, 
                             float c5, 
                             float theta, 
                             const IntervalArithmetic<float>& tInterval, 
                             float* zeros, 
                             int* nZeros, 
                             int depth = 8)
    {
       IntervalArithmetic<float> intC1 {c1};
       IntervalArithmetic<float> intC2 {c2};
       IntervalArithmetic<float> intC3 {c3};
       IntervalArithmetic<float> intC4 {c4};
       IntervalArithmetic<float> intC5 {c5};
       IntervalArithmetic<float> intTheta {theta};
       IntervalArithmetic<float> sinAndCosArg {2.0f * theta};

       sinAndCosArg = sinAndCosArg * tInterval;

       auto resRange = intC1 + (intC2 + intC3 * tInterval) * cos(sinAndCosArg) + (intC4 + intC5 * tInterval) * sin(sinAndCosArg);

       if (resRange.low > 0. || resRange.high < 0. || resRange.low == resRange.high) {
              return;
       }

       if (!depth) {
              auto mid = (tInterval.low + tInterval.high) * 0.5f;

              findZeroInInterval(c1, c2, c3, c4, c5, theta, IntervalArithmetic<float>{tInterval.low, mid}, zeros, nZeros, depth--);
              findZeroInInterval(c1, c2, c3, c4, c5, theta, IntervalArithmetic<float>{mid, tInterval.high}, zeros, nZeros, depth--);
       } else {
              /// Используем метод Ньютона
              float t = (tInterval.low + tInterval.high) * 0.5f;
              float funcRes, deriveFuncRes;

              for (int i{0}; i < 4; i++) {
                     funcRes = c1 + (c2 + c3 * t) * std::cos(2.0f * theta * t) + (c4 + c5 * t) * std::sin(2.0f * theta * t);
                     deriveFuncRes = (c3 + 2 * (c4 + c5 * t) * theta) * 
                                            std::cos(2.0f * theta * t) + 
                                            (c5 - 2 * (c2 + c3 * t) * theta) * 
                                            std::sin(2.0f * theta * t);
                     
                     if (t == 0 || deriveFuncRes == 0) {
                            break;
                     }

                     t = t - funcRes / deriveFuncRes;
              }

              if (t >= tInterval.low - 1e-3f && t < tInterval.high + 1e-3f) {
                     zeros[*nZeros] = t;
                     (*nZeros)++;
              }
       }
    }

    BoundingVolume3<float> AnimatedTransform::boundingMoutionPoint(const Point3<float>& point) const
    {
       if (!_actualAnimated) {
              return {(*_ptrStartTransform)(point)};
       } 
       
       BoundingVolume3<float> bounds {(*_ptrStartTransform)(point), (*_ptrEndTransform)(point)};

       if (!_hasRotate) {
              return bounds;
       }

       float cosTheta = dot(_rotates[0], _rotates[1]);
       float theta = std::cos(clamp(cosTheta, -1.0f, 1.0f));
       float zeros[8];

       for (int i{0}, nZeros{0}; i < 3; i++, nZeros = 0) {
              findZeroInInterval(_c1[i].calc(point), 
                                 _c2[i].calc(point), 
                                 _c3[i].calc(point), 
                                 _c4[i].calc(point), 
                                 _c5[i].calc(point), 
                                 theta, 
                                 IntervalArithmetic<float>{0.f, 1.f}, 
                                 zeros, 
                                 &nZeros);

              for (int j{0}; j < nZeros; j++) {
                     Point3<float> pz = (*this)(point, lerp(zeros[j], _startTime, _endTime));
                     bounds = merge(bounds, pz);
              }
       }

       return {};
    }

    Vec3<float> AnimatedTransform::operator () (const Vec3<float>& vec, float time) const
    {
       if (!_actualAnimated || time <= _startTime) {
              return (*_ptrStartTransform)(vec);
       } else if (time >= _endTime) {
              return (*_ptrEndTransform)(vec);
       }

       auto transform = interpolate(time);

       return transform(vec);
    }

    Point3<float> AnimatedTransform::operator () (const Point3<float>& point, float time) const
    {
       if (!_actualAnimated || time <= _startTime) {
              return (*_ptrStartTransform)(point);
       } else if (time >= _endTime) {
              return (*_ptrEndTransform)(point);
       }

       auto transform = interpolate(time);

       return transform(point);
    }

    Normal3f AnimatedTransform::operator () (const Normal3f& normal, float time) const
    {
       if (!_actualAnimated || time <= _startTime) {
              return (*_ptrStartTransform)(normal);
       } else if (time >= _endTime) {
              return (*_ptrEndTransform)(normal);
       }

       auto transform = interpolate(time);

       return transform(normal);
    }

    Ray AnimatedTransform::operator () (const Ray& ray) const
    {
       if (!_actualAnimated || ray._time <= _startTime) {
              return (*_ptrStartTransform)(ray);
       } else if (ray._time >= _endTime) {
              return (*_ptrEndTransform)(ray);
       }

       auto transform = interpolate(ray._time);

       return transform(ray);
    }

    RayDifferential AnimatedTransform::operator () (const RayDifferential& rayDifferential) const
    {
       if (!_actualAnimated || rayDifferential._time <= _startTime) {
              return (*_ptrStartTransform)(rayDifferential);
       } else if (rayDifferential._time >= _endTime) {
              return (*_ptrEndTransform)(rayDifferential);
       }

       auto transform = interpolate(rayDifferential._time);

       return transform(rayDifferential);
    }
}
