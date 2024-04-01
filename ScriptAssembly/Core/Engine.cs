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
        /// (This is here in case i decide to add the MicrosoftCLR or modified mono runtime)
        /// </summary>
        public static readonly bool IsMono = (Type.GetType("Mono.Runtime") != null);

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public extern static GraphicsBackend GetGraphicsBackend();

        //[MethodImpl(MethodImplOptions.InternalCall)]
        //public extern static void NativeDebuggerBreak();

        [AllowReversePInvokeCalls]
        public static void LaunchDebugger()
        {
        }

        public static void NativeBreak()
        {
            StackTrace trace = new StackTrace();
            Console.WriteLine("Native Offset : {0}", trace.GetFrame(0).GetNativeOffset());
            //NativeDebuggerBreak();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private static extern IntPtr ArrayToPointer(Array Array, int Index);

        /// <summary>
        /// Gets the address of the data in a buffer.
        /// </summary>
        /// <param name="array">Array to get data address from.</param>
        /// <param name="Index">Index to start from.</param>
        [SuppressUnmanagedCodeSecurity]
        public unsafe static IntPtr GetArrayPointer(Array array, int Index = 0)
        {
            return ArrayToPointer(array, Index);
        }

        [SuppressUnmanagedCodeSecurity]
        internal unsafe static IntPtr ToPointer(this Array array, int Index = 0)
        {
            return GetArrayPointer(array, Index);
        }

        /// <summary>
        /// Transforms an primitve array to a byte array.
        /// </summary>
        /// <typeparam name="T">Primitive/Unmanaged type of the array</typeparam>
        /// <returns>A new byte[]</returns>
        public unsafe static byte[] ToByteArray<T>(T[] array) where T : unmanaged
        {
            byte[] buf = new byte[Buffer.ByteLength(array)];
            Buffer.BlockCopy(array, 0, buf, 0, buf.Length);
            return buf;
        }
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
