using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BombastEditor
{
    public partial class BombastEditorForm : Form
    {
        private const int INVALID_ACTOR_ID = 0;

        private bool m_projectLoaded = false;

        private string m_projectDirectory;
        private string m_assetsDirectory;

        private MessageHandler m_messageHandler;

        public BombastEditorForm()
        {
            InitializeComponent();

            try
            {
                IntPtr hInstance = Marshal.GetHINSTANCE(GetType().Module);
                IntPtr hWnd = EditorViewportPanel.Handle;

                NativeMethods.EditorMain(hInstance, IntPtr.Zero, hWnd, 1, EditorViewportPanel.Width, EditorViewportPanel.Height);

                UpdateMenuBar();

                m_messageHandler = new MessageHandler(EditorViewportPanel, this);
            }
            catch(Exception ex)
            {
                MessageBox.Show("Error: " + ex.ToString());
            }
        }

        public MessageHandler GetMessageHandler()
        {
            return m_messageHandler;
        }

        private void Shutdown()
        {
            NativeMethods.Shutdown();
            Application.Exit();
        }

        private void InitializeAssetTree()
        {
            AssetsTreeView.Nodes.Clear();

            var stack = new Stack<TreeNode>();
            var rootDir = new DirectoryInfo(m_assetsDirectory);
            var node = new TreeNode(rootDir.Name);
            node.Tag = rootDir;
            stack.Push(node);

            while(stack.Count > 0)
            {
                var currentNode = stack.Pop();
                var directoryInfo = (DirectoryInfo)currentNode.Tag;
                foreach(var directory in directoryInfo.GetDirectories())
                {
                    FileAttributes attributes = File.GetAttributes(directory.FullName);
                    if((attributes & FileAttributes.Hidden) == 0)
                    {
                        var childDirNode = new TreeNode(directory.Name);
                        childDirNode.Tag = directory;
                        currentNode.Nodes.Add(childDirNode);
                        stack.Push(childDirNode);
                    }
                }

                foreach(var file in directoryInfo.GetFiles())
                {
                    FileAttributes attributes = File.GetAttributes(file.FullName);
                    if((attributes & FileAttributes.Hidden) == 0)
                    {
                        var childNode = new TreeNode(file.Name);
                        childNode.Tag = file.FullName;
                        currentNode.Nodes.Add(childNode);
                    }
                }
            }

            AssetsTreeView.Nodes.Add(node);
        }

        private void ActorsTreeView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            TreeNode node = AssetsTreeView.SelectedNode;
            if(node != null && node.Nodes.Count == 0)
            {
                var file = node.Tag.ToString();
                if(File.Exists(file))
                {
                    Process.Start(file);
                }
            }
        }
        private void UpdateMenuBar()
        {
            if(m_projectLoaded)
            {
                closeProjectToolStripMenuItem.Enabled = true;
            }
            else
            {
                closeProjectToolStripMenuItem.Enabled = false;
            }
        }

        private void openProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void closeProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Shutdown();
        }
    }
}
