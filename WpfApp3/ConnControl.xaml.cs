//////////////////////////////////////////////////////////////////////////////////////
// ConnControl.xaml.cs    : Defines all the functionalitis of WPF Connection Tab    //
// ver 1.0																		    //
//																				    //
// Application            : CSE-687 Project 4  					                    //
// Platform               : Visual Studio 2017 Community Edition                    //
//                          Windows 10 Professional 64-bit, Acer Aspire R5-571TG    //
// Author                 : Jaskaran Singh, EECS Department, Syracuse University    //
//                          315-925-9941, jsingh09@syr.edu							//
//////////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines all the functionalities of connection tab in the GUI. It enables
 * client to connect to the given server by providing server address and path.
 * 
 * Required File:
 * --------------
 * MainWindow.xaml, MainWindow.xaml.cs                  //WPF Main Window
 * SelectionWindow.xaml, SelectionWindow.xaml           //WPF Selection Window
 * Translater.h, Translater.cpp                         //CLR Translator
 * CsMessage.h                                          //Cs Messages
 * DisplayControl.xaml, DisplayControl.xaml.cs          //Display Control
 * 
 * Maintenance History:
 * --------------------
 *  
 * -April 29th, 2019
 *  first release
 */
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace WpfApp3
{
    /// <summary>
    /// Interaction logic for ConnControl.xaml
    /// </summary>
    public partial class ConnControl : UserControl
    {
        //----< Default Constructor >---------------
        public ConnControl()
        {
            InitializeComponent();
        }

        //----< creating a connection between a client and the server >---------------
        internal void Connect_Click(object sender, RoutedEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);
            win.staturBar.Text = "Connected";
            win.tabC.SelectedIndex = 1;
            win.startComm();
        }

        //----< change server address whenever client changes it on the GUI >---------------
        private void ServAddress_KeyDown(object sender, KeyEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            if (e.Key == Key.Enter)
            {
                if (servAddress.Text == "")
                {
                    win.staturBar.Text = "Invalid Address";
                    string message = "Please input valid Address";
                    string title = "Address Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        win.tabC.SelectedIndex = 0;
                    }
                    return;
                }
                win.serverName = servAddress.Text;
                win.staturBar.Text = "Not Connected";
                win.serverChanged();
            }
        }

        //----< change server port whenever client changes it on the GUI >---------------
        private void ServPort_KeyDown(object sender, KeyEventArgs e)
        {
            MainWindow win = (MainWindow)Window.GetWindow(this);

            if (e.Key == Key.Enter)
            {
                if (servPort.Text == "")
                {
                    win.staturBar.Text = "Invalid Port";
                    string message = "Please input valid Port";
                    string title = "Address Null Exception";
                    System.Windows.Forms.MessageBoxButtons buttons = System.Windows.Forms.MessageBoxButtons.OK;
                    System.Windows.Forms.DialogResult result = System.Windows.Forms.MessageBox.Show(message, title, buttons);
                    if (result == System.Windows.Forms.DialogResult.OK)
                    {
                        win.tabC.SelectedIndex = 0;
                    }
                    return;
                }
                win.serverPort = Convert.ToInt32(servPort.Text);
                win.staturBar.Text = "Not Connected";
                win.serverChanged();
            }
        }
    }
}
