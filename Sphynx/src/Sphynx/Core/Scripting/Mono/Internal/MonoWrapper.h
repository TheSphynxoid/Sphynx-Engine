#pragma once

typedef struct _MonoProperty MonoProperty;
typedef struct _MonoClass MonoClass;
typedef struct _MonoObject MonoObject;
typedef struct _MonoReflectionType MonoReflectionType;
typedef struct _MonoException MonoException;
typedef struct _MonoClassField MonoClassField;

namespace Sphynx::Mono {
	class Class;
	class Object {
	private:
		MonoObject* WrappedObject;
		Class* InstanceOf;
	public:
		Object(Class* c);
		Object(Class* c, MonoObject* obj);
		Class* GetClass() { return InstanceOf; }
	};
	class Field {
	private:
		MonoClassField* WrappedField;
	public:
		Field(MonoClassField* field);
		Field(Class c, std::string name);
		void SetValue(Object object);
	};
	class Class
	{
	private:
		MonoClass* WrappedClass;
	public:
		Class(MonoClass* monoclass);
		Object InstantiateDefault();
		Object Instantiate(void** Params, size_t count);
		const char* GetName();
		void* GetNative();
		Field GetField(std::string Name);
	};
}