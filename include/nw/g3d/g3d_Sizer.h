#ifndef NW_G3D_SIZER_H_
#define NW_G3D_SIZER_H_

#include <nw/g3d/g3d_config.h>
#include <nw/g3d/ut/g3d_Inlines.h>

namespace nw { namespace g3d {

class Sizer
{
public:
    struct Chunk
    {
        size_t size;
        ptrdiff_t offset;
    };

    Sizer() : pChunk(NULL), totalSize(0) {}

    size_t GetTotalSize() const { return totalSize; }

    bool IsValid() const { return pChunk != NULL; }

    void Invalidate() { pChunk = NULL; totalSize = 0; }

    template <typename T>
    NW_G3D_FORCE_INLINE T* GetPtr(void* ptr, int type) const
    {
        NW_G3D_ASSERT_NOT_NULL(pChunk);
        return pChunk[type].size ? AddOffset<T>(ptr, pChunk[type].offset) : NULL;
    }

    NW_G3D_FORCE_INLINE void* GetPtr(void* ptr, int type) const
    {
        NW_G3D_ASSERT_NOT_NULL(pChunk);
        return pChunk[type].size ? AddOffset(ptr, static_cast<size_t>(pChunk[type].offset)) : NULL;
    }

protected:
    void CalcOffset(Chunk* chunk, int count)
    {
        int idx = 0;
        chunk[idx].offset = 0;
        for (; idx < count - 1; ++idx)
        {
            NW_G3D_ASSERT(IsAligned(chunk[idx].size));
            chunk[idx + 1].offset = chunk[idx].offset + static_cast<ptrdiff_t>(chunk[idx].size);
        }
        totalSize = static_cast<size_t>(chunk[idx].offset) + chunk[idx].size;
        pChunk = chunk;
    }

private:
    Chunk* pChunk;
    size_t totalSize;
};

} } // namespace nw::g3d

#endif // NW_G3D_SIZER_H_
