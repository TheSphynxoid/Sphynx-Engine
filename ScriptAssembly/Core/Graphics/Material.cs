using Sphynx.Core;
using Sphynx.Graphics;
using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Graphics
{
    /// <summary>
    /// ToDO: Support uniforms
    /// </summary>
    [Header("Material.h", NativeTypeName = "Material")]
    [NativeWrapper("Material", true)]
    [Serializable]
    public sealed class Material : IDisposable
    {
        //Caching.
        private static ShaderPack DefaultShaders;

        private ShaderPack shaders;
        public ShaderPack Shaders { [Pure]get => shaders; set { ReloadShaders(value); shaders = value; } }

        internal List<Texture> textures = null;
        public ref readonly List<Texture> Textures { get => ref textures; }

        internal UIntPtr NativePtr;
        private bool disposedValue;

        #region NativeCalls
        [NativeCppClass]
        internal struct NativeShaderPack
        {
            public UIntPtr VShader;
            public UIntPtr FShader;
            public UIntPtr TessEval;
            public UIntPtr TessControl;
            public UIntPtr GeomShader;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static UIntPtr CreateMaterial(NativeShaderPack shaderPack, UIntPtr[] textures);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static UIntPtr CreateMaterial(NativeShaderPack shaderPack);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static void MatBind(UIntPtr thismat);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        [Pure]
        private extern static void MatUnbind(UIntPtr thismat);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static UIntPtr MatGetDefaultShader(byte shadertype);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static void AddTex(UIntPtr matptr, UIntPtr texptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static void RemoveTex(UIntPtr matptr, UIntPtr texptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static void SetTex(UIntPtr matptr, UIntPtr texptr, uint index);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static int GetUniformLoc(UIntPtr matptr, [MarshalAs(UnmanagedType.LPStr)] string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private extern static void SetUni(UIntPtr matptr, UIntPtr uni, int loc);

        #endregion

        /// <summary>
        /// Creates a new copy of the given <see cref="Material"/>.
        /// </summary>
        /// <param name="mat">The source <see cref="Material"/>.</param>
        public Material(Material mat)
        {
            shaders = mat.shaders;
            if(mat.textures != null)
            {
                textures = mat.textures;
                UIntPtr[] texPtr = new UIntPtr[textures.Count];
                for (int i = 0; i <= texPtr.Length; i++)
                {
                    texPtr[i] = textures[i].GetNative();
                }
                NativePtr = CreateMaterial(shaders.ToNative(), texPtr);
                return;
            }
            Console.WriteLine("Copy Constructor");
            NativePtr = CreateMaterial(shaders.ToNative());

        }

        public Material(ShaderPack shaderpack)
        {
            shaders = shaderpack;
            NativePtr = CreateMaterial(shaders.ToNative());
        }

        public Material(ShaderPack shaderpack, List<Texture> textures)
        {
            shaders = shaderpack;
            this.textures = textures;
            UIntPtr[] texPtr = new UIntPtr[textures.Count];
            for (int i = 0; i <= texPtr.Length; i++) 
            {
                texPtr[i] = textures[i].GetNative();
            }
            NativePtr = CreateMaterial(shaders.ToNative(), texPtr);
        }

        public void ReloadShaders(ShaderPack shaders) { }
        /// <summary>
        /// Binds the buffer for use.
        /// </summary>
        public void Bind() { MatBind(NativePtr); }
        /// <summary>
        /// Unbinds the buffer
        /// </summary>
        [Pure]
        public void Unbind() { MatUnbind(NativePtr); }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [SuppressUnmanagedCodeSecurity]
        [Pure]
        public int GetUniformLocation(string name)
        {
            return GetUniformLoc(NativePtr, name);
        }

        public void SetUniform(Uniform uniform, string name) 
        {
            SetUniform(uniform, GetUniformLoc(NativePtr, name));
        }

        public void SetUniform(Uniform uniform, int location) 
        {
            SetUni(NativePtr,uniform.NativeUniform,location);
            uniform.Location = location;
        }

        //public void SetUniformBuffer(UniformBuffer uniformBuffer, string name) { }
        //public void SetUniformBuffer(UniformBuffer uniformBuffer, int location) { }
        [SuppressUnmanagedCodeSecurity]
        public void AddTexture(Texture tex) 
        { 
            textures.Add(tex);
            AddTex(NativePtr, tex.GetNative());
        }

        [SuppressUnmanagedCodeSecurity]
        public void RemoveTexture(Texture tex) 
        {
            textures.Remove(tex);
            RemoveTex(NativePtr, tex.GetNative());
        }
        /// <summary>
        /// Replace a texture in the given index. 
        /// Ignores if index is out of range.
        /// </summary>
        public void SetTexture(Texture tex, uint index)
        {
            if(index >= textures.Count) { return; }
            textures[(int)index] = tex;
            SetTex(NativePtr, tex.GetNative(), index);
        }

        /// <summary>
        /// Gets the default shader of any given type.
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        [SuppressUnmanagedCodeSecurity]
        public static Shader GetDefaultShader(ShaderType type)
        {
            Shader s;
            if((s = DefaultShaders.GetByType(type)) != null){
                return s;
            }
            s = new(MatGetDefaultShader((byte)type), type);
            DefaultShaders.ReplaceShader(ref s);
            return s;
        }

        [Pure]
        public UIntPtr GetNative() { return  NativePtr; }

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

        // // TODO: override finalizer only if 'Dispose(bool disposing)' has code to free unmanaged resources
        ~Material()
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
