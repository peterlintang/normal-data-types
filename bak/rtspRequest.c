/*============================================================================= 
#     FileName: rtspRequest.c
#         Desc: client connect to RTSP server
#       Author: licaibiao 
#   LastChange: 2017-04-19  
=============================================================================*/ 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXBUF 1024

void Handel_OPTION(char *inbuffer);
void Handel_DESCRIBE(char *inbuffer, int sockfd);
void Handel_SETUP(char *inbuffer, int sockfd);
void Handel_PLAY(char *inbuffer, int sockfd,char *sessionId);
void Handle_PAUSE(char *inbuffer,int sockfd, char *sessionId);
void Handel_Teardown(char *inbuffer, int sockfd,char *sessionId);


int   intsearch(char * buf, char * searchStr, int len);
int   strsearch(char * buf, char * searchStr, int len,char *dest);
int   init_udpsocket(int port, struct sockaddr_in *servaddr, char *mcast_addr);
void  release_udpsocket(int socket_fd, char *mcast_addr);
void  proc_udpsocket(int socket_fd, struct sockaddr_in servaddr,int flag);
	
char sendOPTRequstStr[] = "OPTIONS rtsp://192.168.0.6:8554/testStream RTSP/1.0\r\n";
char userAgentStr[] = "virtual client player\r\n";
char successfullyReplyStr[] = "RTSP/1.0 200 OK\r\n";
char sendDESRequstStr[] = "DESCRIBE rtsp://192.168.0.6:8554/testStream RTSP/1.0\r\n";
char sendSETUPRequstStr[] = "SETUP rtsp://192.168.0.6:8554/testStream/track1 RTSP/1.0\r\n";
char sendPLAYRequstStr[] = "PLAY rtsp://192.168.0.6:8554/testStream/ RTSP/1.0\r\n";
char sendTeardownRequestStr[]="TEARDOWN rtsp://192.168.0.6:8554/testStream/ RTSP/1.0\r\n";
char sendPauseRequestStr[]="PAUSE rtsp://192.168.0.6:8554/testStream/ RTSP/1.0\r\n";
char CseqHeadStr[] = "Cseq: ";					
char UserHeadStr[] = "User-Agent: ";			
char ApplicationHeadStr[] = "Accept: ";			
char TransportHeadStr[] = "Transport: ";       
char SessionHeadStr[] = "Session: ";           
char RangeHeadStr[] = "Range: ";				
int cseqCount=0;

