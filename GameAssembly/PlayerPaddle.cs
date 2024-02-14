using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sphynx;
using Sphynx.Core.Graphics;

namespace GameAssembly
{
    public class PlayerPaddle : Component
    {
        public override void Start()
        {
            gameObject.AddComponent<SpriteRenderer>();
            Logger.Info("Testing for thread");
        }

        public override void Update()
        {
        }
    }
}
