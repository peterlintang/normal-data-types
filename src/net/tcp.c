


/*//////// MODULE CODE //////////////////////////////////////////*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tcp.h"
#include "route.h"
#include "resolve.h"
#include "socket.h"

#define TRUE	1
#define FALSE	0

#define T_S Socket_T
#define T Tcp_T

/*//////// API ROUTINES //////////////////////////////////////////*/

T MODULE_FUN_NAME(Tcp, connect2)(const char *host, u_int16_t port, const char *ifname)
{
	T tcp = (T)calloc(1,sizeof(*tcp));
	if (tcp) {
        /*
         * below is a HACK for now to preserve previous interface API
         */
        fprintf (stdout,"connect2 resolving %s\n",host);
        in_addr_t ip = MODULE_FUN_NAME(Resolve, name2ip)(host);
        if (ip != INADDR_NONE) {
            tcp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_STREAM);

            if(tcp->sock && ifname) {

                struct sockaddr_in addr = { .sin_addr.s_addr = MODULE_FUN_NAME(Route, getIfIP)((char *)ifname), .sin_family = AF_INET };
                
                if(!MODULE_FUN_NAME(Socket, bind)(tcp->sock, (struct sockaddr*)&addr)) {

                    fprintf(stderr, "_connect2 failed to bind to '%s'\n", ifname);

                }

            }

        } else {
            tcp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_UNIX);
        }
        tcp->records = FALSE;
        if (tcp->sock) {
            char *ipstring = MODULE_FUN_NAME(Resolve, ip2string)(ip); /* optimization hack */
            fprintf (stdout, "connect2 setting up socket for %s:%d\n",ipstring,port);
            if (MODULE_FUN_NAME(Socket, setAddress)(tcp->sock, ipstring, port)) {
                tcp->destHostName = strdup(host);
                tcp->destHostPort = port;
                free (ipstring); /* done using this hack */
                /*
                 * another HACK
                 */
                if (tcp->sock->type == SOCKET_STREAM) {
                    memcpy (&tcp->destHostAddr, &tcp->sock->addr, tcp->sock->len);
                }

                /*
                 * here, we use nonblocking connect method!
                 */
                int flags = fcntl (tcp->sock->skd, F_GETFL, 0);
                if (fcntl (tcp->sock->skd, F_SETFL, flags | O_NONBLOCK) < 0) {
                    fprintf (stderr, "connect2 unable to set socket into NONBLOCKING mode!\n");
                    MODULE_FUN_NAME(Tcp, destroy)(&tcp);
                    return NULL;
                }

			  try_connect:
				fprintf(stdout, "connect2 making a connection to %s:%d (%u attempt)\n",host,port,tcp->retryCounter+1);
				if (connect(tcp->sock->skd,(struct sockaddr *)&tcp->sock->addr,tcp->sock->len)==0) {

                    if (fcntl (tcp->sock->skd, F_SETFL, flags) == 0) {
                        tcp->retryCounter = 0; /* reset to 0 */
                        fprintf (stdout, "connect2 connection to %s:%d successful!\n",host,port);
                        return tcp;
                    }
                    fprintf (stderr, "connect2 unable to set socket back into BLOCKING mode!\n");
				} else {
					switch(errno) {
                      case EISCONN:
                          fprintf(stderr, "connect2 already connected?!? tear-down & reopen socket!\n");
                          MODULE_FUN_NAME(Socket, destroy)(&tcp->sock);
                          tcp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_STREAM);
                          if (!tcp->sock) {
                              fprintf (stderr, "connect2 unable to create a new socket!\n");
                              break;
                          }
                      case EAGAIN:
                      case EINTR:
//                          if(SystemExit)
 //                             break;
                          goto try_connect;

                      case EINPROGRESS:
                      case EALREADY: {
                          struct timeval tv = {
                              .tv_sec = TCP_CONNECT_TIMEOUT,
                              .tv_usec = 0
                          };
                          fd_set myset;
                          FD_ZERO (&myset);
                          FD_SET (tcp->sock->skd,&myset);
                          if (select (tcp->sock->skd+1, NULL, &myset, NULL, &tv) > 0) {
                              socklen_t lon = sizeof (int);
                              int valopt;
                              if (getsockopt (tcp->sock->skd, SOL_SOCKET, SO_ERROR, (void*) (&valopt), &lon) == 0) {
                                  if (!valopt) {

                                      if (fcntl (tcp->sock->skd, F_SETFL, flags) == 0) {
                                          tcp->retryCounter = 0; /* reset to 0 */
                                          fprintf (stdout, "connect2 connection to %s:%d successful! (via fcntl)\n",host,port);
                                          return tcp;
                                      }
                                      fprintf (stderr, "connect2 unable to set socket back into BLOCKING mode!\n");
                                  } else {
                                      fprintf (stderr, "connect2 error() %d - %s\n", valopt, strerror (valopt));
                                  }
                              } else {
                                  fprintf (stderr, "connect2 error in getsockopt() %d - %s\n", errno, strerror (errno));
                              }
                          }
                      }
                      case ETIMEDOUT:
//                          if(SystemExit)
 //                             break;

                          if (++tcp->retryCounter < TCP_CONNECT_MAX_RETRY) {
                              if (tcp->sock->type == SOCKET_STREAM) {
                                  fprintf(stdout, "connect2 re-resolving host (%s) for another connection attempt\n",host);
                                  in_addr_t ip2 = MODULE_FUN_NAME(Resolve, name2ip)(host);
                                  if (ip2 != INADDR_NONE) {
                                      if (ip2 != ip) {
                                          ip = ip2;
                                          tcp->sock->addr.in.sin_addr.s_addr = ip;
                                          goto try_connect;
                                      } else {
                                          fprintf(stderr, "connect2 unable to re-resolve host (%s) to a different IP to retry\n",host);
                                      }
                                  } else {
                                      fprintf(stderr, "connect2 unable to re-resolve host (%s)\n",host);
                                  }
                              } else {
                                  goto try_connect;
                              }
                          } else {
                              fprintf (stderr,"connect2 too many retries! giving up!\n");
                          }
                          break;

                      default:
                          fprintf (stderr,"connect2 unhandled error (%s)\n",strerror (errno));
					}
                    fprintf (stderr,"connect2 unable to make a connection to (%s)\n",host);
					MODULE_FUN_NAME(Tcp, destroy)(&tcp);
				}
			} else {
                free (ipstring); /* done using this hack */
				MODULE_FUN_NAME(Tcp, destroy)(&tcp);
			}
		} else {
			fprintf(stderr,"connect2 unable to create network socket\n");
			MODULE_FUN_NAME(Tcp, destroy)(&tcp);
		}
	}
	return (tcp);
}

