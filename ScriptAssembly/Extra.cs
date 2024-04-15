using Sphynx.Graphics;
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
    internal static class DynamicMarshaller
    {
        static AssemblyBuilder DynamicAssembly;
        static ModuleBuilder DynamicModule;
        static List<Type> DynTypes;

        internal static void Initialize()
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
            if (DynamicAssembly == null) Initialize();
            Console.WriteLine(type.FullName);
            var fields = type.GetFields(BindingFlags.NonPublic | BindingFlags.Public |
                         BindingFlags.Instance);
            List<Type> dynaFields = new List<Type>();
            Console.WriteLine($"Field Count : {fields.Length}");
            foreach (var field in fields) 
            {
                if (field.FieldType.IsPrimitive)
                {
                    Console.WriteLine($"public {field.FieldType} {field.Name};");
                    dynaFields.Add(field.FieldType);
                }
                else if (field.FieldType.IsArray)
                {
                    //Planing to avoid MonoArray*.
                    //Array is Pointer to first element.
                    Type ElemType = field.FieldType.GetElementType();
                    ElemType = ElemType.IsClass ? typeof(object) : ElemType;
                    Console.WriteLine($"public {ElemType}* {field.Name};");
                    dynaFields.Add(ElemType.MakePointerType());
                }
                else if (field.FieldType.IsEnum)
                {
                    //Flatten Enum to underlying type.
                    Console.WriteLine($"public Enum:{field.FieldType} {field.Name};");
                    dynaFields.Add(Enum.GetUnderlyingType(field.FieldType));
                }
                else if (field.FieldType.IsValueType)
                {
                    Console.WriteLine($"Flattening Value Type {field.FieldType}.");
                    dynaFields.AddRange(Flatten(field.FieldType));
                }
                else
                {
                    //MonoObject*
                    Console.WriteLine($"public MonoObject* {field.FieldType}/{field.Name};");
                    dynaFields.Add(typeof(object));
                }
            }
            return dynaFields.ToArray();
        }

        public static TypeBuilder PrepareNewType(string typeName)
        {
            return DynamicModule.DefineType(typeName, TypeAttributes.Public | TypeAttributes.SequentialLayout | TypeAttributes.Class | TypeAttributes.AnsiClass);
        }

        public static void AddFields(TypeBuilder dynType, Type[] fields)
        {
            foreach (var field in fields)
            {
                var fb = dynType.DefineField(field.Name, field, FieldAttributes.Public);
            }
        }

        public static Type CreateType(TypeBuilder DynType)
        {
            Type createdType = DynType.CreateType();
            DynTypes.Add(createdType);
            return createdType;
        }
    }
    public static class GraphicsExtensions
    {
        /// <summary>
        /// <c>true</c>, if contains depth data.
        /// </summary>
        public static bool IsDepth(this Texture tex)
        {
            return tex.Format switch
            {
                TextureFormat.Depth_Component16 or TextureFormat.Depth_Component24 or TextureFormat.Depth_Component32 or 
                TextureFormat.Depth_Component32F or TextureFormat.Depth24_Stencil8 or TextureFormat.Depth32F_Stencil8 => true,
                _ => false,
            };
        }
        public static bool IsStencil(this Texture tex)
        {
            return tex.Format switch
            {

                TextureFormat.Stencil or TextureFormat.Depth24_Stencil8 or TextureFormat.Depth32F_Stencil8 => true,
                _ => false
            };
        }
        /// <summary>
        /// <c>true</c>, if contains color data.
        /// </summary>
        public static bool IsImage(this Texture tex)
        {
            return !tex.IsDepth() && tex.Format != TextureFormat.Stencil;
        }

        public static Texture Compress(this Texture tex)
        {
            return null;
        }
    }
}
