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
	};

	class Deallocator_Base {
	protected:
		virtual void I_Delete() = 0;
	public:
		void Delete() {
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
		Deallocator(T* _ptr) {
			ptr = _ptr;
		}
	};
	template<class T>
	class ArrayDeallocator : public Deallocator_Base {
		T* ptr;
		size_t size;
		void I_Delete() {
			delete[] ptr;
		}
	public:
		ArrayDeallocator(T* _ptr) {
			ptr = _ptr;
			size = sizeof(ptr);
		}
	};
	class PointerBase {
	private:
	public:
		virtual ~PointerBase() {};
		virtual void* GetRaw() const = 0;
	protected:
		int RefCount = 0;
		void AddRef() {
			RefCount++;
		}
		void RemoveRef() {
			RefCount--;
		}
		inline int GetRefCount() const { return RefCount; };
		inline void SetRefCount(int c) { RefCount = c; };
	public:
		virtual bool operator==(const PointerBase&& ptr) {
			return this->GetRaw() == ptr.GetRaw();
		}
	};
	class PointerRegistry {
	private:
		std::map<void*,PointerBase*> Addresses;
	public:
		PointerBase* CreatePointer(void* p) {
			for (auto&[v,ptr] : Addresses) {
				if (ptr->GetRaw() == p) {
					//Get an Already existing pointer
					return ptr;
				}
			}
			
		}
	};

	template<class T>
	class Pointer final : PointerBase {
	public:
		typedef std::remove_extent_t<T> Element;
	private:
		__Base_Delegate<void, T&>* delegate;
		T* Object;
		//For Type Erasure.
		Deallocator_Base* dealloc;
		struct ArrayID {
			bool IsArray = false;
			bool IsBounded = true;
			unsigned int Dimensions = 0;
			//std::list<unsigned int> ElementSizePerRank;
		}array_info;
		//Causes Object to self-delete.
		bool SelfDestroy = false;
		//Constructor
		Pointer(T* ptr) :Object(ptr) {
			if (std::is_array<T>::value == true) {
				array_info.IsArray = true;
				dealloc = new ArrayDeallocator<T>(ptr);
				array_info.Dimensions = std::rank<T>{};
				if (array_info.Dimensions == 0)array_info.IsBounded = false;
				if (array_info.IsBounded) {
					const int TotalElements = sizeof(T) / sizeof(std::remove_all_extents_t<T>);
				}
				delegate = NULL;
			}
			else if(std::is_pointer_v<T>) {
				throw Invalid_Type<T>();
			}
			else {
				dealloc = new Deallocator<T>(ptr);
			}
			AddRef();
		};
	public:
		typedef T Type;
		//Move
		Pointer(Pointer&& ptr) noexcept {
			std::swap(this->dealloc, ptr.dealloc);
			std::swap(this->Object, ptr.Object);
			this->SetRefCount(ptr.GetRefCount());
			std::swap(this->SelfDestroy, ptr.SelfDestroy);
		}
		//Copy
		Pointer(const Pointer& ptr) noexcept {
			this->dealloc = ptr.dealloc;
			this->Object = ptr.Object;
			this->SetRefCount(ptr.GetRefCount());
			this->SelfDestroy = ptr.SelfDestroy;
			AddRef();
		}
		//Destructor
		~Pointer() {
			RemoveRef();
			if (GetRefCount() == 0 && Object) {
				delegate->Invoke(*Object);
				dealloc->Delete();
				delete delegate;
			}
		}
		//Releases The Object
		void Release() 
		{
			if (SelfDestroy)delete this;
			else this->~Pointer();
		};
		//Array

		bool IsTypeArray() { return array_info.IsArray; };
		bool IsBoundedArray() { return array_info.IsBounded; };
		template<unsigned int Ix = 0>
		constexpr const unsigned int GetExtentInRank() { return std::extent<T, Ix>::value; };
		//Changes the pointer to the specified object.
		inline void Swap(Pointer ptr) {
			std::swap(this->Object, ptr.Object);
			//this->Object = ptr.Object; 
			ptr.AddRef(); 
		};

		T* GetRaw() { return Object; };
		void* GetRaw() const override { return (void*)Object; };
		T& operator* ()
		{
			return *Object;
		}
		T* operator->() {
			return Object;
		}
		Pointer& operator=(const Pointer& ptr) {
			T* const old = Object;
			int oldcount = RefCount;
			Object = ptr.Object;
			++RefCount;
			if (--oldcount == 0) delete old;
			return *this;
		};
		bool operator==(const Pointer& ptr) {
			return ptr.Object == this->Object;
		};
		bool operator==(const T* ptr) {
			return ptr == this->Object;
		};
		//Copies The Delegate Object
		template<typename Inst>
		void SetDestroyCallBack(Delegate<void, Inst, T&> cb) {
			//auto placeholder = delegate;
			delegate = new Delegate(cb);
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
	template<class Obj, typename... Args>
	[[nodiscard]] static std::enable_if_t<!std::is_array_v<Obj>, Pointer<Obj>*> AllocatePointer(Args&&... args) {
		Pointer<Obj>* ptr = new Pointer<Obj>(new Obj(std::forward<Args>(args)...));
		ptr->SelfDestroy = true;
		return ptr;
	}
	template<class Obj, typename... Args>
	[[nodiscard]] static std::enable_if_t<std::is_array_v<Obj>, Pointer<Obj>*> AllocatePointer(Args&&... args) {
		//I think this shouldn't work.
		Pointer<Obj>* ptr = new Pointer<Obj>(new std::remove_pointer_t<Obj>[std::extent_v<Obj>](std::forward<Args>(args)...));
		ptr->SelfDestroy = true;
		return ptr;
	}
}