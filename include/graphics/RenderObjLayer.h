#pragma once

#include <gfx/lyr/rio_Layer.h>

class CullViewFrustum;
class RenderObjRenderMgr;

class RenderObjLayer : public rio::lyr::Layer, public rio::lyr::IDrawable
{
public:
    enum RenderStep
    {
        cRenderStep_CalcView = 0,
        cRenderStep_UpdateGPUBuffer,
        cRenderStep_Opa,
        cRenderStep_Xlu,
        cRenderStep_Particle,
        cRenderStep_PostFx,
        cRenderStep_Num
    };
    static_assert(cRenderStep_Num == 6);

public:
    RenderObjLayer(const char* name, s32 priority);

    virtual void initialize();

    s32 getViewIndex() const
    {
        return mViewIndex;
    }

    RenderObjRenderMgr* getRenderMgr() const
    {
        return mpRenderMgr;
    }

    void setRenderMgr(RenderObjRenderMgr* p_render_mgr);

    const CullViewFrustum* getCullViewFrustum() const
    {
        return mpCull;
    }

    void setCullViewFrustum(const CullViewFrustum* p_cull)
    {
        mpCull = p_cull;
    }

protected:
    virtual void calcView_(const rio::lyr::DrawInfo& draw_info);
    virtual void updateGPUBuffer_(const rio::lyr::DrawInfo& draw_info);
    virtual void drawOpa_(const rio::lyr::DrawInfo& draw_info);
    virtual void drawXlu_(const rio::lyr::DrawInfo& draw_info);

private:
    void render_(const rio::lyr::DrawInfo& draw_info);

protected:
    RenderObjRenderMgr*     mpRenderMgr;
    s32                     mViewIndex;
    const CullViewFrustum*  mpCull;

    friend class RenderObjRenderMgr;
};
