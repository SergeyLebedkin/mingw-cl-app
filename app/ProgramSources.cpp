#include "ProgramSources.hpp"
#include <string>

// vertex shader for texture processing
const char* cCLProgram_WriteImage = R"(

kernel void WriteImage(write_only image2d_t imageOutput, const uint value) {
    // get coords
    int2 coords = (int2)(get_global_id(0), get_global_id(1));

    // compute results
    uint4 result = (uint4)(value, value, value, value);

    // write image
    write_imageui(imageOutput, coords, result);
}

)";
