


/*//////// MODULE CODE //////////////////////////////////////////*/

#include <sys/socket.h>
#include <arpa/inet.h>			/* for inet_ntoa */
#include <errno.h>
#include <unistd.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "module.h"
#include "socket.h"
#include "resolve.h"
#include "udp.h"

#define T_S Socket_T
#define T Udp_T 

/*//////// API ROUTINES //////////////////////////////////////////*/

T MODULE_FUN_NAME(Udp, connect)(const char *host, u_int16_t port)
{
    T udp = (T)calloc(1,sizeof(*udp));

    if (udp == NULL) 
	{
		return udp;
    }

	udp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_DGRAM);

	if (udp->sock) 
	{
	    struct hostent *hp = gethostbyname (host);
	    if (hp) 
		{
			udp->destHostAddr.sin_addr =* (struct in_addr *) (hp->h_addr_list[0]);
			udp->destHostAddr.sin_family = AF_INET;
			udp->destHostAddr.sin_port = htons (port);
		
			if(connect(udp->sock->skd, (struct sockaddr *)&udp->destHostAddr,sizeof(struct sockaddr_in)) != 0) 
			{
		    	perror("connect");
		    	fprintf(stderr, "connect unable to set remote address\n");
	    		MODULE_FUN_NAME(Udp, destroy)(&udp);
			}

	    } 
		else 
		{
			fprintf(stderr, "connect unable to resolve host (%s)\n",host);
	   		MODULE_FUN_NAME(Udp, destroy)(&udp);
	    }
	    
	} 
	else 
	{
	    fprintf(stderr,"connect unable to create network socket\n");
	    	MODULE_FUN_NAME(Udp, destroy)(&udp);
	}

    
    return (udp);

}

T MODULE_FUN_NAME(Udp, bconnect)(const char *host, u_int16_t port)
{

    T udp = (T)calloc(1,sizeof(*udp));

    if (udp == NULL)
	{
		return udp;
	}

	udp->sock = MODULE_FUN_NAME(Socket, new)(SOCKET_DGRAM);
	MODULE_FUN_NAME(Socket, setOpt)(udp->sock, BCAST);

	if (udp->sock) 
	{
	    struct hostent *hp = gethostbyname (host);
	    if (hp) 
		{
			udp->destHostAddr.sin_addr =* (struct in_addr *) (hp->h_addr_list[0]);
			udp->destHostAddr.sin_family = AF_INET;
			udp->destHostAddr.sin_port = htons (port);
		
			if(connect(udp->sock->skd, (struct sockaddr *)&udp->destHostAddr,sizeof(struct sockaddr_in)) != 0) 
			{
		    	perror("bconnect");
		    	fprintf(stderr, "bconnect unable to set remote address\n");		
	    		MODULE_FUN_NAME(Udp, destroy)(&udp);
			}
	    } 
		else 
		{
			fprintf(stderr,"bconnect unable to resolve host (%s)\n",host);
	    	MODULE_FUN_NAME(Udp, destroy)(&udp);
	    }
	} 
	else 
	{
	    fprintf(stderr,"bconnect unable to create network socket\n");
	    MODULE_FUN_NAME(Udp, destroy)(&udp);
	}

    return (udp);
}

void MODULE_FUN_NAME(Udp, destroy)(T *udpPtr)
{
    if (udpPtr) 
	{
		T udp = *udpPtr;
		if (udp) 
		{
	    	MODULE_FUN_NAME(Socket, destroy)(&udp->sock);
	    	free (udp);
	    	*udpPtr = NULL;
		}
    }
}
	

T_S MODULE_FUN_NAME(Udp, listen)(u_int16_t port)
{

    T_S sock = MODULE_FUN_NAME(Socket, new)(SOCKET_DGRAM);
    if (sock) 
	{
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
			
        if (bind(sock->skd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))>=0)
		{
        } 
		else 
		{
        //    DEBUGP(DERR,"listen","unable to bind to port %d",port);
            fprintf(stderr,"listen unable to bind to port %d\n",port);
            MODULE_FUN_NAME(Socket, destroy)(&sock);
        }
    }
    return sock;
}


T_S MODULE_FUN_NAME(Udp, listen2)(in_addr_t host, u_int16_t port)
{

    T_S sock = MODULE_FUN_NAME(Socket, new)(SOCKET_DGRAM);
    if (sock) 
	{
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(struct sockaddr_in));
        addr.sin_addr.s_addr = host;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
			
        if (bind(sock->skd,(struct sockaddr *)&addr,sizeof(struct sockaddr_in))>=0)
		{
            // all cool
        } 
		else 
		{
          //  DEBUGP(DERR,"listen","unable to bind to host %s, port %d",I (Resolve)->ip2string(host), port);
            fprintf(stderr,"listen unable to bind to host %s, port %d\n", MODULE_FUN_NAME(Resolve, ip2string)(host), port);
            MODULE_FUN_NAME(Socket, destroy)(&sock);
        }
    }
    return sock;
}


