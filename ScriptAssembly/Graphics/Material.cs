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
        static ShaderPack DefaultShaders;

        ShaderPack shaders;
        public ShaderPack Shaders { [Pure]get => shaders; set { ReloadShaders(value); shaders = value; } }

        internal List<Texture> textures = new List<Texture>(0);
        public ref readonly List<Texture> Textures { get => ref textures; }

        internal HandleRef NativePtr;
        bool disposedValue;

        #region NativeCalls
        [NativeCppClass]
        internal struct NativeShaderPack
        {
            public IntPtr VShader;
            public IntPtr FShader;
            public IntPtr TessEval;
            public IntPtr TessControl;
            public IntPtr GeomShader;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr CreateMaterial(NativeShaderPack shaderPack, IntPtr[] textures);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr CreateMaterial(NativeShaderPack shaderPack);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void MatBind(IntPtr thismat);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        [Pure]
        static extern void MatUnbind(IntPtr thismat);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern IntPtr MatGetDefaultShader(byte shadertype);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void AddTex(IntPtr matptr, IntPtr texptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void RemoveTex(IntPtr matptr, IntPtr texptr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetTex(IntPtr matptr, IntPtr texptr, uint index);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern int GetUniformLoc(IntPtr matptr, [MarshalAs(UnmanagedType.LPStr)] string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        static extern void SetUni(IntPtr matptr, IntPtr uni, int loc);

        #endregion

        /// <summary>
        /// Creates a new copy of the given <see cref="Material"/>.
        /// </summary>
        /// <param name="mat">The source <see cref="Material"/>.</param>
        public Material(Material mat)
        {
            shaders = mat.shaders;
            if(mat.textures.Count != 0)
            {
                textures = mat.textures;
                IntPtr[] texPtr = new IntPtr[textures.Count];
                for (int i = 0; i <= texPtr.Length; i++)
                {
                    texPtr[i] = textures[i].GetNative().Handle;
                }
                NativePtr = new HandleRef(this, CreateMaterial(shaders.ToNative(), texPtr));
                return;
            }
            NativePtr = new HandleRef(this, CreateMaterial(shaders.ToNative()));
        }

        public Material(ShaderPack shaderpack)
        {
            shaders = shaderpack;
            if (shaders.Vert == null)
            {
                shaders.Vert = GetDefaultShader(ShaderType.VertexShader);
            }
            if (shaders.Frag == null)
            {
                shaders.Frag = GetDefaultShader(ShaderType.FragmentShader);
            }
            NativePtr = new HandleRef(this, CreateMaterial(shaders.ToNative()));
        }

        public Material(ShaderPack shaderpack, List<Texture> textures)
        {
            shaders = shaderpack;
            if(shaders.Vert == null)
            {
                shaders.Vert = GetDefaultShader(ShaderType.VertexShader);
            }
            if(shaders.Frag == null)
            {
                shaders.Frag = GetDefaultShader(ShaderType.FragmentShader);
            }
            this.textures = textures;
            IntPtr[] texPtr = new IntPtr[textures.Count];
            for (int i = 0; i <= texPtr.Length; i++) 
            {
                texPtr[i] = textures[i].GetNative().Handle;
            }
            NativePtr = new HandleRef(this, CreateMaterial(shaders.ToNative(), texPtr));
        }

        public void ReloadShaders(ShaderPack shaders) { }
        /// <summary>
        /// Binds the buffer for use.
        /// </summary>
        public void Bind() { MatBind(NativePtr.Handle); }
        /// <summary>
        /// Unbinds the buffer
        /// </summary>
        [Pure]
        public void Unbind() { MatUnbind(NativePtr.Handle); }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [SuppressUnmanagedCodeSecurity]
        [Pure]
        public int GetUniformLocation(string name)
        {
            return GetUniformLoc(NativePtr.Handle, name);
        }

        public void SetUniform(Uniform uniform, string name) 
        {
            SetUniform(uniform, GetUniformLoc(NativePtr.Handle, name));
        }

        public void SetUniform(Uniform uniform, int location) 
        {
            //SetUni(NativePtr,uniform.NativeUniform,location);
            uniform.Location = location;
        }

        //public void SetUniformBuffer(UniformBuffer uniformBuffer, string name) { }
        //public void SetUniformBuffer(UniformBuffer uniformBuffer, int location) { }
        [SuppressUnmanagedCodeSecurity]
        public void AddTexture(Texture tex) 
        { 
            textures.Add(tex);
            AddTex(NativePtr.Handle, tex.GetNative().Handle);
        }

        [SuppressUnmanagedCodeSecurity]
        public void RemoveTexture(Texture tex) 
        {
            textures.Remove(tex);
            RemoveTex(NativePtr.Handle, tex.GetNative().Handle);
        }
        /// <summary>
        /// Replace a texture in the given index. 
        /// Ignores if index is out of range.
        /// </summary>
        public void SetTexture(Texture tex, uint index)
        {
            if(index >= textures.Count) { return; }
            textures[(int)index] = tex;
            SetTex(NativePtr.Handle, tex.GetNative().Handle, index);
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
        public HandleRef GetNative() { return  NativePtr; }

        void Dispose(bool disposing)
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