T MODULE_FUN_NAME(Tcp, connect)(const char *host, u_int16_t port)
{
	T tcp = (T)calloc(1,sizeof(*tcp));
	if (tcp) {
        /*
         * below is a HACK for now to preserve previous interface API
         */
        fprintf (stdout,"connect resolving %s\n",host);
        in_addr_t ip = MODULE_FUN_NAME(Resolve, name2ip)(host);
        if (ip != INADDR_NONE) {
            tcp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_STREAM);
        } else {
            tcp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_UNIX);
        }
        tcp->records = FALSE;
        if (tcp->sock) {
            char *ipstring = MODULE_FUN_NAME(Resolve, ip2string)(ip); /* optimization hack */
            fprintf (stdout,"connect setting up socket for %s:%d\n",ipstring,port);
            if (MODULE_FUN_NAME(Socket, setAddress)(tcp->sock, ipstring, port)) {
                tcp->destHostName = strdup(host);
                tcp->destHostPort = port;
                free (ipstring); /* done using this hack */
                /*
                 * another HACK
                 */
                if (tcp->sock->type == SOCKET_STREAM) {
                    memcpy (&tcp->destHostAddr, &tcp->sock->addr, tcp->sock->len);
                }
                
                /*
                 * here, we use nonblocking connect method!
                 */
                int flags = fcntl (tcp->sock->skd, F_GETFL, 0);
                if (fcntl (tcp->sock->skd, F_SETFL, flags | O_NONBLOCK) < 0) {
                    fprintf (stderr,"connect unable to set socket into NONBLOCKING mode!\n");
                    MODULE_FUN_NAME(Tcp, destroy)(&tcp);
                    return NULL;
                }
                
			  try_connect:
				fprintf(stdout, "connect making a connection to %s:%d (%u attempt)\n",host,port,tcp->retryCounter+1);
				if (connect(tcp->sock->skd,(struct sockaddr *)&tcp->sock->addr,tcp->sock->len)==0) {

                    if (fcntl (tcp->sock->skd, F_SETFL, flags) == 0) {
                        tcp->retryCounter = 0; /* reset to 0 */
                        fprintf (stdout, "connect connection to %s:%d successful!\n",host,port);
                        return tcp;
                    }
                    fprintf (stderr,"connect unable to set socket back into BLOCKING mode!\n");
				} else {
					switch(errno) {
                      case EISCONN:
                          fprintf(stderr,"connect already connected?!? tear-down & reopen socket!\n");
                          MODULE_FUN_NAME(Socket, destroy)(&tcp->sock);
                          tcp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_STREAM);
                          if (!tcp->sock) {
                              fprintf (stderr,"connect unable to create a new socket!\n");
                              break;
                          }
                      case EAGAIN:
                      case EINTR:
//                          if(SystemExit)
//                              break;
                          goto try_connect;

                      case EINPROGRESS:
                      case EALREADY: {
                          struct timeval tv = {
                              .tv_sec = TCP_CONNECT_TIMEOUT,
                              .tv_usec = 0
                          };
                          fd_set myset;
                          FD_ZERO (&myset);
                          FD_SET (tcp->sock->skd,&myset);
                          if (select (tcp->sock->skd+1, NULL, &myset, NULL, &tv) > 0) {
                              socklen_t lon = sizeof (int);
                              int valopt;
                              if (getsockopt (tcp->sock->skd, SOL_SOCKET, SO_ERROR, (void*) (&valopt), &lon) == 0) {
                                  if (!valopt) {

                                      if (fcntl (tcp->sock->skd, F_SETFL, flags) == 0) {
                                          tcp->retryCounter = 0; /* reset to 0 */
                                          fprintf (stdout, "connect connection to %s:%d successful! (via fcntl)\n",host,port);
                                          return tcp;
                                      }
                                      fprintf (stderr,"connect unable to set socket back into BLOCKING mode!\n");
                                  } else {
                                      fprintf (stderr,"connect error() %d - %s\n", valopt, strerror (valopt));
                                  }
                              } else {
                                  fprintf (stderr,"connect error in getsockopt() %d - %s\n", errno, strerror (errno));
                              }
                          }
                      }
                      case ETIMEDOUT:
//                          if(SystemExit)
 //                             break;
                          
                          if (++tcp->retryCounter < TCP_CONNECT_MAX_RETRY) {
                              if (tcp->sock->type == SOCKET_STREAM) {
                                  fprintf (stdout, "connect re-resolving host (%s) for another connection attempt\n",host);
                                  in_addr_t ip2 = MODULE_FUN_NAME(Resolve, name2ip)(host);
                                  if (ip2 != INADDR_NONE) {
                                      if (ip2 != ip) {
                                          ip = ip2;
                                          tcp->sock->addr.in.sin_addr.s_addr = ip;
                                          goto try_connect;
                                      } else {
                                          fprintf (stderr,"connect unable to re-resolve host (%s) to a different IP to retry\n",host);
                                      }
                                  } else {
                                      fprintf (stderr,"connect unable to re-resolve host (%s)\n",host);
                                  }
                              } else {
                                  goto try_connect;
                              }
                          } else {
                              fprintf (stderr,"connect too many retries! giving up!\n");
                          }
                          break;
                          
                      default:
                          fprintf (stderr,"connect unhandled error (%s)\n",strerror (errno));
					}
                    fprintf (stderr,"connect unable to make a connection to (%s)\n",host);
					MODULE_FUN_NAME(Tcp, destroy)(&tcp);
				}
			} else {
                free(ipstring); /* done using this hack */
				MODULE_FUN_NAME(Tcp, destroy)(&tcp);
			}
		} else {
			fprintf(stderr,"connect unable to create network socket\n");
			MODULE_FUN_NAME(Tcp, destroy)(&tcp);
		}
	}
	return (tcp);
}

