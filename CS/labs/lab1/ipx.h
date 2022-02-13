#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <math.h>


#define IPX_interrupt 0x7A
#define header_length 30


#define IPX_open 0x00
#define IPX_close 0x01
#define IPX_localTarget 0x02
#define IPX_send 0x03
#define IPX_listen 0x04
#define IPX_event 0x05
#define IPX_cancel 0x06
#define IPX_specialEvent 0x07
#define IPX_interval 0x08
#define IPX_GetInternetwork 0x09
#define IPX_control 0x0A
#define IPX_disconnect 0x0B


#define TargetSocket 0x08
#define TargetNode 0x02


#define CUP 500
#define MAX_BUFFER 12
#define MAX_FILE_SIZE 5000000
#define MAX_WAIT_ITER 30000
#define MAX_PACKET 10000


typedef struct{
	unsigned int Checksum;
	unsigned int Length;
	unsigned char TransportControl;
	unsigned char PacketType;
	unsigned char DestantionNetwork [4];
	unsigned char DestantionNode [6];
	unsigned int DestantionSocket;
	unsigned char SourceNetwork [4];
	unsigned char SourceNode [6];
	unsigned int SourceSocket;
} IPXHeader;


typedef struct {
	void far *LinkAddress;
	void far *ESRAddress;
	unsigned char InUseFlag;
	unsigned char CompletionCode;
	unsigned int SocketNumber;
	unsigned char IPXWorkspace [4];
	unsigned char DriverWorkspace [12];
	unsigned char ImmediateAddress [6];
	unsigned int FragmentCount;
	void far *FragmentAddress;
	unsigned int FragmentSize;
} ECB_1;


typedef struct {
	void far *LinkAddress;
	void far *ESRAddress;
	unsigned char InUseFlag;
	unsigned char CompletionCode;
	unsigned int SocketNumber;
	unsigned char IPXWorkspace [4];
	unsigned char DriverWorkspace [12];
	unsigned char ImmediateAddress [6];
	unsigned int FragmentCount;
	void far *FragmentAddress_1;
	unsigned int FragmentSize_1;
	void far *FragmentAddress_2;
	unsigned int FragmentSize_2;
} ECB_2;


typedef struct {
	void far *LinkAddress;
	void far *ESRAddress;
	unsigned char InUseFlag;
	unsigned char WorkSpace [5];
} SpecialECB;



typedef struct {
	unsigned char NetworkNumber [4];
	unsigned char NodeAddress [6];
	unsigned int Socket;
	unsigned char ImmediateAddress [6];
} IntAddr;


void IPXRelinquishControl() {

	union REGS a;
	union REGS b;
	a.x.bx = 10;
	int86(IPX_interrupt, &a, &b);

}


unsigned int IPXOpenSocket(unsigned int socket_no) {

	union REGS inregs, outregs;
	inregs.x.bx = IPX_open;
	inregs.h.al = 0xFF;
	inregs.x.dx = socket_no;
	int86 (IPX_interrupt, &inregs, &outregs);

	if (outregs.h.al != 0x00)
		return outregs.h.al;

	return 0;

}


void IPXCloseSocket(unsigned int socket_no) {

	union REGS inregs, outregs;

	inregs.x.bx = IPX_close;
	inregs.x.dx = socket_no;
	int86 (IPX_interrupt, &inregs, &outregs);

}


void buildHeader(IPXHeader far* header, unsigned int socket_no) {

	header->PacketType = 0x04;
	strnset(header->DestantionNetwork, 0x00, sizeof header->DestantionNetwork);
	strnset(header->DestantionNode, 0x00, sizeof header->DestantionNode);

	header->DestantionNetwork[0] = 127;
	header->DestantionNetwork[1] = 0;
	header->DestantionNetwork[2] = 0;
	header->DestantionNetwork[3] = 1;

	header->DestantionNode[0] = 255;
	header->DestantionNode[1] = 255;
	header->DestantionNode[2] = 255;
	header->DestantionNode[3] = 255;
	header->DestantionNode[4] = 255;
	header->DestantionNode [5] = 255;

	header->DestantionSocket = socket_no;

}


void buildECB(ECB_2 far * blc, IPXHeader far* header, unsigned int socket_no,
	unsigned int node, unsigned char* data, unsigned int datalen) {

	blc->ESRAddress = NULL;
	blc->SocketNumber = socket_no;
	strnset(blc->ImmediateAddress, 0x00, sizeof blc->ImmediateAddress);
	blc->ImmediateAddress[5] = node;
	blc->FragmentCount = 2;
	blc->FragmentAddress_1 = &header;
	blc->FragmentSize_1 = header_length;
	blc->FragmentAddress_2 = &data;
	blc->FragmentSize_2 = datalen + 1;

}


