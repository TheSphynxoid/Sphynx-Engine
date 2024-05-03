using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core
{
    public enum EntityFlags : byte
    {
        Void = 0x0,
        GameObject = 0x1,
        Component = 0x2,
        Reserved1 = 0x4,
        UserDefined = 0x8
    }
    [StructLayout(LayoutKind.Explicit, Size = 8)]
    [NativeCppClass]
    public struct EntityID
    {
        [FieldOffset(0)]
        public ulong NumericRepresentation;
        [FieldOffset(0)]
        public ushort InstanceID;
        [FieldOffset(2)]
        public ushort GenerationID;
        [FieldOffset(4)]
        public ushort Userflag;
        [FieldOffset(6)]
        public byte Padding;
        [FieldOffset(7)]
        public byte Flag;

        public EntityID(ulong value)
        {
            InstanceID = (ushort)((value >> 0) & 0xFFFF);
            GenerationID = (ushort)((value >> 16) & 0xFFFF);
            Userflag = (ushort)((value >> 32) & 0xFFFF);
            Padding = (byte)((value >> 48) & 0xFF);
            Flag = (byte)((value >> 56) & 0xFF);
        }
        public static explicit operator ulong(EntityID id)
        {

            ulong result = 0;

            result |= (ulong)(id.InstanceID) << 0;
			result |= (ulong)(id.GenerationID) << 16;
			result |= (ulong)(id.Userflag) << 32;
			result |= (ulong)(id.Padding) << 48;
			result |= (ulong)(id.Flag) << 56;

			return result;
		}
        public static bool operator ==(EntityID id1, EntityID id2)
        {
            return id1.NumericRepresentation == id2.NumericRepresentation;
        }
        public static bool operator !=(EntityID id1, EntityID id2)
        {
            return !(id1==id2);
        }
    }

    /// <summary>
    /// Interoperate with the native Entity Registry.
    /// </summary>
    internal sealed class NativeEntityRegistry
    {
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern GameObject GetGOByID(EntityID id);
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern void GetID_internal(EntityID id);

        public void GetID(EntityID id, object obj)
        {

        }
        public void ReturnID(EntityID id) 
        {

        }
        public GameObject GetGameObjectByID(EntityID id)
        {
            if(id.Flag != (byte)EntityFlags.GameObject)
            {
                return null;
            }
            return GetGOByID(id);
        }
    }
}
