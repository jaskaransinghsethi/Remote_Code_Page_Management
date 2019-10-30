Remote Code Page Management allows users to upload cpp code files and convert them into HTML files for better hosting and code reviews
since they are more readable than code files.
It follows Client-Server Architecture

Client is made using WPF C#. It has 3 modes:-
Local Navigation - Local Directory
Remote Navigation - User's directory on the server
Display - Files which got converted

Server is made using C++. It has file system, converter, parser, dependency analyzer.

Between the server and client, there is an interface which allows communication between managed code and unmanaged code. It is created using
C++ CLR.

For better performance and handling multiple concurrent clients, I used multi-threaded apartment model.
