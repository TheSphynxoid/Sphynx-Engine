using Mono.CSharp;
using Sphynx.Core;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics.Contracts;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;

namespace Sphynx.Graphics
{
    //1:1 From Mesh.h
    /// <summary>
    /// Specifies the use of the mesh.
    /// </summary>
    public enum MeshType : byte
    {
        /// <summary>
        /// Changing Buffer.
        /// </summary>
        Dynamic,
        /// <summary>
        /// Constant Buffer.
        /// </summary>
        Static,
        /// <summary>
        /// GL Specific (Meaning : contents will be modified once and used at most a few times.)
        /// </summary>
        Stream
    };

    /// <summary>
    /// Indicates the how the renderer will draw primitives for a mesh.
    /// </summary>
    public enum RenderMode : byte
    {
        Points, Lines, LineLoop, LineStrip, Trig, TrigStrip, TrigFan, 
		LinesAdj = 0xA, LineStripAdj, TrigAdj, TrigStripAdj, Patches /* 1:1 From opengl */
    };

    /// <summary>
    /// Represents the element of data in a <see cref="VertexBuffer"/>.
    /// </summary>
    [NativeCppClass]
    public struct BufferElement
    {
        byte shaderDataType;
        [Pure]
        public ShaderDataType DataType { get { return (ShaderDataType)shaderDataType; } }
        ulong size;
        [Pure]
        public ulong Size { get => size; }
        internal ulong offset;
        [Pure]
        public ulong Offset { get => offset; }
        //bool Normalized = false;
        public bool Normalized { [Pure] readonly get; init; }

        public BufferElement(ShaderDataType dataType, bool norm)
        {
            Normalized = norm;
            shaderDataType = (byte)dataType;
            size = dataType.GetSize();
        }

        //Copied 1:1 from "Mesh.h".
        [Pure]
        public byte GetComponentCount()
        {
            switch ((ShaderDataType)shaderDataType)

            {
                case ShaderDataType.None:       return 0;
                case ShaderDataType.Float:      return 1;
                case ShaderDataType.Float2:     return 2;
                case ShaderDataType.Float3:     return 3;
                case ShaderDataType.Float4:     return 4;
                case ShaderDataType.Double:     return 1;
                case ShaderDataType.Double2:    return 2;
                case ShaderDataType.Double3:    return 3;
                case ShaderDataType.Double4:    return 4;
                case ShaderDataType.Int:        return 1;
                case ShaderDataType.Int2:       return 2;
                case ShaderDataType.Int3:       return 3;
                case ShaderDataType.Int4:       return 4;
                case ShaderDataType.UInt:       return 1;
                case ShaderDataType.UInt2:      return 2;
                case ShaderDataType.UInt3:      return 3;
                case ShaderDataType.UInt4:      return 4;
                case ShaderDataType.Bool:       return 1;
                case ShaderDataType.Mat2x2:     return 2;
                case ShaderDataType.Mat2x3:     return 3;
                case ShaderDataType.Mat2x4:     return 4;
                case ShaderDataType.Mat3x2:     return 2;
                case ShaderDataType.Mat3x3:     return 3;
                case ShaderDataType.Mat3x4:     return 4;
                case ShaderDataType.Mat4x2:     return 2;
                case ShaderDataType.Mat4x3:     return 3;
                case ShaderDataType.Mat4x4:     return 4;
                default:                        return 0;
            }
        }
    }

    /*Because of arrays this type is not a native cpp type.*/
    /// <summary>
    /// Data layout of the Vertex Buffer, a collection of <see cref="BufferElement"/>.
    /// </summary>
    [Header("Mesh.h")]
    public struct BufferLayout
    {
        public BufferElement[] bufferElements;
        public BufferElement[] Elements { get => bufferElements; }
        public uint stride = 0;
        public uint Stride { get => stride; }

        public BufferLayout() { }

        public BufferLayout(BufferElement[] elems)
        {
            ulong offset = 0;
            bufferElements = new BufferElement[elems.Length];
            for (int i = 0; i < elems.Length; i++)
            {
                elems[i].offset = offset;
                bufferElements[i] = elems[i];
                offset += elems[i].Size;
                stride += (uint)elems[i].Size;
            }
        }

    }

    /// <summary>
    /// A Vertex Buffer Object (VBO) is a buffer in GPU memory (See also: <see cref="GPUBuffer"/>)
    /// that contains information about vertexes to be sent to the Vertex Shader.
    /// This Buffer is neccessairy for rendering, if not provided <see cref="Mesh"/> will create one with the data provieded.
    /// </summary>
    public class VertexBuffer
    {
        HandleRef Native;
        GPUBuffer underlaying;

        [NativeCppClass]
        struct NativeBufferLayout
        {
            public IntPtr bufferElements;
            public int count;
            public uint stride;

            public NativeBufferLayout(BufferLayout layout)
            {
                bufferElements = Engine.GetArrayPointer(layout.bufferElements);
                count = layout.bufferElements.Length;
                stride = layout.stride;
            }
        }