int main(int argc, char **argv)
{
    int sockfd,rtpsockfd,rtcpsockfd, len;
    struct sockaddr_in dest;      
	struct sockaddr_in RTPAddr;
    struct sockaddr_in RTCPAddr;
	char serverIP[]="192.168.0.6";
	char mcast_addr[] = "232.231.110.2"; 
    char buffer[MAXBUF + 1] = {0};
	int	serverPort=8554;
    int udpPort=0;

    cseqCount=0;
		
    /* creat socket for rtsp */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {      
        perror("Socket");
        exit(errno);
    }
    printf("socket created\n");

    /* init server add and port */
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;     
    dest.sin_port   = htons(serverPort);    
    if (inet_pton(AF_INET, serverIP, &dest.sin_addr) < 0 ) {           
        perror(serverIP);
        exit(errno);
    }
    printf("address created\n");

    /* connect to server */
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
        perror("Connect error\n");
        exit(errno);
    }
    printf("server connected\n");

    /* MAXBUF use to receive message from stream server */
    bzero(buffer, MAXBUF + 1);

	/* send message to server */
	Handel_OPTION(buffer);
    len = send(sockfd, buffer, strlen(buffer), 0);
    if (len < 0)
        printf("OPTION Reply Msg:\n %s send error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));
    else
        printf("OPTION Reply Msg:\n %s send successfully,total content is %d bytes!\n", buffer, len);
	bzero(buffer, MAXBUF + 1);
	
    /* receive message from server */
    len = recv(sockfd, buffer, MAXBUF, 0); /* OPTION Reply */
    if (len < 0){
        printf("OPTION Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));		
	}
    else{
        printf("OPTION Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);		
	}

	
	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr) -1) == 0){
		Handel_DESCRIBE(buffer, sockfd);
	}
	else{
		printf("the OPTION answer Msg is wrong!\n");
		return 0;
	}
	
	bzero(buffer, MAXBUF + 1);
    /* DESCRIBE Reply */
    len = recv(sockfd, buffer, MAXBUF, 0);
    if (len < 0){
        printf("DESCRIBE Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));		
	}
    else{
        printf("DESCRIBE Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);		
	}

	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr)-1) == 0){
		Handel_SETUP(buffer, sockfd);		
	}
	else{
		printf("the DESCRIBE answer Msg is wrong!\n");
		return 0;
	}
	
	bzero(buffer, MAXBUF + 1);    
    len = recv(sockfd, buffer, MAXBUF, 0);
    if (len < 0){
	    printf("SETUP Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));
	}
    else{
		printf("SETUP Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);
	}
       
	char sessionIdTmp[30];
	bzero(sessionIdTmp,sizeof(sessionIdTmp));

	strsearch(buffer, "Session: ", 9, sessionIdTmp);
	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr)-1) == 0)
	{
		char searchStrTmp[13]="port=";
		udpPort=intsearch(buffer,searchStrTmp,5);
		if(udpPort==0){
			return 0;
		}
		printf("the udp port is %d\n",udpPort);
		
		bzero(&RTPAddr,sizeof(RTPAddr));
		bzero(&RTCPAddr,sizeof(RTCPAddr));
		
		rtpsockfd  = init_udpsocket(udpPort, &RTPAddr, mcast_addr);
		rtcpsockfd = init_udpsocket(udpPort+1, &RTCPAddr, mcast_addr);
	
		Handel_PLAY(buffer, sockfd,sessionIdTmp);
	}
	else
	{
		printf("the SETUP answer Msg is wrong!\n");
		return 0;
	}
	
	bzero(buffer, MAXBUF + 1);
	len = recv(sockfd, buffer, MAXBUF, 0);
    if (len < 0){	
		printf("PLAY Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));	
	}else{
		printf("PLAY Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);
	}
        
	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr)-1) != 0){
		printf("the play answer Msg is wrong!\n");
		return 0;
	}
	
	bzero(buffer, MAXBUF + 1);
	printf("now,receiving RTP packets data......\n");
	proc_udpsocket(rtpsockfd, RTPAddr, 0);
	proc_udpsocket(rtcpsockfd, RTCPAddr, 1);
	
	Handle_PAUSE(buffer, sockfd, sessionIdTmp);
	
	len = recv(sockfd, buffer, MAXBUF, 0);
    if(len < 0){
		printf("PAUSE Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));
	} 
    else{
		printf("PAUSE Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);
	}
        

	if (memcmp(buffer, successfullyReplyStr, sizeof(successfullyReplyStr)-1) != 0){
		printf("the pause answer Msg is wrong!\n");
		return 0;
	}
	
	bzero(buffer, MAXBUF + 1);
	Handel_Teardown(buffer, sockfd,sessionIdTmp);
	len = recv(sockfd, buffer, MAXBUF, 0);
    if (len < 0){
		printf("teardown Reply Msg:\n %s receive error! error code is %d, error content is  %s \n", buffer, errno, strerror(errno));	
	}  
    else{
		printf("teardown Reply Msg:\n %s receive successfully,total content is %dbytes!\n", buffer, len);
	}
        
	release_udpsocket(rtpsockfd, mcast_addr);
	release_udpsocket(rtcpsockfd, mcast_addr);
    return 0;
}

void Handel_OPTION(char *inbuffer)
{
	char strCseq[5]= {0};
	cseqCount++;
	//OPTION Request
	memcpy(inbuffer, sendOPTRequstStr, sizeof(sendOPTRequstStr));
	//Cseq number
	sprintf(strCseq, "%d", cseqCount);
	strcat(inbuffer + sizeof(inbuffer), CseqHeadStr);
	strcat(inbuffer + sizeof(inbuffer), strCseq);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//user-Agent infomation
	strcat(inbuffer + sizeof(inbuffer), UserHeadStr);
	strcat(inbuffer + sizeof(inbuffer),userAgentStr);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
}

void Handel_DESCRIBE(char *inbuffer, int sockfd)
{
	char strCseq[5]= {0};
	int len;
	bzero(inbuffer, MAXBUF + 1);

	cseqCount++;
	//DESCRIBE Request
	memcpy(inbuffer, sendDESRequstStr, sizeof(sendDESRequstStr));
	//Cseq number
	sprintf(strCseq, "%d", cseqCount);
	strcat(inbuffer + sizeof(inbuffer), CseqHeadStr);
	strcat(inbuffer + sizeof(inbuffer), strCseq);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//Application
	strcat(inbuffer + sizeof(inbuffer), ApplicationHeadStr);
	strcat(inbuffer + sizeof(inbuffer),"application/sdp");
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//user-Agent infomation
	strcat(inbuffer + sizeof(inbuffer), UserHeadStr);
	strcat(inbuffer + sizeof(inbuffer),userAgentStr);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	
	len = send(sockfd, inbuffer, strlen(inbuffer), 0);
    if (len < 0){
        printf("DESCRIBE Request Msg:\n %s send error! error code is %d, error content is  %s \n", inbuffer, errno, strerror(errno));		
	}
    else{
		printf("DESCRIBE Request Msg:\n %s send successfully,total content is %d bytes!\n", inbuffer, len);
	}

}

void Handel_SETUP(char *inbuffer, int sockfd)
{
	char strCseq[5]= {0};
	int len;
	bzero(inbuffer, MAXBUF + 1);

	cseqCount++;
	//SETUP Request
	memcpy(inbuffer, sendSETUPRequstStr, sizeof(sendSETUPRequstStr));
	//Cseq number
	sprintf(strCseq, "%d", cseqCount);
	strcat(inbuffer + sizeof(inbuffer), CseqHeadStr);
	strcat(inbuffer + sizeof(inbuffer), strCseq);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//Transport
	strcat(inbuffer + sizeof(inbuffer), TransportHeadStr);
	strcat(inbuffer + sizeof(inbuffer),"RTP/AVP;unicast;client_port=31126-31127");
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//user-Agent infomation
	strcat(inbuffer + sizeof(inbuffer), UserHeadStr);
	strcat(inbuffer + sizeof(inbuffer),userAgentStr);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	len = send(sockfd, inbuffer, strlen(inbuffer), 0);
    if (len < 0){
        printf("SETUP Request Msg:\n %s send error! error code is %d, error content is  %s \n", inbuffer, errno, strerror(errno));		
	}
    else{
        printf("SETUP Request Msg:\n %s send successfully,total content is %d bytes!\n", inbuffer, len);		
	}

}

void Handel_PLAY(char *inbuffer, int sockfd,char* sessionId)
{
	char strCseq[5]= {0};
	int len;
	bzero(inbuffer, MAXBUF + 1);

    cseqCount++;
	//SETUP Request
	memcpy(inbuffer, sendPLAYRequstStr, sizeof(sendPLAYRequstStr));
	//Cseq number
	sprintf(strCseq, "%d", cseqCount);
	strcat(inbuffer + sizeof(inbuffer), CseqHeadStr);
	strcat(inbuffer + sizeof(inbuffer), strCseq);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//Session
	strcat(inbuffer + sizeof(inbuffer), SessionHeadStr);
	strncat(inbuffer + sizeof(inbuffer),sessionId,strlen(sessionId));
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//Range
	strcat(inbuffer + sizeof(inbuffer), RangeHeadStr);
	strcat(inbuffer + sizeof(inbuffer),"npt=0.000-");
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//user-Agent infomation
	strcat(inbuffer + sizeof(inbuffer), UserHeadStr);
	strcat(inbuffer + sizeof(inbuffer),userAgentStr);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	len = send(sockfd, inbuffer, strlen(inbuffer), 0);
    if (len < 0){
        printf("PLAY Request Msg:\n %s send error! error code is %d, error content is  %s \n", inbuffer, errno, strerror(errno));		
	}
    else{
        printf("PLAY Request Msg:\n %s send successfully,total content is %d bytes!\n", inbuffer, len);		
	}

}


void Handle_PAUSE(char *inbuffer, int sockfd,char *sessionId)
{
	char strCseq[5]= {0};
	int len;
	bzero(inbuffer, MAXBUF + 1);

	cseqCount++;
	//SETUP Request
	memcpy(inbuffer, sendPauseRequestStr, sizeof(sendPauseRequestStr));
	//Cseq number
	sprintf(strCseq, "%d", cseqCount);
	strcat(inbuffer + sizeof(inbuffer), CseqHeadStr);
	strcat(inbuffer + sizeof(inbuffer), strCseq);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//Session
	strcat(inbuffer + sizeof(inbuffer), SessionHeadStr);
	strncat(inbuffer + sizeof(inbuffer),sessionId,strlen(sessionId));
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//user-Agent infomation
	strcat(inbuffer + sizeof(inbuffer), UserHeadStr);
	strcat(inbuffer + sizeof(inbuffer),userAgentStr);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	len = send(sockfd, inbuffer, strlen(inbuffer), 0);
    if (len < 0){
		printf("PAUSE Request Msg:\n %s send error! error code is %d, error content is  %s \n", inbuffer, errno, strerror(errno));
	}  
    else{
		printf("PAUSE Request Msg:\n %s send successfully,total content is %d bytes!\n", inbuffer, len);	
	}     
}
	


void Handel_Teardown(char *inbuffer, int sockfd,char *sessionId)
{
	char strCseq[5]= {0};
	int len;
	bzero(inbuffer, MAXBUF + 1);

	cseqCount++;
	//SETUP Request
	memcpy(inbuffer, sendTeardownRequestStr, sizeof(sendTeardownRequestStr));
	//Cseq number
	sprintf(strCseq, "%d", cseqCount);
	strcat(inbuffer + sizeof(inbuffer), CseqHeadStr);
	strcat(inbuffer + sizeof(inbuffer), strCseq);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//Session
	strcat(inbuffer + sizeof(inbuffer), SessionHeadStr);
	strncat(inbuffer + sizeof(inbuffer),sessionId,strlen(sessionId));
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	//user-Agent infomation
	strcat(inbuffer + sizeof(inbuffer), UserHeadStr);
	strcat(inbuffer + sizeof(inbuffer),userAgentStr);
	strcat(inbuffer + sizeof(inbuffer), "\r\n");
	len = send(sockfd, inbuffer, strlen(inbuffer), 0);
    if (len < 0){
		printf("TEARDOWN Request Msg:\n %s send error! error code is %d, error content is  %s \n", inbuffer, errno, strerror(errno));
	}   
    else{
		printf("TEARDOWN Request Msg:\n %s send successfully,total content is %d bytes!\n", inbuffer, len);
	}
}

 int intsearch(char* buf, char* searchStr, int len)
{
	char *tmp1;
	char *tmp2;
	int	port=0;
	
	tmp1=buf;
	if(buf==NULL||searchStr==NULL||len==0){
		return -1;	
	}

	while((tmp2=strchr(tmp1,*searchStr))!=NULL){
		if(memcmp(tmp2,searchStr,len)==0){
			tmp2+=len;
			while(*tmp2>='0'&&*tmp2<='9'){
				port=(int)((*tmp2-'0')+port*10);
				tmp2++;
			}
			break;
		}
		else{
			tmp2+=1;
		}
		
		tmp1=tmp2;
	}
	if(tmp2==NULL){
		return -1;		
	}
	else{
		return port;		
	}
}

  int strsearch(char* buf, char* searchStr, int len, char *dest)
{
	char *tmp1;
	char *tmp2;
	char *tmp3;
	int	num=0;
	
	tmp1=buf;
	tmp3=dest;
	if(buf==NULL||searchStr==NULL||len==0||dest==NULL) return -1;

	while((tmp2=strchr(tmp1,*searchStr))!=NULL){
		if(memcmp(tmp2,searchStr,len)==0){
			tmp2+=len;
			
			while((*tmp2>='0'&&*tmp2<='9')||(*tmp2>='A'&&*tmp2<='Z')){
				*tmp3=*tmp2;
				tmp3++;
				tmp2++;				
			}
			break;
		}
		else{
			tmp2+=1;			
		}

		tmp1 = tmp2;
	}
	if(tmp2==NULL){
		return -1;		
	}
	else{
		return 1;	
	}
}
  
int init_udpsocket(int port, struct sockaddr_in *servaddr, char *mcast_addr){
	
	int err = -1;
    int socket_fd;                                      
	
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);    
    if (socket_fd < 0 ){
        perror("socket()");
        return -1;
    }  
   
    memset(servaddr, 0, sizeof(struct sockaddr_in));
    servaddr->sin_family 	   = AF_INET;
    servaddr->sin_addr.s_addr  = htonl(INADDR_ANY);
    servaddr->sin_port 		   = htons(port);
   
    err = bind(socket_fd,(struct sockaddr*)servaddr, sizeof(struct sockaddr_in)) ;
    if(err < 0){
        perror("bind()");
        return -2;
    }
     
	/*set enable MULTICAST LOOP */                                       
    int loop = 1;
    err = setsockopt(socket_fd,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));
    if(err < 0){
        perror("setsockopt():IP_MULTICAST_LOOP");
        return -3;
    }
   
	/* Join multicast group */ 
	struct ip_mreq mreq; 
    mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr); 
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);      
    err = setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(struct ip_mreq));
    if (err < 0){
        perror("setsockopt():IP_ADD_MEMBERSHIP");
        return -4;
    }
	return socket_fd;
}

void release_udpsocket(int socket_fd, char *mcast_addr){
	struct ip_mreq mreq; 
    mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr); 
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); 
	setsockopt(socket_fd, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(struct ip_mreq));
    close(socket_fd);
}

void proc_udpsocket(int socket_fd, struct sockaddr_in servaddr,int flag){
	int n;
	int i;
	int addr_len;
	char buff[MAXBUF + 1] = {0};

	memset(buff, 0, MAXBUF);    
	addr_len = sizeof(struct sockaddr_in);                                                        
    n = recvfrom(socket_fd, buff, MAXBUF, 0,(struct sockaddr*)&servaddr, &addr_len);
    if(n == -1){
        perror("recvfrom()");
    }   
	if(flag == 0){
	    printf("Recv %d Byte RTP message from server\n", n);			
	}else{
		printf("Recv %d byte RTCP message from server,they are :\n", n);	
		for(i=0; i<n; i++){
			printf("%c ",buff[i]);
		}
		printf("\n\n");	
	}
}


