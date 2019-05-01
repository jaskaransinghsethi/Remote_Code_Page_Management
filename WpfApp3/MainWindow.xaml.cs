//////////////////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs    : Defines all the functionalitis of WPF GUI            //
// ver 1.3																		//
//																				//
// Application    : CSE-687 Project 4					                        //
// Platform       : Visual Studio 2017 Community Edition                        //
//                  Windows 10 Professional 64-bit, Acer Aspire R5-571TG        //
// Author         : Jaskaran Singh, EECS Department, Syracuse University        //
//                  315-925-9941, jsingh09@syr.edu							    //
//////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines all the functionalities of WPF GUI. It has 4 views
 * 1. Connection Tab            - Connect to the server with server address and port
 * 2. Navigation View           - Display all the files and folders on the client
 * 3. Remote View               - Display all the files and folders on the server
 * 4. Display View              - Display a list of converted files
 * 
 * Required File:
 * --------------
 * MainWindow.xaml, MainWindow.xaml.cs                  //WPF Main Window
 * SelectionWindow.xaml, SelectionWindow.xaml           //WPF Selection Window
 * Translater.h, Translater.cpp                         //CLR Translator
 * CsMessage.h                                          //Cs Messages
 * ConnControl.xaml, ConnControl.xaml.cs                //Connection Control
 * LocalNavControl.xaml, LocalNavControl.xaml.cs        //Local Navigation Control
 * RemoteNavControl.xaml, RemoteNavControl.xaml.cs      //Remote Navigation Control
 * DisplayControl.xaml, DisplayControl.xaml.cs          //Display Control
 * 
 * Maintenance History:
 * --------------------
 * -April 30th, 2019
 *  Added Communication channel functionality to the WPF
 * 
 * -April 25th, 2019
 *  Added seperate ConnControl, LocalNavControl, RemoteNavControl, DisplayControl
 * 
 * -April 8th, 2019
 *  Implemented threading to improve performance issues
 *  
 * -April 5th, 2019
 *  first release
 */

using System;
using System.Collections.Generic;
using System.Windows;
using System.IO;
using System.Diagnostics;
using System.Threading;
using MsgPassingCommunication;
using System.Threading.Tasks;

namespace WpfApp3
{
    /// <summary>
    /// Main Window Class
    /// </summary>
    public partial class MainWindow : Window
    {
        //----< Main Window constructor >----------------
        public MainWindow()
        {
            InitializeComponent();
            Console.Title = "Project 4 GUI Console";
        }

        #region  Main Window Variables

        internal string serverName;
        internal int serverPort;
        internal string clientName;
        internal int clientPort;

        //Endpoints for the server
        internal CsEndPoint serverEndPoint_;

        //Object of C++ CLI Translator
        internal Translater translater;

        //Endpoints for the client
        internal CsEndPoint clientEndPoint_;

        //Reciever thread on the client
        private Thread rcvThrd = null;

        //Message processing dispatcher
        private Dictionary<string, Action<CsMessage>> dispatcher_
            = new Dictionary<string, Action<CsMessage>>();

        internal Stack<string> pathStack_ = new Stack<string>();

        //Path for saving files
        internal string saveFilePath;

        //Path from where files are sent to the server
        internal string sendFilePath;
        internal string selectedDir = "";

        //String to store path
        internal string path { get; set; }

        //List of String to store converted files
        internal List<string> convertedFiles { get; set; } = new List<string>();

        //String to store regex
        internal string regex { get; set; }

        //String to store patterns
        internal string patterns { get; set; }

        //Type of mode in which application is going to run
        internal string mode { get; set; }
        #endregion

        //----< Window loaded function >---------------
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            //setting up server and client uri
            setUp();

            if (mode == "DEMO")
                ATU();
        }

        //----< process incoming messages on child thread >----------------
        internal void processMessages()
        {
            ThreadStart thrdProc = () => {
                while (true)
                {
                    CsMessage msg = translater.getMessage();
                    try
                    {
                        string msgId = msg.value("command");
                        Console.Write("\n  client getting message \"{0}\"", msgId);
                        if (dispatcher_.ContainsKey(msgId))
                            dispatcher_[msgId].Invoke(msg);
                    }
                    catch (Exception)
                    {
                        //Console.Write("\n  {0}", ex.Message);
                        //msg.show();
                    }
                }
            };
            rcvThrd = new Thread(thrdProc);
            rcvThrd.IsBackground = true;
            rcvThrd.Start();
        }

        //----< add client processing for message with key >---------------
        private void addClientProc(string key, Action<CsMessage> clientProc)
        {
            dispatcher_[key] = clientProc;
        }

        //----< Starting up comm after client is connected to the server >---------------
        internal void startComm()
        {
            NavRemote.Remote_Dirs.Items.Clear();
            NavRemote.Remote_Files.Items.Clear();

            translater = new Translater();
            translater.listen(clientEndPoint_);
            processMessages();

            //load dispatcher
            loadDispatcher();

            pathStack_.Push("../Storage");

            NavRemote.Remote_CurrPath.Text = "Storage";
            pathStack_.Push("../Storage");

            saveFilePath = translater.setSaveFilePath("../../../SaveFiles");
            sendFilePath = translater.setSendFilePath("../../../SendFiles");

            NavRemote.refreshDisplay();

            loadFiles();
        }

