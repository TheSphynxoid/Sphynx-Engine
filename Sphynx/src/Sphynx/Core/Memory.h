#pragma once
#include <new>
#include <vector>

#define DetailBlockSize sizeof(BlockDetails) * PageCount * 15

namespace Sphynx::Core {
	//TODO: InComplete.
	class Memory {
	private:
		class Deleter_Base {
		protected:
			virtual void _delete() = 0;
		public:
			void Delete() {
				_delete();
			}
		};
		template<class T>
		class Deleter : public Deleter_Base
		{
			Deleter(T* ptr) {
				ptr->~T();
			}
		};
	private:
		struct BlockDetails {
			void* Block;
			size_t Size;
			BlockDetails* PrevBlock = nullptr;
			BlockDetails* NextBlock = nullptr;
			Deleter_Base* _deleter;
			bool isEmpty;
		};
		inline static void** Pages;
		inline static size_t PageSize = sizeof(void*) * 2048;
		inline static size_t PageCount;
		inline static std::vector<BlockDetails> Blocks;
		inline static BlockDetails* DetailList;
		inline static bool AllPagesFull = false;
		static BlockDetails& GetFreeBlock(size_t size) {
			for (auto i = Blocks.begin(); i < Blocks.end(); i++) {
				//Get the Block Detail.
				auto& block = *i;
				//Check if there is available space in the All Pages.
				if (block.Size >= size && block.isEmpty) {
					//There is Space.
					//The block is Now Full.
					block.isEmpty = false;
					//The Block's Size will change.
					auto leftout = block.Size - size;
					//The Blocks is an Exact fit.
					if (leftout == 0) {
						if ((i + 1) < Blocks.end()) {
							block.NextBlock = &(*std::next(i));
							block.PrevBlock = &(*std::prev(i));
						}
					}
					//The Block is not an exact fit.
					else {
						BlockDetails b = { (void*)((size_t)block.Block + size),leftout,&block,nullptr,nullptr,true };
						b.PrevBlock = &(*std::prev(i));
						Blocks.insert(std::next(i), b);
					}
					return block;
				}
			}
		}
		static BlockDetails& GetBlockDetail(void* BlockPtr) {
			return Blocks[0];
			//TODO:
		}
	public:
		static void InitMemory() {
			DetailList = (BlockDetails*)malloc(sizeof(BlockDetails) * PageCount * 15);
			memset(DetailList, 0, DetailBlockSize);
			for (int i = 0; i < PageCount; i++) {
				Pages[i] = malloc(PageSize);
				DetailList[i] = { Pages[i],sizeof(PageSize),nullptr,nullptr,nullptr,true };
			}
		}
		static void SetPageSize(size_t Size) {
			PageSize = Size;
			for (int i = 0; i < PageCount; i++) {
				//TODO: Write a Page struct that holds Pages Size.
				//Calling Realloc has a chance to invalidate all Object within the page.
				//Pages[i] = realloc(Pages[i], Size);
			}
		}
		static void AllocatePages(size_t numPages) {
			for (int i = PageCount; i < PageCount + numPages; i++) {
				Pages[i] = malloc(PageSize);
			}
			PageCount += numPages;
		}
		static void* GetPage(size_t index) {
			return Pages[index];
		}
		template<class T, class ...Args>
		static T* AllocateObject(Args&& ...args) {
			auto b = GetFreeBlock(sizeof(T));
			if (!std::is_array_v<T>)
				b._deleter = new Deleter<T>();
			return new(b.Block) T(std::forward<Args>(args)...);
		}
		static void ReleaseObject(void* block) {
			auto b = Memory::GetBlockDetail(block);
			b._deleter->Delete();
			Deallocate(block);
		}
		static void* Allocate(size_t size) {
			return GetFreeBlock(size).Block;
		}

		static void Deallocate(void* block) {
			for (auto& blockDetail : Blocks) {
				if (blockDetail.Block == block) {
					blockDetail.isEmpty = true;
					memset(block, 0, blockDetail.Size);
				}
			}
		}
	};
}

void* operator new(size_t size) {
	return Sphynx::Core::Memory::Allocate(size);
}
void* operator new[](size_t size) {
	return Sphynx::Core::Memory::Allocate(size);
}
void operator delete(void* ptr) {
	Sphynx::Core::Memory::Deallocate(ptr);
}
void operator delete[](void* ptr) {
	Sphynx::Core::Memory::Deallocate(ptr);
}