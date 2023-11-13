using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sphynx;
using Sphynx.Core.Graphics;

namespace GameAssembly
{
    public class TestComponent : Component
    {
        public override void Start()
        {
            Logger.Info("C# Called");
            Window.Bounds = new Bounds(Window.Bounds.Width + 100, Window.Bounds.Height + 100);
        }

        public override void Update()
        {
            if (Input.IsKeyDown(Keys.Up)){
            }
        }
    }
}
