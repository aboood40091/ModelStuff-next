#pragma once

#include <container/SafeArray.h>
#include <math/rio_Vector.h>

#include <nw/g3d/g3d_ShapeObj.h>

namespace rio {

class Camera;
class OrthoProjection;
class PerspectiveProjection;

}

class CullViewFrustum
{
    struct Plane
    {
        Plane()
            : normal{1.0f, 0.0f, 0.0f}
            , distance(0.0f)
        {
        }

        rio::Vector3f   normal;
        f32             distance;
    };
    static_assert(sizeof(Plane) == 0x10);

public:
    CullViewFrustum();

    nw::g3d::ViewVolume& getViewVolume() { return mViewVolume; }
    const nw::g3d::ViewVolume& getViewVolume() const { return mViewVolume; }

    void update(const rio::Camera& camera, const rio::PerspectiveProjection& projection);
    void update(const rio::Camera& camera, const rio::OrthoProjection& projection);

    bool testIntersectionSphere(const rio::Vector3f& pos, f32 radius) const;   // I think
    bool testIntersectionAABB(const rio::Vector3f& min, const rio::Vector3f& max) const;

private:
    UnsafeArray<Plane, 4>   mPlane;
    nw::g3d::ViewVolume     mViewVolume;
};
static_assert(sizeof(CullViewFrustum) == 0xC0);
