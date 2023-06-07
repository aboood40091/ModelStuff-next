#pragma once

#include <misc/rio_Types.h>

#include <string>

class StreamSrc;

class StreamFormat
{
public:
    virtual u8 readU8(StreamSrc* src, bool is_be) = 0;
    virtual u16 readU16(StreamSrc* src, bool is_be) = 0;
    virtual u32 readU32(StreamSrc* src, bool is_be) = 0;
    virtual u64 readU64(StreamSrc* src, bool is_be) = 0;
    virtual s8 readS8(StreamSrc* src, bool is_be) = 0;
    virtual s16 readS16(StreamSrc* src, bool is_be) = 0;
    virtual s32 readS32(StreamSrc* src, bool is_be) = 0;
    virtual s64 readS64(StreamSrc* src, bool is_be) = 0;
    virtual f32 readF32(StreamSrc* src, bool is_be) = 0;
    virtual void readBit(StreamSrc* src, void* data, u32 bitnum) = 0;
    virtual void readString(StreamSrc* src, std::string* dst, u32 size) = 0;
    virtual u32 readMemBlock(StreamSrc* src, void* dst, u32 size) = 0;
    virtual void writeU8(StreamSrc* src, bool is_be, u8 value) = 0;
    virtual void writeU16(StreamSrc* src, bool is_be, u16 value) = 0;
    virtual void writeU32(StreamSrc* src, bool is_be, u32 value) = 0;
    virtual void writeU64(StreamSrc* src, bool is_be, u64 value) = 0;
    virtual void writeS8(StreamSrc* src, bool is_be, s8 value) = 0;
    virtual void writeS16(StreamSrc* src, bool is_be, s16 value) = 0;
    virtual void writeS32(StreamSrc* src, bool is_be, s32 value) = 0;
    virtual void writeS64(StreamSrc* src, bool is_be, s64 value) = 0;
    virtual void writeF32(StreamSrc* src, bool is_be, f32 value) = 0;
    virtual void writeBit(StreamSrc* src, const void* data, u32 bitnum) = 0;
    virtual void writeString(StreamSrc* src, const std::string& str, u32 size) = 0;
    virtual void writeMemBlock(StreamSrc* src, const void* data, u32 size) = 0;
    virtual void writeDecorationText(StreamSrc* src, const std::string&) = 0;
    virtual void writeNullChar(StreamSrc* src) = 0;
    virtual void skip(StreamSrc* src, u32 size) = 0;
    virtual void flush(StreamSrc* src) = 0;
    virtual void rewind(StreamSrc* src) = 0;
};