void MODULE_FUN_NAME(Tcp, destroy)(T *tcpPtr)
{
	if (tcpPtr) 
	{
		T tcp = *tcpPtr;
		if (tcp) 
		{
			MODULE_FUN_NAME(Socket, destroy)(&tcp->sock);
            if (tcp->srcHostName)  free(tcp->srcHostName);
            if (tcp->destHostName) free(tcp->destHostName);
			free (tcp);
			*tcpPtr = NULL;
		}
	}
}
	
T MODULE_FUN_NAME(Tcp, accept)(T_S sock)
{
	T tcp = (T)calloc(1,sizeof(*tcp));
	if (tcp) 
	{
		if (sock && sock->flags & SOCKET_LISTEN_FLAG) 
		{
			tcp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_DUMMY);
			if (tcp->sock) 
			{
				uint32_t addrLen = sizeof (struct sockaddr_in);
			  try_accept:
				fprintf(stdout, "accept waiting for incoming connection...\n");
				if ((tcp->sock->skd = accept (sock->skd,(struct sockaddr *)&tcp->srcHostAddr, &addrLen))>=0) 
				{
                    char *host = MODULE_FUN_NAME(Resolve, ip2string)(tcp->srcHostAddr.sin_addr.s_addr);
					tcp->sock->flags |= SOCKET_ACCEPT_FLAG;
					tcp->srcHostPort = ntohs (tcp->srcHostAddr.sin_port);
					tcp->retryCounter = 0; /* reset to 0 */
					fprintf (stdout, "accept accepted incoming connection from %s:%u\n", host, tcp->srcHostPort);
                    tcp->srcHostName = host;
                    tcp->records = FALSE;
				} 
				else 
				{
					switch (errno) 
					{
                      case EINTR:
                      case EAGAIN:
//                          if(SystemExit)
 //                             break;
					
                          if (tcp->retryCounter++ < TCP_ACCEPT_MAX_RETRY) 
						  {
                              fprintf (stdout, "accept trying again (%u retries)...\n",tcp->retryCounter);
                              goto try_accept;
                          } 
						  else 
						  {
                              fprintf (stderr,"accept too many retries! giving up!\n");
                          }
                          break;
                          
                      default:
                          fprintf (stderr,"accept unhandled error: %s\n",strerror (errno));
					}
					MODULE_FUN_NAME(Tcp, destroy)(&tcp);
				}
			} 
			else 
			{
				fprintf (stderr,"accept unable to create network socket\n");
                MODULE_FUN_NAME(Tcp, destroy)(&tcp);
			}
		} 
		else 
		{
			fprintf (stderr,"accept invalid socket to accept from!\n");
            MODULE_FUN_NAME(Tcp, destroy)(&tcp);
		}
	}
	return tcp;
}

