#pragma once
#include <memory>
#include <map>
#include <type_traits>
#include <exception>
#include <list>
#include "Delegate.h"

///These are Utility so they are not Object nor GameObject nor Components.

namespace Sphynx {
	template <typename T>
	class Invalid_Type : public std::exception {
	public:
		Invalid_Type() : std::exception("Provided an unvalid Type") {};
		Invalid_Type(const char* e) : std::exception(e) {};
	};

	struct Dtor {
	private:
		void(*_dtor)(void*);
		void* obj;
	public:
		template<class T>
		Dtor(T* t) noexcept{
			obj = t;
			//Since we can't have a pointer to the destructor.
			_dtor = [](void* o) ->void {((T*)o)->~T(); };
		}
		void Destroy()noexcept {
			_dtor(obj);
		}
	};

	class Deallocator_Base {
	protected:
		virtual void I_Delete() = 0;
	public:
		void Delete()noexcept {
			I_Delete();
		}
	};
	//I just Discovered std::allocator...
	//                              F
	template<class T>
	class Deallocator : public Deallocator_Base {
		T* ptr;
		void I_Delete() {
			delete ptr;
		}
	public:
		Deallocator(T* _ptr)noexcept {
			ptr = _ptr;
		}
	};
	template<class T>
	class ArrayDeallocator : public Deallocator_Base {
		T* ptr;
		size_t Size;
		void I_Delete() {
			delete[] ptr;
		}
	public:
		ArrayDeallocator(T* _ptr)noexcept {
			ptr = _ptr;
			Size = sizeof(ptr);
		}
	};
	class PointerBase {
	private:
	public:
		virtual ~PointerBase()noexcept {};
		virtual void* GetRaw() const = 0;
	protected:
		int RefCount = 0;
		void AddRef()noexcept {
			RefCount++;
		}
		void RemoveRef()noexcept {
			RefCount--;
		}
		inline int GetRefCount() const noexcept { return RefCount; };
		inline void SetRefCount(int c)noexcept { RefCount = c; };
	public:
		virtual bool operator==(const PointerBase&& ptr)const noexcept {
			return this->GetRaw() == ptr.GetRaw();
		}
	};

	template<class T>
	class Pointer final : PointerBase {
	public:
		typedef std::remove_extent_t<T> Element;
	private:
		BaseDelegate<void, T&>* delegate;
		T* Object = nullptr;
		//For Type Erasure.
		Deallocator_Base* dealloc = nullptr;
		struct ArrayID {
			constexpr bool IsArray = std::is_array<T>::value;
			bool IsBounded = true;
			unsigned int Dimensions = 0;
			int TotalElements;
			//std::list<unsigned int> ElementSizePerRank;
		}array_info;
		//Causes Object to self-delete.
		bool SelfDestroy = false;
		//Constructor
	public:
		Pointer()noexcept {}
		Pointer(T* ptr)noexcept :Object(ptr) {
			if (std::is_array<T>::value == true) {
				array_info.IsArray = true;
				dealloc = new ArrayDeallocator<T>(ptr);
				array_info.Dimensions = std::rank<T>{};
				if (array_info.Dimensions == 0)array_info.IsBounded = false;
				if (array_info.IsBounded) {
					array_info.TotalElements = sizeof(T) / sizeof(std::remove_all_extents_t<T>);
				}
				delegate = NULL;
			}
			else if(std::is_pointer_v<T>) {
				//throw Invalid_Type<T>("A pointer of any type is not allowed");

				//Weird.
				this->Object = ptr;
			}
			else {
				dealloc = new Deallocator<T>(ptr);
			}
			AddRef();
		};
		typedef T Type;
		//Move
		Pointer(Pointer&& ptr) noexcept {
			std::swap(this->dealloc, ptr.dealloc);
			std::swap(this->Object, ptr.Object);
			this->SetRefCount(ptr.GetRefCount());
			std::swap(this->SelfDestroy, ptr.SelfDestroy);
		}
		//Copy
		Pointer(const Pointer& ptr) = delete;
		//Destructor
		~Pointer()noexcept {
			RemoveRef();
			if (GetRefCount() == 0 && Object) {
				delegate->Invoke(*Object);
				dealloc->Delete();
				delete delegate;
			}
		}
		//Releases The Object
		void Release() noexcept
		{
			if (SelfDestroy)delete this;
			else this->~Pointer();
		};
		//Array

