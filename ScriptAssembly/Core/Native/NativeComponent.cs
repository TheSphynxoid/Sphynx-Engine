using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Runtime.Hosting;
using System.Runtime.InteropServices;

namespace Sphynx.Core.Native
{
    internal sealed class NativeComponent : IDisposable
    {
        public readonly ulong NativeID;
        private UIntPtr NativePtr;

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool NativeFinalize(UIntPtr NativePointer);

        [AllowReversePInvokeCalls]
        public void Dispose()
        {
            NativeFinalize(NativePtr);
        }

        public NativeComponent()
        {
        }

    }
}
