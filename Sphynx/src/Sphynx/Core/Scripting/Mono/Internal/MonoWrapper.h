#pragma once

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
	class Class;
	class Object {
	private:
		MonoObject* WrappedObject;
		Class* InstanceOf;
	public:
		Object() = default;
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
		Object Instantiate();
		const char* GetName();
		Field GetField(std::string Name);
	};
}