#pragma once

#include <common/aglResShaderArchive.h>
#include <common/aglShaderProgramArchive.h>

struct ModelShaderArchive
{
    agl::ShaderProgramArchive*              obj;
    agl::ResBinaryShaderArchiveData*        res_binary_archive;
    agl::ResShaderArchiveData*              res_archive;
    bool                                    initialized;
};
static_assert(sizeof(ModelShaderArchive) == 0x10);
