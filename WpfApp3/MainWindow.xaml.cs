//////////////////////////////////////////////////////////////////////////////////
// WPF GUI        : Defines all the functionalitis of WPF GUI                   //
// ver 1.0																		//
//																				//
// Application    : CSE-687 Project 3					                        //
// Platform       : Visual Studio 2017 Community Edition                        //
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG        //
// Author         : Jaskaran Singh, EECS Department, Syracuse University        //
//                  315-925-9941, jsingh09@syr.edu							    //
//////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines all the functionalities of WPF GUI. It has two views
 * 1. Navigation View
 * 2. Display View
 * Navigation view supports navigation of files and folders recursively. It also
 * supports Regex & file patterns.
 * Display view supports displaying all converted valid html file in a browser
 * or using a selection windows pop box
 * 
 * Maintenance History:
 * --------------------
 * -April 5th, 2019
 *  first release
 */

using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;
using System.IO;
using System.Diagnostics;
using System.Threading;

namespace WpfApp3
{

    /// <summary>
    /// Main Window
    /// </summary>
    public partial class MainWindow : Window
    {
        Translator trans;
        public string path { get; set; }
        List<string> convertedFiles { get; set; } = new List<string>();

        public string regex { get; set; }

        public string patterns { get; set; }

        public string options { get; set; }

        //Main Window constructor
        public MainWindow()
        {
            InitializeComponent();
        }

        //A function to load files when window is loaded
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Thread worker;
            worker = new Thread(()=> {
                Dispatcher.Invoke(() => {
                    path = Directory.GetCurrentDirectory();
                    path = getAncestorPath(3, path);
                    regex = txtRegexs.Text;
                    patterns = txtPatterns.Text;
                    if (subdirs.IsChecked.Equals(true))
                        options = "/s";
                    else
                        options = "";
                    LoadNavTab(path);
                });
            });
            worker.Start();
        }

        //A function to get ancestor of all folder at a given path
        private string getAncestorPath(int n, string path)
        {
            for (int i = 0; i < n; ++i)
                path = Directory.GetParent(path).FullName;
            return path;
        }
        
        //A function to load file and folder into the listbox
        private void LoadNavTab(string path)
        {
            Dirs.Items.Clear();
            CurrPath.Text = path;
            string[] dirs = Directory.GetDirectories(path);
            Dirs.Items.Add("..");
            foreach (string dir in dirs)
            {
                DirectoryInfo di = new DirectoryInfo(dir);
                string name = System.IO.Path.GetDirectoryName(dir);
                Dirs.Items.Add(di.Name);
            }
            string[] files = Directory.GetFiles(path);
            foreach (string file in files)
            {
                string name = System.IO.Path.GetFileName(file);
                Dirs.Items.Add(name);
            }
        }

        //A function to display all the converted html files into Display view
        private void displayCFiles()
        {
            convertedF.Items.Clear();
            foreach (string file in convertedFiles)
            {
                string name = System.IO.Path.GetFileName(file);
                convertedF.Items.Add(name);
            }
        }

