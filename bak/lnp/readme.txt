
wondershaper 限速命令

SOL_SOCKET: 通用类型socket套接字选项
SO_BROADCAST 使用udp套接字， 发送数据到子网的每个机器上，输入参数为整型变量，0禁止，1使能
SO_DEBUG 调试套接字，仅支持tcp套接字，输入参数为整型变量，0禁止，1使能, 使用trpt查看跟踪信息 ?
SO_DONTROUTE 发出的数据分组不经过正常的路由机制,分组根据目的地址和掩码选择一个网络接口发送
SO_ERROR 获取套接字错误信息,数据类型为int的变量类型
SO_KEEPALIVE 设置tcp连接的保持，适用长时间没有数据交流的连接,参数类型为整型变量
SO_LINGER 设置tcp连接关闭时，发送缓冲区的数据如何处理, onoff为0时，默认处理：close立刻返回，发送缓冲区的数据处理未知
			onoff为1，linger非0，为超时时间，在超时时间之前，发送缓冲区的数据，失败了close返回；linger为0,立刻返回，
			缓冲区的数据丢掉
SO_OOBINLINE 带外数据在普通流中接收 ?
SO_RCVBUF SO_SNDBUF 缓冲区大小设置, udp tcp的意义有所不同,client端在connect前设置，server端在listen前设置, 接收窗口/滑动窗口是在
	syn时获取的
SO_RCVLOWAT SO_SNDLOWAT 缓冲区下限设置, select轮询时会用到
SO_RCVTIMEO SO_SNDTIMEO 超时设置, 影响read readv recv recvfrom recvmsg write writev send sendto sendmsg
SO_REUSEADDR 地址重用
SO_EXCLUSIVEADDRUSE 独占端口，不允许其他应用占用此端口
SO_TYPE 套接字类型
SO_BINDTODEVICE 套接字网络接口绑定,数据会从此接口收发， 参数为网络接口的字符串名字
SO_PRIORITY 套接字优先级, 设置套接字发送报文的优先级,优先级范围0-6, int类型

IPPROTO_IP: 设置ip层协议选项
IP_HDRINCL	用户手动天聪ip头部,原始套接字类型，ip数据包不分片
IP_OPTIONS 设置ip头部的选项信息
IP_TOS	设置服务类型, IPTOS_LOWDELAY IPTOS_RELIABILITY IPTOS_THROUGHPUT IPTOS_LOWCOST
IP_TTL 生存时间选项

IPPROTO_TCP: 设置tcp层选项
TCP_KEEPALIVE:存活探测时间间隔，SO_KEEPALIVE设置下才起作用, 默认值7200s
TCP_MAXRT: 最大重传时间,报文发送在最大重传时间内没收到回复，则进行数据的重传
TCP_MAXSEG:tcp连接最大分节大小(mss)
TCP_NODELAY TCP_CORK nagle算法相关