uint32_t MODULE_FUN_NAME(Udp, read)(T udp, char **buf, uint32_t size)
{

    int dynamicBuffer = 0;
    int32_t returnValue = 0, totalReceived = 0, recordSize = 0;
    uint32_t delay = 10000;
    
	if (udp == NULL || buf == NULL || size == 0)
	{
		fprintf(stderr, "arg error udp: %p, buf: %p, size: %u\n", udp, buf, size);
		return 0;
	}

	if (!*buf) 
	{
    	dynamicBuffer = 1;
	    
    	*buf = (char *) calloc(size+1, sizeof(char));
    	if (buf == NULL) 
		{
		//	DEBUGP (DERR,"_udpRead","cannot allocate memory for %lu bytes!",(unsigned long)size+1);
			fprintf(stderr,"_udpRead cannot allocate memory for %lu bytes!\n",(unsigned long)size+1);
			return 0;
    	}

    	(*buf)[size] = '\0';
	}
	
	while (totalReceived < sizeof(recordSize)) 
	{
    	if ((returnValue = recv(udp->sock->skd, ((char*)&recordSize)+totalReceived, sizeof(recordSize)-totalReceived, 0)) < 0) 
		{
//		if(errno == EINTR && !SystemExit)
			if (errno == EINTR)
	    		continue;
	
			if (dynamicBuffer)
	    		free(*buf);
			
			//	DEBUGP(DERR, "_udpRead", "unable to receive record size");
			fprintf(stderr, "_udpRead unable to receive record size\n");
			return 0;
    	} 
		else if (returnValue == 0) 
		{
	//		DEBUGP(DALL, "_udpRead", "peer closed the connection");
			fprintf(stdout, "_udpRead peer closed the connection\n");
			return 0;
    	}
    	totalReceived += returnValue;
	}
	
	if (recordSize > size) 
	{
    	if (dynamicBuffer)
			free(*buf);
	
   // DEBUGP(DERR, "_udpRead", "record(%d) is too large for allocated buffer (%d)", recordSize, size);
    	fprintf(stderr, "_udpRead record(%d) is too large for allocated buffer (%d)\n", recordSize, size);
    	return 0;
	}
	
	totalReceived = 0;
	
	while (totalReceived < size) 
	{
	    if ((returnValue = recv(udp->sock->skd, *buf+totalReceived, size-totalReceived, 0)) < 0) 
		{
//			if(errno == EINTR && !SystemExit) {
			if (errno == EINTR) 
			{
		    //	DEBUGP(DDEBUG, "_udpRead", "signal interrupted recv() call (%s), retrying", strerror(errno));
		    	fprintf(stderr, "_udpRead signal interrupted recv() call (%s), retrying\n", strerror(errno));
		    	continue;
			}
	    
	//	DEBUGP (DERR,"_udpRead","udp socket read error (%s)", strerror(errno));
			fprintf(stderr, "_udpRead udp socket read error (%s)\n", strerror(errno));
			if (dynamicBuffer)
		    	free(*buf);
		    
			totalReceived = 0;
			break;
	    } 
		else if (returnValue == 0) 
		{
	//	DEBUGP(DALL, "_udpRead", "peer closed the connection");
			fprintf(stdout, "_udpRead peer closed the connection\n");
			break;		
	    }
	    
	    totalReceived += returnValue;
	    
	    if (totalReceived < size) 
		{
		//	DEBUGP(DALL, "_udpRead", "interrupted recv() (%s), retrying", strerror(errno));
			fprintf(stdout, "_udpRead interrupted recv() (%s), retrying\n", strerror(errno));
			usleep (delay);
			delay *= 2;
	    }
	}

    return totalReceived;
}


uint32_t MODULE_FUN_NAME(Udp, write)(T udp, char  *buf, uint32_t size)
{
    int32_t returnValue = 0, totalSent = 0;
    uint32_t delay = 10000;
    
    if (udp && udp->sock && buf && size) 
	{
		while (totalSent < sizeof(size)) {
	    	if ((returnValue = send(udp->sock->skd, ((char*)&size)+totalSent, sizeof(size)-totalSent, 0)) < 0) 
			{
	//	if(errno == EINTR && !SystemExit)
				if (errno == EINTR)
		    		continue;
	    
			//	DEBUGP (DERR, "_udpWrite", "error while sending record size");
				fprintf(stderr, "_udpWrite error while sending record size\n");
				return 0;
	    	}
	    
	    	totalSent += returnValue;
		}
	
		totalSent = 0;
	
		while (totalSent < size) 
		{
	    	if ((returnValue = send(udp->sock->skd, buf+totalSent, size-totalSent, 0)) < 0) 
			{
			//	DEBUGP (DERR, "_udpWrite", "udp socket write error (errno %d)", errno);
				fprintf(stderr, "_udpWrite udp socket write error (errno %d)\n", errno);
				totalSent = 0;
				break;
	    	} 
			else if (returnValue == 0) 
			{
			//	DEBUGP (DALL, "_udpWrite", "peer closed the connection");
				fprintf(stdout, "_udpWrite peer closed the connection\n");
				break;
	    	}
	    
	    	totalSent += returnValue;
	    
	    	if (totalSent < size) 
			{
			//	DEBUGP (DERR, "_udpWrite", "interrupted send() (errno %d), retrying", errno);
				fprintf(stderr, "_udpWrite interrupted send() (errno %d), retrying\n", errno);
				usleep (delay);
				delay *= 2;
	    	}
		
		}
    
    }

    return totalSent;
}

#undef T_S
#undef T


