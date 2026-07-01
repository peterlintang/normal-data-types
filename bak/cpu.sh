#!/bin/bash

#cpu
# look how many cpus in system
grep 'model name' /proc/cpuinfo | wc -l

#stress：一个 Linux 系统压力测试工具，用作异常进程模拟平均负载升高的场景
#sysstat：包含了常用的 Linux 性能工具，用来监控和分析系统的性能。这个包的两个命令 mpstat 和 pidstat
# mpstat：一个常用的多核 CPU 性能分析工具，可实时查看每个 CPU 的性能指标以及所有 CPU 的平均指标
# pidstat：一个常用的进程性能分析工具，用来实时查看进程的 CPU、内存、I/O 以及上下文切换等性能指标
# stress --cpu 1 --timeout 600
# watch -d uptime / -d highlight change color
sudo apt-get install stress sysstat
sudo apt-get install hping3
sudo apt-get install fio
sudo apt-get install bpfcc-tools
sudo apt-get install blktrace
sudo apt-get install iperf
sudo apt-get install iperf3
sudo apt-get install netper
sudo apt-get install apache2-utils

#节拍率 HZ 是内核的可配选项，不同的系统可能设置不同数值，你可以通过查询 /boot/config 内核选项来查看它的配置值，用户空间程序并不能
#直接访问该变量
#grep 'CONFIG_HZ=' /boot/config-$(uname -r)
#CONFIG_HZ=250
#为了方便用户空间程序，内核还提供了一个用户空间节拍率 USER_HZ，它总是固定为 100，也就是 1/100 秒

#vmstat：一个常用的系统性能分析工具，主要用来分析系统的内存使用情况，也常用来分析 CPU 上下文切换和中断的次数
#sysbench： 是一个多线程的基准测试工具，一般用来评估不同系统参数下的数据库负载情况
#top：显示了系统总体的 CPU 和内存使用情况，以及各个进程的资源使用情况
#ps：只显示每个进程的资源使用情况
#perf top，类似于 top，它能够实时显示占用 CPU 时钟最多的函数或者指令，因此可以用来查找热点函数
#pstree //查看进程父子关系
#execsnoop(-bpfcc)：一个专为短时进程设计的工具。它通过 ftrace 实时监控进程的 exec() 行为，并输出短时进程的基本信息，包括进程 PID、父进程 PID、命令行参数以及执行的结果
#/proc/softirqs 提供了软中断的运行情况
#/proc/interrupts 提供了硬中断的运行情况
#dstat：可以同时查看 CPU 和 I/O 这两种资源的使用情况，便于对比分析
#strace  正是最常用的跟踪进程系统调用的工具
#strace -p pid
#sar：是一个系统活动报告工具，既可以实时查看系统的当前活动，又可以配置保存和报告历史统计数据
#hping3：是一个可以构造 TCP/IP 协议数据包的工具，可以对系统进行安全审计、防火墙测试等
#tcpdump： 是一个常用的网络抓包工具，常用来分析各种网络问题
#taskset:cpu绑定
#nice
#renice


#ram
#oom_adj 的范围是 [-17, 15]，数值越大，表示进程越容易被 OOM 杀死；其中 -17 表示禁止 OOM
#把 sshd 进程的 oom_adj 调小为 -16，这样， sshd 进程就不容易被 OOM 杀死
#echo -16 > /proc/$(pidof sshd)/oom_adj
#free：显示整个系统的内存使用情况
#第一列，total 是总内存大小
#第二列，used 是已使用内存的大小，包含了共享内存
#第三列，free 是未使用内存的大小
#第四列，shared 是共享内存的大小
#第五列，buff/cache 是缓存和缓冲区的大小
#最后一列，available 是新进程可用内存的大小，不仅包含未使用内存，还包括了可回收的缓存，所以一般会比未使用内存更大

#清理文件页、目录项、Inodes等各种缓存
#$ echo 3 > /proc/sys/vm/drop_caches

