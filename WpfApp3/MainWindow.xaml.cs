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
 * -April 8th, 2019
 *  Implemented threading to improve performance issues
 *  
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
using System.Threading.Tasks;

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
        async private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            Thread worker;
            worker = new Thread(() =>
            {
                path = Directory.GetCurrentDirectory();
                path = getAncestorPath(3, path);
                Dispatcher.Invoke(() =>
                {
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
            await Task.Delay(1000);
            ATU();
        }

        //A function to demonstrate all the requirements are fullfilled
        private void ATU()
        {
            Thread thread = new Thread(() =>
            {
                Console.WriteLine("=======================================Demonstrating Requirements=======================================");
                Console.WriteLine("Req1: This project uses Microsoft Visual Studio 15.9 2017 Enterprise Endition");
                Console.WriteLine("      This project is based on C++ static library, C++ CLR/CLI DLLs & WPF Console Application");
                Console.WriteLine("Req2: This is project used WPF GUI and C# to implement all the functionalities of Client");
                Console.WriteLine("Req3: Interface - IPublisher is defined for Publisher that declares methods to access all the publisher" +
                    " facilities and an Object Factory which returns a pointer to Publisher of IPublisher type");
                Console.WriteLine("      Path to IPublisher.h:  " + System.IO.Path.GetFullPath("../../../Publisher/IPublisher.h"));
                Console.WriteLine("      Path to Publisher.h & Object Factory:  " + System.IO.Path.GetFullPath("../../../Publisher/Publisher.h"));
                Console.WriteLine("Req4: Publisher and IPublisher are built as DLLs, IPublisher exposes the Publisher methods to the C++ CLR/CLI translator");
                Console.WriteLine("Req5: The C++ CLR/CLI translator is created to support interaction of managed code with native code");
                Console.WriteLine("      Translator is built as Dynamic link library to support this interaction");
                Console.WriteLine("      Path to Translator.h:  " + System.IO.Path.GetFullPath("../../../Translator/Translator.h"));
                Console.WriteLine("Req6: The WPF GUI is implemented with two views. Navigation view for selection of path, giving pattern and regex");
                Console.WriteLine("      Display view shows all the converted files. It supports two mode. Open files in browser or Open files in new window");
                Console.WriteLine("Req7: This is Automated Test Suite");
                Console.WriteLine("=======================================Demonstrating Requirements=======================================");
                try
                {
                    Dispatcher.Invoke(() =>
                    {
                        Button_Click(this, null);
                    });
                }
                catch(SystemException ex)
                {
                    Console.WriteLine(ex.Message);
                }
            });

            thread.Start();
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
            else if (File.Exists(temp))
            {
                string fileName = Dirs.SelectedValue as string;
                try
                {
                    string fpath = System.IO.Path.Combine(path, fileName);
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
            bool isSelected = browse.IsSelected;
            try
            {
                Thread worker;
                worker = new Thread(() =>
                {
                    if (isSelected)
                    {
                        openInBrowser();
                    }
                    else
                    {
                        Dispatcher.Invoke(() =>
                        {
                            openInSelW();
                        });
                    }
                    try
                    {
                        Dispatcher.Invoke(() =>
                        {
                            staturBar.Text = "Ready";
                        });
                    }
                    catch (SystemException ex)
                    {
                        Console.WriteLine(ex.Message);
                    }
                });
                worker.Start();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        //A function to open converted file in the browser
        private void openInBrowser()
        {
            foreach (string file in convertedFiles)
            {
                try
                {
                    Dispatcher.Invoke(() =>
                    {
                        staturBar.Text = "Busy - Open Converted Files in Firefox";
                    });
                    var process = new Process();
                    process.StartInfo.FileName = "firefox";
                    process.StartInfo.Arguments = file;
                    process.Start();
                    process.WaitForExit();
                }
                catch (SystemException ex)
                {
                    string msg = ex.Message;
                    try
                    {
                        Dispatcher.Invoke(() =>
                        {
                            SelectionWindow popup = new SelectionWindow();
                            popup.codeView.Text = msg;
                            popup.Show();
                        });
                    }
                    catch (Exception s)
                    {
                        Console.WriteLine(s.Message);
                    }
                }
            }
        }

        //A function to open converted file in selection window
        private void openInSelW()
        {
            foreach (string file in convertedFiles)
            {
                try
                {
                    SelectionWindow popup = new SelectionWindow();
                    staturBar.Text = "Busy - Open Converted Files in Selection Window";
                    string contents = File.ReadAllText(file);
                    popup.codeView.Text = contents;
                    popup.ShowDialog();
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

        //A function to handle publish button in Navigation view
        async private void Button_Click(object sender, RoutedEventArgs e)
        {
            staturBar.Text = "Converting";
            await Task.Delay(500);
            Thread worker;
            worker = new Thread(() =>
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
                Dispatcher.Invoke(async () =>
                {
                    displayCFiles();
                    if (e != null)
                    {
                        string message = "All files have been published successfully";
                        string title = "Conversion Complete";
                        System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                        System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                        if (result == System.Windows.Forms.DialogResult.OK)
                        {
                            tabC.SelectedIndex = 1;
                        }
                        staturBar.Text = "Ready";
                    }
                    else
                    {
                        await Task.Delay(1000);
                        staturBar.Text = "All files have been published successfully";
                        await Task.Delay(1000);
                        tabC.SelectedIndex = 1;
                        await Task.Delay(1000);
                        DisplayF_Click(this, null);
                    }
                });
            });
            worker.Start();
        }

        //A function which is invoked when we change regex and press enter
        private void TxtRegexs_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                if (txtRegexs.Text == "")
                {
                    staturBar.Text = "Regex Empty";
                    string message = "Please input valid Regex";
                    string title = "Regex Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        tabC.SelectedIndex = 0;
                    }
                    return;
                }
                regex = txtRegexs.Text;
                staturBar.Text = "Ready";
            }
        }
        
        //A function which is invoked when we change patterns and press enter
        private void TxtPatterns_KeyDown(object sender, KeyEventArgs e)
        {
            if(e.Key == Key.Enter)
            {
                if (txtRegexs.Text == "")
                {
                    staturBar.Text = "Patterns Empty";
                    string message = "Please input valid Pattern";
                    string title = "Patterns Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        tabC.SelectedIndex = 0;
                    }
                    return;
                }
                patterns = txtPatterns.Text;
                staturBar.Text = "Ready";
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
                    Thread worker = new Thread(() =>
                    {
                        Dispatcher.Invoke(() =>
                        {
                            string file = convertedF.SelectedItem as string;
                            string path = System.IO.Path.Combine("../../../convertedPages", file);
                            path = System.IO.Path.GetFullPath(path);
                            var process = new Process();
                            process.StartInfo.FileName = "firefox";
                            process.StartInfo.Arguments = path;
                            process.Start();
                        });
                    });
                    worker.Start();
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
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
                Thread worker = new Thread(() =>
                {
                    Dispatcher.Invoke(() =>
                    {
                        string file = convertedF.SelectedItem as string;
                        string path = System.IO.Path.Combine("../../../convertedPages", file);
                        path = System.IO.Path.GetFullPath(path);
                        var process = new Process();
                        process.StartInfo.FileName = "firefox";
                        process.StartInfo.Arguments = path;
                        process.Start();
                    });
                });
                worker.Start();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        //A function to clean up threads after application is closed
        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            Environment.Exit(99);
        }
    }
}
