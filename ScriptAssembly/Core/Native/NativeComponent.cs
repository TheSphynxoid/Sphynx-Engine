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
<<<<<<< HEAD
        internal static extern bool NativeFinalize(UIntPtr NativePointer);
=======
        internal static extern bool NativeFinalize(NativeComponent ncomponent);
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d

        [AllowReversePInvokeCalls]
        public void Dispose()
        {
<<<<<<< HEAD
            NativeFinalize(NativePtr);
=======
            NativeFinalize(this);
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
        }

        public NativeComponent()
        {
        }

    }
}
