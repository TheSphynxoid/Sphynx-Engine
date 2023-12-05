#pragma once
#ifndef POLYVEC
#define POLYVEC

#include <memory>
//Still thinking if this is possible
template<class T, typename _Alloc = std::allocator<T>>
class PolymorphicVector {
private:
	void* Stack;
	size_t VecSize;
	bool IsBase;
public:
	PolymorphicVector() = default;
};
#else
#endif