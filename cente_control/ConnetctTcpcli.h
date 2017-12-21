#ifndef CONNETCTTCPCLI__H
#define CONNETCTTCPCLI__H

#include <WINSOCK2.H>
#include <STDIO.H>

#pragma  comment(lib,"ws2_32.lib")

SOCKET connectTcpCli();
SOCKET connectTcpSer();

#endif