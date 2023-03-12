#pragma once
#include "Events/Event.h"
#include "Logger.h"
#include "Pointer.h"
#include "Memory.h"
//#define NotImplementedFunction(name,...) name(__VA_ARGS__){Core_Warn(#name, " not implemented");};
#define NotImplementedFunction(sig) sig{Core_Warn(#sig, " not implemented");};
namespace Sphynx::Util {
	void InternalSkipCurrentEvent(Events::EventSystem& es) {
		es.SkipCurrentEvent();
	}
	////Holds an Object and a void** representing it.
	//template<class T>
	//union PointerData {
	//	//The Object.
	//	T* data;
	//	//Representing an object as a series of Pointers.
	//	void** ptr;
	//};
	//template<class T>
	//Pointer<T> WeirdMemberCopy(T* old) {
	//	PointerData<T> pd = PointerData<T>();
	//	pd.data = old;
	//	auto nobj = (T*)Sphynx::Internal::Memory::Allocate(sizeof(T));
	//	//Redundunt Copy
	//	memcpy(nobj, old, sizeof(T));
	//	for (int i = 0; i < sizeof(T) / sizeof(size_t); i++) {
	//		//Will this be reliable ?
	//		if (Sphynx::Internal::Memory::IsUsedBlock(pd.ptr[i])) {
	//			//This May or may not fuck things up...
	//			//This Will probably fuck things up because of destructor and object resource management
	//			//Unless i do this recusively which may be stupid.
	//			//On the other hand if this is used for serialization this may not cause problems, right ?
	//			//TODO: THINK.
	//			memcpy(((void**)nobj)[i], pd.ptr[i], Sphynx::Internal::Memory::GetBlockSize(pd.ptr[i]));
	//		}
	//	}
	//	return Pointer<T>(nobj);
	//}
};