#pragma once
#ifndef Sphynx_LinkedPool
#define Sphynx_LinkedPool
#include <memory>
#include <queue>
#define Def_Start_Size 10

namespace Sphynx {
	template<class T>
	class LinkedPool {
	private:
		typedef T Type;
		typedef T* Container;
		Container container;
		size_t ByteSize;
		size_t Count;
		std::queue<int> FreeIndexes;
		size_t currCount;
	public:
		LinkedPool(){
			ByteSize = sizeof(T) * Def_Start_Size;
			container = (T*)malloc(ByteSize);
			Count = Def_Start_Size;
		}
		void push(T elem) {
			if (FreeIndexes.empty()) {
				container[currCount] = elem;
				currCount++;
			}
			else {
				container[FreeIndexes.front()] = elem;
				FreeIndexes.pop();
			}
		}
		void remove(int index) {
			FreeIndexes.push(index);
			delete& container[index];
		}
		void remove_cmp(T cond) {
			for (int i = 0; i < CurrCount; i++) {
				if (cond == container[i]) {
					FreeIndexes.push(i);
					delete& cotainer[i];
					return;
				}
			}
		}
		size_t Size() { return currCount; };
		
	};
}
#else
#endif