void IPXSendPacket(ECB_2 far * blc) {

	union REGS inregs, outregs;
	struct SREGS segreg;

	inregs.x.bx = IPX_send;
	inregs.x.si = FP_OFF ( blc );
	segreg.es = FP_SEG ( blc );
	int86x (IPX_interrupt, &inregs, &outregs, &segreg);

}



void IPXListenForPacket(ECB_2 far* blc) {

	union REGS inregs, outregs;
	struct SREGS segreg;

	inregs.x.bx = IPX_listen;
	inregs.x.si = FP_OFF( blc );
	segreg.es = FP_SEG( blc );
	int86x (IPX_interrupt, &inregs, &outregs, &segreg);

}


unsigned IPXSendFile(unsigned char* filename, unsigned log) {

	unsigned char DispString[80];

	IPXHeader Header;
	ECB_2 My_ECB;
	ECB_2 far *My_ECB_ptr = &My_ECB;

	unsigned char buffer[CUP + MAX_BUFFER];

	unsigned long long nsend;
	FILE *file;
	unsigned long long nbytes = 0;
	unsigned long long i;
	unsigned long long t;
	unsigned int _i = 0;
	int iter=0;
	unsigned long long k = 1;


	My_ECB.ESRAddress = NULL;
	My_ECB.SocketNumber = TargetSocket;
	strnset(My_ECB.ImmediateAddress, 0x00, sizeof My_ECB.ImmediateAddress);
	My_ECB.ImmediateAddress[5] = TargetNode;
	My_ECB.FragmentCount = 2;
	My_ECB.FragmentAddress_1 = &Header;
	My_ECB.FragmentSize_1 = header_length;


	Header.PacketType = 0x04;
	strnset(Header.DestantionNetwork, 0x00, sizeof Header.DestantionNetwork);
	strnset(Header.DestantionNode, 0x00, sizeof Header.DestantionNode);
	Header.DestantionNetwork[0] = 127;
	Header.DestantionNetwork[1] = 0;
	Header.DestantionNetwork[2] = 0;
	Header.DestantionNetwork[3] = 1;
	Header.DestantionNode[0] = 255;
	Header.DestantionNode[1] = 255;
	Header.DestantionNode[2] = 255;
	Header.DestantionNode[3] = 255;
	Header.DestantionNode[4] = 255;
	Header.DestantionNode [5] = TargetNode;
	Header.DestantionNode [5] = 255;
	Header.DestantionSocket = TargetSocket;


	if (log) {

		printf("Our filename: %s\n", filename);

	}

	file = fopen(filename, "rb");
	if (log) {
		printf("Open file code: %X\n", file);
	}

	if (file == 0) {
		return 1;
	}



	fseek(file, 0L, SEEK_END);
	nbytes = ftell(file);
	fseek(file, 0L, SEEK_SET);


	if (log) {
		printf("Size of file: %llu\n", nbytes);
	}

	if (nbytes > (unsigned long long)MAX_FILE_SIZE) {
		if (log) {
			printf("Max file size occured: %llu | %llu\n", nbytes, MAX_FILE_SIZE);
		}
		return 2;
	}

	if (nbytes > CUP) {
		nsend = ceil(nbytes / (double)(CUP));
	}
	else {
		nsend = 1;
	}

	if (log) {
		printf("NSEND = %llu, NBYTES = %llu\n", nsend, nbytes);
	}

	for (i = 0; i < nbytes; i += CUP) {

		if (nbytes - i > CUP) {
	  		My_ECB.FragmentSize_2 = CUP + MAX_BUFFER;
		}
		else {
		 	My_ECB.FragmentSize_2 = (unsigned int) nbytes - (unsigned int) i + MAX_BUFFER;
		}

		t = My_ECB.FragmentSize_2;

		buffer[0] = (nsend & 0xFF000000) >> 24;
		buffer[1] = (nsend & 0x00FF0000) >> 16;
		buffer[2] = (nsend & 0x0000FF00) >> 8;
		buffer[3] = nsend & 0x000000FF;

		buffer[4] = (i & 0xFF000000) >> 24;
		buffer[5] = (i & 0x00FF0000) >> 16;
		buffer[6] = (i & 0x0000FF00) >> 8;
		buffer[7] = i & 0x000000FF;

		buffer[8] = (t & 0xFF000000) >> 24;
		buffer[9] = (t & 0x00FF0000) >> 16;
		buffer[10] = (t & 0x0000FF00) >> 8;
		buffer[11] = t & 0x000000FF;

		My_ECB.FragmentAddress_2 = &buffer;


		IPXSendPacket(My_ECB_ptr);
		while (My_ECB.InUseFlag) {};
		for (iter = 0; iter < MAX_WAIT_ITER; iter++) {};

		if (log) {
			printf("i = %llu, nsend = %llu, %.3f%%\n", k, nsend, ((float)k) / nsend * 100);
		}
		k += 1;

	}

	if (log) {
		printf ("End. Message sended with result: %x\n", My_ECB.CompletionCode);
	}

	return 0;

}


