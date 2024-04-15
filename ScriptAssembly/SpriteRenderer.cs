using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using GlmSharp;
using Sphynx.Core;
using Sphynx.Graphics;

namespace Sphynx
{
    public sealed class SpriteRenderer : Component
    {
        /// <summary>
        /// Default vertex shader for sprites used to generate default material.
        /// </summary>
        public static readonly Shader SpriteVertShader = AssetManager.LoadShader("data\\assets\\Shaders\\sprite.vs", ShaderType.VertexShader);
        /// <summary>
        /// Default fragment shader for sprites used to generate default material.
        /// </summary>
        public static readonly Shader SpriteFragShader = AssetManager.LoadShader("data\\assets\\Shaders\\sprite.fs", ShaderType.FragmentShader);
        //public static readonly Shader AtlasShader;
        /// <summary>
        /// Default Material for sprite rendering.
        /// </summary>
        public static readonly Material SpriteDefaultMaterial =
            new(new ShaderPack(SpriteVertShader, SpriteFragShader));
        /// <summary>
        /// Used material for rendering.
        /// </summary>
        public Material Material { get; set; }
        public Vector2 Center { get; set; }
        public Vector2 Size { get; set; }

        //If the sprite is unspecified this one will be used.
        public static Texture DefaultSprite = AssetManager.LoadTexture("data\\assets\\cardback.jpg", TextureType.Texture2D);
        Texture sprite;
        public Texture Sprite { get => sprite; set { SetSprite(value); Invalidate(); } }

        //This is large code for construction.
        //To avoid creating a list with the verts, and a BufferLayout instance it's all made for this call once.
        static readonly VertexBuffer vb = new(new[]{
            // positions    // texture coords
            1.0f, 1.0f,     1.0f, 1.0f,   // top right
            1.0f, -1.0f,    1.0f, 0.0f,   // bottom right
            -1.0f, -1.0f,   0.0f, 0.0f,   // bottom left
            -1.0f, 1.0f,    0.0f, 1.0f    // top left 
            },
            20, new BufferLayout()
            {
                BufferElements = new BufferElement[]{ new(ShaderDataType.Float2, false),
                    new(ShaderDataType.Float2, false) }
            });

        static readonly IndexBuffer ib = new IndexBuffer(new uint[]{
            0, 1, 2,
            2, 3, 0
        });
        
        readonly Mesh plane = new (vb, ib);

        Uniform ModelUniform = new Uniform();

        static mat4 OrthoProj = new mat4();

        static readonly Uniform ProjViewUniform = new Uniform();

        RenderObject renderObject;

        public SpriteRenderer()
        {
            Material = new(SpriteDefaultMaterial);
            Material.AddTexture(DefaultSprite);
            renderObject.mat = Material;
            renderObject.mesh = plane;
            //Uniform setup.
            OrthoProj = mat4.Ortho(0.0f, Window.Bounds.Width, 0.0f, Window.Bounds.Height, 0.1f, 1.0f);
            Material.SetUniform(ModelUniform, 1);
            Material.SetUniform(ProjViewUniform, 1);
        }
        /// <summary>
        /// Change the Sprite.
        /// </summary>
        /// <param name="NewSprite"></param>
        void SetSprite(Texture NewSprite)
        {
            this.sprite = Sprite;
            //Set the default size to sprite size.
            Size = Sprite.Size;
            Center = Size / 2f;
            Material.SetTexture(NewSprite, 0);
        }

        public void Invalidate()
        {
            //Regen Material.

        }

        public override void Update()
        {
            Renderer.Submit(renderObject);
        }

        internal void HandleResize()
        {
        }
    }
}