T_S MODULE_FUN_NAME(Tcp, listen)(u_int16_t port)
{
	T_S sock = MODULE_FUN_NAME(Socket, new)(SOCKET_STREAM);
	if (sock) 
	{
		struct sockaddr_in addr;
		int32_t val = 10; /* option for reusing addr */
		memset(&addr, 0, sizeof(struct sockaddr_in));
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		setsockopt(sock->skd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val));
			
		if (bind(sock->skd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))>=0)
		{
			if (listen(sock->skd, SOMAXCONN) >= 0) 
			{
				sock->flags |= SOCKET_LISTEN_FLAG;
			} 
			else 
			{
				fprintf(stderr, "listen %s\n",strerror(errno));
				MODULE_FUN_NAME(Socket, destroy)(&sock);
			}
		} 
		else 
		{
			fprintf(stderr,"listen unable to bind to port %d\n",port);
			MODULE_FUN_NAME(Socket, destroy)(&sock);
		}
	}
	return sock;
}

/* not sure if this belongs here... */
int MODULE_FUN_NAME(Tcp, setTimeout)(T tcp, int32_t seconds)
{
	if (tcp && seconds >= 0) 
	{
		struct timeval tv;
		tv.tv_sec = seconds;
		tv.tv_usec = 0;
		if (tcp->sock) 
		{
			setsockopt(tcp->sock->skd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
			setsockopt(tcp->sock->skd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
			return TRUE;
		}
	}
	return FALSE;
	
}

void  MODULE_FUN_NAME(Tcp, useRecords)(T tcp, int state)
{
    if (tcp) 
	{
        tcp->records = state;
    }
}

/*
  Implement sslRead-similar interface, but data is read
  into the buffer directly with a single system call.
*/
uint32_t   MODULE_FUN_NAME(Tcp, read)(T tcp, char **buf, uint32_t size)
{
    int dynamicBuffer = FALSE;
    int32_t returnValue = 0, totalReceived = 0, recordSize = 0;
    uint32_t delay = 1000;
    
    if (tcp && buf && size) 
	{
        if (tcp->records) 
		{
            fprintf(stdout, "_tcpRead reading %u bytes with records\n", size);

            if (recv(tcp->sock->skd, &recordSize, sizeof(recordSize), 0) <= 0) 
			{
                fprintf(stderr, "_tcpRead unable to receive record size\n");
                return 0;
            }
            
            fprintf(stdout, "_tcpRead received %u record size\n", recordSize);
            recordSize = ltohel (recordSize); // was sent to us as Little Endian
            fprintf(stdout, "_tcpRead converted record size from little endian to host: %u\n", recordSize);
            
            if (recordSize > size) 
			{
                fprintf(stderr, "_tcpRead record(%d) is too large for allocated buffer (%d)\n", recordSize, size);
                return 0;
            }
        }
        
        if (!*buf) 
		{
            dynamicBuffer = TRUE;
            *buf = (char *) calloc(size+1, sizeof(char));
            if(!*buf) 
			{
                fprintf (stderr, "_tcpRead cannot allocate memory for %lu bytes!\n",(unsigned long)size+1);
                return 0;
            }
            (*buf)[size] = '\0';
        }

        totalReceived = 0;
        do 
		{
            if((returnValue = recv(tcp->sock->skd, *buf+totalReceived, size-totalReceived, 0)) < 0) 
			{
              //  if(errno == EINTR && !SystemExit) 
                if(errno == EINTR) 
				{
                    fprintf(stdout, "_tcpRead signal interrupted recv() call (%s), retrying\n", strerror(errno));
                    continue;
                }
                fprintf (stderr, "_tcpRead tcp socket read error (%s)\n", strerror(errno));
                if(dynamicBuffer) 
				{
                    free(*buf);
                    *buf = NULL;
                }
                totalReceived = 0;
                break;
            } 
			else
			{
                if(returnValue == 0) 
				{
                    fprintf(stdout, "_tcpRead peer closed the connection\n");
                    break;		
                }
			}
	    
            totalReceived += returnValue;

            if (!tcp->records) break;

            if(totalReceived < size) 
			{
                fprintf(stdout, "_tcpRead interrupted recv() (%s), retrying\n", strerror(errno));
                usleep (delay);
                delay *= 2;
            }
            
        } while(tcp->records && totalReceived < size);

        if (totalReceived && dynamicBuffer) 
		{
            /* re-size the buffer to only contain the amount of data actually read! */
            *buf = realloc (*buf, totalReceived + 1);
            (*buf)[totalReceived] = '\0'; /* safety */
        }
        
    }
    return totalReceived;
}

/*
  Plain TCP send interface. Just write the data and worry
  about nothing.
*/
uint32_t MODULE_FUN_NAME(Tcp, write)(T tcp, char  *buf, uint32_t size)
{
    int32_t returnValue = 0, totalSent = 0;
    uint32_t delay = 10000;
    
    if(tcp && tcp->sock && buf && size) 
	{
        if (tcp->records) 
		{
            uint32_t tmp_size = htolel (size); // send size as Little Endian

            fprintf(stdout, "_tcpWrite writing %u bytes with records\n", size);
            
            if(send(tcp->sock->skd,&tmp_size,sizeof(tmp_size), 0) < 0) {
                fprintf (stderr, "_tcpWrite error while sending record size\n");
                return 0;
            }
        }
        
        totalSent = 0;
	
        while(totalSent < size) 
		{
            if((returnValue = send(tcp->sock->skd, buf+totalSent, size-totalSent, 0)) < 0) 
			{
                fprintf (stderr, "_tcpWrite tcp socket write error (errno %d)\n", errno);
                totalSent = 0;
                break;
	
            } 
			else
			{
                if(returnValue == 0) 
				{
                    fprintf(stdout, "_tcpWrite peer closed the connection\n");
                    break;
                }
			}
	    
            totalSent += returnValue;
	    
            if(totalSent < size) 
			{
                fprintf(stderr, "_tcpWrite interrupted send() (errno %d), retrying\n", errno);
                usleep (delay);
                delay *= 2;
            }
        }
    }

    return totalSent;
}



