using System;
using System.Drawing;
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

        BombastEditorForm m_parentForm;
        Panel m_editorViewportPanel;
        bool m_viewportFocus;
        Point m_mouseDownLocation;

        public MessageHandler(Panel editorViewportPanel, BombastEditorForm editorForm)
        {
            m_parentForm = editorForm;
            m_editorViewportPanel = editorViewportPanel;
            m_viewportFocus = false;
            m_mouseDownLocation = new Point(0, 0);
        }

        void CheckForViewportFocus()
        {
            Point position = Cursor.Position;
            Point relativeToForm = m_editorViewportPanel.PointToClient(position);
            m_viewportFocus = (relativeToForm.X >= 0 && relativeToForm.Y >= 0 && relativeToForm.X < m_editorViewportPanel.Width && relativeToForm.Y < m_editorViewportPanel.Width);
            if (m_viewportFocus)
            {
                m_mouseDownLocation = position;
            }
        }

        public bool PreFilterMessage(ref Message m)
        {
            //If clicking, check if mouse in viewport to simulate focus
            if (m.Msg == WM_MBUTTONDOWN || m.Msg == WM_LBUTTONDOWN || m.Msg == WM_RBUTTONDOWN)
            {
                CheckForViewportFocus();
            }

            if (m.HWnd == m_editorViewportPanel.Handle || (m_viewportFocus && (m.Msg == WM_KEYUP || m.Msg == WM_KEYDOWN)))
            {
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

                        //Check if clicked in the viewport (And barely moved mouse between mouse up/down)
                        //Raycast to see if intersects with anything
                        if(m_viewportFocus && m.Msg == WM_LBUTTONUP)
                        {
                            Point pos = Cursor.Position;
                            var distance = Math.Round(Math.Sqrt(Math.Pow((pos.X - m_mouseDownLocation.X), 2) + Math.Pow(pos.Y - m_mouseDownLocation.Y, 2)), 1);
                            if (distance < 3)
                            {
                                m_parentForm.TryPickActor();
                            }
                        }

                        return true;
                }
            }

            return false;
        }
    }
}
