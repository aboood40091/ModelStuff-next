#pragma once

#include <agl/g3d/aglModelShaderAssign.h>

#include <nw/g3d/g3d_MaterialObj.h>
#include <nw/g3d/g3d_ModelObj.h>

namespace agl { namespace g3d {

class ModelEx;

class MaterialEx
{
public:
    MaterialEx();
    ~MaterialEx();

    nw::g3d::MaterialObj* getMaterialObj() const
    {
        return mpMaterialObj;
    }

    void init(agl::g3d::ModelEx* p_model, u32 index, sead::Heap* heap);

    void replaceUBO(const nw::g3d::fnd::GfxBuffer_t& buffer);
    void fixUpUBO();

private:
    ModelEx* mpModelEx;
    nw::g3d::MaterialObj* mpMaterialObj;
    ShaderProgram* mpProgram;
    nw::g3d::fnd::GfxBuffer_t mMatBlock;
    bool _20;
};
static_assert(sizeof(MaterialEx) == 0x24, "agl::g3d::MaterialEx size mismatch");

class ModelEx : public nw::g3d::ModelObj
{
public:
    ModelEx();
    virtual ~ModelEx();

    ModelShaderAssign& getShaderAssign(s32 index) const
    {
        return mpShaderAssign[index];
    }

    MaterialEx& getMaterialEx(s32 index) const
    {
        return mpMaterialEx[index];
    }

    void createEx(sead::Heap* heap);
    void destroyEx();

private:
    ModelShaderAssign* mpShaderAssign;
    MaterialEx* mpMaterialEx;
};
static_assert(sizeof(ModelEx) == 0x40, "agl::g3d::ModelEx size mismatch");

} }