#cachestat 和 cachetop：Linux 系统中并没有直接提供缓存命中率的接口，这两个正是查看系统缓存命中情况的工具。cachestat 提供了整个操>作系统缓存的读写命中情况。cachetop 提供了每个进程的缓存命中情况，它们都是BBC软件包的一部分
sudo apt-get install bpfcc-tools

#指定文件的缓存大小
#pcstat：是一个基于 Go 语言开发的工具，所以安装它之前，首先应该安装 Go 语言
#sudo apt-get install golang
#git clone https://gitcode.com/gh_mirrors/pc/pcstat
#cd pcstat/
#go build
#sudo cp pcstat/pcstat /usr/local/bin/
#pcstat /bin/ls	#查看这个文件的缓存情况

#如果为系统调用设置直接 I/O 的标志，就可以绕过系统缓存。要判断应用程序是否用了直接 I/O，最简单的方法当然是用strace观察它的系统调用>，查找应用程序在调用它们时的选项
#pgrep 命令来查找案例进程的 PID 号
#strace -p $(pgrep app)

#sysstat 软件包中的 vmstat ，可以观察内存的变化情况
#memleak(-bpfcc) 可以跟踪系统或指定进程的内存分配、释放请求，然后定期输出一个未释放内存和相应调用栈的汇总情况，是bbc包中的一个工具
#pidof 指定应用的pid
#memleak -p $(pidof xxx) -a

#swap
#页最小阈值，通过内核选项 /proc/sys/vm/min_free_kbytes 来间接设置。而其他两个阈值，都是根据页最小阈值计算生成的，计算方法如下 ：
#pages_low = pages_min*5/4
#pages_high = pages_min*3/2


#numa
#numactl：查看处理器在 Node 的分布情况

#三个内存阈值（页最小阈值、页低阈值和页高阈值），都可以通过内存域在 proc 文件系统中的接口 /proc/zoneinfo 来查看
#某个 Node 内存不足时，系统可以从其他 Node 寻找空闲内存，也可以从本地内存中回收内存。具体选哪种模式，可以通过 /proc/sys/vm/zone_reclaim_mode 来调整：
# 默认的 0 ，也就是刚刚提到的模式，表示既可以从其他 Node 寻找空闲内存，也可以从本地回收内存
# 1、2、4 都表示只回收本地内存，2 表示可以回写脏数据回收内存，4 表示可以用 Swap 方式回收内存

#对文件页的回收，当然就是直接回收缓存，或者把脏页写回磁盘后再回收
#对匿名页的回收，其实就是通过 Swap 机制，把它们写入磁盘后再释放内存
#在实际回收内存时Linux 提供了一个  /proc/sys/vm/swappiness 选项，用来调整使用 Swap 的积极程度：swappiness 的范围是 0-100，数值越大，越积极使用 Swap，也就是更倾向于回收匿名页；数值越小，越消极使用 Swap，也就是更倾向于回收文件页
# 创建Swap文件，配置大小为 8GB
# fallocate -l 8G /mnt/swapfile
# 修改权限只有根用户可以访问
# chmod 600 /mnt/swapfile
# 配置Swap文件
# mkswap /mnt/swapfile
# 开启Swap
# swapon /mnt/swapfile
# swapoff -a#guanbi swap
#pmap - report memory map of a process

#找出了 Swap 发生的根源。另一个问题就是，刚才的 Swap 到底影响了哪些应用程序呢？
#$ for file in /proc/*/status ; do awk '/VmSwap|Name|^Pid/{printf $2 " " $3}END{ print ""}' $file; done | sort -k 3 -n -r | head

#io
#df -h /dev/sda1
#df -i /dev/sda1
# free 输出的 Cache，是页缓存和可回收 Slab 缓存的和，可以从 /proc/meminfo ，直接得到它们的大小
# cat /proc/meminfo | grep -E "SReclaimable|Cached"
# 查看所有目录项和各种文件系统索引节点的缓存情况
# cat /proc/slabinfo | grep -E '^#|dentry|inode'
# 按下c按照缓存大小排序，按下a按照活跃对象数排序
#$ slabtop

