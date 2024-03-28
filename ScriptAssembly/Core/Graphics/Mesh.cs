using Sphynx.Core;
using Sphynx.Graphics;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Graphics
{
    //1:1 From Mesh.h
    /// <summary>
    /// Specifies the use of the mesh.
    /// </summary>
    enum MeshType : byte
    {
        //Changing Buffer
        Dynamic,
        //Constant Buffer
        Static,
        //GL Specific (Meaning : contents will be modified once and used at most a few times.)
        Stream
    };

    [NativeCppClass]
    public struct BufferElement
    {
        //ShaderDataType Type = ShaderDataType::None;
        ShaderDataType shaderDataType;
        [Pure]
        public ShaderDataType DataType { get { return shaderDataType; } }
        //size_t Size = 0;
        long size;
        [Pure]
        public long Size { get => size; }
        //size_t Offset = 0;
        internal long offset;
        [Pure]
        public long Offset { get => offset; }
        //bool Normalized = false;
        public bool Normalized { [Pure] readonly get; init; }

        public BufferElement(ShaderDataType dataType, bool norm)
        {
            Normalized = norm;
            shaderDataType = dataType;
            size = dataType.GetSize();
        }

        [Pure]
        public uint GetComponentCount()
        {
            switch (shaderDataType)

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

    [Header("Mesh.h")]
    [NativeCppClass]
    public struct BufferLayout
    {
        public BufferElement[] bufferElements;
        public BufferElement[] Elements { get => bufferElements; }
        uint stride;
        public uint Stride { get => stride; }
    }

    [NativeWrapper("Mesh", true)]
    [Header("Mesh.h")]
    public class Mesh
    {

        public Mesh()
        {
        }
    }
}
