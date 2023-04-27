#pragma once
#ifndef Sphynx_Pool
#define Sphynx_Pool
#include <memory>
#include <deque>
#ifndef Def_Start_Size
#define Def_Start_Size 10
#ifndef SPH_Forward
#define SPH_Forward(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
#endif
#ifndef SPH_Move
#define SPH_Move(...)	static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#endif
#endif


namespace Sphynx {
	//Pool is a container that does not ensure element position,what this means is that a new elements is not guaranteed to be "size-1"
	//this container reuses the space of removed elements as-is instead of moving them.
	//this container is neither sequanced (Elements are in the same buffer but not next to each other calling defragment fixes this) nor linked.
	template<class T>
	class Pool final {
	private:
		T* container = nullptr;
		size_t ObjCount;
		std::deque<size_t> FreeIndexes;
		size_t currCount = 0;
		short delCount = 0;
	public:
		typedef void(Fill_t)(T& value);
		Pool() {
			container = (T*)malloc(sizeof(T) * Def_Start_Size);
			if (!container)throw std::bad_alloc();
			memset(container, 0, sizeof(T) * Def_Start_Size);
			ObjCount = Def_Start_Size;
		}
		Pool(Fill_t func) {
			container = (T*)malloc(sizeof(T) * Def_Start_Size);
			if (!container)throw std::bad_alloc();
			for (int i = 0; i < Def_Start_Size; i++) {
				func(container[i]);
			}
			memset(container, 0, sizeof(T) * Def_Start_Size);
			ObjCount = Def_Start_Size;
		}
		Pool(size_t size) {
			container = (T*)malloc(sizeof(T) * size);
			if (!container)throw std::bad_alloc();
			memset(container, 0, sizeof(T) * size);
			ObjCount = size;
		}
		Pool(size_t size, Fill_t func) {
			container = (T*)malloc(sizeof(T) * size);
			if (!container)throw std::bad_alloc();
			for (int i = 0; i < size; i++) {
				func(container[i]);
			}
			ObjCount = size;
		}
		//May invalidate all other pointers if the the buffer location changes
		void push(T& elem) noexcept {
			if (FreeIndexes.empty()) {
				if (ObjCount == currCount) {
					reserve(ObjCount + Def_Start_Size);
				}
				container[currCount] = elem;
				currCount++;
			}
			else {
				container[FreeIndexes.front()] = elem;
				FreeIndexes.pop();
			}
		}
		void insert(T& elem, size_t pos) {
			if (this->ObjCount > pos <= 0) {
				if (currCount + 1 > ObjCount) {
					//Expand the Array.
					reserve(ObjCount + Def_Start_Size);
				}
				for (int i = currCount; i > pos; i--) {
					container[i + 1] = container[i];
				}
				container[pos] = elem;
			}
		}
		//Will not invalidate any pointer except the deleted one.
		void remove(size_t index) {
			if (index != currCount - 1) {
				FreeIndexes.push(index);
			}
			container[index].~T();
			if (delCount++ >= 5) {
				defragment();
			}
		}
		//Will not invalidate any pointer except the deleted one.
		void remove_cmp(T& cond) {
			for (int i = 0; i < currCount; i++) {
				if (cond == container[i]) {
					FreeIndexes.push(i);
					container[i].~T();
					delCount++;
					return;
				}
			}
		}
		//doesn't ensure the validity of each object. (uses for loop so it's never out of bounds)
		template<typename Pred, std::enable_if_t<std::is_invocable_v<Pred>>>
		void unchecked_for_each(Pred _pred) {
			for (int i = 0; i < currCount; i++) {
				_pred(container[i]);
			}
		}
		const size_t capacity()const noexcept { return ObjCount; };
		const size_t size()const noexcept { return currCount; };
		const bool not_constructed() noexcept { return !container; };
		//Moves objects to empty spots in the buffer making them in sequance
		//Still not tested
		void defragment() {
			//while (!FreeIndexes.empty()) {
			//	size_t index = FreeIndexes.front();
			//	FreeIndexes.pop();
			//	memset((void*)&container[index], 0, sizeof(T));
			//	container[index] = SPH_Move((T)container[index + 1]);
			//}

			for (auto index : FreeIndexes) {
				memset((void*)&container[index], 0, sizeof(T));
				container[index] = SPH_Move((T)container[index + 1]);
			}
			FreeIndexes.clear();
		}
		////
		//std::enable_if_t<!std::is_copy_constructible_v<T>, void> defragment() {
		//}
		void shrink_to_fit() noexcept {
			if (currCount < ObjCount) {
				auto newalloc = (T*)realloc(container, sizeof(T) * currCount);
				if (newalloc) {
					container = newalloc;
					ObjCount = currCount;
				}
			}
		}
		//Only expands the Pool
		//May invalidate pointers
		void reserve(size_t n) noexcept {
			if (n > ObjCount) {
				auto newalloc = (T*)realloc(container, sizeof(T) * n);
				if (newalloc) {
					container = newalloc;
					ObjCount = n;
				}
			}
		}
		//May invalidate pointers
		void resize(size_t count, T value) noexcept {
			if (count == ObjCount)return;
			auto newalloc = (T*)realloc(container, sizeof(T) * count);
			if (newalloc) {
				container = newalloc;
				ObjCount = count;
				std::fill_n(container, count, value);
			}
		}
		//No bounds checking and no empty value checking
		T* data()const noexcept {
			return container;
		}
		//Skips empty objects to make the pool seem sequenced.
		T& operator[](size_t index) {
			if (std::find(FreeIndexes.begin(), FreeIndexes.end(), index) == FreeIndexes.end()) {
				return container[index];
			}
			return container[index + 1];
		}
	};
}
#else
#endif