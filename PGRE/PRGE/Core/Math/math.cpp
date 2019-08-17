//
//  math.inl
//  PRGE
//
//  Created by Асиф Мамедов on 17/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "math.hpp"

#include <cmath>

using namespace std;

namespace PRGE
{
    bool solveLinearSystem2x2(const float a[2][2], float b0, float b1, float& x0, float& x1)
    {
        float det = a[0][0] * a[1][1] - a[1][0] * a[0][1];
        
        if (det <= 0.0f) {
            return false;
        }
        
        x0 = (a[1][1] * b0 - a[0][1] * b1) / det;
        x1 = (b0 - a[0][0] * x0) * a[0][1];
        
        if (isnan(x0) || isnan(x1)) {
            return false;
        }
        
        return true;
    }

}
