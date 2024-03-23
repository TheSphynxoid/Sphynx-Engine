using Sphynx.Graphics;
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
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal extern static UIntPtr RM_LoadShader([MarshalAs(UnmanagedType.LPStr)] string shaderPath, byte type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal extern static UIntPtr RM_LoadTexture([MarshalAs(UnmanagedType.LPStr)] string texPath, ushort textype, char Compress);

        public static Shader LoadShader(string Path, ShaderType type)
        {
            Logger.Info("Called");
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
    }
}