unsigned IPXListenFile(unsigned char* filename, unsigned log) {

	unsigned char RecieveString[CUP + MAX_BUFFER];
	IPXHeader Header;
	ECB_2 My_ECB;
	ECB_2 far *My_ECB_ptr = &My_ECB;
	FILE* file;
	unsigned int socket_res = 0;
	long long nsend;
	long long i;
	long long pos;
	unsigned long long npacket;
	long long int length_packet;
	unsigned int _i;
  	long long nrecieved = 0;
  	long long iter;
  	long long nlost = 0;

	My_ECB.ESRAddress = NULL;
	My_ECB.SocketNumber = TargetSocket;
	My_ECB.FragmentCount = 2;
	My_ECB.FragmentAddress_1 = &Header;
	My_ECB.FragmentSize_1 = header_length;
	My_ECB.FragmentAddress_2 = &RecieveString;
	My_ECB.FragmentSize_2 = sizeof RecieveString;

	IPXOpenSocket(TargetSocket);

	if (socket_res != 0x00) {
		if (log) {
			printf ("Socket error %X\n", socket_res);
		}
		IPXCloseSocket(TargetSocket);
		return;
	}


	file = fopen(filename,"wb+");
	if (file == 0) {

		if (log) {
			printf("File open occured: %X\n", file);
		}
		return 1;

	}
	fseek(file, MAX_FILE_SIZE, SEEK_SET);

	IPXListenForPacket(My_ECB_ptr);
	while (My_ECB.InUseFlag)
	{};

	nsend = ((long long)(RecieveString[0]) << 24) + ((long long)(RecieveString[1]) << 16) + ((long long)(RecieveString[2]) << 8) + (long long)(RecieveString[3]);
	pos = ((long long)(RecieveString[4]) << 24) + ((long long)(RecieveString[5]) << 16) + ((long long)(RecieveString[6]) << 8) + (long long)(RecieveString[7]);
	length_packet = ((unsigned)(RecieveString[8]) << 24) + ((unsigned)(RecieveString[9]) << 16) + ((unsigned)(RecieveString[10]) << 8) + (unsigned)(RecieveString[11]);

	if (((unsigned long long)nsend) * ((unsigned long long)CUP) > (unsigned long long)MAX_FILE_SIZE) {
		if (log) {
			printf("Max file size occured: %llu | %llu\n", ((unsigned long long)nsend) * ((unsigned long long)CUP), MAX_FILE_SIZE);
		}
		return 2;
	}

	My_ECB.FragmentSize_2 = length_packet;


	fseek(file, pos, SEEK_SET);
	fwrite(RecieveString + MAX_BUFFER, My_ECB.FragmentSize_2 - MAX_BUFFER, 1, file);
  	nrecieved += My_ECB.FragmentSize_2 - MAX_BUFFER;


	for (i = 1; i < nsend; ++i) {
		IPXListenForPacket(My_ECB_ptr);
		iter = 0;

		while (My_ECB.InUseFlag)
		{
			IPXRelinquishControl();
		};
		pos = ((long long)(RecieveString[4]) << 24) + ((long long)(RecieveString[5]) << 16) + ((long long)(RecieveString[6]) << 8) + (long long)(RecieveString[7]);


		length_packet = ((unsigned)(RecieveString[8]) << 24) + ((unsigned)(RecieveString[9]) << 16) + ((unsigned)(RecieveString[10]) << 8) + (unsigned)(RecieveString[11]);
		My_ECB.FragmentSize_2 = length_packet;
	    nrecieved += My_ECB.FragmentSize_2 - MAX_BUFFER;
		fseek(file, pos, SEEK_SET);
		fwrite(RecieveString + MAX_BUFFER, My_ECB.FragmentSize_2 - MAX_BUFFER, 1, file);

		if (log) {
			printf("i = %llu, nsend = %llu, %.3f%%\n", i, nsend, ((float)i) / nsend * 100);
		}

	}

	if (log) {
		printf("i = %llu, nsend = %llu, %.3f%%\n", i, nsend, ((float)i) / nsend * 100);
	}

	fseek(file, nrecieved, SEEK_SET);
	close(file);

	if (log) {
		printf("Got message\n");
	}

	IPXCloseSocket(TargetSocket);

	return 0;

}
