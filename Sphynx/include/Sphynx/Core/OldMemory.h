#pragma once
#ifndef Sphynx_Memory
#define Sphynx_Memory
#include <new>
#include <memory>
#define Mem_Excep
#include "Exceptions.h"
#include "Pool.h"
#define DetailBlockSize sizeof(BlockDetail) * PageCount * 15

namespace Sphynx::Core {
	//TODO: Incomplete.
	class OMemory final {
	private:
		struct BlockDetail {
			void* Block;
			size_t Size;
			BlockDetail* PrevBlock = nullptr;
			BlockDetail* NextBlock = nullptr;
			bool isEmpty;
		};
		inline static size_t PageSize = sizeof(void*) * 2048ul * 10;
		inline static void** Pages = NULL;
		inline static size_t PageCount = 1;
		inline static std::vector<BlockDetail> Blocks;
		//inline static Pool<BlockDetail> Details;
		inline static BlockDetail* DetailList = NULL;
		inline static size_t BlockCount = 0;
		inline static size_t MaxBlockCount = 0;

		static BlockDetail& GetFreeBlock(size_t size) {
			for (auto i = 0; i < BlockCount; ++i) {
				//First Fit algorithm 
				//Get the Block Detail.
				auto block = DetailList[i];
				//Check if there is available space in the All Pages.
				if (block.Size >= size && block.isEmpty) {
					//There is Space.
					//The block is Now Full.
					block.isEmpty = false;
					//The Block's size will change.
					const auto leftout = block.Size - size;
					//The Blocks is an Exact fit.
					if (leftout == 0) {
						if ((i + 1) < BlockCount)
							block.NextBlock = &DetailList[i + 1];
						if ((i - 1) >= 0)
							block.PrevBlock = &DetailList[i - 1];
					}
					//The Block is not an exact fit.
					else {
						//Change the Old BlockDetail.
						DetailList[i] = { (void*)((size_t)block.Block + size),leftout,&block,&DetailList[i + 1],true };
						//Move it forward.
						for (int z = BlockCount; z > i; --z) {
							DetailList[z] = DetailList[z - 1];
						}
						//Insert the new BlockDetail.
						DetailList[i] = { block.Block,size,&DetailList[i - 1],&DetailList[i + 1],false };
						++BlockCount;
						if (BlockCount == MaxBlockCount) {
							auto nblock = (BlockDetail*)realloc(DetailList, sizeof(BlockDetail) * (MaxBlockCount + 20));
							if (!nblock) {
								throw Sphynx::Exception::OutOfMemory("Unable to allocate BlockDetails");
							}
							DetailList = nblock;
							MaxBlockCount += 20;
						}
					}
					return DetailList[i];
				}
			}
			auto p = (void**)realloc(Pages, sizeof(void*) * (++PageCount));
			if (!p) {
				throw Sphynx::Exception::OutOfMemory("Unable to allocate new Page");
			}
			Pages = p;
			Pages[PageCount] = malloc(PageSize);
			++BlockCount;
			if (BlockCount == MaxBlockCount) {
				auto nblock = (BlockDetail*)realloc(DetailList, sizeof(BlockDetail) * (MaxBlockCount + 30));
				if (!nblock) {
					throw Sphynx::Exception::OutOfMemory("Unable to allocate BlockDetails");
				}
				DetailList = nblock;
				MaxBlockCount += 30;
			}
			DetailList[BlockCount++] = { Pages[PageCount],PageSize,nullptr,&DetailList[BlockCount - 1],true };
			return GetFreeBlock(size);
		}
		static BlockDetail& GetBlockDetail(void* BlockPtr) {
			for (auto& b : Blocks) {
				if (b.Block == BlockPtr)return b;
			}
		}
		static void InitMemory() {
			DetailList = (BlockDetail*)malloc((sizeof(BlockDetail) * PageCount * 30));
			MaxBlockCount = 30 * PageCount;
			if (!DetailList)throw Sphynx::Exception::OutOfMemory("Unable to allocate BlockDetails");
			Pages = (void**)malloc(sizeof(void*) * PageCount);
			memset(DetailList, 0, DetailBlockSize);
			for (int i = 0; i < PageCount; ++i) {
				Pages[i] = malloc(PageSize);
				DetailList[i] = { Pages[i],PageSize,nullptr,nullptr,true };
				BlockCount++;
			}
		}
	public:
		void CollateBlocks() {
			size_t newBlocksCount;
			BlockDetail* CollatedBlocks;
			
			for (size_t i = 0; i < BlockCount; ++i) {
				
			}
		}
		static void SetPageSize(size_t Size) noexcept {
			PageSize = Size;
			for (int i = 0; i < PageCount; ++i) {
				//TODO: Write a Page struct that holds Pages size.
				//Calling Realloc has a chance to invalidate all Object within the page.
				//Pages[i] = realloc(Pages[i], size);
			}
		}
		//Unsafe
		static void* AllocatePrivateBlock(size_t size) noexcept {
			return malloc(size);
		}
		static void DeallocatePrivateBlock(void* block) noexcept {
			free(block);
		}
		static void** GetPagesArray() {
			return Pages;
		}
		static size_t GetPageSize() {
			return PageSize;
		}
		static void* Allocate(size_t size) {
			if (!Pages)
				InitMemory();
			return GetFreeBlock(size).Block;
		}

		static void Deallocate(void* block, size_t size) noexcept {
			for (auto i = 0; i < BlockCount; ++i) {
				if (DetailList[i].Block == block) {
					DetailList[i].isEmpty = true;
				}
			}
		}
	};
}
#ifndef Sph_New
#define Sph_New

//Will Work on it later.

//#ifdef _DEBUG && _WIN32
//[[nodiscard]] _Ret_notnull_ _Post_writable_byte_size_(size)
//#endif
//inline void* operator new(std::size_t size) {
//	return Sphynx::Core::Memory::Allocate(size);
//}
//#ifdef _DEBUG && _WIN32
//[[nodiscard]] _Ret_notnull_ _Post_writable_byte_size_(size)
//#endif
//inline void* operator new[](std::size_t size) {
//	return Sphynx::Core::Memory::Allocate(size);
//}
//inline void operator delete(void* ptr, size_t size) {
//	Sphynx::Core::Memory::Deallocate(ptr, size);
//}
#endif
#endif