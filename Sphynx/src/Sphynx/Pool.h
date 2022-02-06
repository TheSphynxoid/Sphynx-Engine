#pragma once
#ifndef Sphynx_Pool
#define Sphynx_Pool
#include <memory>
#include <queue>
#ifndef Def_Start_Size
#define Def_Start_Size 10
#endif

namespace Sphynx {
	//Pool is a container that does not ensure element position,what this means is that a new elements is not guaranteed to be "size-1"
	//this container reuses the space of removed elements as-is instead of moving them.
	//this container is neither sequanced nor linked.
	template<class T>
	class Pool final{
	private:
		T* container = nullptr;
		size_t Count;
		std::queue<size_t> FreeIndexes;
		size_t currCount = 0;
	public:
		typedef void(Fill_t)(T& value);
		Pool() {
			container = (T*)malloc(sizeof(T) * Def_Start_Size);
			if (!container)throw std::bad_alloc();
			memset(container, 0, sizeof(T) * Def_Start_Size);
			Count = Def_Start_Size;
		}
		Pool(Fill_t func) {
			container = (T*)malloc(sizeof(T) * Def_Start_Size);
			if (!container)throw std::bad_alloc();
			for (int i = 0; i < Def_Start_Size; i++) {
				func(container[i]);
			}
			memset(container, 0, sizeof(T) * Def_Start_Size);
			Count = Def_Start_Size;
		}
		Pool(size_t size) {
			container = (T*)malloc(sizeof(T) * size);
			if (!container)throw std::bad_alloc();
			memset(container, 0, sizeof(T) * size);
			Count = size;
		}
		Pool(size_t size, Fill_t func) {
			container = (T*)malloc(sizeof(T) * size);
			if (!container)throw std::bad_alloc();
			for (int i = 0; i < size; i++) {
				func(container[i]);
			}
			Count = size;
		}
		//May invalidate all other pointers if the reallocation happens
		void push(T& elem) noexcept{
			if (FreeIndexes.empty()) {
				if (Count == currCount) {
					auto newalloc = (T*)realloc(container, sizeof(T) * (Count + Def_Start_Size) * 2);
					if (newalloc) {
						container = newalloc;
						Count += Def_Start_Size;
					}
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
		void insert(T& elem, size_t pos) {
			if ( this->Count > pos <= 0 ) {
				if (currCount + 1 > Count) {
					//Expand the Array.
					reserve(Count + Def_Start_Size);
				}
				for (int i = currCount; i > pos; i--) {
					container[i + 1] = container[i];
				}
				container[pos] = elem;
			}
		}
		//Will not invalidate any pointer except the deleted one.
		void remove(size_t index) {
			FreeIndexes.push(index);
			container[index].~T();
		}
		//Will not invalidate any pointer except the deleted one.
		void remove_cmp(T& cond) {
			for (int i = 0; i < currCount; i++) {
				if (cond == container[i]) {
					FreeIndexes.push(i);
					container[i].~T();
					return;
				}
			}
		}
		template<typename Pred>
		void for_each(Pred _pred) {
			for (int i = 0; i < currCount; i++) {
				_pred(container[i]);
			}
		}
		const size_t capacity()const noexcept{ return Count; };
		const size_t size()const noexcept{ return currCount; };
		const bool not_constructed() { return !container; };
		void shrink_to_fit() noexcept{
			if (currCount < Count) {
				auto newalloc = (T*)realloc(container, sizeof(T) * currCount);
				if (newalloc) {
					container = newalloc;
					Count = currCount;
				}
			}
		}
		//Only expands the Pool
		//May invalidate pointers
		void reserve(size_t n) noexcept{
			if (n > Count) {
				auto newalloc = (T*)realloc(container, sizeof(T) * n);
				if (newalloc) {
					container = newalloc;
					Count = n;
				}
			}
		}
		//May invalidate pointers
		void resize(size_t count, T value) noexcept{
			if (count == Count)return;
			auto newalloc = (T*)realloc(container, sizeof(T) * count);
			if (newalloc) {
				container = newalloc;
				Count = count;
				std::fill_n(container, count, value);
			}
		}
		T& operator[](size_t index) {
			return container[index];
		}
	};
}
#else
#endif