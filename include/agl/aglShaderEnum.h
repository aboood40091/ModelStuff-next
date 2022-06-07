#pragma once

#include <basis/seadTypes.h>

namespace agl {

// No idea which headers these are actually supposed to go to

enum ShaderType
{
    cShaderType_Vertex,
    cShaderType_Fragment,
    cShaderType_Geometry,
    cShaderType_Num
};

enum ShaderSymbolType
{
    cShaderSymbolType_Uniform,
    cShaderSymbolType_UniformBlock,
    cShaderSymbolType_Sampler,
    cShaderSymbolType_Attribute,
    cShaderSymbolType_Num,
};

}
