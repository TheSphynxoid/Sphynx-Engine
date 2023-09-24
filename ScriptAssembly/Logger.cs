using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public class Logger
    {

        public enum LogLevel
        {
            trace = 0,
            debug,
            info,
            warn,
            err,
            critical,
            off,
            n_levels
        }

<<<<<<< HEAD
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //internal static extern Stream GetInStream();
        //[MethodImpl(MethodImplOptions.InternalCall)]
        //internal static extern Stream GetOutStream();

        //internal static void Initialize()
        //{
        //    Console.SetIn(new StreamReader(GetInStream(), Encoding.ASCII));
        //    Console.SetOut(new StreamWriter(GetOutStream(), Encoding.ASCII));
        //}
=======
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Stream GetInStream();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Stream GetOutStream();

        internal static void Initialize()
        {
            Console.SetIn(new StreamReader(GetInStream(), Encoding.ASCII));
            Console.SetOut(new StreamWriter(GetOutStream(), Encoding.ASCII));
        }
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void spdLog(string message, LogLevel logLevel);

#if DEBUG
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Trace(string message)
        {
            spdLog(message, LogLevel.trace);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Info(string message) 
        {
            spdLog(message, LogLevel.info);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Debug(string message)
        {
            spdLog(message, LogLevel.debug);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Warn(string message)
        {
            spdLog(message, LogLevel.warn);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Error(string message)
        {
            spdLog(message, LogLevel.err);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static void Critical(string message)
        {
            spdLog(message, LogLevel.critical);
        }
#else

        public static void Trace(string message) { }

        public static void Info(string message) { }

        public static void Debug(string message) { }

        public static void Warn(string message) { }

        public static void Error(string message) { }

        public static void Critical(string message) { }
#endif

    }
}
