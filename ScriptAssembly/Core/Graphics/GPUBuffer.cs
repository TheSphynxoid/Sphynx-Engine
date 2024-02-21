using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core.Graphics
{
    public class GPUBuffer
    {
        public enum MapAccess : byte
        {
            Read, Write, ReadWrite
        }
        public enum UsageHint : byte
        {
            Stream, Static, Dynamic
        }
        public enum AccessHint : byte
        {
            Draw, Read, Copy
        }

        private ulong size;
        public ulong Size { get => size; set => Resize(value); }

        public GPUBuffer(ulong size)
        {

        }

        public void Resize(ulong size)
        {
            Size = size;
        }

        public UIntPtr Map(MapAccess access) { return UIntPtr.Zero; }
        public void Unmap() { }
        public void Bind() { }
        public void Unbind() { }
    }
}
