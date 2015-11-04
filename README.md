ARP-Spoofer
===========
An ARP Spoofing tool which can perform request and reply based ARP Cache poisoning.
### Files
* **main_poisoner.c** - This is the code used to perform **request** based ARP cache poisoning.
* **reply_spoofer.c** - To perform **reply** based ARP cache poisoning.
### Terms
* Attacker - The Computer which runs this tool.
* Victim - The Computer which is to be attacked.
### Algorithm
* This code works by sending ARP Request(Request based attack) packets with the gateway router's IP as the source IP, the attacker's MAC address as the src MAC address, destination IP with victim's IP and destination MAC with the victim's MAC in the ARP header.
* Ethernet header is crafted with src MAC as attacker's MAC, dst MAC victim's MAC
* After the Request packets are recieved by the Victim, it will give ARP replies to the request, and also caches the source identity of the receieved ARP requests in its table.
* (Optional) Victim confirms the validity of the cached entry by sending a reply back to the Attacker
* Attacker will always dissemble as the router and replies back.
### How to run it
* Change the fill_attacker_MAC function to include the attacker's MAC address.
* Compile the source file.
* While executing the code, Pass your Interface name (ex: wlan0, eth0), gateway router's IP, victim's IP as command line arguments to the executable.
* Thats it!. The victim can't access the internet (DOS).
* Only exit the tool when you are done!
