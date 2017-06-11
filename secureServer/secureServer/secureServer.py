import os
import sys
import ssl

import socket
import struct
import traceback



def deal_with_client(connstream):

    try:
        data = connstream.read(8) #size of a name
        print "length:", len(data)
        sz = struct.unpack("<Q", data)[0]
        print "Unpacked length", sz
        filename_packed = connstream.read(sz)
        filename = str(filename_packed)
        print "Downloading :", filename
        full_filename = os.path.join(sys.argv[1]+"\\", filename)
        fsize = os.path.getsize(full_filename)
        print " Of size ... ", fsize
        connstream.write(struct.pack("<Q", fsize))
        f = open(full_filename, "rb")
        data = f.read()
        connstream.write(data)
    except:
        traceback.print_exc(file=sys.stdout)
 

def Usage(args):
    print "%s\t: Usage %s data_folder port cert_folder" % (args[0], args[0])

def main(args):
    if len(args) < 4:
        Usage(args);
        sys.exit();
    

    bindsocket = socket.socket()
    bindsocket.bind(('', int (args[2], 10)))
    bindsocket.listen(5)


    while True:
         try:
            newsocket, fromaddr = bindsocket.accept()
            newsocket.settimeout(120)
            connstream = None
            try:
                connstream = ssl.wrap_socket(newsocket,
                                     server_side=True,
                                     certfile=os.path.join(args[3], "domain.crt"),
                                     keyfile=os.path.join(args[3], "domain.key"))
                connstream.settimeout(120)
                deal_with_client(connstream)
            except:
                traceback.print_exc(file=sys.stdout)
                continue
            
         except:
            traceback.print_exc(file=sys.stdout)
         finally:
            if  not connstream is None :
                connstream.shutdown(socket.SHUT_RDWR)
                connstream.close()    


if __name__ == "__main__":
    main(sys.argv)