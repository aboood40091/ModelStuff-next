#pragma once

#include <graphics/ModelG3d.h>

#include <container/Buffer.h>
#include <container/PtrArray.h>

class ModelResource;
class ShaderParamAnimation;
class ShapeAnimation;
class SkeletalAnimation;
class TexturePatternAnimation;
class VisibilityAnimation;

class AnimModel
{
private:
    static AnimModel* createFromModel_(
        ModelResource* p_mdl_res,
        ModelG3d* p_model,
        s32 skl_anim_num, s32 tex_anim_num, s32 shu_anim_num, s32 vis_anim_num, s32 sha_anim_num,
        const PtrArray<ModelResource>* p_anim_mdl_res_array = nullptr
    );
    static void destroyWithoutModel_(
        AnimModel*& p_anim_model
    );

public:
    static AnimModel* create(
        ModelResource* p_mdl_res,
        const char* name,
        s32 skl_anim_num, s32 tex_anim_num, s32 shu_anim_num, s32 vis_anim_num, s32 sha_anim_num,
        Model::BoundingMode bounding_mode = Model::cBoundingMode_Disable
    );
    static AnimModel* create(
        ModelResource* p_mdl_res,
        const char* name,
        s32 view_num,
        s32 skl_anim_num, s32 tex_anim_num, s32 shu_anim_num, s32 vis_anim_num, s32 sha_anim_num,
        Model::BoundingMode bounding_mode
    );
    static void destroy(
        AnimModel*& p_anim_model
    );

protected:
    AnimModel(ModelG3d* p_model, u32 skl_anim_num, u32 tex_anim_num, u32 shu_anim_num, u32 vis_anim_num, u32 sha_anim_num);
    virtual ~AnimModel();

    AnimModel(const AnimModel&) = delete;
    AnimModel(AnimModel&&) = delete;
    AnimModel& operator=(const AnimModel&) = delete;
    AnimModel& operator=(AnimModel&&) = delete;

public:
    ModelG3d* getModel() const { return mpModel; }
    ModelResource* getModelResource() const { return mpModelResource; }

    SkeletalAnimation*          getSklAnim(s32 index) const { return mpSklAnim[index]; }
    TexturePatternAnimation*    getTexAnim(s32 index) const { return mpTexAnim[index]; }
    ShaderParamAnimation*       getShuAnim(s32 index) const { return mpShuAnim[index]; }
    VisibilityAnimation*        getVisAnim(s32 index) const { return mpVisAnim[index]; }
    ShapeAnimation*             getShaAnim(s32 index) const { return mpShaAnim[index]; }

    void init(ModelResource* p_mdl_res, const PtrArray<ModelResource>* p_anim_mdl_res_array = nullptr);

    void playAnmFrameCtrl();
    void calcMdl();

private:
    ModelG3d*                           mpModel;
    ModelResource*                      mpModelResource;
    Buffer<SkeletalAnimation*>          mpSklAnim;
    Buffer<TexturePatternAnimation*>    mpTexAnim;
    Buffer<ShaderParamAnimation*>       mpShuAnim;
    Buffer<VisibilityAnimation*>        mpVisAnim;
    Buffer<ShapeAnimation*>             mpShaAnim;
};
//static_assert(sizeof(AnimModel) == 0x30);

inline AnimModel* AnimModel::createFromModel_(
    ModelResource* p_mdl_res,
    ModelG3d* p_model,
    s32 skl_anim_num, s32 tex_anim_num, s32 shu_anim_num, s32 vis_anim_num, s32 sha_anim_num,
    const PtrArray<ModelResource>* p_anim_mdl_res_array
)
{
    AnimModel* p_anim_model = new AnimModel(p_model, skl_anim_num, tex_anim_num, shu_anim_num, vis_anim_num, sha_anim_num);
    p_anim_model->init(p_mdl_res, p_anim_mdl_res_array);
    return p_anim_model;
}

inline void AnimModel::destroyWithoutModel_(
    AnimModel*& p_anim_model
)
{
    if (p_anim_model != nullptr)
    {
        delete p_anim_model;
        p_anim_model = nullptr;
    }
}

inline AnimModel* AnimModel::create(
    ModelResource* p_mdl_res,
    const char* name,
    s32 skl_anim_num, s32 tex_anim_num, s32 shu_anim_num, s32 vis_anim_num, s32 sha_anim_num,
    Model::BoundingMode bounding_mode
)
{
    ModelG3d* p_model = ModelUtil::createG3d(*p_mdl_res, name, skl_anim_num, tex_anim_num, shu_anim_num, vis_anim_num, sha_anim_num, bounding_mode);
    return createFromModel_(p_mdl_res, p_model, skl_anim_num, tex_anim_num, shu_anim_num, vis_anim_num, sha_anim_num);
}

inline AnimModel* AnimModel::create(
    ModelResource* p_mdl_res,
    const char* name,
    s32 view_num,
    s32 skl_anim_num, s32 tex_anim_num, s32 shu_anim_num, s32 vis_anim_num, s32 sha_anim_num,
    Model::BoundingMode bounding_mode
)
{
    ModelG3d* p_model = ModelUtil::createG3d(*p_mdl_res, name, view_num, skl_anim_num, tex_anim_num, shu_anim_num, vis_anim_num, sha_anim_num, bounding_mode);
    return createFromModel_(p_mdl_res, p_model, skl_anim_num, tex_anim_num, shu_anim_num, vis_anim_num, sha_anim_num);
}

inline void AnimModel::destroy(
    AnimModel*& p_anim_model
)
{
    if (p_anim_model != nullptr)
    {
        ModelG3d* p_model = p_anim_model->getModel();

        destroyWithoutModel_(p_anim_model);

        delete p_model;
    }
}
