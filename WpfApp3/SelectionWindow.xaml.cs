//////////////////////////////////////////////////////////////////////////////////////////////////
// WPF Selection Window        : Opens header files, cpp files & HTML files in a new window     //
// ver 1.0																		                //
//																				                //
// Application                 : CSE-687 Project 3					                            //
// Platform                    : Visual Studio 2017 Community Edition                           //
//                               Windows 10 Professional 64-bit, Acer Aspire R5-571TG           //
// Author                      : Jaskaran Singh, EECS Department, Syracuse University           //
//                               315-925-9941, jsingh09@syr.edu							        //
//////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package defines a new selection window. Selection window is used to open files in a new window
 * using Main Window.
 * 
 * Maintenance History:
 * --------------------
 * -April 5th, 2019
 *  first release
 */

using System.Windows;

namespace WpfApp3
{
    /// <summary>
    /// Interaction logic for SelectionWindow.xaml
    /// </summary>
    public partial class SelectionWindow : Window
    {
        //Default Constructor
        public SelectionWindow()
        {
            InitializeComponent();
        }
    }
}