#对 I/O 请求排序的过程，也就是我们熟悉的 I/O 调度。事实上，Linux 内核支持四种 I/O 调度算法：
# 第一种 NONE ，完全不使用任何 I/O 调度器，常用在虚拟机中（此时磁盘 I/O 调度完全由物理机负责）
# 第二种 NOOP ，是一个先入先出的队列，只做一些最基本的请求合并，常用于 SSD 磁盘
# 第三种 CFQ（Completely Fair Scheduler），也被称为完全公平调度器，是现在很多发行版的默认 I/O 调度器，它为每个进程维护了一个 I/O >调度队列，并按照时间片来均匀分布每个进程的 I/O 请求
# 第四种DeadLine 调度算法，分别为读、写请求创建了不同的 I/O 队列，可以提高机械磁盘的吞吐量，并确保达到最终期限（deadline）的请求被
#优先处理。多用在 I/O 压力比较重的场景，比如数据库等
##### 磁盘性能指标

# 使用率：磁盘处理 I/O 的时间百分比。过高的使用率（比如超过 80%），通常意味着磁盘 I/O 存在性能瓶颈。当使用率是 100% 的时候，磁盘依
#然有可能接受新的 I/O 请求
# 饱和度：磁盘处理 I/O 的繁忙程度。过高的饱和度，意味着磁盘存在严重的性能瓶颈。当饱和度为 100% 时，磁盘无法接受新的 I/O 请求
# IOPS（Input/Output Per Second）：每秒的 I/O 请求数
# 吞吐量：每秒的 I/O 请求大小
# 响应时间： I/O 请求从发出到收到响应的间隔时间
# fio：测试磁盘的 IOPS、吞吐量以及响应时间等核心指标
#需要测试出，不同 I/O 大小（一般是 512B 至 1MB 中间的若干值）分别在随机读、顺序读、随机写、顺序写等各种场景下的性能情况。用性能工具
#得到的这些指标，可以作为后续分析应用程序性能的依据。一旦发生性能问题，你就可以把它们作为磁盘性能的极限值，进而评估磁盘 I/O 的使用>情况

