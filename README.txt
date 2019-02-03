Joanthan Chang
CS 3013 C02
Project 2

Part 1
This part contains an module that intercepts the open, close, and
read syscalls. The open and close syscalls will now print out
information to the syslog. The read syscall will now scan the 
recieved buffer for the string "VIRUS" and will printout a 
warning if it finds one.

This program can be tested by runnind the commands:
sudo insmod av.ko
./testere
sudo rmmod av.ko
And then checking the syslog.

The testere program will open, close, and read two txt files, one 
of which will trigger the anti-virus.

Part 2
This part contains a module that will lookup the ancestors, siblings,
and chidlren of a given process id. This information will then
be printed out to the syslog.

This program can be tested by running the commands:
sudo insmod procAncestry.ko
./testere
./testere 1
./testere 1 2 14
sudo rmmod procAncestry.ko
And then reading the outpu

The testere program will by default run with its own pid but will
can also run a given list of pids. This program will also output the information
to the terminal to avoid the need to check the syslog. (The information
is still printed to the syslog).