using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Discrod_Bot
{
    public static class SQF_VM
    {
        [DllImport(@"SQF-VM.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern string start_program(string content);
    }
}
