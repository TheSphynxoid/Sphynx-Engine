#pragma once
#include <memory>

namespace Sphynx {
	class Deallocator_Base {
	protected:
		virtual void I_Delete() = 0;
	public:
		void Delete() {
			I_Delete();
		}
	};
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
	//Bad on arrays
	template<class T>
	struct Pointer {
	private:
		T* Object;
		int RefCount = 0;
		//For Type Erasure.
		Deallocator_Base* dealloc;
		Pointer(T* ptr) :Object(ptr) { dealloc = new Deallocator<T>(ptr); };
	public:
		template<class Obj, typename... Args>
		static Pointer<Obj> CreatePointer(Args&&... args) {
			return Pointer<Obj>(new Obj(std::forward<Args>(args)))
		}
		~Pointer() {
			dealloc.Delete();
		}
		T* GetRaw() { return Object; };
		
	};
	template<class T>
	using SharedPointer = Pointer<T>;
}