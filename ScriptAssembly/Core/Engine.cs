using Mono.CSharp;
using Sphynx.Graphics;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.Contracts;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core
{
    public enum GraphicsBackend
    {
        OpenGL, Vulkan, DirectX11, DirectX12
    }
    /// <summary>
    /// Engine Utility Class.
    /// </summary>
    public static class Engine
    {
        /// <summary>
        /// Checks if the Engine is running.
        /// </summary>
        [Pure]
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool IsRunning();
        /// <summary>
        /// Gets the args that the Application received to start.
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static string[] GetEngineArgs();
        /// <summary>
        /// If the C# Scripting Engine is Running on Mono this has a true value. (This is here in case i decide to add the MicrosoftCLR)
        /// </summary>
        public static readonly bool IsMono = (Type.GetType("Mono.Runtime") != null);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static GraphicsBackend GetGraphicsBackend();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void NativeDebuggerBreak();

        [AllowReversePInvokeCalls]
        public static void LaunchDebugger()
        {

        }

        public static void NativeBreak()
        {
            StackTrace trace = new StackTrace();
            Console.WriteLine("Native Offset : {0}",trace.GetFrame(0).GetNativeOffset());
            NativeDebuggerBreak();
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
