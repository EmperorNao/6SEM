#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <math.h>
#include "ipx.h"


void main () {

	unsigned char filename[80];
	printf ("Write filename\n");
	scanf ("%s", filename);
	IPXSendFile(filename, 1);

}
