using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;

namespace Sphynx.Core.Graphics
{
    //Copied As-is from Texture.h
    public enum TextureWrappingMode : ushort
    {
        Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
    }
    public enum TextureFilterMode : ushort
    {
        Nearest, Linear
    }
    public enum TextureMipmapMode : ushort
    {
        NearestMipmapNearest, NearestMipmapLinear, LinearMipmapNearest, LinearMipmapLinear
    }
    /// <summary>
    /// Represents how the data is interpeted (EX: RGB, Depth) by the backend.
    /// </summary>
    public enum TextureFormat : ushort
    {
        Stencil,Depth_Component16, Depth_Component24, Depth_Component32, Depth_Component32F, Depth24_Stencil8, Depth32F_Stencil8,
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
        Texture1D,Texture1D_Array,Texture2D, Texture2D_Array, Texture3D, CubeMap, Rectangle
    };

    //TODO: Implement Buffers and Allow for mapping texture data (and buffer orphaning).
    [Header("Texture.h")]
    public sealed class Texture : IDisposable
    {

        private UIntPtr nativePointer;
        private UIntPtr Nativeid;
        /// <summary>
        /// Used for interoping with Engine.
        /// </summary>
        public UIntPtr ID { get => Nativeid; }
        private Vector3 dims;
        public Vector3 Dimensions { get => dims; set => Resize(value); }
        public Vector2 Size { get => new(dims.x, dims.y); set => Resize(new Vector3(value, dims.z)); }
        public int Depth { get => (int)dims.z; set { dims.z = value; Resize(dims); } }
        private TextureType type;
        public TextureType Type { get => type; }
        private TextureDataFormat dataformat;
        public TextureDataFormat DataFormat { get => dataformat; }
        private TextureFormat format;
        public TextureFormat Format { get => format; }
        private bool disposedValue;


        /// <summary>
        /// Returns a pointer to a native texture object.
        /// Note: the pointer holds a reference to an abstract class.
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal extern static UIntPtr CreateTexture(byte[] data, int width, int height, ushort texturetype, int mipmaplevel, ushort textureformat,
            ushort datatype, ushort wrap, ushort filter, ushort mipmapmode);

        /// <summary>
        /// Automatically creates Mipmaps for the texture.
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        public extern static void GenerateMipmaps();

        /// <summary>
        /// Returned by call to native function <see cref="GetTexInfo(UIntPtr)"/> and hold infomation about the texture.
        /// </summary>
        internal struct TexInfo
        {
            public UIntPtr NativeID;
            public TextureDataFormat dataFormat;
            public TextureFormat format;
            public Vector3 Dimension;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal extern static TexInfo GetTexInfo(UIntPtr Pointer);


        /// <summary>
        /// This constructor doesn't allocated memory for a texture.
        /// </summary>
        public Texture() 
        {

        }
        //Create from native ptr.
        internal Texture(UIntPtr nativeptr)
        {
            TexInfo info = GetTexInfo(nativeptr);
            dims = info.Dimension;
            format = info.format;
            dataformat = info.dataFormat;
            Nativeid = info.NativeID;
        }

        /// <summary>
        /// Allocate an empty <see cref="Texture"/> with the specified size.
        /// </summary>
        public Texture(Vector2 size)
        {
            //CreateTexture(0,)
        }

        public Texture(byte[] buffer, Vector3 dimensions, TextureDataFormat textureDataFormat, TextureFormat format)
        {
        }
        /// <summary>
        /// Read from GPU and returns a buffer with the texture data. 
        /// This is very costly because of 1) GPU and CPU syncing and 2) copying the buffer from native to managed.
        /// </summary>
        /// <param name="Compress">if <c>true</c> compresses the data before returning</param>
        /// <returns>Texture Pixel Data. It can be release in managed.</returns>
        public byte[] ReadAllBytes(bool Compress) 
        {
            return null;
        }
        /// <summary>
        /// Returns a new Compressed texture.
        /// </summary>
        /// <returns>A new <see cref="Texture"/>.</returns>
        public Texture Compress()
        {
            return null;
        }

        //public void CopyInto(Texture Tex, Bounds SrcBounds, int SrcZ, int Depth = 0, int SrcMipLevel = 0, Bounds DstBounds = Bounds.Zero, int DstMipLevel = 0, int SrcX = 0, int SrcY = 0, 
        //    int SrcZ = 0, int DstX = 0, int DstY = 0, int DstZ = 0)
        //{
        //}
        public void CopyInto(Texture Dst, Bounds SrcBounds, int SrcZ, Bounds DstBounds, int DstZ)
        {

        }

        public void Resize(Vector3 ndim)
        {

        }

        public void Bind() { }
        public void Unbind() { }

        private void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }

                // TODO: free unmanaged resources (unmanaged objects) and override finalizer
                // TODO: set large fields to null
                disposedValue=true;
            }
        }

        // TODO: override finalizer only if 'Dispose(bool disposing)' has code to free unmanaged resources
        ~Texture()
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
