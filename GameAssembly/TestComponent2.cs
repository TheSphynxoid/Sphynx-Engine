using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameAssembly
{
    public class TestComponent2 : Sphynx.Component
    {
        public override void Start()
        {
            Sphynx.Logger.Debug("Cool !");
        }
    }
}
