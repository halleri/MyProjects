Code source from www.linuxhowtos.org/C_C++socket.htm. Beej's Guide also helped, also. (https://beej.us/guide/bgnet/output/html/multipage/index.html

Please have two PuTTY windows open. To do this, open one window, right click on the upper portion of the application and then click on duplicate session.

Open another session.

Compile server and make by typing 'make' in the command prompt.

To start server, type chatserve and then enter a valid port number 
example would be server 3000

To start client, go to the other window and type 
chatclient [hostname] [port]

For the hostname, I used either flip1.engr.oregonstate.edu or flip3.engr.oregonstate.edu

It may be different for you. You must type a port number or else you will receive a core dump.

When the client connects to the server, enter your handle. You can now type messages between server and client. To exit program, type \quit in either server or client window. You must type this when there is a prompt to type something.
