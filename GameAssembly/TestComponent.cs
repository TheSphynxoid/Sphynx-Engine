using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sphynx;

namespace GameAssembly
{
    public class TestComponent : Sphynx.Component
    {
        public override void Start()
        {

        }

        public override void Update()
        {
            if (Input.IsKeyDown(Keys.Up)){
            }
        }
    }
}
