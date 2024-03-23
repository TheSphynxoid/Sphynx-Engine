using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Sphynx.Core;
using Sphynx.Graphics;

namespace Sphynx
{
    public sealed class SpriteRenderer : Component
    {
        /// <summary>
        /// Default shader for sprites. 
        /// Used to generate default material.
        /// </summary>
        public static readonly Shader SpriteFragShader = AssetManager.LoadShader("data\\assets\\Shaders\\sprite.fs", ShaderType.FragmentShader);
        //public static readonly Shader AtlasShader;
        /// <summary>
        /// Default Material for sprite rendering.
        /// </summary>
        public static readonly Material SpriteDefaultMaterial =
            new Material(new ShaderPack(Material.GetDefaultShader(ShaderType.VertexShader), SpriteFragShader));
        /// <summary>
        /// Used material for rendering.
        /// </summary>
        public Material Material { get; set; }
        public Vector2 Center { get; set; }
        public Vector2 Size { get; set; }
        //public static Lazy<Texture> DefaultSprite = 
        //    new(()=> { return AssetManager.LoadTexture("data\\assets\\cardback.jpg", TextureType.Texture2D); });
        public static Texture DefaultSprite = AssetManager.LoadTexture("data\\assets\\cardback.jpg", TextureType.Texture2D);
        private Texture sprite;
        public Texture Sprite { get => sprite; set { SetSprite(value); Invalidate(); } }

        public SpriteRenderer()
        {
            Material = new(SpriteDefaultMaterial);
            Material.AddTexture(DefaultSprite);
        }
        /// <summary>
        /// Change the Sprite.
        /// </summary>
        /// <param name="NewSprite"></param>
        private void SetSprite(Texture NewSprite)
        {
            this.sprite = Sprite;
            //Set the default size to sprite size.
            Size = Sprite.Size;
            Center = Size / 2f;
            Material.SetTexture(NewSprite, 0);
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
