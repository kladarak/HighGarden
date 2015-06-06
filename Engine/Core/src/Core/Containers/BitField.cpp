#include "BitField.h"

void BitField::Set(uint32_t inBit)
{
	Bit bit(inBit);
	EnsureCapacity(bit);
	mBits[bit.mIndex] |= bit.mBitMask;
}

void BitField::Clear(uint32_t inBit)
{
	Bit bit(inBit);
	EnsureCapacity(bit);
	mBits[bit.mIndex] &= ~bit.mBitMask;
}

bool BitField::Get(uint32_t inBit) const
{
	Bit bit(inBit);
	return (mBits.size() > bit.mIndex) 
		&& ((mBits[bit.mIndex] & bit.mBitMask) != 0);
}

uint32_t BitField::NumBits() const
{
	return mBits.size() * BITS_PER_ELEM;
}

void BitField::EnsureCapacity(const Bit& inBit)
{
	if (inBit.mIndex >= mBits.size())
	{
		mBits.resize(inBit.mIndex + 1, 0);
	}
}
