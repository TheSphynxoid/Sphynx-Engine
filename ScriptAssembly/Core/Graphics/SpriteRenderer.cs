using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core.Graphics
{
    public sealed class SpriteRenderer : Component
    {
        public Material Material { get; set; }
        public Vector2 Center { get; set; }
        public Vector2 Size { get; set; }

        public override void Update()
        {

        }

    }
}
