ARP-Spoofer
===========

* Please feel free to use this code and modify!

* This Application can be used to perform "Denial of service" Attack

### Terms

* Attacker - The Computer which runs this tool.
* Victim - The Computer which is to be attacked.

### Algorithm

* Send an ARP Request with the router's IP as the source IP and the attacker's MAC address as the src MAC address in ARP header, 
   dst IP : victim's IP and dst MAC : attacker's MAC

* Ethernet src: attacker's MAC, dst: victim's MAC
						
* Victim will give ARP reply to the request, and also cache it.
									
* Victim confirms the validity of the cached entry by sending a reply back to the Attacker

* Attacker will always dissemble as the router and replies back.

### How to use it

* Pass your Interface name ( ex: wlan0,eth0 ) as an argument to the command line.

* Add your MAC address to the "fill\_attackers\_mac" function. ( You can find this using ifconfig command ).

* Add MAC address of the victim to "fill\_victims\_mac" function. ( You can find this using any packet sniffer )

* Add the Router's IP which is to be spoofed.

* Add the Victim's IP address.

* After adding, all of these, just compile the file 'main\_spoofer' and run it with sudo permissions

* Thats it!. The victim can't access internet.

* Only exit the tool when you are done!
