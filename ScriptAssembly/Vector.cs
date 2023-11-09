using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public struct Vector2
    {
        public float x, y;

        public Vector2(float scalar) { x = scalar; y = scalar; }
        public Vector2(float X, float Y) { x = X; y = Y;}

        public bool Equals(Vector2 other)
        {
            return other.x == x && other.y == y;
        }

        public static Vector2 operator+(Vector2 left, Vector2 right)
        {
            return new Vector2(left.x + right.x, left.y + right.y) ;
        }

        public static Vector2 operator-(Vector2 left, Vector2 right)
        {
            return new Vector2(left.x - right.x, left.y - right.y);
        }
        
        public static Vector2 operator*(Vector2 left,float scalar)
        {
            return new Vector2(left.x * scalar, left.y * scalar);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static float Dot(Vector2 left, Vector2 right);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public float Dot(ref Vector2 right)
        {
            return Dot(this, right);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static float Distance(Vector2 v, Vector2 v2);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public float Distance(ref Vector2 v)
        {
            return Distance(this, v);
        }
        //Cast operators
        public static implicit operator Vector3(Vector2 v) => new Vector3(v.x, v.y, 0);
        public static implicit operator Vector4(Vector2 v) => new Vector4(v.x, v.y, 0, 0);
        public static explicit operator Vector2(Vector3 v3) => new Vector2(v3.x, v3.y);
        public static explicit operator Vector2(Vector4 v4) => new Vector2(v4.x, v4.y);

        public override string ToString() => $"({x},{y})";
    }
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float scalar) { x = scalar;y=scalar;z = scalar; }
        public Vector3(float X = 0, float Y = 0, float Z = 0) { x = X; y=Y; z=Z; }

        public bool Equals(Vector3 other)
        {
            return other.x == x && other.y == y && z == other.z;
        }

        public static Vector3 operator +(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x + right.x, left.y + right.y, left.z+right.z);
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.x - right.x, left.y - right.y, left.z-right.z);
        }

        public static Vector3 operator *(Vector3 left, float scalar)
        {
            return new Vector3(left.x * scalar, left.y * scalar, left.z * scalar);
        }

        //Upcast
        public static implicit operator Vector4(Vector3 v) => new Vector4(v.x, v.y, v.z, 0);

        public override string ToString() => $"({x},{y},{z})";

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static float Dot(Vector3 left, Vector3 right);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public float Dot(Vector3 right)
        {
            return Dot(this, right);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static Vector3 Cross(Vector3 left, Vector3 right);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public Vector3 Cross(Vector3 right)
        {
            return Cross(this, right);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static float Distance(Vector3 v, Vector3 v2);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public float Distance(ref Vector3 v)
        {
            return Distance(this, v);
        }
    }
    public struct Vector4
    {
        public float x, y, z, w;
        public Vector4(float scalar) { x=scalar;y=scalar;z=scalar; w=scalar; }
        public Vector4(float X, float Y, float Z, float W) { x=X; y=Y; z=Z; w=W; }



        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static float Dot(Vector4 left, Vector4 right);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public float Dot(Vector4 right)
        {
            return Dot(this, right);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static float Distance(Vector4 v, Vector4 v2);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public float Distance(Vector4 v)
        {
            return Distance(this, v);
        }
    }
    
}
