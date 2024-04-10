using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Runtime.Hosting;
using System.Runtime.InteropServices;
using System.Security;

namespace Sphynx.Core.Native
{
    [StructLayout(LayoutKind.Sequential)]
    internal struct NativeComponent : IDisposable
    {
        internal HandleRef NativePtr;

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool NativeFinalize(HandleRef NativePointer);

        [AllowReversePInvokeCalls]
        public void Dispose()
        {
            NativeFinalize(NativePtr);
        }
    }

    internal static class NativeComponentFactory
    {
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern IntPtr GetNativeCompByName(IntPtr GOHandle, string name);
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern IntPtr CreateNativeComponent();
    }
}
