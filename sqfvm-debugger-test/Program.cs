using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace sqfvm_debugger_test
{
    class Program
    {
        static Stream stream;
        static void Main(string[] args)
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine("Use CTRL-C to enter commands or to communicate with the server.");
            Console.WriteLine("':?' will output help.");
            Console.ResetColor();
            bool continueExecution = true;
            bool isInInterrupt = false;
            Console.CancelKeyPress += (sender, e) => {
                e.Cancel = true;
                isInInterrupt = true;
                Console.ForegroundColor = ConsoleColor.Yellow;
                Console.Write("> ");
                var input = Console.ReadLine();
                Console.ResetColor();
                if (String.IsNullOrWhiteSpace(input))
                {
                    return;
                }
                if (input[0] == ':')
                {
                    switch (input.Substring(1).ToLower())
                    {
                        case "q":
                            continueExecution = false;
                            e.Cancel = false;
                            break;
                        case "?":
                        case "h":
                        default:
                            Console.ForegroundColor = ConsoleColor.Yellow;
                            Console.WriteLine(":? - displays this information.");
                            Console.WriteLine(":h - displays this information.");
                            Console.WriteLine(":q - exits the execution loop.");
                            Console.ResetColor();
                            break;
                    }
                    isInInterrupt = false;
                    return;
                }
                if (stream == null)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine("Stream not yet connected. To quit, use ':q'");
                    Console.ResetColor();
                }
                var bytes = Encoding.ASCII.GetBytes(input);
                stream.Write(bytes, 0, bytes.Length);
                stream.WriteByte(0);
                isInInterrupt = false;
            };

            while (continueExecution)
            {
                try
                {
                    ConsoleCancelEventHandler handler = (sender, e) => { continueExecution = false; };
                    using (var client = new TcpClient("localhost", 9090))
                    {
                        stream = client.GetStream();
                        var buffer = new byte[1 << 11];
                        while (client.Connected)
                        {
                            if (!continueExecution)
                            {
                                break;
                            }

                            var read = stream.Read(buffer, 0, buffer.Length);
                            if (read == 0)
                            {
                                break;
                            }
                            var str = Encoding.ASCII.GetString(buffer, 0, read);
                            while (isInInterrupt) { Thread.Sleep(100); }
                            Console.WriteLine(str);
                        }
                    }
                }
                catch (Exception ex)
                {
                    while (isInInterrupt) { Thread.Sleep(100); }
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine(ex.Message);
                    Console.ResetColor();
                }
            }
        }
    }
}
