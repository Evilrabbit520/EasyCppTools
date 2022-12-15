/**
 * @file md5.h
 * @author Evilrabbit (evilrabbit520@gmail.com)
 * @brief md5 header file definition.
 * @version 1.1.2
 * @date 2022-12-15
 * 
 * @copyright Copyright (c) 2021-2022 Evilrabbit. All rights reserved.
 * 
 */

#ifndef EASYCPPTOOLS_SRC_MD5_H__
#define EASYCPPTOOLS_SRC_MD5_H__

#include <cstring>
#include <iostream>

#ifdef __cplusplus
extern "C"{
#endif

namespace Hash
{

class MD5
{
public:
	typedef unsigned int size_type; // must be 32bit

	MD5();
	MD5(const std::string& text);
	void update(const unsigned char* buf, size_type length);
	void update(const char* buf, size_type length);
	MD5& finalize();

	//return hex representation of digest as string, 无参默认小写, upper(u)大写, lower(l)小写
	std::string hexdigest() const;
	std::string hexdigest(std::string style) const;

private:
	void init();
	typedef unsigned char uint1; //  8bit
	typedef unsigned int uint4;  // 32bit
	enum { blocksize = 64 }; // VC6 won't eat a const static int here

	void transform(const uint1 block[blocksize]);
	static void decode(uint4 output[], const uint1 input[], size_type len);
	static void encode(uint1 output[], const uint4 input[], size_type len);

	bool finalized;
	uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
	uint4 count[2];   // 64bit counter for number of bits (lo, hi)
	uint4 state[4];   // digest so far
	uint1 digest[16]; // the result

	// low level logic operations
	static inline uint4 F(uint4 x, uint4 y, uint4 z);
	static inline uint4 G(uint4 x, uint4 y, uint4 z);
	static inline uint4 H(uint4 x, uint4 y, uint4 z);
	static inline uint4 I(uint4 x, uint4 y, uint4 z);
	static inline uint4 rotate_left(uint4 x, int n);
	static inline void FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
};
} // namespace Hash

#ifdef __cplusplus
} //extern "C"
#endif

#endif // EASYCPPTOOLS_SRC_MD5_H__
