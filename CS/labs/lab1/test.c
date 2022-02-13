		printf("First 30 byte in packet. Client:");
		for(_i = 0; _i < 30; _i++)
			printf("%d ", RecieveString[_i + 12]);
		printf("\n");


				printf("First 30 elements from buffer: ");
		for(_i = 0; _i < 30; _i++)
			printf("%d ", buffer[_i + MAX_BUFFER]);
		printf("\n");