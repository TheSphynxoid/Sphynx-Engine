#pragma once
#include <malloc.h>
#include <algorithm>
#include <vector>
#pragma push_macro("max")
#undef max

namespace Sphynx::Util {
	//1-bit 8 booleans, each bit is a bool
	class BitBool {
	private:
		bool GetBit(unsigned short n) {
			if (n > 7) { return 255; }
			else {
				return (_char_val >> n) & 1;
			}
		};
		void ClearBit(unsigned char n) {
			_char_val &= ~((unsigned char)(0b1) << n);
		};
		void SetBit(unsigned char n) {
			_char_val |= ((unsigned char)(0b1) << n);
		};
	public:
		unsigned char _char_val;
		bool operator[](int idx) {
			return GetBit(idx);
		};
		void SetBool(int idx, bool value) {
			if (value)SetBit(idx);
			else {
				ClearBit(idx);
			}
		};
		void ToggleBool(unsigned char n) {
			_char_val ^= ((unsigned char)(0b1) << n);
		};
	};
	class BitBoolArray {
	private:
		typedef struct NodeMap {
			int ArrayIndex;
			short BitIndex;
			short Align;
		}NodeMap;
		BitBool* elements;
		std::vector<NodeMap> indexMap;
		size_t count;
		size_t ByteCount;
	public:
		BitBoolArray(const size_t count) {
			//Allocate Array.
			ByteCount = ((count + (int)(count % 8 != 0)) / 8);
			void* ptr = malloc(sizeof(BitBool) * ByteCount);
			if (!ptr) { throw std::bad_alloc();/*Could Not allocate memory block*/ }
			memset(ptr, 0, sizeof(BitBool) * (count + (int)(count % 8 != 0) / 8));
			indexMap = std::vector<NodeMap>(count + (count * 8));
			for (int i = 0; i < count / 8; i++) {
				indexMap[i] = { i,1 };
				indexMap[i + 1] = { i,2 };
				indexMap[i + 2] = { i,3 };
				indexMap[i + 3] = { i,4 };
				indexMap[i + 4] = { i,5 };
				indexMap[i + 5] = { i,6 };
				indexMap[i + 6] = { i,7 };
				indexMap[i + 7] = { i,8 };
			}
			this->count = count;
		}
		~BitBoolArray() {
			free(elements);
			indexMap.clear();
		}
		//Returns Count (Starts with 1)
		constexpr size_t Count()const noexcept { return count; };
		//Will Segfault if index out of reach.
		BitBool GetUnmapped(int index) {
			return elements[index];
		}
		//Returns the bool value as if this was a collection of bools. To Get the BitBool Object Call GetUnmapped.
		//No Bound Checks.
		bool operator[](int index) {
			NodeMap& Mapped = indexMap[index];
			return elements[Mapped.ArrayIndex][Mapped.BitIndex];
		}
	};
}
#pragma pop_macro("max")