using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;


namespace Discord_Bot
{
    public static class SQF_VM
    {
        [DllImport(@"SQF-VM", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern unsafe IntPtr start_program(string input);
        public static unsafe string StartProgram(string content)
        {

            IntPtr ret = start_program(content);
            return System.Runtime.InteropServices.Marshal.PtrToStringAnsi(ret);
        }
    }
}
