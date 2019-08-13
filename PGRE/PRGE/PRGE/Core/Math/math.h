#ifndef MATH_H
#define MATH_H

#include "../core.h"

#include <stdexcept>

using namespace std;

#define NAN_OR_INF_XY(x, y) if(isnan(x) || isnan(y) || isinf(x) || isinf(y)) throw invalid_argument("The x and y arguments passed are not valid")
#define NAN_OR_INF_XYZ(x, y, z) if(isnan(x) || isnan(y) || isnan(z) || isinf(x) || isinf(y) || isinf(z)) throw invalid_argument("The x, y and z arguments passed are not valid")
#define NAN_OR_INF_XYZW(x, y, z, w) if(isnan(x) || isnan(y) || isnan(z) || isnan(w) || isinf(x) || isinf(y) || isinf(z) || isinf(w)) throw invalid_argument("The x, y, z and w arguments passed are not valid")

#endif // MATH_H