        //----< Setting up server & client address and port >---------------
        private void setUp()
        {
            try
            {
                //Get commandline arguments
                string[] args = Environment.GetCommandLineArgs();
                serverName = args[1];
                serverPort = Convert.ToInt32(args[2]);
                clientName = args[3];
                clientPort = Convert.ToInt32(args[4]);
                mode = args[5];

                ConnCont.servAddress.Text = serverName;
                ConnCont.servPort.Text = serverPort.ToString();

                //Set client address
                clientEndPoint_ = new CsEndPoint();
                clientEndPoint_.machineAddress = clientName;
                clientEndPoint_.port = clientPort;

                //Set server address
                serverEndPoint_ = new CsEndPoint();
                serverEndPoint_.machineAddress = serverName;
                serverEndPoint_.port = serverPort;
            }
            catch (Exception)
            {
                Console.WriteLine("\n Invalid Commandline Arguments");
                Console.WriteLine("\n Commandline:- [serverAddress] [serverPort] [clientAddress] [clientPort] [demo/application] [path] [regex]");
            }
        }

        //----< Setting up server address and port if user changes it >---------------
        internal void serverChanged()
        {
            serverEndPoint_.machineAddress = serverName;
            serverEndPoint_.port = serverPort;
        }

        //----< helper function to load dispatcher on the client >---------------
        private void loadDispatcher()
        {
            DispatcherLoadGetDirs();
            DispatcherLoadGetFiles();
            DispatcherLoadSendFile();
            DispatcherSaveFiles();
            DispatchercodePublish();
        }

        //----< strip off name of first part of path >---------------------
        public string removeFirstDir(string path)
        {
            string modifiedPath = path;
            int pos = path.IndexOf("/");
            modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
            return modifiedPath;
        }

        //----< load codePublish processing into dispatcher dictionary >------
        private void DispatchercodePublish()
        {
            Action<CsMessage> codePublish = (CsMessage rcvMsg) =>
            {
                Console.Write("\n  processing publish request's reply");
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("Output"))
                    {
                        if (mode != "DEMO")
                        {
                            string title = "Result:";
                            System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                            System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(enumer.Current.Value, title, buttons);
                        }
                        Dispatcher.Invoke(() => {
                            staturBar.Text = "Ready";
                        });
                    }

                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            DispView.convertedF.Items.Add(System.IO.Path.GetFileName(file));
                            convertedFiles.Add(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
                Dispatcher.Invoke(() => {
                    tabC.SelectedIndex = 3;
                });
                
            };
            addClientProc("codePublish", codePublish);
        }

