#include <graphics/CullViewFrustum.h>

#include <gfx/rio_Camera.h>
#include <gfx/rio_Projection.h>
#include <math/rio_Matrix.h>

CullViewFrustum::CullViewFrustum()
{
}

namespace {

static inline void C_MTXMultVec(const rio::BaseMtx34f& m, const rio::BaseVec3f& src, rio::BaseVec3f* dst)
{
    RIO_ASSERT(dst != nullptr);

    rio::BaseVec3f tmp = {
        m.m[0][0] * src.x + m.m[0][1] * src.y + m.m[0][2] * src.z + m.m[0][3],
        m.m[1][0] * src.x + m.m[1][1] * src.y + m.m[1][2] * src.z + m.m[1][3],
        m.m[2][0] * src.x + m.m[2][1] * src.y + m.m[2][2] * src.z + m.m[2][3]
    };

    *dst = tmp;
}

}

void CullViewFrustum::update(const rio::Camera& camera, const rio::PerspectiveProjection& projection)
{
    rio::Matrix34f view_mtx;
    camera.getMatrix(&view_mtx);

    rio::Matrix34f inv_view_mtx;
    inv_view_mtx.setInverse(view_mtx);

    mViewVolume.SetFrustum(
        projection.getTop(), projection.getBottom(),
        projection.getLeft(), projection.getRight(),
        projection.getNear(), projection.getFar(),
        *nw::g3d::math::Mtx34::Cast(inv_view_mtx.a)
    );

    rio::Matrix34f billboard_mtx;
    billboard_mtx.setTranspose(view_mtx);

    billboard_mtx.m[0][3] = -(billboard_mtx.m[0][0] * view_mtx.m[0][3] +
                              billboard_mtx.m[0][1] * view_mtx.m[1][3] +
                              billboard_mtx.m[0][2] * view_mtx.m[2][3]);

    billboard_mtx.m[1][3] = -(billboard_mtx.m[1][0] * view_mtx.m[0][3] +
                              billboard_mtx.m[1][1] * view_mtx.m[1][3] +
                              billboard_mtx.m[1][2] * view_mtx.m[2][3]);

    billboard_mtx.m[2][3] = -(billboard_mtx.m[2][0] * view_mtx.m[0][3] +
                              billboard_mtx.m[2][1] * view_mtx.m[1][3] +
                              billboard_mtx.m[2][2] * view_mtx.m[2][3]);

    billboard_mtx.m[0][2] *= -1;
    billboard_mtx.m[1][2] *= -1;
    billboard_mtx.m[2][2] *= -1;

    rio::Vector3f pos = {
        billboard_mtx.m[0][3],
        billboard_mtx.m[1][3],
        billboard_mtx.m[2][3]
    };

    f32 fovy_tan = std::tan(projection.fovy() * 0.5f);
    f32 fovx_tan = projection.aspect() * fovy_tan;

    rio::Vector3f planes[4];

    planes[2].z = fovy_tan + fovy_tan * projection.offset().y * 2;
    f32 v1 = rio::Mathf::sqrt(rio::Mathf::square(planes[2].z) + 1.0f);
    f32 inv_v1 = 1.0f / v1;
    planes[2].y = -inv_v1;
    planes[2].z *= inv_v1;

    planes[3].z = fovy_tan - fovy_tan * projection.offset().y * 2;
    f32 v2 = rio::Mathf::sqrt(rio::Mathf::square(planes[3].z) + 1.0f);
    f32 inv_v2 = 1.0f / v2;
    planes[3].y =  inv_v2;
    planes[3].z *= inv_v2;

    planes[1].z = fovx_tan - fovx_tan * projection.offset().x * 2;
    f32 v3 = rio::Mathf::sqrt(rio::Mathf::square(planes[1].z) + 1.0f);
    f32 inv_v3 = 1.0f / v3;
    planes[1].x =  inv_v3;
    planes[1].z *= inv_v3;

    planes[0].z = fovx_tan + fovx_tan * projection.offset().x * 2;
    f32 v4 = rio::Mathf::sqrt(rio::Mathf::square(planes[0].z) + 1.0f);
    f32 inv_v4 = 1.0f / v4;
    planes[0].x = -inv_v4;
    planes[0].z *= inv_v4;

    planes[0].y = 0.0f;
    planes[1].y = 0.0f;
    planes[2].x = 0.0f;
    planes[3].x = 0.0f;

    billboard_mtx.m[0][3] = 0.0f;
    billboard_mtx.m[1][3] = 0.0f;
    billboard_mtx.m[2][3] = 0.0f;

    for (s32 i = 0; i < 4; i++)
    {
        C_MTXMultVec(billboard_mtx, planes[i], &(planes[i]));
        mPlane[i].normal = planes[i];
        mPlane[i].distance = mPlane[i].normal.dot(pos);
    }
}

