using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace System.Runtime.CompilerServices
{
    [EditorBrowsable(EditorBrowsableState.Never)]
    internal class IsExternalInit { }
}

namespace Sphynx.Core
{
    //Incomplete.
    internal class DynamicMarshaller
    {
        static AssemblyBuilder DynamicAssembly;
        static ModuleBuilder DynamicModule;
        static List<Type> DynTypes;

        internal static void Initalize()
        {
            var DynAsmName = new AssemblyName("Sphynx.Dynamic");
            DynAsmName.CultureInfo = CultureInfo.InvariantCulture;
            DynamicAssembly = AssemblyBuilder.DefineDynamicAssembly(DynAsmName, AssemblyBuilderAccess.RunAndSave);
            DynamicModule = DynamicAssembly.DefineDynamicModule(DynAsmName.Name ?? "Sphynx.Dynamic");
        }

        /// <summary>
        /// Flattens the struct to a list of Types.
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static Type[] Flatten(Type type) 
        {
            if (DynamicAssembly == null) Initalize();
            Console.WriteLine(type.FullName);
            var fields = type.GetFields(BindingFlags.NonPublic | BindingFlags.Public |
                         BindingFlags.Instance);
            List<Type> DynaFields = new List<Type>();
            Console.WriteLine($"Field Count : {fields.Length}");
            foreach (var field in fields) 
            {
                if (field.FieldType.IsPrimitive)
                {
                    Console.WriteLine($"public {field.FieldType} {field.Name};");
                    DynaFields.Add(field.FieldType);
                }
                else if (field.FieldType.IsArray)
                {
                    //Planing to avoid MonoArray*.
                    //Array is Pointer to first element.
                    Type ElemType = field.FieldType.GetElementType();
                    ElemType = ElemType.IsClass ? typeof(object) : ElemType;
                    Console.WriteLine($"public {ElemType}* {field.Name};");
                    DynaFields.Add(ElemType.MakePointerType());
                }
                else if (field.FieldType.IsEnum)
                {
                    //Flatten Enum to underlaying type.
                    Console.WriteLine($"public Enum:{field.FieldType} {field.Name};");
                    DynaFields.Add(Enum.GetUnderlyingType(field.FieldType));
                }
                else if (field.FieldType.IsValueType)
                {
                    Console.WriteLine($"Flattening Value Type {field.FieldType}.");
                    DynaFields.AddRange(Flatten(field.FieldType));
                }
                else
                {
                    //MonoObject*
                    Console.WriteLine($"public MonoObject* {field.FieldType}/{field.Name};");
                    DynaFields.Add(typeof(object));
                }
            }
            return DynaFields.ToArray();
        }

        public static TypeBuilder PrepareNewType(string TypeName)
        {
            return DynamicModule.DefineType(TypeName, TypeAttributes.Public | TypeAttributes.SequentialLayout | TypeAttributes.Class | TypeAttributes.AnsiClass);
        }

        public static void AddFields(TypeBuilder DynType, Type[] fields)
        {
            foreach (var field in fields)
            {
                var fb = DynType.DefineField(field.Name, field, FieldAttributes.Public);
            }
        }

        public static Type CreateType(TypeBuilder DynType)
        {
            Type CreatedType = DynType.CreateType();
            DynTypes.Add(CreatedType);
            return CreatedType;
        }
    }
}