#pragma once

#include <container/seadBuffer.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <prim/seadSafeString.h>
#include <thread/seadCriticalSection.h>

namespace agl { namespace detail {

class PrivateResource
{
    SEAD_SINGLETON_DISPOSER(PrivateResource)

public:
    PrivateResource();
    virtual ~PrivateResource();

    sead::Heap* getShaderCompileHeap() { return mShaderCompileHeap; }
    const sead::Heap* getShaderCompileHeap() const { return mShaderCompileHeap; }

private:
    sead::Heap* mWorkHeap; // ExpHeap*
    sead::Heap* mShaderCompileHeap; // ExpHeap*
    sead::SafeString* _18; // HeapSafeString?
    sead::Buffer<u8> _1c;
    u32 _24;
    u8 _28[4];
    sead::Heap* mDebugHeap;
    sead::CriticalSection mCriticalSection;
};
static_assert(sizeof(PrivateResource) == 0x70, "agl::detail::PrivateResource size mismatch");


} }
