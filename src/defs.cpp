#include "defs.h"

void normalise_vector(float a, float b, float& a_out, float& b_out, 
                      float scale_factor = 1.0f){ // Optional scale factor to apply
    float common = std::sqrt( a * a + b * b );
    a_out = a / common * scale_factor;
    b_out = b / common * scale_factor;
};