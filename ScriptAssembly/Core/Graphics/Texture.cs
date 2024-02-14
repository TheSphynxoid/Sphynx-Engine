using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

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
		RG16SNorm, R3_G3_B2, RGB, RGB4, RGB5, RGB8, RGB8SNorm, RGB10, RGB12, RGB16, RGB16SNorm, RGBA, RGBA2, RGBA4, RGB5_A1, RGBA8,
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
    [Header("Texture.h")]
    public sealed class Texture : IDisposable
    {
        internal UIntPtr Nativeid;
        public UIntPtr ID { get => Nativeid; }
        private Vector2 size;
        public Vector2 Size { get => size; set { Resize(value); size = value; } }
        private TextureType type;
        public TextureType Type { get => type; }
        private TextureDataFormat dataformat;
        public TextureDataFormat DataFormat { get => dataformat; }
        private TextureFormat format;
        private bool disposedValue;

        public TextureFormat Format { get => format; }

        [DllImport("__Internal")]
        internal extern static uint CreateTexture();

        [DllImport("__Internal")]
        public extern static void GenerateMipmaps();

        //Does nothing.
        internal Texture() { }

        internal Texture(UIntPtr native)
        {

        }

        /// <summary>
        /// Pre-allocate a Texture with the specified size.
        /// </summary>
        public Texture(Vector2 size)
        {

        }

        public Texture(Byte[] buffer, ulong datasize, TextureDataFormat textureDataFormat, TextureFormat format)
        {
        }
        /// <summary>
        /// Read from GPU and returns a buffer with the texture data. (Repeated call will slow down the game because of GPU and CPU Syncing)
        /// </summary>
        /// <param name="Compress">if True compresses the data before returning</param>
        /// <returns>Texture Pixel Data</returns>
        public Byte[] ReadAllBytes(bool Compress) 
        {
            return null;
        }
        /// <summary>
        /// Returns a new Compressed texture.
        /// </summary>
        /// <returns></returns>
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

        public void Resize(Vector2 nsize)
        {

        }

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
