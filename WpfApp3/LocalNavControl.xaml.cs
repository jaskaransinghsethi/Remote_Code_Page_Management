///////////////////////////////////////////////////////////////////////////////////////////
// LocalNavControl.xaml.cs : Defines all the functionalitis of WPF Local Navigation      //
// ver 1.3																		         //
//																				         //
// Application            : CSE-687 Project 4  					                         //
// Platform               : Visual Studio 2017 Community Edition                         //
//                          Windows 10 Professional 64-bit, Acer Aspire R5-571TG         //
// Author                 : Jaskaran Singh, EECS Department, Syracuse University         //
//                          315-925-9941, jsingh09@syr.edu							     //
///////////////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines all the functionalities of local navigation view. It display local
 * dirs and files on the GUI. It enables client to upload directory on to the server and
 * then publish it.
 * 
 * Required File:
 * --------------
 * MainWindow.xaml, MainWindow.xaml.cs                  //WPF Main Window
 * SelectionWindow.xaml, SelectionWindow.xaml           //WPF Selection Window
 * Translater.h, Translater.cpp                         //CLR Translator
 * CsMessage.h                                          //Cs Messages
 * LocalNavControl.xaml, LocalNavControl.xaml.cs        //Local Navigation Control
 * DisplayControl.xaml, DisplayControl.xaml.cs          //Display Control
 * 
 * Maintenance History:
 * --------------------
 * -April 30th, 2019
 *  Added Communication channel functionality to the WPF
 * 
 * -April 8th, 2019
 *  Implemented threading to improve performance issues
 *  
 * -April 5th, 2019
 *  first release
 */
using MsgPassingCommunication;
using System;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace WpfApp3
{
    /// <summary>
    /// Interaction logic for LocalNavControl.xaml
    /// </summary>
    public partial class LocalNavControl : UserControl
    {
        //String to store options
        internal string options { get; set; }

        //----< Default Constructor >---------------
        public LocalNavControl()
        {
            InitializeComponent();
        }

        //----< load file and folder into the GUI >---------------
        internal void LoadNavTab(string path)
        {
            try
            {
                Dirs.Items.Clear();
                CurrPath.Text = path;
                string[] dirs = Directory.GetDirectories(path);
                Dirs.Items.Add("..");
                foreach (string dir in dirs)
                {
                    DirectoryInfo di = new DirectoryInfo(dir);
                    if (di.Name != "Storage")
                        Dirs.Items.Add(di.Name);
                }
                string[] files = Directory.GetFiles(path);
                foreach (string file in files)
                {
                    string name = System.IO.Path.GetFileName(file);
                    Dirs.Items.Add(name);
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine("\n Exception caught:" + ex.Message);
            }
        }

        //----< handle mouse double click on files or folders in the Navigation view >---------------
        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            var temp = System.IO.Path.Combine(win.path, Dirs.SelectedItem.ToString());

            //detect whether its a directory or file
            if (Directory.Exists(temp))
            {
                string selectedDir = Dirs.SelectedItem.ToString();
                if (selectedDir == "..")
                    win.path = getAncestorPath(1, win.path);
                else
                    win.path = System.IO.Path.Combine(win.path, selectedDir);
                LoadNavTab(win.path);
            }
            else if (File.Exists(temp))
            {
                string fileName = Dirs.SelectedValue as string;
                try
                {
                    string fpath = System.IO.Path.Combine(win.path, fileName);
                    string contents = File.ReadAllText(fpath);
                    SelectionWindow popup = new SelectionWindow();
                    popup.codeView.Text = contents;
                    popup.Show();
                }
                catch (Exception ex)
                {
                    string msg = ex.Message;
                    SelectionWindow popup = new SelectionWindow();
                    popup.codeView.Text = msg;
                    popup.Show();
                }
            }
        }

        //----< A function which is invoked when we change regex and press enter >---------------
        private void TxtRegexs_KeyDown(object sender, KeyEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            if (e.Key == Key.Enter)
            {
                if (txtRegexs.Text == "")
                {
                    win.staturBar.Text = "Regex Empty";
                    string message = "Please input valid Regex";
                    string title = "Regex Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        win.tabC.SelectedIndex = 1;
                    }
                    return;
                }
                win.regex = txtRegexs.Text;
                win.staturBar.Text = "Ready";
            }
        }

        //----< A function which is invoked when we change patterns and press enter >---------------
        private void TxtPatterns_KeyDown(object sender, KeyEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            if (e.Key == Key.Enter)
            {
                if (txtPatterns.Text == "")
                {
                    win.staturBar.Text = "Patterns Empty";
                    string message = "Please input valid Pattern";
                    string title = "Patterns Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        win.tabC.SelectedIndex = 0;
                    }
                    return;
                }
                win.patterns = txtPatterns.Text;
                win.staturBar.Text = "Ready";
            }
        }

        //----< A function to get ancestor of all folder at a given path >---------------
        internal string getAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }

        //----< /A function which is invoked when we uncheck subdir checkbox >---------------
        private void Subdirs_unChecked(object sender, RoutedEventArgs e)
        {

            if (subdirs.IsChecked.Equals(true))
                options = "/s";
            else
                options = "";
        }

        //----< A function which is invoked when we check subdir checkbox >---------------
        private void Subdirs_Checked(object sender, RoutedEventArgs e)
        {

            if (subdirs.IsChecked.Equals(true))
                options = "/s";
            else
                options = "";
        }

        //----< A function to handle browse button in navigation tab >---------------
        private void Button_btnbrowse(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            var FD = new System.Windows.Forms.FolderBrowserDialog();
            if (FD.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                win.path = FD.SelectedPath;
                LoadNavTab(win.path);
            }
        }

        //----< upload selected folder on to the server directory >---------------
        async internal void Upload_Click(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            win.staturBar.Text = "Uploading";
            await Task.Delay(500);
            Thread worker = new Thread(() =>
            {
                var path = win.path;
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
                msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
                msg.add("command", "saveFiles");
                msg.add("regex", win.regex);
                msg.add("patterns", win.patterns);
                msg.add("options", options);
                msg.add("path", win.path);
                win.translater.postMessage(msg);
            });
            worker.Start();
        }

        //----< publish the uploaded folder on to the server >---------------
        async internal void Publish_Click(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            win.staturBar.Text = "Converting";
            await Task.Delay(500);
            Thread worker = new Thread(() =>
            {
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
                msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
                msg.add("command", "codePublish");
                msg.add("regex", win.regex);
                msg.add("patterns", win.patterns);
                msg.add("options", options);
                msg.add("path", win.path);
                win.translater.postMessage(msg);
            });
            worker.Start();
        }
    }
}
