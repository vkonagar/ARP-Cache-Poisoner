ARP-Spoofer
===========

* To be finished within holidays

* This Application can be used to perform "Man in the Middle" Attack

### Terms

* Attacker - The Computer which runs this tool.
* Victim - The Computer which is to be attacked.

### Algorithm

* 1) Send a ARP Request with the router's IP as the source IP and the attacker's MAC as srcMAC in ARP header, 
   dst IP : victim's IP and dst MAC : attacker's MAC
* ( Ethernet src: attacker's MAC, dst: victim's MAC )
						
* Victim will give ARP reply to the request, and also cache it.
									
* Victim confirms the validity of the cached entry by sending a reply back to the Attacker

* Attacker will always dissemble as the router and replies back.

### How to use it

* Add your MAC address to the "fill\_attackers\_mac" fucntion.

* Add MAC address of the victim to "fill\_victims\_mac" function.

* Add the Router's IP which is to be spoofed.

* Add the Victim's IP address.

* After adding, all of these, just compile the file 'main\_spoofer' and run it with sudo permissions

* Thats it!. The victim can't access internet.

* Only exit the tool when you are done!

