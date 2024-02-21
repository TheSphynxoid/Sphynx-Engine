using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core.Graphics
{
    public sealed class SpriteRenderer : Component
    {
        /// <summary>
        /// Default shader for sprites. 
        /// Used to generate default material.
        /// </summary>
        public static readonly Shader SpriteFragShader = AssetManager.LoadShader("data\\assets\\Shaders\\sprite.fs", ShaderType.FragmentShader);
        /// <summary>
        /// Default Material for sprite rendering.
        /// </summary>
        public static readonly Material SpriteDefaultMaterial;
        /// <summary>
        /// Used material for rendering.
        /// </summary>
        public Material Material { get; set; }
        public Vector2 Center { get; set; }
        public Vector2 Size { get; set; }
        private Texture sprite;
        public Texture Sprite { get => sprite; set { sprite = value; Invalidate(); } }

        public SpriteRenderer(Texture Sprite)
        {
            this.sprite = Sprite;
            //Set the default size to sprite size.
            Size = Sprite.Size;
            Material = SpriteDefaultMaterial;
        }

        internal void Invalidate()
        {
            //Regen Material.

        }

        public override void Update()
        {

        }

    }
}
