using System;
using System.Threading;
using System.Windows.Forms;

namespace BombastEditor
{
    class Program
    {

        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            BombastEditorForm form = new BombastEditorForm();

            MessageHandler messageHandler = form.GetMessageHandler();
            Application.AddMessageFilter(messageHandler);

            Application.Run(form);
        }
    }
}
