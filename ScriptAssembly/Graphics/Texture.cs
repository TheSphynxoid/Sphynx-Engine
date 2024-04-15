using System;
using Sphynx.Core;
using System.Collections.Generic;
using System.Dynamic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using GlmSharp;

namespace Sphynx.Graphics
{
    //Copied As-is from Texture.h
    public enum TextureWrappingMode : ushort
    {
        Repeat, MirroredRepeat, ClampToEdge, ClampToBorder, Default
    }
    public enum TextureFilterMode : ushort
    {
        Nearest, Linear, Default
    }
    public enum TextureMipmapMode : ushort
    {
        NearestMipmapNearest, NearestMipmapLinear, LinearMipmapNearest, LinearMipmapLinear, Default
    }
    /// <summary>
    /// Represents how the data is interpeted (EX: RGB, Depth) by the backend.
    /// </summary>
    public enum TextureFormat : ushort
    {
        Stencil, Depth_Component16, Depth_Component24, Depth_Component32, Depth_Component32F, Depth24_Stencil8, Depth32F_Stencil8,
        Red, Red8, Red8SNorm, Red16, Red16SNorm, RG, RG8, RG8SNorm, RG16,
        RG16SNorm, R3_G3_B2, RGB565, RGB, RGB4, RGB5, RGB8, RGB8SNorm, RGB10, RGB12, RGB16, RGB16SNorm, RGBA, RGBA2, RGBA4, RGB5_A1, RGBA8,
        RGBA8SNorm, RGB10_A2, UIntRGB10_A2, RGBA12, RGBA16, SRGB8, SRGB8_A8, BGR, BGRA, FloatR16, FloatRG16,
        FloatRGB16, FloatRGBA16, FloatR32, FloatRG32, FloatRGB32, FloatRGBA32, FloatRG11_B10, RGB9_E5, IntRed8, UIntRed8, IntRed16,
        UIntRed16, IntRed32, UIntRed32, IntRG8, UIntRG8, IntRG16, UIntRG16, UIntRG32, IntRGB8, UIntRGB8, IntRGB16, UIntRGB16,
        IntRGB32, UIntRGB32, IntRGBA8, UIntRGBA8, IntRGBA16, UIntRGBA16, IntRGBA32, UIntRGBA32, CompressedRed, CompressedRG, CompressedRGB,
        CompressedRGBA, CompressedSRGB, CompressedSRGB_A, CompressedRed_RGTC1, SignedCompressedRed_RGTC1, CompressedRG_RGTC2,
        CompressedRGBA_BPTC_Unorm, CompressedSRGB_A_BPTC_UNorm, FloatCompressedRGB_BPTC, UFloatCompressedRGB_BPTC
    }
    /// <summary>
    /// Represents the pixel data format.
    /// </summary>
    public enum TextureDataFormat : ushort
    {
        UByte, Byte, UShort, Short, UInt, Int, HalfFloat, Float, UByte_3_3_2,
        UByte_2_3_3_REV, UShort_5_6_5, UShort_5_6_5_REV, UShort_4_4_4_4,
        UShort_4_4_4_4_REV, UShort_5_5_5_1, UShort_1_5_5_5_REV, UInt_8_8_8_8,
        UInt_8_8_8_8_REV, UInt_10_10_10_2, UInt_2_10_10_10_REV, UInt_24_8
    }
    public enum TextureType : ushort
    {
        Texture1D, Texture1D_Array, Texture2D, Texture2D_Array, Texture3D, CubeMap, Rectangle
    };

    public struct TexCopyTarget
    {
        public Texture Destination;
        public Vector3 DstTo;
        public Vector3 SrcFrom;
        public uint MipmapLevel;
    }

    //TODO: Implement Buffers and Allow for mapping texture data (and buffer orphaning).
    [Header("Texture.h")]
    public sealed class Texture : IDisposable
    {
        internal HandleRef NativePointer;
        /// <summary>
        /// Is this Texture attached to a <see cref="FrameBuffer"/>.
        /// </summary>
        internal bool IsAttached;
        HandleRef Nativeid;
        /// <summary>
        /// Used for interoping with Engine.
        /// </summary>
        public HandleRef ID { get => Nativeid; }

        Vector3 dims;
        public Vector3 Dimensions { get => dims; set => Resize(value); }
        public Vector2 Size { get => new(dims.x, dims.y); set => Resize(new Vector3(value, dims.z)); }
        public int Depth { get => (int)dims.z; set { var ndim = dims; ndim.z = value; Resize(ndim); } }
        readonly TextureType type;
        public TextureType Type { get => type; }
        TextureDataFormat dataformat;
        public TextureDataFormat DataFormat { get => dataformat; }
        TextureFormat format;
        public TextureFormat Format { get => format; }
        bool disposedValue;

        static TextureMipmapMode defMipmapMode = GetDefaultMipmapMode();
        public static TextureMipmapMode DefaultMipmapMode { get => defMipmapMode; set { defMipmapMode = value; SetDefaultMipmapMode(value); } }

        static TextureWrappingMode defWrappingMode = GetDefaultWrappingMode();
        public static TextureWrappingMode DefaultWrappingMode { get => defWrappingMode; set { defWrappingMode = value; SetDefaultWrappingMode(value); } }

