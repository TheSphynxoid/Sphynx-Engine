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
        HandleRef nativePtr;

        List<Texture> attachments = new List<Texture>();
        public List<Texture> ColorAttachments { get; }

        Texture depthAttachment;
        public bool HasDepthAttachment { get; }

        public bool IsValid { get; }

        static FrameBuffer defaultFB = new(GetDefaultFrameBuffer());
        /// <summary>
        /// Default framebuffers cannot change their buffer attachments, 
        /// but a particular default framebuffer may not have images associated with certain buffers.
        /// Usually, <see cref="DefaultFrameBuffer"/> represents the window surface.
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
        static extern void AttachTexture(IntPtr fb, IntPtr tex);
        
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
            nativePtr = new HandleRef(this, Create(size.x, size.y, Engine.GetArrayPointer(tex), tex.Length));
            foreach (var t in tex)
            {
                if (t.IsDepthTexture() && depthAttachment != null)
                {
                    HasDepthAttachment = true;
                    depthAttachment = t;
                    t.IsAttached = true;
                    continue;
                }
                attachments.Add(t);
                t.IsAttached = true;
            }
        }
        /// <summary>
        /// Attached <see cref="Texture"/> live as long as the <see cref="FrameBuffer"/> does,
        /// so references to them must be handled carefully.
        /// </summary>
        /// <param name="tex">Texture to attach to the FrameBuffer</param>
        public void AttachTexture(Texture tex)
        {
            AttachTexture(nativePtr.Handle, tex.NativePointer.Handle);
            tex.IsAttached = true;
            attachments.Add(tex);
        }

        /// <summary>
        /// Binds the <see cref="FrameBuffer"/> for use.
        /// </summary>
        public void Bind(FrameBufferBinding binding = FrameBufferBinding.ReadWrite)
        {
            Bind(nativePtr.Handle, (byte)binding);
        }
        /// <summary>
		///Unbinds the framebuffer (the <see cref="DefaultFrameBuffer"/> will be bound to be used for reading or writing).
        /// </summary>
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
        /// Binds <see cref="DefaultFrameBuffer"/> directly.
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
        /// <para>Returns a handle to the native framebuffer used by the Graphics backend.</para>
        /// Details:
        /// <para>This returns a pointer to the Core's Framebuffer interface, not the native handle used by the graphics backend.</para>
        /// TODO: Expose the handle/ID used by the graphics backend.
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
                    // dispose managed state (managed objects)
                }
                //this internal call frees all attachments.
                Release(nativePtr.Handle);
                depthAttachment.NativePointer = new HandleRef(null, IntPtr.Zero);
                GC.SuppressFinalize(depthAttachment);
                depthAttachment = null;
                foreach (var tex in ColorAttachments)
                {
                    tex.NativePointer = new HandleRef(null, IntPtr.Zero);
                    GC.SuppressFinalize(tex);
                }
                ColorAttachments.Clear();
                nativePtr = new HandleRef(null, IntPtr.Zero);
                disposedValue=true;
            }
        }

        ~FrameBuffer()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: false);
        }

        /// <summary>
        /// The native <see cref="FrameBuffer"/> frees all attached textures.
        /// </summary>
        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