void CullViewFrustum::update(const rio::Camera& camera, const rio::OrthoProjection& projection)
{
    rio::Matrix34f view_mtx;
    camera.getMatrix(&view_mtx);

    rio::Matrix34f inv_view_mtx;
    inv_view_mtx.setInverse(view_mtx);

    const f32 top    = projection.top();
    const f32 bottom = projection.bottom();
    const f32 left   = projection.left();
    const f32 right  = projection.right();
    const f32 z_near = projection.getNear();
    const f32 z_far  = projection.getFar();

    mViewVolume.SetOrtho(
        top, bottom,
        left, right,
        z_near, z_far,
        *nw::g3d::math::Mtx34::Cast(inv_view_mtx.a)
    );

    rio::Vector3f pt[8] = {
        { left,  top,    -z_near },
        { right, top,    -z_near },
        { right, bottom, -z_near },
        { left,  bottom, -z_near },
        { left,  top,    -z_far  },
        { right, top,    -z_far  },
        { right, bottom, -z_far  },
        { left,  bottom, -z_far  }
    };

    rio::Vector3f center { 0.0f, 0.0f, 0.0f };
    for (s32 i = 0; i < 8; i++)
    {
        C_MTXMultVec(inv_view_mtx, pt[i], &(pt[i]));

        center.x += pt[i].x;
        center.y += pt[i].y;
        center.z += pt[i].z;
    }
    center *= 1.0f / 8;

    static const s32 cFaceCorner[4][3] = {
        { 1, 2, 5 },
        { 0, 3, 4 },
        { 0, 1, 4 },
        { 2, 3, 7 }
    };

    for (s32 i = 0; i < 4; i++)
    {
        const rio::Vector3f& p0 = pt[cFaceCorner[i][0]];

        rio::Vector3f edge_0, edge_1, normal;
        edge_0.setSub(pt[cFaceCorner[i][1]], p0);
        edge_1.setSub(pt[cFaceCorner[i][2]], p0);
        normal.setCross(edge_0, edge_1);

        const f32 length = normal.length();
        if (length > 0.0f)
        {
            normal *= 1.0f / length;

            if (normal.dot(center - p0) < 0.0f)
                normal *= -1.0f;

            mPlane[i].normal = normal;
            mPlane[i].distance = normal.dot(p0);
        }
        else
        {
            mPlane[i].normal = { 0.0f, 0.0f, 0.0f };
            mPlane[i].distance = 0.0f;
        }
    }
}

bool CullViewFrustum::testIntersectionSphere(const rio::Vector3f& pos, f32 radius) const
{
    for (s32 i = 0; i < 4; i++)
        if ((mPlane[i].normal.dot(pos) - mPlane[i].distance) < -radius)
            return false;

    return true;
}

bool CullViewFrustum::testIntersectionAABB(const rio::Vector3f& min, const rio::Vector3f& max) const
{
    nw::g3d::AABB aabb;
    aabb.min.Set(min.x, min.y, min.z);
    aabb.max.Set(max.x, max.y, max.z);
    return mViewVolume.TestIntersection(aabb);
}
