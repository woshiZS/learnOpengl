#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double()
{
    // return a random real in [0, 1]
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    // Return a random real in [min, max]
    return min + (max - min) * random_double();
}

// Common Headers

#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "color.h"
#include "hittable.h"

#endif