        BufferLayout layout;
        public BufferLayout Layout { get => layout; set { layout = value; SetLayout(Native.Handle, new NativeBufferLayout(value)); } }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr CreateVB(IntPtr floatbuffer, nuint count);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr GetUnderlayingBuffer(IntPtr NativeVB);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetLayout(IntPtr NativeVB, NativeBufferLayout l);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetData(IntPtr native, IntPtr data, ulong size, ulong offset);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern nuint GetVBSize(IntPtr NativeVB);

        public VertexBuffer(float[] verts, int vertcount, BufferLayout? bufferLayout = null)
        {
            Native = new(this, CreateVB(Engine.GetArrayPointer(verts), (nuint)vertcount));
            underlaying = new(GetUnderlayingBuffer(Native.Handle), (nuint)(sizeof(float) * vertcount));
            if(bufferLayout is BufferLayout b)Layout = b;
        }

        public void SetData(float[] data, nuint size) 
        {
            SetData(Native.Handle, Engine.GetArrayPointer(data), size, 0);
        }

        internal IntPtr GetNative()
        {
            return Native.Handle;
        }

        public GPUBuffer GetBuffer()
        {
            return underlaying;
        }

        public nuint Size { get => GetVBSize(Native.Handle); }
    }

    /// <summary>
    /// An Index Buffer Object contains the order of the vertexes in <see cref="VertexBuffer"/> to be drawn.
    /// This Buffer is optional for rendering.
    /// </summary>
    public class IndexBuffer
    {
        HandleRef Native;
        GPUBuffer underlaying;

        uint count;
        public uint Count { get => count; }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr CreateIB(IntPtr data,  nuint size);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr GetUnderlayingBuffer(IntPtr native);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetData_int(IntPtr native, IntPtr buf, nuint size);

        public IndexBuffer(uint[] indexes)
        {
            Native = new(this, CreateIB(indexes.ToPointer(), (nuint)indexes.LongLength));
            underlaying = new(GetUnderlayingBuffer(Native.Handle), (nuint)(Count * sizeof(uint)));
            count = (uint)indexes.Length;
        }

        public void SetData(uint[] indexes)
        {
            count = (uint)indexes.Length;
            SetData_int(Native.Handle, indexes.ToPointer(), (nuint)(count * sizeof(uint)));
        }

        internal IntPtr GetNative()
        {
            return Native.Handle;
        }

        public GPUBuffer GetBuffer()
        {
            return underlaying;
        }
    }

    [NativeWrapper("Mesh", true)]
    [Header("Mesh.h")]
    public class Mesh
    {
        internal HandleRef NativePtr;
        internal List<VertexBuffer> VBOs;
        internal IndexBuffer IBO;
        internal RenderMode mode = RenderMode.Trig;
        public RenderMode RenderMode { get => mode; set { mode = value;SetRenderMode(NativePtr.Handle, (byte)value); } }
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr CreateMeshEmpty();
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr Create(IntPtr VBuffer, IntPtr IBuffer);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr CreateList(IntPtr VBList, int Count, IntPtr IBuffer);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void AddVB(IntPtr native, IntPtr VB);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetVBs(IntPtr native, int count, IntPtr VBs);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetIB(IntPtr native, IntPtr ib);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void Bind(IntPtr native);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void Unbind(IntPtr native);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetRenderMode(IntPtr native, byte mode);

        public Mesh()
        {
            NativePtr = new(this, CreateMeshEmpty());
        }

        public Mesh(VertexBuffer vbo, IndexBuffer ibo)
        {
            VBOs = new List<VertexBuffer>
            {
                vbo
            };
            IBO = ibo;
            NativePtr = new(this, Create(vbo.GetNative(), ibo.GetNative()));
        }

        public Mesh(List<VertexBuffer> vbs, IndexBuffer ib)
        {
            VBOs = vbs;
            IBO = ib;
            int count = vbs.Count;
            IntPtr[] ptrs = new IntPtr[count];
            for (int i = 0; i < count; i++)
            {
                ptrs[i] = vbs[i].GetNative();
            }
            NativePtr = new(this, CreateList(ptrs.ToPointer(), count, ib.GetNative()));
        }

        public void AddVertexBuffer(VertexBuffer vbo)
        {
            VBOs.Add(vbo);
            AddVB(NativePtr.Handle, vbo.GetNative());
        }

        public void SetVertexBuffers(List<VertexBuffer> vbs)
        {
            VBOs = vbs;
            IntPtr[] ptrs = new IntPtr[vbs.Count];
            for (int i = 0; i < vbs.Count; i++)
            {
                ptrs[i] = vbs[i].GetNative();
            }
            SetVBs(NativePtr.Handle, vbs.Count, ptrs.ToPointer());
        }

        public void SetIndexBuffer(IndexBuffer ibo)
        {
            SetIB(NativePtr.Handle, ibo.GetNative());
        }

        public void Bind()
        {
            Bind(NativePtr.Handle);
        }

        public void Unbind()
        {
            Unbind(NativePtr.Handle);
        }

        public HandleRef GetNative()
        {
            return NativePtr;
        }
    }
}
