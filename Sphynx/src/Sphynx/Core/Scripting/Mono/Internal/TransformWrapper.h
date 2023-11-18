#pragma once

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

namespace Sphynx::Mono {
	class TransformWrapper
	{
	private:
		static inline MonoClass* TransformClass;
	public:
		friend class MonoRuntime;

	};
}
