using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace make_sounds
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("No work here");
                return;
            }

            File.Delete(args[0]);
            FileStream output = File.OpenWrite(args[0]);

            try
            {
                FileStream[] streams = new FileStream[args.Length - 1];

                for (int i = 0; i < args.Length - 1; i++)
                {
                    streams[i] = File.OpenRead(args[i + 1]);
                }

                output.WriteByte(0xff);
                output.WriteByte((byte)streams.Length);

                int addr = 2 + 3 * streams.Length;

                for (int i = 0; i < streams.Length; i++)
                {
                    output.WriteByte((byte)(addr >> 16));
                    output.WriteByte((byte)(addr >> 8));
                    output.WriteByte((byte)(addr));
                    addr += (int)streams[i].Length + 1;
                }

                for (int i = 0; i < streams.Length; i++)
                {
                    byte[] buffer = new byte[1024];
                    int len;
                    while ((len = streams[i].Read(buffer, 0, 1024)) > 0)
                    {
                        output.Write(buffer, 0, len);
                    }
                    output.WriteByte(0xff);
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
