using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace sqfvm_debugger_test
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                using (var client = new TcpClient("localhost", 9090))
                {
                    var stream = client.GetStream();
                    var buffer = new byte[1 << 11];
                    while (client.Connected)
                    {
                        var read = stream.Read(buffer, 0, buffer.Length);
                        if (read == 0)
                        {
                            break;
                        }

                        var str = Encoding.ASCII.GetString(buffer, 0, read);
                        Console.WriteLine(str);
                        Console.Write("> ");
                        var input = Console.ReadLine();
                        if (String.IsNullOrWhiteSpace(input))
                        {
                            continue;
                        }
                        var bytes = Encoding.ASCII.GetBytes(input);
                        stream.Write(bytes, 0, bytes.Length);
                        stream.WriteByte(0);
                    }
                }
            }
            catch(Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine(ex.Message);
                Console.ResetColor();
                Console.WriteLine("Press any key to continue...");
                Console.ReadKey();
            }
        }
    }
}
