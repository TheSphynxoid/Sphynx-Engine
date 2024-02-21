using Sphynx.Core;
using Sphynx.Core.Graphics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    [Header("Material.h", NativeTypeName = "Material")]
    [NativeWrapper("Material",true)]
    public sealed class Material
    {
        private ShaderPack shaders;
        public ShaderPack Shaders {  get => shaders; set { ReloadShaders(value);shaders = value; } }

        internal List<Texture> textures;
        public ref readonly List<Texture> Textures { get => ref textures; }

        internal UIntPtr NativePtr;
        internal UIntPtr NativeID;

        /// <summary>
        /// This is not a pointer.
        /// </summary>
        public UIntPtr ID { get => NativeID; }

        internal Material(UIntPtr native)
        {
            
        }

        public Material() 
        {

        }

        public Material(ShaderPack shaders)
        {

        }

        public Material(ShaderPack shaders, List<Texture> textures)
        {

        }

        public void ReloadShaders(ShaderPack shaders) { }
        public void Bind() { }
        public void Unbind() { }
        //public int GetUniformLocation(string name) { return 0; }
        public void SetUniform(Uniform uniform, string name) { }
        public void SetUniform(Uniform uniform, int location) { }
        //public void SetUniformBuffer(UniformBuffer uniformBuffer, string name) { }
        //public void SetUniformBuffer(UniformBuffer uniformBuffer, int location) { }
        public void AddTexture(Texture tex) 
        { 
            textures.Add(tex);
        }
        public void RemoveTexture() { }

        public UIntPtr GetNative() { return  NativePtr; }
    }
}
