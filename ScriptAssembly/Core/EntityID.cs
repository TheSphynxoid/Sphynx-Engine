using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core
{
    public enum EntityFlags : ushort
    {
        Void = 0x0,
        GameObject = 0x1,
        Component = 0x2,
        Reserved1 = 0x4,
        UserDefined = 0x8
    }
    [StructLayout(LayoutKind.Explicit, Size = 8)]
    public struct EntityID
    {
        [FieldOffset(0)]
        public ulong NumericRepresentation;
        [FieldOffset(0)]
        public ushort InstanceID;
        [FieldOffset(2)]
        public ushort GenerationID;
        [FieldOffset (4)]
        public ushort Userflag;
        [FieldOffset(6)]
        public byte Padding;
        [FieldOffset(7)]
        public byte Flag;
    }
}
