using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace omp
{
    internal class Log : Lucid.Core.Logger.Listener
    {
        private System.IO.FileStream file = null;

        public Log()
        {
            if (System.IO.File.Exists("results.log"))
                System.IO.File.Delete("results.log");

            file = System.IO.File.Create("results.log");

            String header = " cat\tmessage\n" + "-----\t------------------------------------------------------------\n";
            byte[] text = new UTF8Encoding(true).GetBytes(header);
            file.Write(text, 0, text.Length);
        }

		public override void OnMessage(String category, String message)
        {
            String formatted = category + " \t" + message + "\n";

            byte[] text = new UTF8Encoding(true).GetBytes(formatted);
            file.Write(text, 0, formatted.Length);
        }
    }

}
