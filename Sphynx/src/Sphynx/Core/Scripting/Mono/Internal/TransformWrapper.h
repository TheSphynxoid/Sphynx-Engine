#pragma once
#include "GameObject.h"

struct _MonoObject;
typedef _MonoObject MonoObject;
struct _MonoProperty;
typedef _MonoProperty MonoProperty;
struct _MonoClass;
typedef _MonoClass MonoClass;
struct _MonoReflectionType;
typedef _MonoReflectionType MonoReflectionType;
struct _MonoException;
typedef _MonoException MonoException;
struct _MonoClassField;
typedef _MonoClassField MonoClassField;

namespace Sphynx::Mono {
	class TransformWrapper
	{
	private:
		static inline MonoClass* TransformClass;
		MonoObject* TransformObject;
	public:
		TransformWrapper();
		//Takes a Managed GameObject
		TransformWrapper(MonoObject* GO);
		friend class MonoRuntime;

	};
}
