#pragma once

#include <heap/seadDisposer.h>

class BgScrollMgr;

// This is actually in a different file
class ScrollEffectMgr
{
public:
    ScrollEffectMgr(BgScrollMgr* scroll_mgr);

public:
    u32 _0[0x3c / sizeof(u32)];
    struct // This should be a separate class
    {
        u32 _0;
        f32 _4;
        f32 _8;
        u32 _c;
        f32 _10;
        f32 _14;
    } _3c;
};
static_assert(sizeof(ScrollEffectMgr) == 0x54);

class BgScrollMgr
{
    SEAD_SINGLETON_DISPOSER(BgScrollMgr)

public:
    BgScrollMgr();

    f32 getZoom() const { return mZoom; }

    f32 getScreenLeft() const { return mScreenLeft; }
    f32 getScreenCenterX() const { return mScreenCenterX; }
    f32 getScreenRight() const { return mScreenRight; }
    f32 getScreenTop() const { return mScreenTop; }
    f32 getScreenCenterY() const { return mScreenCenterY; }
    f32 getScreenBottom() const { return mScreenBottom; }

    ScrollEffectMgr& getScrollEffectMgr() { return mScrollEffectMgr; }
    const ScrollEffectMgr& getScrollEffectMgr() const { return mScrollEffectMgr; }

private:
    f32 mZoom;
    f32 _14;
    sead::BoundBox2f _0x18;
    sead::BoundBox2f _0x28;
    sead::BoundBox2f _0x38;
    sead::BoundBox2f _0x48;
    sead::BoundBox2f _0x58;
    f32 mScreenWidth;       // In world space
    f32 mScreenHeight;      // ^^^
    f32 mScreenLeft;        // ^^^
    f32 mScreenCenterX;     // ^^^
    f32 mScreenRight;       // ^^^
    f32 mScreenTop;         // ^^^
    f32 mScreenCenterY;     // ^^^
    f32 mScreenBottom;      // ^^^
    sead::BoundBox2f _0x88;
    sead::BoundBox2f _0x98;
    void* mBoundMgr;
    void* mScrollLimitMgr;
    void* mZoomAreaMgr;
    void* mBoundVelMgr;
    void* mTrackingMgr;
    void* _bc;
    void* _c0;
    void* mAutoScrollMgr;
    void* mConfigMgr;
    u32 mScrollLimitInfo[0x2c / sizeof(u32)];
    ScrollEffectMgr mScrollEffectMgr;
};
static_assert(sizeof(BgScrollMgr) == 0x14C, "BgScrollMgr size mimatch");
