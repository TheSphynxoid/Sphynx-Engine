using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    [StructLayout(LayoutKind.Explicit)]
    public struct Matrix2X2
    {
        //Union.
        [FieldOffset(0)]
        public Vector2 v1;
        [FieldOffset(0)]
        public float f1;
        [FieldOffset(sizeof(float))]
        public float f2;
        [FieldOffset(sizeof(float) * 2)]
        public Vector2 v2;
        [FieldOffset(sizeof(float) * 2)]
        public float f3;
        [FieldOffset(sizeof(float) * 3)]
        public float f4;

        public Matrix2X2(float _f1, float  _f2, float _f3, float _f4)
        {
            f1 = _f1;
            f2 = _f2;
            f3 = _f3;
            f4 = _f4;
        }
    }
    public class Matrix4X4
    {
    }
}
