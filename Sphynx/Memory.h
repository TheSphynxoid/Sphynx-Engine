#pragma once

namespace Sphynx::Core {
	struct Block final {
	private:
		size_t Size;
		void* BlockStart;
	public:
		Block()noexcept : BlockStart(0), Size(0) {}
		Block(void* p, size_t size)noexcept : BlockStart(p), Size(size) {}
		size_t GetSize()const noexcept { return Size; }
		void Fill(int val)noexcept {
			memset(BlockStart, val, Size);
		}
		void Clear()noexcept {
			Fill(0);
		}
		template<typename T, size_t align = alignof(T)>
		std::unique_ptr<T[]> ToTArray_aligned(size_t& Count) {
			if (align % 2) {
				Core_Error("Alignment is not power of 2.");
				return nullptr;
			}
			void* AlignedBlock = std::align(align, sizeof(T), BlockStart, Size);
			if (!AlignedBlock) {
				Core_Error("Unable to align block.");
				return nullptr;
			}
			Count = Size % align;
			std::unique_ptr<T[]> p = std::make_unique<T[]>(Count);
			return p;
		}
		template<typename T, size_t align = alignof(T)>
		std::unique_ptr<T[]> ToTArray(size_t& Count) {
			Count = Size % align;
			std::unique_ptr<T[]> p = std::make_unique<T[]>(Count);
			return p;
		}

		friend class Memory;
	};
	class Memory final
	{
	private:
		inline static std::unordered_map<void*, Block> AllocatedBlocks = std::unordered_map<void*, Block>();
	public:
		inline static const Block NullBlock = Block(0, 0);
		static Block AllocateBlock(size_t blocksize) {
			void* mem = malloc(blocksize);
			Block b(mem, blocksize);
			AllocatedBlocks.insert(std::pair(mem, b));
			return SPH_Move(b);
		}
		static void FreeBlock(void* blockptr) {
			auto b = AllocatedBlocks.find(blockptr);
			if (b != AllocatedBlocks.end()) {
				free(blockptr);
				auto bval = (*b).second;
				bval.BlockStart = nullptr;
				bval.Size = 0;
				AllocatedBlocks.erase(blockptr);
			}
		}
		static Block& GetBlock(void* address)noexcept {
			return AllocatedBlocks.find(address) == AllocatedBlocks.end() ? Block(static_cast<const Block&>(NullBlock)) : AllocatedBlocks[address];
		}
		bool IsEngineBlock(const Block& b) {
			return AllocatedBlocks.find(b.BlockStart) == AllocatedBlocks.end();
		}
	private:
	};

}