# http://stackoverflow.com/questions/82831/check-if-a-file-exists-using-python
import os.path
# https://docs.python.org/3/library/multiprocessing.html
from multiprocessing import Process

#https://docs.python.org/2/library/socket.html
import socket
import sys
import errno
import signal
import time

MAX_CHARS = 1024

# https://docs.python.org/3/library/multiprocessing.html
def start(host, server_port, data_port, filename):
    if filename == 0:
        p = Process(target=get_dir, args=(data_port,))
    else:
        p = Process(target=get_file, args=(data_port, filename,))
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((host, server_port))
    except socket.error as msg:
        s.close()
        sys.exit()
    p.start()
    time.sleep(.3) 
# if there is no filename then we get the directory    
    if filename == 0:
	#sends the port number and directory request back to the server
        s.sendall("port " + str(data_port) + " dir");
    else:
	#sends the port number and file request back to the server
        s.sendall("port " + str(data_port) + " file " + filename);

    s.close()
    p.join()


# https://docs.python.org/2/library/socket.html

def get_dir(server_port):
    host = ''     
    serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serv.bind((host, server_port))
    serv.settimeout(3)
    try:
        serv.listen(1)
        conn, addr = serv.accept()
	addr = socket.getfqdn(addr[0]);
    except socket.timeout as msg:
        print "ERROR: Server timed out"
        sys.exit()
    while 1:
        data = conn.recv(MAX_CHARS)
        if not data: break
        print data
    conn.close()

# https://docs.python.org/2/library/socket.html
def get_file(server_port, filename):
    host = '';                 
    serv = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serv.bind((host, server_port))
    serv.settimeout(3)
    
    try:
        serv.listen(1)
        conn, addr = serv.accept()	
	addr = socket.getfqdn(addr[0])
    except socket.error as msg:
        print "ERROR: Server timed out"
        sys.exit()

    while 1:
	data = conn.recv(MAX_CHARS)
	if not data:break
	conn.send(data)
	print "%s:%s says %s" %(addr, server_port, data)

    if os.path.isfile(filename):
        f = open(filename, 'w')
	print 'Receiving \"%s\" from %s:%s' %(filename, addr, server_port) 
	print 'File transfer complete.'
	conn.close()
    
#signal handler for sigint
#http://stackoverflow.com/questions/1112343/how-do-i-capture-sigint-in-python
def signal_handler(signal, frame):
    sys.exit(0)

#https://docs.python.org/2/library/sys.html
#www.pythonforbeginners.com/argv/more-fun-with-sys-argv
def main():
    signal.signal(signal.SIGINT, signal_handler)
    if (len(sys.argv) >= 5):
        host = sys.argv[1]
        server_port = sys.argv[2]
        command = sys.argv[3]
        data_port = sys.argv[4]
        if (len(sys.argv) > 5):
	    host = sys.argv[1]
	    server_port = sys.argv[2]
	    command = sys.argv[3]
            filename = sys.argv[4]
            data_port = sys.argv[5]
        
        if command == "-l":
	    print "Receiving directory structure from %s:%s" %(host, data_port)
            start(host, int(server_port), int(data_port), 0)
        if command == "-g":
            start(host, int(server_port), int(data_port), filename)
    else:
	print "You must enter the proper format. Please see below" 
        print "python ftclient.py <host> <server_port> -l <data port>"
        print "python ftclient.py <host> <server_port> -g <file> <data port>"


if __name__ == "__main__":
    main()

