//////////////////////////////////////////////////////////////////////////////////
// DisplayControl.xaml.cs    : Defines all the functionalitis of WPF GUI        //
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
 * This package defines all the functionalities of Display View in the GUI. It display
 * all the converted files on the GUI. It has two modes. Open in browser or Open in Selection
 * Window. It enables user to view converted HTML files
 * 
 * Required File:
 * --------------
 * MainWindow.xaml, MainWindow.xaml.cs                  //WPF Main Window
 * SelectionWindow.xaml, SelectionWindow.xaml           //WPF Selection Window
 * Translater.h, Translater.cpp                         //CLR Translator
 * CsMessage.h                                          //Cs Messages
 * LocalNavControl.xaml, LocalNavControl.xaml.cs        //Local Navigation Control
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
using System;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace WpfApp3
{
    /// <summary>
    /// Interaction logic for DisplayControl.xaml
    /// </summary>
    public partial class DisplayControl : UserControl
    {
        //String to store path for converted html files
        const string convPath = "../Storage/convertedPages";

        //Default constructor
        public DisplayControl()
        {
            InitializeComponent();
        }

        //A function to display all the converted html files into Display view
        internal void displayCFiles()
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            convertedF.Items.Clear();
            foreach (string file in win.convertedFiles)
            {
                string name = System.IO.Path.GetFileName(file);
                convertedF.Items.Add(name);
            }
        }

        //A function to handle open all converted files button in the Display view
        internal void DisplayF_Click(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            try
            {
                Thread worker;
                worker = new Thread(() =>
                {
                    openConvFiles(win);
                    try
                    {
                        Dispatcher.Invoke(() =>
                        {
                            win.staturBar.Text = "Busy";
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
        private void openConvFiles(MainWindow win)
        {
            foreach (string file in win.convertedFiles)
            {
                string fileName = System.IO.Path.GetFileName(file);
                CsMessage msg = new CsMessage();
                msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
                msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
                msg.add("command", "sendFile");
                string fpath = System.IO.Path.Combine(convPath, file);
                msg.add("path", convPath);
                msg.add("fileName", fileName);
                msg.add("convertedFile", "1");
                win.translater.postMessage(msg);
            }
        }

        //A function to handle mouse double click on file in Display tab
        private void ConvertedF_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            string fileName = (string)convertedF.SelectedItem;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
            msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
            msg.add("command", "sendFile");
            string fpath = System.IO.Path.Combine(convPath, fileName);
            msg.add("path", convPath);
            msg.add("fileName", fileName);
            msg.add("convertedFile", "1");
            win.translater.postMessage(msg);
        }

        //A function to right click mouse button in file menu in display tab
        private void brwserButton(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            string fileName = (string)convertedF.SelectedItem;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
            msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
            msg.add("command", "sendFile");
            string fpath = System.IO.Path.Combine(convPath, fileName);
            msg.add("path", convPath);
            msg.add("fileName", fileName);
            msg.add("convertedFile", "1");
            win.translater.postMessage(msg);
        }

        //A function to right click mouse button in file menu in display tab
        private void sourceButton(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            string fileName = (string)convertedF.SelectedItem;
            CsMessage msg = new CsMessage();
            msg.add("to", CsEndPoint.toString(win.serverEndPoint_));
            msg.add("from", CsEndPoint.toString(win.clientEndPoint_));
            msg.add("command", "sendFile");
            string fpath = System.IO.Path.Combine(convPath, fileName);
            msg.add("path", convPath);
            msg.add("fileName", fileName);
            msg.add("convertedFile", "2");
            win.translater.postMessage(msg);
        }

    }

}
