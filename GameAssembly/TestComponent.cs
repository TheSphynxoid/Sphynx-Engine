using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sphynx;

namespace GameAssembly
{
    public class TestComponent : Component
    {
        public override void Start()
        {
            Logger.Info("C# Called");
        }

        public override void Update()
        {
            if (Input.IsKeyDown(Keys.Up)){
            }
        }
    }
}
