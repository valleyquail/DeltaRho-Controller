//
// Created by nikesh on 7/3/23.
//

#ifndef DELTARHO_CONTROLLER_LWIPOPTS_H
#define DELTARHO_CONTROLLER_LWIPOPTS_H

// Generally you would define your own explicit list of lwIP options
// (see https://www.nongnu.org/lwip/2_1_x/group__lwip__opts.html)
//
// This example uses a common include to avoid repetition
#include "Robot_Config.h"
#include "lwipopts_examples_common.h"

#if !NO_SYS
#define TCPIP_THREAD_STACKSIZE 2048
#define DEFAULT_THREAD_STACKSIZE 1024
#define DEFAULT_RAW_RECVMBOX_SIZE 8
#define TCPIP_MBOX_SIZE 8
#define LWIP_TIMEVAL_PRIVATE 0

// not necessary, can be done either way
#define LWIP_TCPIP_CORE_LOCKING_INPUT 1
#endif

#define LWIP_TIMEVAL_PRIVATE 0

#ifdef SOCKET_CONNECTION

#define LWIP_SOCKET 1
#define TCP_LISTEN_BACKLOG 1
#define LWIP_SO_RCVBUF 1
#define RECV_BUFSIZE_DEFAULT 256

#define DEFAULT_TCP_RECVMBOX_SIZE 128
#define DEFAULT_UDP_RECVMBOX_SIZE TCPIP_MBOX_SIZE
#define DEFAULT_ACCEPTMBOX_SIZE TCPIP_MBOX_SIZE

#define LWIP_SO_RCVTIMEO 1

#endif

#ifdef MQTT_CONNECTION
#define MEMP_NUM_SYS_TIMEOUT (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 1)
#endif
// allow override in some examples
#ifndef LWIP_SOCKET
#define LWIP_SOCKET 0
#endif
#endif // DELTARHO_CONTROLLER_LWIPOPTS_H
