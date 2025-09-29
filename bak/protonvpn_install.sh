$https://account.protonvpn.com/dashboardV2
wget https://repo.protonvpn.com/debian/dists/stable/main/binary-all/protonvpn-stable-release_1.0.8_all.deb
sudo dpkg -i ./protonvpn-stable-release_1.0.8_all.deb && sudo apt update
echo "0b14e71586b22e498eb20926c48c7b434b751149b1f2af9902ef1cfe6b03e180 protonvpn-stable-release_1.0.8_all.deb" | sha256sum --check -
sudo apt install proton-vpn-gnome-desktop
sudo apt install libayatana-appindicator3-1 gir1.2-ayatanaappindicator3-0.1 gnome-shell-extension-appindicator
$sudo apt autoremove proton-vpn-gnome-desktop && sudo apt purge protonvpn-stable-release
$Identify the Proton VPN connection name with the command:
$nmcli connection show --active
$Delete all these connections using the following command
$nmcli connection delete pvpn-killswitch
$proton vpn
$382262135@qq.com
$[UAS[AI8XAS=CUR>

