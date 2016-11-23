using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Xml;

namespace BombastEditor
{
    public partial class BombastEditorForm : Form
    { 
        private const int INVALID_ACTOR_ID = 0;

        private string m_projectDirectory;
        private string m_projectName;
        private string m_assetsDirectory;

        private bool m_projectLoaded;
        private string m_activeLevelResourceName; 

        private List<XmlNode> m_actorsXmlNodes = new List<XmlNode>();

        private MessageHandler m_messageHandler;

        public BombastEditorForm()
        {
            InitializeComponent();

            try
            {
                UpdateFormComponents();

                m_messageHandler = new MessageHandler(EditorViewportPanel, this);
            }
            catch(Exception ex)
            {
                MessageBox.Show("Error: " + ex.ToString());
            }

            var defaultProject = Properties.Settings.Default.DefaultProjectPath;
            if(!string.IsNullOrEmpty(defaultProject))
            {
                //OpenProject(defaultProject);
            }

            if(Properties.Settings.Default.RecentlyOpenedProjects == null)
            {
                Properties.Settings.Default.RecentlyOpenedProjects = new List<string>();
            }

            UpdateRecentProjectMenuItem();
        }

        private void BombastEditorForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            Properties.Settings.Default.Save();
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

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Shutdown();
        }

        #region Project Management
        internal void OpenProject(string projectFilePath)
        {
            var projectFileInfo = new FileInfo(projectFilePath);
            m_projectDirectory = projectFileInfo.Directory.FullName;
            m_projectName = projectFileInfo.Name;
            m_assetsDirectory = Path.Combine(m_projectDirectory, GlobalSettings.AssetsFolderName);

            Properties.Settings.Default.DefaultProjectPath = projectFilePath;
            AddRecentlyOpenedProjectItem(projectFilePath);

            IntPtr hInstance = Marshal.GetHINSTANCE(GetType().Module);
            IntPtr hWnd = EditorViewportPanel.Handle;
            NativeMethods.InitializeBombastProject(hInstance, IntPtr.Zero, hWnd, 1, EditorViewportPanel.Width, EditorViewportPanel.Height, m_projectDirectory);
            
            m_projectLoaded = true;
            InitializeAssetTree();
            UpdateFormComponents();
        }

        private void UpdateFormComponents()
        {
            if (m_projectLoaded)
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

            if (!string.IsNullOrEmpty(openProjectDialog.FileName))
            {
                OpenProject(openProjectDialog.FileName);
            }
        }

        private void closeProjectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NativeMethods.Shutdown();

            m_projectDirectory = "";
            m_assetsDirectory = "";
            m_projectName = "";
            m_projectLoaded = false;

            m_activeLevelResourceName = "";

            Properties.Settings.Default.DefaultProjectPath = "";

            UpdateFormComponents();
            InitializeAssetTree();
            InitializeActors();
        }
        #endregion

        #region Level Management
        internal void OpenLevel(string levelResourceName)
        {
            m_activeLevelResourceName = levelResourceName;
            NativeMethods.OpenLevel(m_activeLevelResourceName);
            InitializeActors();
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

        private void newLevelToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }
        #endregion

        #region Assets Management
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
                        var resourceInfo = BombastResourceHelper.GetBombastResourceFromFilepath(file.FullName, m_assetsDirectory);
                        childNode.Tag = resourceInfo;
                        currentNode.Nodes.Add(childNode);
                    }
                }
            }

            AssetsTreeView.Nodes.Add(node);
        }

        private void AssetsTreeView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            TreeNode node = AssetsTreeView.SelectedNode;
            if (node != null && node.Nodes.Count == 0)
            {
                var resource = (BombastResource)node.Tag;
                OpenResource(resource);
            }
        }
        #endregion

        #region Actor Management
        private void InitializeActors()
        {
            ActorsTreeView.Nodes.Clear();

            //If no project loaded just abort
            if (!m_projectLoaded)
            {
                return;
            }

            int[] actorsList = GetActorList();

            m_actorsXmlNodes.Add(null);

            for(int i=0; i<actorsList.GetLength(0); i++)
            {
                uint actorId = Convert.ToUInt32(actorsList[i]);

                TreeNode node = new TreeNode();

                XmlElement actorXml = null; // GetActorXml(actorId);

                if(actorXml != null)
                {
                    node.Name = actorsList[i].ToString();
                    m_actorsXmlNodes.Add(actorXml);
                    node.Text = actorXml.GetAttribute("type");
                }
                else
                {
                    node.Text = "<unknown actor - no xml found>";
                }
                ActorsTreeView.Nodes.Add(node);
            }
        }

        private XmlElement GetActorXml(uint actorId)
        {
            int xmlSize = NativeMethods.GetActorXmlSize(actorId);
            if (xmlSize == 0)
                return null;

            IntPtr tempArray = Marshal.AllocCoTaskMem((xmlSize + 1) * sizeof(char));
            NativeMethods.GetActorXml(tempArray, actorId);
            string actorXml = Marshal.PtrToStringAnsi(tempArray);
            Console.WriteLine(actorXml);
            Marshal.FreeCoTaskMem(tempArray);

            XmlDocument actorDoc = new XmlDocument();
            actorDoc.Load(new StringReader(actorXml));
            return actorDoc.DocumentElement;
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
        #endregion

        #region Resource Management
        private void OpenResource(BombastResource resource)
        {
            var resourceType = resource.ResourceType;

            switch (resourceType)
            {
                case BombastResourceType.PROJECT:
                    OpenProject(resource.FullFilepath);
                    break;
                case BombastResourceType.LEVEL:
                    OpenLevel(resource.ResourceName);
                    break;
                default:
                    Process.Start(resource.FullFilepath);
                    break;
            }
        }
        #endregion

        #region Recently Opened Projects
        private void AddRecentlyOpenedProjectItem(string projectPath)
        {
            var index = Properties.Settings.Default.RecentlyOpenedProjects.IndexOf(projectPath);

            //If does not contain, add to front. Otherwise move to front if not already there
            if (index == -1)
            {
                Properties.Settings.Default.RecentlyOpenedProjects.Add(projectPath);
            }
            else if(index != Properties.Settings.Default.RecentlyOpenedProjects.Count - 1)
            {
                Properties.Settings.Default.RecentlyOpenedProjects.RemoveAt(index);
                Properties.Settings.Default.RecentlyOpenedProjects.Add(projectPath);
            }

            UpdateRecentProjectMenuItem();
        }

        private void UpdateRecentProjectMenuItem()
        {
            recentProjectsMenuItem.DropDownItems.Clear();
            var recentProjects = Properties.Settings.Default.RecentlyOpenedProjects;
            if (recentProjects != null && recentProjects.Count > 0)
            {
                recentProjects.Reverse();
                foreach (var recentProject in recentProjects)
                {
                    ToolStripMenuItem item = new ToolStripMenuItem(recentProject);
                    item.Click += new EventHandler(this.recentProjectMenuItem_Click);
                    recentProjectsMenuItem.DropDownItems.Add(item);
                }
            }
            else
            {
                recentProjectsMenuItem.Enabled = false;
            }
        }


        private void recentProjectMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = (ToolStripMenuItem)sender;

            var resourceFilePath = item.Text;
            var resourceInfo = BombastResourceHelper.GetBombastResourceFromFilepath(resourceFilePath);

            OpenResource(resourceInfo);
        }
#endregion
    }
}
