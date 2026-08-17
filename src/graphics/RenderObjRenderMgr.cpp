#include <graphics/RenderObjRenderMgr.h>
#include <graphics/RenderObj.h>
#include <graphics/RenderObjLayer.h>

#include <algorithm>

RenderObjRenderMgr::RenderObjRenderMgr()
    : mpCallback(nullptr)
    , mShadowMap(nullptr)
    , mReflectionMap(nullptr)
{
}

void RenderObjRenderMgr::clear()
{
    mRenderObj.clear();
    mRenderObjOpa.clear();
    mRenderObjXlu.clear();
  //mRenderObjShadow.clear();
}

void RenderObjRenderMgr::calc()
{
    // RenderObj calc is done using multi-threading in vanilla NSMBU

    for (RenderObj* obj : mRenderObj)
        obj->calc();
}

void RenderObjRenderMgr::calcView(s32 view_index, const rio::Camera& camera, const rio::Projection& projection, const CullViewFrustum* p_cull)
{
    ViewInfo& view_info = getViewInfo(view_index);
    camera.getMatrix(&view_info.view_mtx);
    view_info.proj_mtx = static_cast<const rio::Matrix44f&>(projection.getMatrix());
  //view_info.p_depth_shadow_mtx = p_depth_shadow_mtx;
    view_info.p_cull = p_cull;
  //view_info._7c = param_6;

    for (RenderObj* obj : mRenderObj)
        obj->updateView(view_index, view_info.view_mtx, view_info.proj_mtx, this);

    std::sort(mRenderObjXlu.begin(), mRenderObjXlu.end(), [](const RenderObj* a, const RenderObj* b)
    {
       return a->getOrderPos().z < b->getOrderPos().z;
    });
}

void RenderObjRenderMgr::calcGPU(s32 view_index)
{
    ViewInfo& view_info = getViewInfo(view_index);

    mModelEnvView.setUniformData(view_index, view_info.view_mtx, view_info.proj_mtx);

  //view_info.p_depth_shadow_mtx = nullptr;

    for (RenderObj* obj : mRenderObj)
        obj->calcGPU(view_index, view_info.view_mtx, view_info.proj_mtx, this);
}

void RenderObjRenderMgr::drawOpa(s32 view_index, const rio::lyr::DrawInfo& draw_info)
{
    const ViewInfo& view_info = getViewInfo(view_index);

    if (mpCallback)
        mpCallback->preDrawOpa(view_index, draw_info);

    for (RenderObj* obj : mRenderObjOpa)
        obj->drawOpa(view_index, view_info.view_mtx, view_info.proj_mtx, this);

    if (mpCallback)
        mpCallback->postDrawOpa(view_index, draw_info);
}

void RenderObjRenderMgr::drawXlu(s32 view_index, const rio::lyr::DrawInfo& draw_info)
{
    const ViewInfo& view_info = getViewInfo(view_index);

    if (mpCallback)
        mpCallback->preDrawXlu(view_index, draw_info);

    for (RenderObj* obj : mRenderObjXlu)
        obj->drawXlu(view_index, view_info.view_mtx, view_info.proj_mtx, this);

    if (mpCallback)
        mpCallback->postDrawXlu(view_index, draw_info);
}

void RenderObjRenderMgr::pushBackRenderObj(RenderObj* obj, bool draw_opa, bool draw_xlu)
{
    mRenderObj.push_back(obj);

    if (draw_opa)
        mRenderObjOpa.push_back(obj);

    if (draw_xlu)
        mRenderObjXlu.push_back(obj);

  //if (obj->hasShadow())
  //    mRenderObjShadow.push_back(obj);
}

void RenderObjRenderMgr::pushBackRenderObj(RenderObj* obj, bool draw_opa, bool draw_xlu, const rio::Vector3f& order_pos)
{
    obj->getOrderPos() = order_pos;
    pushBackRenderObj(obj, draw_opa, draw_xlu);
}

void RenderObjRenderMgr::createView(RenderObjLayer* p_layer)
{
    RIO_ASSERT(u32(mModelEnvView.getViewNum()) == getViewNum());
    s32 view_index = getViewNum();
    ViewInfo& view_info = mViewInfo.emplace_back();
    view_info.p_layer = p_layer;
    mModelEnvView.addView();
    RIO_ASSERT(u32(mModelEnvView.getViewNum()) == getViewNum());
    p_layer->mViewIndex = view_index;
}

void RenderObjRenderMgr::clearView()
{
    mModelEnvView.clearView();
    mViewInfo.clear();
}
