using GlmSharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using Sphynx.Core;

namespace Sphynx.Graphics
{
    /// <summary>
    /// The binding point of the framebuffer.
    /// </summary>
    public enum FrameBufferBinding : byte
    {
        Read, Write, ReadWrite
    };
    /// <summary>
    /// Specifies the buffer to clear.
    /// </summary>
    public enum ClearBuffer : byte
    {
        Color, Depth, Stencil
    };
    /// <summary>
    /// A <see cref="FrameBuffer"/> is a collection of buffers that can be used as the destination for rendering.
    /// Note that the term "buffer" here refers to a specific location in the framebuffer. 
    /// An image may or may not be associated with a particular buffer in a framebuffer. 
    /// Buffers in FBOs are also called "attachment points". they're the locations where images can be attached. 
    /// </summary>
    public sealed class FrameBuffer : IDisposable
    {
        readonly HandleRef nativePtr;

        List<Texture> attachments = new List<Texture>();
        public List<Texture> ColorAttachments { get; }

        Texture depthAttachment;
        public bool HasDepthAttachment { get; }

        public bool IsValid { get; }

        static FrameBuffer defaultFB = new(GetDefaultFrameBuffer());
        /// <summary>
        /// Default framebuffers cannot change their buffer attachments, 
        /// but a particular default framebuffer may not have images associated with certain buffers
        /// </summary>
        public static FrameBuffer DefaultFrameBuffer => defaultFB;

        bool disposedValue;

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern IntPtr Create(int width, int height, IntPtr texArray, int count);
        
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void Release(IntPtr fb);
        
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern IntPtr GetDefaultFrameBuffer();
        
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void Bind(IntPtr fb, byte binding);

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void Unbind(IntPtr fb);

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void ClearBuffer(IntPtr fb, byte clear);

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void Clear(IntPtr fb);

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void Invalidate(IntPtr fb);

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void SetClearColor(IntPtr fb, vec4 color);

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void Resize(IntPtr fb, vec2 size);

        FrameBuffer(IntPtr nativePtr)
        {

        }

        public FrameBuffer(ivec2 size, Texture[] tex)
        {
            nativePtr = new(this, Create(size.x, size.y, Engine.GetArrayPointer(tex), tex.Length));
            foreach (var t in tex)
            {
                if (t.Format == TextureFormat.Depth24_Stencil8 && depthAttachment != null)
                {
                    HasDepthAttachment = true;
                    depthAttachment = t;
                    continue;
                }
                attachments.Add(t);
            }
        }

        public void AttachTexture(Texture tex)
        {

        }

        /// <summary>
        /// Binds the <see cref="FrameBuffer"/> for use.
        /// </summary>
        /// <param name="binding"></param>
        public void Bind(FrameBufferBinding binding = FrameBufferBinding.ReadWrite)
        {
            Bind(nativePtr.Handle, (byte)binding);
        }

        public void Unbind()
        {
            Unbind(nativePtr.Handle);
        }

        public void Resize(ivec2 size)
        {
            Resize(nativePtr.Handle, size);
        }

        /// <summary>
        /// Sets the color used in the Clear/ClearBuffer to clear ColorAttachments.
        /// </summary>
        public void SetClearColor(vec4 color)
        {
            SetClearColor(nativePtr.Handle, color);
        }
        /// <summary>
        /// Clears the specified buffer.
        /// </summary>
        public void ClearBuffer(ClearBuffer bufferSpec)
        {
            ClearBuffer(nativePtr.Handle, (byte)bufferSpec);
        }
        /// <summary>
        /// Clears all buffers.
        /// </summary>
        public void Clear()
        {
            Clear(nativePtr.Handle);
        }

        /// <summary>
        /// Binds the default framebuffer.
        /// </summary>
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void BindDefault();
        /// <summary>
        /// Causes the framebuffer to be recreated.
        /// </summary>
        public void Invalidate()
        {
            Invalidate(nativePtr.Handle);
        }
        /// <summary>
        /// Returns a handle to the native framebuffer used by the Graphics backend.
        /// </summary>
        public IntPtr GetNative()
        {
            return nativePtr.Handle;
        }

        void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }
                //this internal call frees all attachments.
                Release(nativePtr.Handle);
                disposedValue=true;
            }
        }

        ~FrameBuffer()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: false);
        }

        void IDisposable.Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
