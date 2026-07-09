
#install virtualbox
sudo apt-get install virtualbox
#install docker-machine
base=https://github.com/docker/machine/releases/download/v0.16.0 &&
  curl -L $base/docker-machine-$(uname -s)-$(uname -m) >/tmp/docker-machine &&
  sudo mv /tmp/docker-machine /usr/local/bin/docker-machine &&
  chmod +x /usr/local/bin/docker-machine

#lxj@ubuntu:~/work/1_Software/software$ sudo mkdir /etc/vbox
#lxj@ubuntu:~/work/1_Software/software$ sudo vi /etc/vbox/networks.conf
#lxj@ubuntu:~/work/1_Software/software$ cat /etc/vbox/networks.conf 
#* 10.0.0.0/8 192.168.0.0/16
#
#* 2001::/64
#lxj@ubuntu:~/work/1_Software/software$ 