        static TextureFilterMode defFilterMode = GetDefaultFilterMode();
        public static TextureFilterMode DefaultFilterMode { get => defFilterMode; set { defFilterMode = value; SetDefaultFilterMode(value); } }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetDefaultFilterMode(TextureFilterMode mode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetDefaultWrappingMode(TextureWrappingMode mode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetDefaultMipmapMode(TextureMipmapMode mode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern TextureFilterMode GetDefaultFilterMode();

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern TextureWrappingMode GetDefaultWrappingMode();

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern TextureMipmapMode GetDefaultMipmapMode();

        /// <summary>
        /// Returns a pointer to a native texture object.
        /// Note: the pointer holds a reference to an abstract class.
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal static extern IntPtr CreateTexture(IntPtr data, int width, int height, int depth, ushort textureType, int mipmapLevel, ushort textureFormat,
            ushort datatype, ushort wrap, ushort filter, ushort mipmapMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void GenMipmaps(IntPtr tex);


        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void Bind(IntPtr tex);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void Unbind(IntPtr tex);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void Release(IntPtr tex);
        /// <summary>
        /// Automatically creates Mipmaps for the texture.
        /// </summary>
        [SuppressUnmanagedCodeSecurity]
        public void GenerateMipmaps()
        {
            GenMipmaps(NativePointer.Handle);
        }

        /// <summary>
        /// Returned by call to native function <see cref="GetTexInfo(IntPtr)"/> and hold information about the texture.
        /// </summary>
        [NativeCppClass]
        struct TexInfo
        {
            public IntPtr NativeID;
            public TextureDataFormat DataFormat;
            public TextureFormat Format;
            public Vector3 Dimension;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern TexInfo GetTexInfo(IntPtr pointer);

        [SuppressUnmanagedCodeSecurity]
        internal void SetUp()
        {
            TexInfo info = GetTexInfo(NativePointer.Handle);
            dims = info.Dimension;
            format = info.Format;
            dataformat = info.DataFormat;
            Nativeid = new HandleRef(this, info.NativeID);
        }

        /// <summary>
        /// This constructor doesn't allocate memory for a texture.
        /// </summary>
        public Texture()
        {
        }

        //Create from native ptr.
        internal Texture(IntPtr nativePtr)
        {
            NativePointer = new HandleRef(this, nativePtr);
            SetUp();
        }
        /// <summary>
        /// Creates and allocates a texture with the given data.
        /// </summary>
        /// <param name="buffer">Can be null to not specify the data.</param>
        /// <param name="dimensions">The dimensions of the texture in pixels.</param>
        /// <param name="type">Texture type.</param>
        /// <param name="textureDataFormat">The pixel data format in the given buffer (i.e: RGB ,RGBA ,etc...).</param>
        /// <param name="format">How to interpret the texture in memory by the backend.</param>
        /// <param name="mipmapLevel"></param>
        /// <param name="wrap">Wrapping method used by the backend.</param>
        /// <param name="filter">Filtering mode to be used.</param>
        /// <param name="mipmapMode">MipMapping mode.</param>
        public Texture(byte[] buffer, ivec3 dimensions, TextureType type, TextureDataFormat textureDataFormat, TextureFormat format, int mipmapLevel = 0,
            TextureWrappingMode wrap = TextureWrappingMode.Default, TextureFilterMode filter = TextureFilterMode.Default,
            TextureMipmapMode mipmapMode = TextureMipmapMode.Default)
        {
            mipmapMode = (mipmapMode == TextureMipmapMode.Default) ? defMipmapMode : mipmapMode;
            wrap = (wrap == TextureWrappingMode.Default) ? defWrappingMode : wrap;
            filter = (filter == TextureFilterMode.Default) ? defFilterMode : filter;

            this.type = type;
            this.format = format;
            dataformat = textureDataFormat;

            NativePointer = new HandleRef(this, CreateTexture(Engine.GetArrayPointer(buffer), dimensions.x, dimensions.y, dimensions.z,
                (ushort)type, mipmapLevel, (ushort)format, (ushort)dataformat, (ushort)wrap, (ushort)filter, (ushort)mipmapMode));
            SetUp();
        }

        /// <summary>
        /// Read from GPU and returns a buffer with the texture data. 
        /// This is very costly because of 1) GPU and CPU syncing and 2) copying the buffer from native to managed.
        /// </summary>
        /// <param name="compress">if <c>true</c> compresses the data before returning</param>
        /// <returns>Texture Pixel Data. It can be release in managed.</returns>
        public byte[] ReadAllBytes(bool compress)
        {
            return null;
        }

        //public void CopyInto(Texture Tex, Bounds SrcBounds, int SrcZ, int Depth = 0, int SrcMipLevel = 0, Bounds DstBounds = Bounds.Zero, int DstMipLevel = 0, int SrcX = 0, int SrcY = 0, 
        //    int SrcZ = 0, int DstX = 0, int DstY = 0, int DstZ = 0)
        //{
        //}
        public void CopyInto(Texture dst, Bounds srcBounds, int srcZ, Bounds dstBounds, int dstZ)
        {

        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void ResizeTexture(IntPtr ptr, Vector3 dim);

        [SuppressUnmanagedCodeSecurity]
        public void Resize(Vector3 nDim)
        {
            ResizeTexture(NativePointer.Handle, nDim);
            dims = nDim;
        }

        public void Bind() { Bind(NativePointer.Handle); }
        public void Unbind() { Unbind(NativePointer.Handle); }
        public HandleRef GetNative() { return NativePointer; }

        void Dispose(bool disposing)
        {
            if (!disposedValue && !IsAttached)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }
                
                Release(NativePointer.Handle);
                NativePointer = new HandleRef(null, IntPtr.Zero);
                disposedValue=true;
            }
        }

        // TODO: override finalizer only if 'Dispose(bool disposing)' has code to free unmanaged resources
        ~Texture()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: false);
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }
}