        //A function to handle mouse double click on files or folders in the Navigation view
        private void Dirs_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {

            var temp = System.IO.Path.Combine(path, Dirs.SelectedItem.ToString());

            //detect whether its a directory or file
            if (Directory.Exists(temp))
            {
                string selectedDir = Dirs.SelectedItem.ToString();
                if (selectedDir == "..")
                    path = getAncestorPath(1, path);
                else
                    path = System.IO.Path.Combine(path, selectedDir);
                LoadNavTab(path);
            }
            else if(File.Exists(temp))
            {
                string fileName = Dirs.SelectedValue as string;
                try
                {
                    string fpath = System.IO.Path.Combine(path,fileName);
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

        //A function to handle open all converted files in the Display view
        private void DisplayF_Click(object sender, RoutedEventArgs e)
        {
            if (browse.IsSelected)
            {
                foreach (string file in convertedFiles)
                {
                    staturBar.Text = "Busy";
                    try
                    {
                        var process = new Process();
                        process.StartInfo.FileName = "firefox";
                        process.StartInfo.Arguments = file;
                        process.Start();
                        process.WaitForExit();
                    }
                    catch(Exception ex)
                    {
                        string msg = ex.Message;
                        SelectionWindow popup = new SelectionWindow();
                        popup.codeView.Text = msg;
                        popup.Show();
                    }
                }
                staturBar.Text = "Ready";
            }
            else
            {
                foreach (string file in convertedFiles) {
                    try
                    {
                        string contents = File.ReadAllText(file);
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
        }

        //A function to handle publish button in Navigation view
        private void Button_Click(object sender, RoutedEventArgs e)
        {
            staturBar.Text = "Converting";
            Thread worker;
            worker = new Thread(() =>
            {
                Dispatcher.Invoke(() =>
                {
                    trans = new Translator();
                    List<String> args = new List<string>();
                    String pPath = System.IO.Path.GetFullPath("../../MainWindow.xaml.cs");
                    args.Add(pPath);
                    args.Add(path);
                    args.Add(options);
                    List<String> temp = new List<string>();
                    args.AddRange(patterns.Split(' '));
                    args.Add(regex);
                    convertedFiles = trans.startProject(args);
                    displayCFiles();
                    MessageBox.Show("All files have been published");
                    tabC.SelectedIndex = 1;
                    staturBar.Text = "Ready";
                });
            });
            worker.Start();
        }

        //A function which is invoked when we change regex and press enter
        private void TxtRegexs_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {
                regex = txtRegexs.Text;
            }
        }
        
        //A function which is invoked when we change patterns and press enter
        private void TxtPatterns_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                patterns = txtPatterns.Text;
            }
        }
        
        //A function which is invoked when we uncheck subdir checkbox
        private void Subdirs_unChecked(object sender, RoutedEventArgs e)
        {
            if (subdirs.IsChecked.Equals(true))
                options = "/s";
            else
                options = "";
        }

        //A function which is invoked when we check subdir checkbox
        private void Subdirs_Checked(object sender, RoutedEventArgs e)
        {
            if (subdirs.IsChecked.Equals(true))
                options = "/s";
            else
                options = "";
        }

        //A function to handle mouse double click on file in Display tab
        private void ConvertedF_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            if (browse.IsSelected)
            {
                try
                {
                    string file = convertedF.SelectedItem as string;
                    string path = System.IO.Path.Combine("../../../convertedPages", file);
                    path = System.IO.Path.GetFullPath(path);
                    var process = new Process();
                    process.StartInfo.FileName = "firefox";
                    process.StartInfo.Arguments = path;
                    process.Start();
                    process.WaitForExit();
                }
                catch (Exception ex)
                {
                    string msg = ex.Message;
                    SelectionWindow popup = new SelectionWindow();
                    popup.codeView.Text = msg;
                    popup.Show();
                }
            }
            else
                openSourceCode(this,e);
        }

        // A function to open converted html files in the  selection window
        private void openSourceCode(object sender, MouseButtonEventArgs e)
        {
            try
            {
                string file = convertedF.SelectedItem as string;
                string fpath = System.IO.Path.Combine("../../../convertedPages", file);
                fpath = System.IO.Path.GetFullPath(fpath);
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

        //A function to handle browse button in navigation tab
        private void Button_btnbrowse(object sender, RoutedEventArgs e)
        {
            var FD = new System.Windows.Forms.FolderBrowserDialog();
            if (FD.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                path = FD.SelectedPath;
                LoadNavTab(path);
            }
        }

        //A function to handle exit in button file menu in navigation tab
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown(99);
        }

        //A function to right click mouse button in file menu in display tab
        private void sourceButton(object sender, RoutedEventArgs e)
        {
            try
            {
                string file = convertedF.SelectedItem as string;
                string fpath = System.IO.Path.Combine("../../../convertedPages", file);
                fpath = System.IO.Path.GetFullPath(fpath);
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
        
        //A function to right click mouse button in file menu in display tab
        private void brwserButton(object sender, RoutedEventArgs e)
        {
            try
            {
                string file = convertedF.SelectedItem as string;
                string path = System.IO.Path.Combine("../../../convertedPages", file);
                path = System.IO.Path.GetFullPath(path);
                var process = new Process();
                process.StartInfo.FileName = "firefox";
                process.StartInfo.Arguments = path;
                process.Start();
                process.WaitForExit();
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
                SelectionWindow popup = new SelectionWindow();
                popup.codeView.Text = msg;
                popup.Show();
            }
        }

        //A function to clean up threads after application is closed
        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            Environment.Exit(99);
        }
    }
}
