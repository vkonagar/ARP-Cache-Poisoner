#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdint.h>
#include<assert.h>
#include<unistd.h>
#include<linux/if_ether.h>

#define BUFF_SIZE 42;
#define ETHERNET_LINK_SIZE 14;

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


int main()
{
	int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
	if( fd == -1 )
	{
		perror("Error opening ARP Socket\n");
	}

	/*
		Buffer for storing the data 
	*/
	uint8_t buffer[200];

	while( 1 )
	{	
		int a;
		int total = a;
		int read = 0;
		read(fd,buffer,10);
		while( total != 0 )
		{
			//read(fd,buffer,20);
			total = total - a;
			printf("Read bytes are %d\n",a);
			read = read + a;
		}
		/*
			Buffer has Ethernet-ARP
			So, skip ethernet header
		*/
		arp_hdr* arp_packet = (arp_hdr*)(buffer+ETHERNET_LINK_SIZE);
		assert(arp_packet!=NULL);
		printf("\n%d\n",ntohs(arp_packet->opcode));
	}
}
