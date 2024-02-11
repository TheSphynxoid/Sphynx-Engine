using Sphynx.Core.Graphics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
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
        internal extern static UIntPtr RM_LoadShader([MarshalAs(UnmanagedType.LPStr)] string shaderPath, byte type);
        [DllImport("__Internal")]
        internal extern static UIntPtr RM_LoadTexture([MarshalAs(UnmanagedType.LPStr)] string texPath, ushort textype, char Compress);

        public static Shader LoadShader(string Path, ShaderType type)
        {
            return new Shader(RM_LoadShader(Path, (byte)type), type);
        }
        public static Texture LoadTexture(string Path, TextureType textype,bool Compress)
        {
            Texture texture = new Texture();
            texture.Nativeid = RM_LoadTexture(Path, (ushort)textype, (char)(Compress ? 0b0 : 0b1));
            return texture;
        }
    }
}
