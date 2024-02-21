using Sphynx.Core.Graphics;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core
{
    [NativeWrapper("ResourceManager")]
    [Header("ResourceManager.h")]
    public static class AssetManager
    {
        struct TexReturn
        {
            UIntPtr nativeID;
            TextureDataFormat dataformat;
            TextureFormat format;
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal extern static UIntPtr RM_LoadShader([MarshalAs(UnmanagedType.LPStr)] string shaderPath, byte type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal extern static UIntPtr RM_LoadTexture([MarshalAs(UnmanagedType.LPStr)] string texPath, ushort textype, char Compress);

        public static Shader LoadShader(string Path, ShaderType type)
        {
            return new Shader(RM_LoadShader(Path, (byte)type), type);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="Path"></param>
        /// <param name="textype"></param>
        /// <param name="Compress"></param>
        /// <returns></returns>
        /// <exception cref="FileNotFoundException"></exception>
        public static Texture LoadTexture(string Path, TextureType textype, bool Compress = false)
        {
            if (!File.Exists(Path))
            {
                Logger.Error($"{0} Does not exist.");
            }
            Texture texture = new(RM_LoadTexture(Path, (ushort)textype, (char)(Compress ? 0b0 : 0b1)));
            return texture;
        }

        

        //public static Texture LoadTexture(string Path, TextureType textype)
        //{
        //    System.Drawing.Image image = System.Drawing.Image.FromFile(Path);
        //    System.Drawing.Imaging.BitmapData bd = new System.Drawing.Imaging.BitmapData();
        //    byte[] Buffer;
        //    using (var ms = new MemoryStream())
        //    {
        //        image.Save(ms, image.RawFormat);
        //        Buffer = ms.ToArray();
        //    }
        //    return null;
        //}
    }


    //public static class SystemDrawingExtention
    //{
    //    public static TextureFormat ConvertFromPixelFormat(this System.Drawing.Imaging.PixelFormat format)
    //    {
    //        switch (format)
    //        {
    //            case System.Drawing.Imaging.PixelFormat.Indexed:
    //                break;
    //            case System.Drawing.Imaging.PixelFormat.Gdi:
    //                break;
    //            case System.Drawing.Imaging.PixelFormat.Alpha:
    //                return TextureFormat.Red;
    //            case System.Drawing.Imaging.PixelFormat.PAlpha:
    //                break;
    //            case System.Drawing.Imaging.PixelFormat.Extended:
    //                break;
    //            case System.Drawing.Imaging.PixelFormat.Canonical:
    //                return TextureFormat.RGBA8;
    //            case System.Drawing.Imaging.PixelFormat.Format1bppIndexed:
    //                //Palette.
    //                break;
    //            case System.Drawing.Imaging.PixelFormat.Format4bppIndexed:
    //                //Palette.
    //                break;
    //            case System.Drawing.Imaging.PixelFormat.Format8bppIndexed:
    //                //Palette.
    //                break;
    //            case System.Drawing.Imaging.PixelFormat.Format16bppGrayScale:
    //                return TextureFormat.Red;
    //            case System.Drawing.Imaging.PixelFormat.Format16bppRgb555:
    //                return TextureFormat.RGB5;
    //            case System.Drawing.Imaging.PixelFormat.Format16bppRgb565:
    //                return TextureFormat.RGB565;
    //            case System.Drawing.Imaging.PixelFormat.Format16bppArgb1555:
    //                return TextureFormat.RGB5_A1;
    //            case System.Drawing.Imaging.PixelFormat.Format24bppRgb:
    //                return TextureFormat.RGB8;
    //            case System.Drawing.Imaging.PixelFormat.Format32bppRgb:
    //                //What does it mean the remainging 8 bits are unused ?
    //                //No sure if this or the uncommented one.
    //                //return TextureFormat.RGB8;
    //                return TextureFormat.RGBA8;
    //            case System.Drawing.Imaging.PixelFormat.Format32bppArgb:
    //                return TextureFormat.RGBA8;
    //            case System.Drawing.Imaging.PixelFormat.Format32bppPArgb:
    //                return TextureFormat.RGBA8SNorm;
    //            case System.Drawing.Imaging.PixelFormat.Format48bppRgb:
    //                return TextureFormat.RGB16;
    //            case System.Drawing.Imaging.PixelFormat.Format64bppArgb:
    //                return TextureFormat.RGB16;
    //            case System.Drawing.Imaging.PixelFormat.Format64bppPArgb:
    //                return TextureFormat.RGBA16;
    //            case System.Drawing.Imaging.PixelFormat.Max:
    //                break;
    //            default:
    //                break;
    //        }
    //        return 0;
    //    }
    //    public static Texture ToTexture(this System.Drawing.Image image)
    //    {
    //        return null;
    //    }
    //}
}

