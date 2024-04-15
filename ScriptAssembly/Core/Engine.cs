using System;
using Sphynx.Graphics;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.Contracts;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;

namespace Sphynx.Core
{
    public enum GraphicsBackend
    {
        OpenGL, Vulkan, DirectX11, DirectX12
    }
    /// <summary>
    /// Engine Utility Class.
    /// Contains some marshalling utilities.
    /// </summary>
    public static class Engine
    {
        ///// <summary>
        ///// Gets the args that the Application received to start.
        ///// </summary>
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public extern static string[] GetEngineArgs();
        /// <summary>
        /// If the C# Execution Engine is Running on Mono this has a true value.
        /// (This is here in case I decide to add the MicrosoftCLR or modified mono runtime)
        /// </summary>
        public static readonly bool IsMono = (Type.GetType("Mono.Runtime") != null);

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public extern static GraphicsBackend GetGraphicsBackend();

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public extern static void NativeDebuggerBreak();

        [AllowReversePInvokeCalls]
        static void RuntimeSetup()
        {
            AppDomain.CurrentDomain.AssemblyResolve += (sender, args) => {
                var resourceName = "Sphynx." + new AssemblyName(args.Name).Name + ".dll";
                using (var stream = typeof(Engine).Assembly.GetManifestResourceStream(resourceName))
                {
                    byte[] assemblyData = new byte[stream.Length];
                    stream.Read(assemblyData, 0, assemblyData.Length);
                    return Assembly.Load(assemblyData);
                }
            };
        }

        public static void NativeBreak()
        {
            StackTrace trace = new StackTrace();
            Console.WriteLine("Native Offset : {0}", trace.GetFrame(0).GetNativeOffset());
            //NativeDebuggerBreak();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr ArrayToPointer(Array array, int index);

        /// <summary>
        /// Gets the address of the data in a buffer or returns <see cref="IntPtr.Zero"/> in case the array is null.
        /// </summary>
        /// <param name="array">Array to get data address from.</param>
        /// <param name="index">Index to start from.</param>
        [SuppressUnmanagedCodeSecurity]
        public static unsafe IntPtr GetArrayPointer(Array array, int index = 0)
        {
            return array != null? ArrayToPointer(array, index) : IntPtr.Zero;
        }

        [SuppressUnmanagedCodeSecurity]
        internal static unsafe IntPtr ToPointer(this Array array, int index = 0)
        {
            return GetArrayPointer(array, index);
        }

        /// <summary>
        /// Transforms a primitive array to a byte array.
        /// </summary>
        /// <typeparam name="T">Primitive/Unmanaged type of the array</typeparam>
        /// <returns>A new byte[]</returns>
        public static unsafe byte[] ToByteArray<T>(T[] array) where T : unmanaged
        {
            byte[] buf = new byte[Buffer.ByteLength(array)];
            Buffer.BlockCopy(array, 0, buf, 0, buf.Length);
            return buf;
        }

        /// <summary>
        /// Used for debugging in core to check the structure of an object in memory. Calls DebuggerBreak in native.
        /// </summary>
        /// <param name="obj"></param>
        /// <param name="unbox">Set to true if the object is a value type.</param>
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void CoreCheckObject(object obj, bool unbox);
    }
    /// <summary>
    /// Marker for the originating header for wrapped type.
    /// </summary>
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct, Inherited = false, AllowMultiple = false)]
    public sealed class HeaderAttribute : System.Attribute
    {
        readonly string headerName;
        public string NativeTypeName { get; set; }

        public string HeaderName { get => headerName; }
        public HeaderAttribute(string headername) { this.headerName = headername; }
    }

    /// <summary>
    /// Marker for Wrappers and additional information.
    /// </summary>
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct, Inherited = false, AllowMultiple = false)]
    internal sealed class NativeWrapperAttribute : System.Attribute
    {
        readonly string nativeName;
        readonly bool isVirtual;

        public NativeWrapperAttribute(string nativeName, bool isVirtual = false)
        {
            this.nativeName = nativeName;
            this.isVirtual = isVirtual;
        }

        public string NativeName
        {
            get { return nativeName; }
        }
        public bool IsVirtual { get => isVirtual; }
    }
}
