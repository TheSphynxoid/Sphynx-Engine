using System;
using System.Collections;
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

        public static Texture DefaultSprite = AssetManager.LoadTexture("data\\assets\\cardback.jpg", TextureType.Texture2D);
        private Texture sprite;
        public Texture Sprite { get => sprite; set { SetSprite(value); Invalidate(); } }

        //This is large code for construction.
        //To avoid creating a list with the verts, and a BufferLayout instance it's all made for this call once.
        static readonly VertexBuffer VB = new VertexBuffer(new float[]{
            // positions        // texture coords
            1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,   // bottom right
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
            -1.0f, 1.0f, 0.0f,  0.0f, 1.0f    // top left 
            },
            20, new BufferLayout()
            {
                bufferElements = new BufferElement[]{ new BufferElement(ShaderDataType.Float3, false),
                    new BufferElement(ShaderDataType.Float2, false) }
            });

        static readonly IndexBuffer IB = new IndexBuffer(new uint[]{
            0, 1, 2,
            2, 3, 0
        });
        private Mesh Plane;

        private RenderObject renderObject;

        public SpriteRenderer()
        {
            Material = new(SpriteDefaultMaterial);
            Material.AddTexture(DefaultSprite);
            renderObject.mat = Material;
            renderObject.mesh = Plane;
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
            Renderer.Submit(renderObject);
        }

    }
}