(#badblocks、smartctl 等工具，检测磁盘的硬件问题，
#e2fsck 等来检测文件系统的错误
#blockdev 工具设置
#tune2fs 工具，可以调整文件系统的特性
#ionice 来调整进程的 I/O 调度优先级)
#iostat：提供了每个磁盘的使用率、IOPS、吞吐量等各种常见的性能指标，这些指标实际上来自  /proc/diskstats
#iostat 只提供磁盘整体的 I/O 性能数据，要观察进程的 I/O 情况，可以使用 pidstat 和 iotop 这两个工具
#iotop：一个类似于 top 的工具，可以按照 I/O 大小对进程排序，然后找到 I/O 较大的那些进程
#运行下面的 lsof 命令，看看进程 18940 都打开了哪些文件
#lsof -p 18940

#filetop(-bpfcc)：bcc 软件包的一部分，主要跟踪内核中文件的读写情况，并输出线程 ID（TID）、读写大小、读写类型以及文件名称
#opensnoop(-bpfcc)：同属于 bcc 软件包，可以动态跟踪内核中的 open 系统调用

# 由于这两个容器共享同一个网络命名空间，所以我们只需要进入app的网络命名空间即可
# PID=$(docker inspect --format {{.State.Pid}} app)
# -i表示显示网络套接字信息
# nsenter --target $PID --net -- lsof -i


#// fio 支持 I/O 的重放。借助前面提到过的 blktrace，再配合上 fio，就可以实现对应用程序 I/O 模式的基准测试。你需要先用 blktrace ，记
#录磁盘设备的 I/O 访问情况；然后使用 fio ，重放 blktrace 的记录
# 使用blktrace跟踪磁盘I/O，注意指定应用程序正在操作的磁盘
#$ blktrace /dev/sdb	# TODO shi fou ke yi zhi jie cao zuo sdb??
# 查看blktrace记录的结果
# ls
#sdb.blktrace.0  sdb.blktrace.1

# 将结果转化为二进制文件
#$ blkparse sdb -d sdb.bin

# 使用fio重放日志
#$ fio --name=replay --filename=/dev/sdb --direct=1 --read_iolog=sdb.bin

#net
#网络配置 套接字信息
#ifconfig 和 ip 只显示了网络接口收发数据包的统计信息，但在实际的性能问题中，网络协议栈中的统计信息，也必须关注。
#netstat 或者ss ，来查看套接字、网络栈、网络接口以及路由表的信息，更推荐使用 ss 来查询网络的连接信息，因为它比 netstat 提供了更好的性能
#ifconfig eth0
#ip -s addr show dev eth0
#netstat -nlp
#ss -ltnp
#协议栈统计信息
#netstat -s
#ss -s
##### 网络吞吐和 PPS
#给 sar 增加 -n 参数就可以查看网络的统计信息，比如网络接口（DEV）、网络接口错误（EDEV）、TCP、UDP、ICMP 等等
#sar -n DEV 1
#查询Bandwidth
#ethtool eth0 | grep Speed
# 连通性和延时
#ping -c3 114.114.114.114
#各协议层的性能测试
#转发性能
#网络接口层和网络层，它们主要负责网络包的封装、寻址、路由以及发送和接收。在这两个网络协议层中，每秒可处理的网络包数 PPS，就是最重要
#的性能指标。特别是 64B 小包的处理能力，值得特别关注
#hping3 ：不仅可以作为一个 SYN 攻击的工具来使用。还是一个测试网络包处理能力的性能工具
#pktgen：Linux 内核自带的高性能网络测试工具 。支持丰富的自定义选项，方便根据实际需要构造所需网络包，从而更准确地测试出目标服务器>的性能
#/*
#function pgset() {
#    local result
#    echo $1 > $PGDEV
#
#    result=`cat $PGDEV | fgrep "Result: OK:"`
#    if [ "$result" = "" ]; then
#         cat $PGDEV | fgrep Result:
#    fi
#}
#
# 为0号线程绑定eth0网卡
#PGDEV=/proc/net/pktgen/kpktgend_0
#pgset "rem_device_all"   # 清空网卡绑定
#pgset "add_device eth0"  # 添加eth0网卡
#
# 配置eth0网卡的测试选项
#PGDEV=/proc/net/pktgen/eth0
#pgset "count 1000000"    # 总发包数量
#pgset "delay 5000"       # 不同包之间的发送延迟(单位纳秒)
#pgset "clone_skb 0"      # SKB包复制
#pgset "pkt_size 64"      # 网络包大小
#pgset "dst 192.168.0.30" # 目的IP
#pgset "dst_mac 11:11:11:11:11:11"  # 目的MAC
#
# 启动测试
#PGDEV=/proc/net/pktgen/pgctrl
#pgset "start"
#测试完成后，结果可以从 /proc 文件系统中获取
#$ cat /proc/net/pktgen/eth0
#*/
#
#TCP/UDP 性能
#iperf 和 netperf 都是最常用的网络性能测试工具，测试 TCP 和 UDP 的吞吐量。它们都以客户端和服务器通信的方式，测试一段时间内的平均>吞吐量
#HTTP 性能
#ab、webbench：都是常用的 HTTP 压力测试工具。ab 是 Apache 自带的 HTTP 压测工具，主要测试 HTTP 服务的每秒请求数、请求延迟、吞吐量>以及请求延迟的分布情况
#ab -c 1000 -n 10000 http://192.168.0.30/
#应用负载性能
#用 iperf 或者 ab 等测试工具，得到 TCP、HTTP 等的性能数据后，这些无法代表应用程序的实际性能。为了得到应用程序的实际性能，就要求性能
#工具本身可以模拟用户的请求负载，而 iperf、ab 这类工具就无能为力了。幸运的是，可以用 wrk、TCPCopy、Jmeter 或者 LoadRunner 等实现这>个目标
#wrk：一个 HTTP 性能测试工具，内置了 LuaJIT，方便根据实际需求，生成所需的请求负载，或者自定义响应的处理方法，wrk 工具本身不提供 yum 或 apt 的安装方法，需要通过源码编译来安装

