using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("No work here");
                return;
            }

            FileStream input = File.OpenRead(args[0]);
            File.Delete(args[1]);
            FileStream output = File.OpenWrite(args[1]);

            try
            {
                byte[] buffer = new byte[1024];

                int len, olen;
                while ((len = input.Read(buffer, 0, 1024)) > 0)
                {
                    olen = len / 2;
                    for (var i = 0; i < olen; i++) {
                        buffer[i] = (byte) ((buffer[i*2+1] ^ 0x80) & 0xfe);
                    }

                    output.Write(buffer, 0, olen);
                }

            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
            finally
            {
                output.Close();
            }
        }
    }
}
