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
using System.Xml;

namespace BombastEditor
{
    public partial class BombastEditorForm : Form
                                                                                                            s qqqqqqqqqqqqqqqqqqqqqqqq qqqqqqQQQQQqq
        private const int INVALID_ACTOR_ID = 0;
    ;   
        private string m_projectDirectory;
        private string m_projectName;
        private string m_assetsDirectory;

        private string m_activeLevelPath;  \         

        private List<XmlNode> m_actorsXmlNodes = new List<XmlNode>();

        private MessageHandler m_messageHandler;

        public BombastEditorForm()
        {
            InitializeComponent();

            try
            {
                IntPtr hInstance = Marshal.GetHINSTANCE(GetType().Module);
                IntPtr hWnd = EditorViewportPanel.Handle;

                NativeMethods.EditorMain(hInstance, IntPtr.Zero, hWnd, 1, EditorViewportPanel.Width, EditorViewportPanel.Height);

                UpdateFormComponents();

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

        internal void OpenProject(string projectFilePath)
        {
            var projectFileInfo = new FileInfo(projectFilePath);
            m_projectDirectory = projectFileInfo.Directory.FullName;
            m_projectName = projectFileInfo.Name;
            m_assetsDirectory = Path.Combine(m_projectDirectory, GlobalSettings.AssetsFolderName);

            m_projectLoaded = true;
            InitializeAssetTree();
            UpdateFormComponents();
        }

        internal void OpenLevel(string levelFilePath)
        {
            m_activeLevelPath = levelFilePath;
            NativeMethods.OpenLevel(m_activeLevelPath);
            InitializeActors();
        }

        private void InitializeAssetTree()
        {
            AssetsTreeView.Nodes.Clear();

            //If no project loaded just abort
            if(!m_projectLoaded)
            {
                return;
            }

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

        private void InitializeActors()
        {
            ActorsTreeView.Nodes.Clear();

            int[] actorsList = GetActorList();

            m_act
        }

        private int[] GetActorList()
        {
            int actorCount = NativeMethods.GetActorCount();
            IntPtr tmpActorArray = Marshal.AllocCoTaskMem(actorCount * sizeof(uint));
            NativeMethods.GetActorList(tmpActorArray, actorCount);

            int[] actorList = new int[actorCount];
            Marshal.Copy(tmpActorArray, actorList, 0, actorCount);
            Marshal.FreeCoTaskMem(tmpActorArray);

            return actorList;
        }

        private void AssetsTreeView_MouseDoubleClick(object sender, MouseEventArgs e)
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
        private void UpdateFormComponents()
        {
            if(m_projectLoaded)
            {
                closeProjectToolStripMenuItem.Enabled = true;
                Text = "Bombast Editor | " + m_projectName; 
            }
            else
            {
                closeProjectToolStripMenuItem.Enabled = false;
                Text = "Bombast Editor";
            }
        }

        private void newProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewProjectDialog newProjectDialog = new NewProjectDialog();
            newProjectDialog.ProjectCreated = OpenProject;
            newProjectDialog.ShowDialog();
        }

        private void openProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openProjectDialog = new OpenFileDialog();
            openProjectDialog.Filter = "Bombast Project File|*.bproject";
            openProjectDialog.ShowDialog();

            if(!string.IsNullOrEmpty(openProjectDialog.FileName))
            {
                OpenProject(openProjectDialog.FileName);
            }
        }

        private void closeProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_projectDirectory = "";
            m_assetsDirectory = "";
            m_projectName = "";
            m_projectLoaded = false;

            m_activeLevelPath = "";

            UpdateFormComponents();
            InitializeAssetTree();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Shutdown();
        }

        private void openLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.InitialDirectory = m_assetsDirectory;
            dialog.Filter = "Bombast Level|*.bmap";
            dialog.FilterIndex = 1;
            dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                OpenLevel(dialog.FileName);
            }
        }
    }
}
