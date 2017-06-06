import os
import sys
import ssl

import socket
import struct


def deal_with_client(connstream):
    data = connstream.read(8) #size of a name
    sz = struct.unpack("<Q", data)
    filename_packed = connstream.read(sz)
    filename = str(filename_packed)
    full_filename = os.path.join(sys.argv[1]+"\\", filename)
    fsize = os.path.getsize(full_filename)
    connstream.write(struct.pack("<Q", fsize))
    f = open(full_filename, "rb")
    data = f.read()
    connstream.write(data)
 

def Usage():
    print "%s\t: Usage %s data_folder port cert_folder" % (args[0], args[0])

def main(args):
    if len(args) < 4:
        Usage(args);
        sys.exit();
    

    bindsocket = socket.socket()
    bindsocket.bind(('', int (args[2], 10)))
    bindsocket.listen(5)


    while True:
        newsocket, fromaddr = bindsocket.accept()
        connstream = ssl.wrap_socket(newsocket,
                                     server_side=True,
                                     certfile=os.path.join(argv[3], "domain.crt"),
                                     keyfile=os.path.join(argv[3], "domain.key"))
        try:
            deal_with_client(connstream)
        finally:
            connstream.shutdown(socket.SHUT_RDWR)
            connstream.close()    


if __name__ == "__main__":
    main(sys.argv)