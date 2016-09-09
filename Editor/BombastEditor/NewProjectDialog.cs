using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BombastEditor
{
    public partial class NewProjectDialog : Form
    {
        public delegate void ProjectCreatedEvent(string projectFilePath);
        public ProjectCreatedEvent ProjectCreated;

        public NewProjectDialog()
        {
            InitializeComponent();
        }

        private void NewProjectDialog_Load(object sender, EventArgs e)
        {
            var defaultFolderPath = "";
            var userDocumentsFolder = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
            if(!string.IsNullOrEmpty(userDocumentsFolder))
            {
                defaultFolderPath = Path.Combine(userDocumentsFolder, GlobalSettings.DefaultProjectsFolderName);
            }
            else
            {
                //If no documents folder detected just default to drive root.
                defaultFolderPath = Path.GetPathRoot(Environment.CurrentDirectory) + GlobalSettings.DefaultProjectsFolderName;
            }

            folderTextInput.Text = defaultFolderPath;

            nameTextInput.Text = GlobalSettings.DefaultProjectName;
        }

        private void browseFolderButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            DialogResult res = fbd.ShowDialog();
            if (!string.IsNullOrEmpty(fbd.SelectedPath))
            {
                folderTextInput.Text = fbd.SelectedPath;
            }
        }

        private void createProjectButton_Click(object sender, EventArgs e)
        {
            var path = folderTextInput.Text;
            var name = nameTextInput.Text;

            if (string.IsNullOrEmpty(path))
            {
                MessageBox.Show("You must specify a path!", "Create Project", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (string.IsNullOrEmpty(name))
            {
                MessageBox.Show("You must specify a name!", "Create Project", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            var fullProjectPath = Path.Combine(path, name);

            if (Directory.Exists(fullProjectPath))
            {
                MessageBox.Show("Project cannot be saved into a folder with existing files. Please choose a different directory/project name!", "Create Project", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if(BombastHelper.CreateProject(fullProjectPath, name))
            {
                var projectFilePath = Path.Combine(fullProjectPath, name + ".bproject");
                ProjectCreated(projectFilePath);
                Close();
            }
        }
    }
}
