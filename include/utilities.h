#ifndef __UTILITIES_H__
#define __UTILITIES_H__

template <typename dtype>
inline void Delete(dtype* pointer)
{
    if(pointer != nullptr)
        free(pointer);
    pointer = nullptr;
}

template <typename dtype> inline dtype& setBit(dtype& data, unsigned char position)
{
    return data |= (1 << position);
}

template <typename dtype> inline dtype& clearBit(dtype& data, unsigned char position)
{
    return data &= ~(1 << position);
}

template <typename dtype> inline dtype& flipBit(dtype& data, unsigned char position)
{
    return data ^= (1 << position);
}

template <typename dtype> inline bool isBitSet(const dtype data, unsigned char position)
{
    return (data & (1 << position)) == 0 ? false: true;
}

#endif