using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sphynx;
using Sphynx.Core;
using Sphynx.Core.Graphics;

namespace GameAssembly
{
    public class PlayerPaddle : Component
    {
        public override void Start()
        {
            //gameObject.AddComponent<SpriteRenderer>();
            Logger.Info("Batata");
            //Shader shader = AssetManager.LoadShader("data/assets/Shaders/sprite.fs", ShaderType.FragmentShader);
        }

        public override void Update()
        {
        }
    }
}
