//using System;
//using System.Collections;
//using System.Collections.Generic;
//using System.Linq;
//using System.Runtime.CompilerServices;
//using System.Runtime.InteropServices;
//using System.Text;
//using System.Threading.Tasks;

using System.Runtime.InteropServices;
using System;

#region MonoExample
//This is the Memory class example provided by mono in the C# specifications.
//http://docs.go-mono.com/?link=ecmaspec%3a25.8

//public unsafe class Memory
//{
//    // Handle for the process heap. This handle is used in all calls to the
//    // HeapXXX APIs in the methods below.  
//    static int ph = GetProcessHeap();
//    // Private instance constructor to prevent instantiation.  
//    private Memory() { }
//    // Allocates a memory block of the given size. The allocated memory is  
//    // automatically initialized to zero.  
//    public static void* Alloc(int size)
//    {
//        void* result = HeapAlloc(ph, HEAP_ZERO_MEMORY, size);
//        if (result == null) throw new OutOfMemoryException();
//        return result;
//    }
//    // Copies count bytes from src to dst. The source and destination  
//    // blocks are permitted to overlap.  
//    public static void Copy(void* src, void* dst, int count)
//    {
//        byte* ps = (byte*)src;
//        byte* pd = (byte*)dst;
//        if (ps > pd)
//        {
//            for (; count != 0; count--) *pd++ = *ps++;
//        }
//        else if (ps < pd)
//        {
//            for (ps += count, pd += count; count != 0; count--) *--pd = *--
//            ps;
//        }
//    }
//    // Frees a memory block.  
//    public static void Free(void* block)
//    {
//        if (!HeapFree(ph, 0, block)) throw new InvalidOperationException();
//    }
//    // Re-allocates a memory block. If the reallocation request is for a  
//    // larger size, the additional region of memory is automatically  
//    // initialized to zero.  
//    public static void* ReAlloc(void* block, int size)
//    {
//        void* result = HeapReAlloc(ph, HEAP_ZERO_MEMORY, block, size);
//        if (result == null) throw new OutOfMemoryException();
//        return result;
//    }
//    // Returns the size of a memory block.  
//    public static int SizeOf(void* block)
//    {
//        int result = HeapSize(ph, 0, block);
//        if (result == -1) throw new InvalidOperationException();
//        return result;
//    }
//    // Heap API flags  
//    const int HEAP_ZERO_MEMORY = 0x00000008;
//    // Heap API functions  
//    [DllImport("kernel32")]
//    static extern int GetProcessHeap();
//    [DllImport("kernel32")]
//    static extern void* HeapAlloc(int hHeap, int flags, int size);
//    [DllImport("kernel32")]
//    static extern bool HeapFree(int hHeap, int flags, void* block);
//    [DllImport("kernel32")]
//    static extern void* HeapReAlloc(int hHeap, int flags,
//    void* block, int size);
//    [DllImport("kernel32")]
//    static extern int HeapSize(int hHeap, int flags, void* block);
//}
#endregion

//namespace Sphynx.Core.Native
//{
//    ///// Managed Buffer with memory from native.
//    /// <summary>
//    /// Native-backed fixed sized array.
//    /// </summary>
//    [Serializable]
//    public class NativeBuffer<T> : IDisposable where T : unmanaged
//    {
//        private readonly nuint ElementSize = (nuint)Marshal.SizeOf<T>();
//        private nuint size;
//        /// <summary>
//        /// Buffer size in Bytes.
//        /// </summary>
//        public nuint Size { get => size; }

//        private HandleRef ptr;
//        private bool disposedValue;

//        /// <summary>
//        /// Pointer to unmanaged memory containing the buffer.
//        /// </summary>
//        public unsafe IntPtr Buffer { get => ptr.Handle; }

//        public nuint Count => size / ElementSize;

//        public T this[nuint index] { get { 
//                unsafe {
//                    var p = (IntPtr)(ptr.Handle.ToInt64() + (long)(ElementSize * index));

//                } 
//            } }

//        public NativeBuffer(nuint size)
//        {
//            this.size = size;
//            ptr = new HandleRef(this,Memory.malloc(size));

//            unsafe
//            {
//                for(nuint i = 0; i <= Count; i++)
//                {
//                    IntPtr elem = new(ptr.Handle.ToPointer() + (ElementSize * i));
//                }
//            }
//        }

//        public static unsafe explicit operator IntPtr(NativeBuffer<T> buf)
//        {
//            return buf.Buffer;
//        }

//        protected virtual void Dispose(bool disposing)
//        {
//            if (!disposedValue)
//            {
//                if (disposing)
//                {
//                    // TODO: dispose managed state (managed objects)
//                }

//                // TODO: free unmanaged resources (unmanaged objects) and override finalizer
//                Memory.free(Buffer);
//                ptr = new HandleRef(null, IntPtr.Zero);
//                // TODO: set large fields to null
//                disposedValue=true;
//            }
//        }

//        // // TODO: override finalizer only if 'Dispose(bool disposing)' has code to free unmanaged resources
//        ~NativeBuffer()
//        {
//            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
//            Dispose(disposing: false);
//        }

//        public void Dispose()
//        {
//            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
//            Dispose(disposing: true);
//            GC.SuppressFinalize(this);
//        }
//    }

//    /// <summary>
//    /// Supporting 64-bit Memory operations.
//    /// </summary>
//    public static class Memory
//    {
//        /// <summary>
//        /// A direct call to malloc avoids the engine managing the buffer if I ever implement it thus you must free it yourself.
//        /// Calls native C malloc.
//        /// </summary>
//        /// <param name="size">Size in bytes.</param>
//        /// <returns>A pointer to a buffer.</returns>
//        [MethodImpl(MethodImplOptions.InternalCall)]
//        internal unsafe static extern IntPtr malloc(nuint size);
//        //[MethodImpl(MethodImplOptions.InternalCall)]
//        //internal unsafe static extern IntPtr malloc_aligned(nuint size, nuint alignment);
//        /// <summary>
//        /// A direct call to malloc avoids the engine managing the buffer if I ever implement it thus you must free it yourself.
//        /// Calls native C malloc.
//        /// </summary>
//        /// <param name="ptr">Old buffer pointer.</param>
//        /// <param name="size">Size in bytes.</param>
//        /// <returns>A new pointer to a buffer.</returns>
//        [MethodImpl(MethodImplOptions.InternalCall)]
//        internal unsafe static extern IntPtr realloc(IntPtr ptr, nuint size);
//        [MethodImpl(MethodImplOptions.InternalCall)]
//        internal unsafe static extern void free(IntPtr ptr);
//        [MethodImpl(MethodImplOptions.InternalCall)]
//        internal unsafe static extern void memcpy(byte[] Source, nuint offset, nuint count, IntPtr dest);
//        [MethodImpl(MethodImplOptions.InternalCall)]
//        internal unsafe static extern void memset(IntPtr dest, nuint buffersize, int val);

//        public static IntPtr Allocate(nuint size)
//        {
//            return malloc(size);
//        }

//        public static IntPtr Reallocate(IntPtr mem, nuint size) 
//        {
//            return realloc(mem,size);
//        }

//        public static void Free(IntPtr mem)
//        {
//            free(mem);
//        }

//        public static void Set(IntPtr ptr, nuint size, int value)
//        {
//            if (ptr != IntPtr.Zero) { memset(ptr, size, value); }
//        }

//        public static void Copy(byte[] bytes, nuint offset, nuint count, IntPtr dest)
//        {
//            memcpy(bytes, offset, count, dest);
//        }
//    }
//}
