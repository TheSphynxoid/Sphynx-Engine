using Sphynx.Core;
using Sphynx.Core.Native;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Security.Permissions;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Graphics
{
    public sealed class GPUBuffer : IDisposable
    {
        public enum MapAccess : byte
        {
            Read, Write, ReadWrite
        }

        public enum UsageHint : byte
        {
            Stream, Static, Dynamic
        }

        public enum AccessHint : byte
        {
            Draw, Read, Copy
        }

        HandleRef NativeObject;

        ulong size;
        public ulong Size { get => size; }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr CreateBuffer(ulong size, IntPtr data, byte usage, byte access);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void ReallocateBuffer(IntPtr native, ulong size, byte[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void InvalidateBuffer(IntPtr native);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetBufferData(IntPtr native, IntPtr data, ulong size, ulong offset);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void BindBuffer(IntPtr native);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void UnbindBuffer(IntPtr native);

        /// <summary>
        /// For types that derive from GPUBuffer in native.
        /// </summary>
        /// <param name="native"></param>
        /// <param name="bSize"></param>
        internal GPUBuffer(IntPtr native, nuint bSize)
        {
            NativeObject = new HandleRef(this,native);
            size = bSize;
        }

        public GPUBuffer(nuint sizeInBytes, byte[] data, UsageHint usageHint, AccessHint accessHint)
        {
            size = sizeInBytes;
            if(sizeInBytes > (ulong)data.LongLength)
            {
                //Discard data, allocate empty buffer and inform caller.
                Logger.Warn("Buffer smaller than specified size, data ignored and buffer still allocated.");
                data = null;
            }
            NativeObject = new HandleRef(this, CreateBuffer(sizeInBytes, Engine.GetArrayPointer(data, 0), (byte)usageHint, (byte)accessHint));
        }

        /// <summary>
        /// Mainly to be used for buffer orphaning.
        /// </summary>
        /// <param name="size"></param>
        /// <param name="data"></param>
        public void Reallocate(ulong size, byte[] data)
        {
            this.size = size;
            ReallocateBuffer(NativeObject.Handle, size, data);
        }

        public void Invalidate()
        {
            InvalidateBuffer(NativeObject.Handle);
        }

        public void SetData(byte[] data, ulong offset)
        {
            SetBufferData(NativeObject.Handle, data.ToPointer(), (ulong)data.LongLength, offset);
        }

        //public HandleRef Map(MapAccess access) { return HandleRef.Zero; }
        //public void Unmap() { }
        public void Bind() { BindBuffer(NativeObject.Handle); }
        public void Unbind() { UnbindBuffer(NativeObject.Handle); }

        public void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}
