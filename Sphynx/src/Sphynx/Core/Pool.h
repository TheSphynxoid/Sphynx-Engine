#pragma once
#ifndef Sphynx_LinkedPool
#define Sphynx_LinkedPool
#include <memory>
#include <queue>
#define Def_Start_Size 10

namespace Sphynx {
	//Pool is a container that does not ensure element position,what this means is that a new elements is not guaranteed to be size-1
	//this container reuses the space of removed elements as-is instead of moving them.
	//this container is neither sequanced nor linked.
	template<class T>
	class Pool {
	private:
		typedef T* Container;
		Container container = nullptr;
		size_t Count;
		std::queue<size_t> FreeIndexes;
		size_t currCount = 0;
	public:
		Pool(){
			container = (T*)malloc(sizeof(T) * Def_Start_Size);
			//
			memset(container, 0, sizeof(T) * Def_Start_Size);
			Count = Def_Start_Size;
		}
		//May invalidate all other pointers if the reallocation happens
		void push(T elem) {
			if (FreeIndexes.empty()) {
				if (Count == currCount) {
					container = (T*)realloc(container, sizeof(T) * (Count + Def_Start_Size));
					Count += Def_Start_Size;
				}
				//memcpy(&container[currCount], &elem, sizeof(T));
				container[currCount] = elem;
				currCount++;
			}
			else {
				container[FreeIndexes.front()] = elem;
				FreeIndexes.pop();
			}
		}
		//Will not invalidate any pointer except the deleted one.
		void remove(size_t index) {
			FreeIndexes.push(index);
			container[index].~T();
		}
		//Will not invalidate any pointer except the deleted one.
		void remove_cmp(T cond) {
			for (int i = 0; i < currCount; i++) {
				if (cond == container[i]) {
					FreeIndexes.push(i);
					container[i].~T();
					return;
				}
			}
		}
		size_t capacity() { return Count; };
		size_t size() { return currCount; };
		
		void shrink_to_fit() {
			if(currCount < Count)
				container = (T*)realloc(container, sizeof(T) * currCount);
		}
		//Only expands the Pool
		//May invalidate pointers
		void reserve(size_t n) {
			if (n > Count) {
				container = (T*)realloc(container, sizeof(T) * n);
				Count = n;
			}
		}
		//May invalidate pointers
		void resize(size_t count, T value) {
			if (count == Count)return;
			container = (T*)realloc(container, sizeof(T) * count);
			std::fill_n(container, count, value);
			Count = count;
		}
		T* operator[](size_t index) {
			return &container[index];
		}
	};
}
#else
#endif