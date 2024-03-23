using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using Sphynx;
using Sphynx.Core;
using Sphynx.Graphics;

namespace GameAssembly
{
    public class PlayerPaddle : Component
    {
        SpriteRenderer spriteRenderer;
        public override void Start()
        {
            spriteRenderer = gameObject.AddComponent<SpriteRenderer>();
        }

        public override void Update()
        {
        }
    }
}