        //----< load getDirs processing into dispatcher dictionary >------
        private void DispatcherLoadGetDirs()
        {
            Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
            {
                Action clrDirs = () =>
                {
                    NavRemote.clearDirs();
                };
                Dispatcher.Invoke(clrDirs, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("dir"))
                    {
                        Action<string> doDir = (string dir) =>
                        {
                            NavRemote.addDir(dir);
                        };
                        Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
                    }
                }
                Action insertUp = () =>
                {
                    NavRemote.insertParent();
                };
                Dispatcher.Invoke(insertUp, new Object[] { });
            };
            addClientProc("getDirs", getDirs);
        }

        //----< load getFiles processing into dispatcher dictionary >------
        private void DispatcherLoadGetFiles()
        {
            Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
            {
                Action clrFiles = () =>
                {
                    NavRemote.clearFiles();
                };
                Dispatcher.Invoke(clrFiles, new Object[] { });
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("file"))
                    {
                        Action<string> doFile = (string file) =>
                        {
                            NavRemote.addFiles(file);
                        };
                        Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
                    }
                }
            };
            addClientProc("getFiles", getFiles);
        }

        //----< load getFiles processing into dispatcher dictionary >------
        private void DispatcherLoadSendFile()
        {
            Action<CsMessage> sendFile = (CsMessage rcvMsg) =>
            {
                Console.Write("\n  processing incoming file");
                string fileName = "";
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("sendingFile"))
                    {
                        fileName = enumer.Current.Value;
                        break;
                    }
                }
                if (fileName.Length > 0)
                {
                    Action<string> act = (string fileNm) => {
                        if (rcvMsg.attributes.ContainsKey("convertedFile"))
                            if (rcvMsg.attributes["convertedFile"] == "1")
                                showConvFile(fileNm);
                            else
                                showFile(fileNm);
                        else
                            showFile(fileNm);
                    };
                    Dispatcher.Invoke(act, new object[] { fileName });
                }
            };
            addClientProc("sendFile", sendFile);
        }

        //----< load converted HTML file either in browser or in the selection window >------
        private void showConvFile(string fileName)
        {
            try
            {
                if (DispView.browse.IsSelected)
                {
                    string filePath = saveFilePath + "\\" + fileName;
                    filePath = System.IO.Path.GetFullPath(filePath);
                    Thread workerThread = new Thread( () => {
                        var process = new Process();
                        process.StartInfo.FileName = "firefox";
                        process.StartInfo.Arguments = filePath;
                        process.Start();
                        process.WaitForExit();
                        Dispatcher.Invoke(() => staturBar.Text = "Ready");
                    });
                    workerThread.Start();
                }
                else
                {
                    showFile(fileName);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("\n Exception caught:" + ex.Message);
            }
        }

        //----< load saveFiles processing into dispatcher dictionary >------
        private void DispatcherSaveFiles()
        {
            Action<CsMessage> saveFiles = (CsMessage rcvMsg) =>
            {
                Console.Write("\n  processing save file request's reply");
                var enumer = rcvMsg.attributes.GetEnumerator();
                while (enumer.MoveNext())
                {
                    string key = enumer.Current.Key;
                    if (key.Contains("Output"))
                    {
                        if (mode != "DEMO")
                        {
                            string title = "Result:";
                            System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                            System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(enumer.Current.Value, title, buttons);
                        }
                        Dispatcher.Invoke(() => { staturBar.Text = "Ready"; });
                        return;
                    }
                }
            };
            addClientProc("saveFiles", saveFiles);
        }

        //----< load file text in the selection window >------
        private void showFile(string fileName)
        {
            string fpath = saveFilePath + "\\" + fileName;
            fpath = System.IO.Path.GetFullPath(fpath);
            string contents = File.ReadAllText(fpath);
            SelectionWindow popup = new SelectionWindow();
            popup.codeView.Text = contents;
            popup.Show();
        }

        //----< load files into GUI when window is loaded  >------
        private void loadFiles()
        {
            Thread worker;
            worker = new Thread(() =>
            {
                try
                {
                    path = Directory.GetCurrentDirectory();
                    path = NavLocal.getAncestorPath(3, path);
                    Dispatcher.Invoke(() =>
                    {
                        regex = NavLocal.txtRegexs.Text;
                        patterns = NavLocal.txtPatterns.Text;
                        if (NavLocal.subdirs.IsChecked.Equals(true))
                            NavLocal.options = "/s";
                        else
                            NavLocal.options = "";

                        NavLocal.LoadNavTab(path);
                    });
                }
                catch(Exception ex)
                {
                    Console.WriteLine("Exception caught: " + ex.Message);
                }
            });
            worker.Start();
            //ATU();
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
                Console.WriteLine("      Publisher and IPublisher are built as DLLs, IPublisher exposes the Publisher methods to the C++ CLR/CLI translator");
                Console.WriteLine("Req4: This project provides Graphical User Interface (GUI) for the client that supports navigating remote directories to find a project for conversion, " +
                                        "and supports displaying the conversion results in a way that meets Project #3 requirements.");
                Console.WriteLine("Req5: This project provide message designs appropriate for this application. All messages are instances of the same Message class, but have a specified set of attributes " +
                                            "and body contents suited for the intended task.");
                Console.WriteLine("      Translator is built as Dynamic link library to support this interaction");
                Console.WriteLine("      Path to Translator.h:  " + System.IO.Path.GetFullPath("../../../Translator/Translator.h"));
                Console.WriteLine("Req6: This project support converting source code in the server and, with a separate request, " +
                                         "transferring one or more converted files back to the local client, using the communication channel.");
                Console.WriteLine("      Display view shows all the converted files. It supports two mode. Open files in browser or Open files in new window");
                Console.WriteLine("Req7: This project demonstrate correct operations for two concurrent clients.");
                Console.WriteLine("Req8: This is Automated Test Suite");
                Console.WriteLine("=======================================Demonstrating Requirements=======================================");
                Dispatcher.Invoke(async () =>
                {
                    Console.WriteLine("\n Client running on: " + clientName + ":" + clientPort);
                    string[] args = Environment.GetCommandLineArgs();
                    ConnCont.Connect_Click(null, null);
                    try
                    {
                        path = System.IO.Path.GetFullPath(args[6]);
                        regex = args[7];
                        NavLocal.txtRegexs.Text = regex;
                        NavLocal.CurrPath.Text = path;
                        Console.WriteLine("\n Path given to be published:" + path);
                        Console.WriteLine("\n Regex given:" + regex);
                    }
                    catch (Exception)
                    {
                        Console.WriteLine("\n Exception caught in ATU");
                    }
                    await Task.Delay(1000);
                    NavLocal.Upload_Click(null, null);
                    await Task.Delay(2000);
                    NavLocal.Publish_Click(null, null);
                });
            });
            thread.Start();
        }
        
        
        //----< clean up all threads after application is closed  >------
        private void Window_Unloaded(object sender, RoutedEventArgs e)
        {
            Environment.Exit(99);
        }

        //----< A function to handle browse button in navigation tab  >------
        private void Button_btnbrowse(object sender, RoutedEventArgs e)
        {
            var FD = new System.Windows.Forms.FolderBrowserDialog();
            if (FD.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                path = FD.SelectedPath;
                NavLocal.LoadNavTab(path);
            }
        }

        //----< A function to handle exit in button file menu in navigation tab  >------
        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown(99);
        }

    }
}
