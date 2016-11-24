using System;
using System.Windows.Forms;
using static BombastEditor.NativeMethods;

namespace BombastEditor
{
    public class MessageHandler : IMessageFilter
    {
        const int WM_MOUSEMOVE = 0x0200;
        const int WM_LBUTTONDOWN = 0x0201;
        const int WM_LBUTTONUP = 0x0202;
        const int WM_LBUTTONDBLCLK = 0x0203;
        const int WM_RBUTTONDOWN = 0x0204;
        const int WM_RBUTTONUP = 0x0205;
        const int WM_RBUTTONDBLCLK = 0x0206;
        const int WM_MBUTTONDOWN = 0x0207;
        const int WM_MBUTTONUP = 0x0208;
        const int WM_MBUTTONDBLCLK = 0x0209;

        const int WM_KEYDOWN = 0x0100;
        const int WM_KEYUP = 0x0101;
        const int WM_SYSKEYDOWN = 0x0104;
        const int WM_SYSKEYUP = 0x0105;
        const int WM_CLOSE = 0x0010;

        Panel m_editorViewportPanel;

        public MessageHandler(Panel editorViewportPanel, BombastEditorForm editorForm)
        {
            m_editorViewportPanel = editorViewportPanel;
        }

        public bool PreFilterMessage(ref Message m)
        {
            if (m.HWnd == m_editorViewportPanel.Handle)
            {
                //Focus if clicking in Editor Panel
                if(m.Msg == WM_MBUTTONDOWN || m.Msg == WM_LBUTTONDOWN || m.Msg == WM_RBUTTONDOWN)
                {
                    m_editorViewportPanel.Focus();
                }

                switch (m.Msg)
                {
                    case WM_MOUSEMOVE:
                    case WM_LBUTTONDOWN:
                    case WM_RBUTTONDOWN:
                    case WM_MBUTTONDOWN:
                    case WM_KEYDOWN:
                    case WM_KEYUP:
                    case WM_SYSKEYDOWN:
                    case WM_SYSKEYUP:
                    case WM_LBUTTONUP:
                    case WM_LBUTTONDBLCLK:
                    case WM_RBUTTONUP:
                    case WM_RBUTTONDBLCLK:
                    case WM_MBUTTONUP:
                    case WM_MBUTTONDBLCLK:
                    case WM_CLOSE:
                        NativeMethods.WndProc(m_editorViewportPanel.Handle, m.Msg, m.WParam.ToInt32(), m.LParam.ToInt32());
                        return true;
                }
            }

            return false;
        }
    }
}
