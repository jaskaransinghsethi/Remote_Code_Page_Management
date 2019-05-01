///////////////////////////////////////////////////////////////////////////////////////////
// RemoteNavControl.xaml.cs    : Defines all the functionalitis of WPF Connection Tab    //
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
 * This package defines all the functionalities of remote navigation view. It display remote
 * dirs and files on the GUI. It enables client to publish the given directory on the server.
 * 
 * Required File:
 * --------------
 * MainWindow.xaml, MainWindow.xaml.cs                  //WPF Main Window
 * SelectionWindow.xaml, SelectionWindow.xaml           //WPF Selection Window
 * Translater.h, Translater.cpp                         //CLR Translator
 * CsMessage.h                                          //Cs Messages
 * RemoteNavControl.xaml, RemoteNavControl.xaml.cs      //Remote Navigation Control
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
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace WpfApp3
{
    /// <summary>
    /// Interaction logic for RemoteNavControl.xaml
    /// </summary>
    public partial class RemoteNavControl : UserControl
    {
        #region Control Variables

        //String to store options
        internal string remoteOptions { get; set; }

        //String to store patterns
        internal string remotePatterns { get; set; }

        //String to store regex
        internal string remoteRegex { get; set; }

        #endregion

        //----< Default Constructor >---------------
        public RemoteNavControl()
        {
            InitializeComponent();
        }

        //----< setting up the value of remote options >---------------
        private void Remote_subdirs_Unchecked(object sender, RoutedEventArgs e)
        {
            remoteOptions = "";
        }

        //----< setting up the value of remote options >---------------
        private void Remote_subdirs_Checked(object sender, RoutedEventArgs e)
        {
            remoteOptions = "/s";
        }

        //----< setting up the value of remote patterns >---------------
        private void Remote_txtPatterns_KeyDown(object sender, KeyEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            if (e.Key == Key.Enter)
            {
                if (Remote_txtPatterns.Text == "")
                {
                    win.staturBar.Text = "Patterns Empty";
                    string message = "Please input valid Pattern";
                    string title = "Patterns Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        win.tabC.SelectedIndex = 2;
                    }
                    return;
                }
                remotePatterns = Remote_txtPatterns.Text;
                win.staturBar.Text = "Ready";
            }
        }

        //----< setting up the value of remote regex >---------------
        private void Remote_txtRegexs_KeyDown(object sender, KeyEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            if (e.Key == Key.Enter)
            {
                if (Remote_txtRegexs.Text == "")
                {
                    win.staturBar.Text = "Regex Empty";
                    string message = "Please input valid Regex";
                    string title = "Regex Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        win.tabC.SelectedIndex = 2;
                    }
                    return;
                }
                remoteRegex = Remote_txtRegexs.Text;
                win.staturBar.Text = "Ready";
            }
        }

        //----< sending a cs message to publish the code on the comm >---------------
        async internal void Remote_Publish_Click(object sender, RoutedEventArgs e)
        {
            remoteRegex = Remote_txtRegexs.Text;
            remotePatterns = Remote_txtPatterns.Text;
            MainWindow win = (MainWindow)Window.GetWindow(this);
            win.staturBar.Text = "Converting";
            await Task.Delay(500);
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
            msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
            msg.add("command", "codePublish");
            msg.add("regex", remoteRegex);
            msg.add("patterns", remotePatterns);
            msg.add("options", remoteOptions);
            msg.add("path", Remote_CurrPath.Text);
            win.translater.postMessage(msg);
        }

        //----< adding directories to the GUI >---------------
        internal void addDir(string dir)
        {
            Remote_Dirs.Items.Add(dir);
        }

        //----< adding files to the GUI >---------------
        internal void addFiles(string dir)
        {
            Remote_Files.Items.Add(dir);
        }

        //----< deleting all directories from the GUI >---------------v
        internal void clearDirs()
        {
            Remote_Dirs.Items.Clear();
        }

        //----< deleting all files from the GUI >---------------
        internal void clearFiles()
        {
           Remote_Files.Items.Clear();
        }

        //----< adding a dummy dir to traverse to the parent directory >---------------
        internal void insertParent()
        {
            Remote_Dirs.Items.Insert(0, "..");
        }

        //----< sending message on to the comm to retrieve all the files and directories >---------------
        private void Refresh_Click(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            CsEndPoint serverEndPoint = new CsEndPoint();
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
            msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
            msg.add("command", "getDirs");
            msg.add("path", win.pathStack_.Peek());
            win.translater.postMessage(msg);
            msg.remove("command");
            msg.add("command", "getFiles");
            win.translater.postMessage(msg);
        }

        //----< refresh all the files and directories on the GUI >---------------
        internal void refreshDisplay()
        {
            Refresh_Click(this, null);
        }

        //----< show file text on MouseDoubleClick >-----------------------
        private void Remote_Files_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            string fileName = (string)Remote_Files.SelectedItem;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
            msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
            msg.add("command", "sendFile");
            msg.add("path", win.pathStack_.Peek());
            msg.add("fileName", fileName);
            win.translater.postMessage(msg);
        }

        //----< update current directory on MouseDoubleClick >-----------------------
        private void Remote_Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            // build path for selected dir
            win.selectedDir = (string)Remote_Dirs.SelectedItem;
            string path = win.pathStack_.Peek() + "/" + win.selectedDir;
            if (win.selectedDir == "..")
            {
                if (win.pathStack_.Count > 1)  // don't pop off "Storage"
                    win.pathStack_.Pop();
                else
                    return;
            }
            else
            {
                win.pathStack_.Push(path);
            }

            // display path in Dir TextBlock
            Remote_CurrPath.Text = win.removeFirstDir(win.pathStack_.Peek());

            // build message to get dirs and post it
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
            msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
            msg.add("command", "getDirs");
            msg.add("path", win.pathStack_.Peek());
            win.translater.postMessage(msg);

            // build message to get files and post it
            msg.remove("command");
            msg.add("command", "getFiles");
            win.translater.postMessage(msg);
        }
    }
}
