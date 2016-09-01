using System;
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
            Application.Idle += new EventHandler(messageHandler.Application_Idle);
            Application.Run(form);
        }
    }
}