		constexpr bool IsTypeArray()const noexcept { return array_info.IsArray; };
		bool IsBoundedArray() { return array_info.IsBounded; };
		template<unsigned int Ix = 0>
		constexpr const unsigned int GetExtentInRank() { return std::extent<T, Ix>::value; };
		//Changes the pointer to the specified object.
		inline void Swap(Pointer ptr) noexcept{
			std::swap(this->Object, ptr.Object);
			//this->Object = ptr.Object; 
			ptr.AddRef(); 
		};

		T* GetRaw()const noexcept { return Object; };
		void* GetRaw() const noexcept override { return (void*)Object; };
		T& operator* ()const noexcept
		{
			return *Object;
		}
		T* operator->() noexcept {
			return Object;
		}
		Pointer& operator=(const Pointer& ptr) noexcept {
			T* const old = Object;
			int oldcount = RefCount;
			Object = ptr.Object;
			++RefCount;
			if (--oldcount == 0) delete old;
			return *this;
		};
		/*Pointer& operator=(const T& ptr) noexcept {
			
		}*/
		bool operator=(Pointer&& ptr) noexcept{
			if (this != &ptr) {
				this->Object = ptr.Object;
				this->array_info = ptr.array_info;
				this->dealloc = ptr.dealloc;
				this->delegate = ptr.delegate;
			}
		};
		bool operator==(const T* ptr) noexcept {
			return ptr == this->Object;
		};
		//Copies The Delegate Object
		template<typename Inst>
		void SetDestroyCallBack(Delegate<void, Inst, T&> cb) {
			//auto placeholder = delegate;
			delegate = new Delegate<void, Inst, T&>(cb);
			//return placeholder;
		}
		template<class Obj, typename... Args>
		friend std::enable_if_t<!std::is_array_v<Obj>, Pointer<Obj>*> AllocatePointer(Args&&... args);
		template<class Obj, typename... Args>
		friend std::enable_if_t<std::is_array_v<Obj>, Pointer<Obj>*> AllocatePointer(Args&&... args);
		template<class Obj, typename... Args>
		friend std::enable_if_t<!std::is_array_v<Obj>, Pointer<Obj>> CreatePointer(Args&&... args);
		template<class Obj, typename... Args>
		friend std::enable_if_t<std::is_array_v<Obj>, Pointer<Obj>> CreatePointer(Args&&... args);
	};
	template<class Obj, typename... Args>
	[[nodiscard]] static std::enable_if_t<!std::is_array_v<Obj>, Pointer<Obj>> CreatePointer(Args&&... args) {
		return Pointer<Obj>(new Obj(std::forward<Args>(args)...));
	}
	template<class Obj, typename... Args>
	[[nodiscard]] static std::enable_if_t<std::is_array_v<Obj>, Pointer<Obj>> CreatePointer(Args&&... args) {
		//This is probably wrong.
		return Pointer<Obj>(new std::remove_pointer_t<Obj>[std::extent_v<Obj>](std::forward<Args>(args)...));
	}
	//Remove THIS.
	//Application.cpp line 76 uses this
	template<class Obj, typename... Args>
	[[nodiscard]] static std::enable_if_t<!std::is_array_v<Obj>, Pointer<Obj>*> AllocatePointer(Args&&... args) {
		Pointer<Obj>* ptr = new Pointer<Obj>(new Obj(std::forward<Args>(args)...));
		ptr->SelfDestroy = true;
		return ptr;
	}
	template<class Obj, typename... Args>
	[[nodiscard]] static std::enable_if_t<std::is_array_v<Obj>, Pointer<Obj>*> AllocatePointer(Args&&... args) {
		//I think this shouldn't work.
		const size_t Size = std::index_sequence<sizeof...(args)>::Size();
		Pointer<Obj>* ptr = new Pointer<Obj>(new std::remove_pointer_t<Obj>[std::extent_v<Obj>]());
		ptr->SelfDestroy = true;
		return ptr;
	}

	//Pointer Registry
	//Manages all pointers and ensures proper deletion of unrefrenced object
	class PointerRegistry {
	private:
		inline static std::map<size_t, PointerBase*> PtrReg;
	public:
		static PointerBase* GetPointer(size_t address) {
			return PtrReg[address];
		}
	};
}