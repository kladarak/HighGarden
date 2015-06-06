#pragma once

#include <stdint.h>
#include <vector>

class BitField
{
public:
	void		Set(uint32_t inBit);
	void		Clear(uint32_t inBit);

	bool		Get(uint32_t inBit) const;
	uint32_t	NumBits() const;

private:	
	enum { BITS_PER_ELEM = sizeof(uint32_t) * 8 };

	struct Bit
	{
		Bit (uint32_t inBit)
			: mIndex	(inBit / BITS_PER_ELEM)
			, mBitMask	(1 << (inBit % BITS_PER_ELEM))
		{ 
		}

		uint32_t mIndex;
		uint32_t mBitMask;
	};

	void		EnsureCapacity(const Bit& inBit);

	std::vector<uint32_t> mBits;
};
