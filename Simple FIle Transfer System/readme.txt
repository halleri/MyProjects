Note:The sources I mainly used were
http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html
https://docs.python.org/2/howto/sockets.html

Other sources are cited in each program

First open up two separate terminals

To compile and run ftserver:
run the makefile by typing 'make' into one terminal
make sure you are in the correct directory
once ftserver is compiled, typed ftserver port number
for example, ftserver 30021

To run ftclient:

type python ftclient.py hostname port -l request port
(this will display the files in the directory)
for example python ftclient.py flip1 (assume this is the hostname of the server, if it is something else, then use that host name) 30021 (assume this is the port you opened on the server) -l 30020 

or

python ftclient.py hostname port -g filename request port
(this will transfer a file from one port to a request port)

for example python ftclient.py flip1 30021 -g text.txt 30020

Note: Server and client are mutliprocess. Server uses fork() and client uses Process()


