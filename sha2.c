#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>

int main(const int argc, const char **argv)
{
	// This code assumes that and int is of size 32 bits
	long lim = (long)pow(2, 32) - 1;
//	printf("lim: %ld\n", lim);
	
	if ((long)UINT_MAX != lim) {
		printf("ERROR: the size of int in this system is not 32bit.\n");
		return 1;
	}

	
	unsigned int h0 = 0x6a09e667;
	unsigned int h1 = 0xbb67ae85;
	unsigned int h2 = 0x3c6ef372;
	unsigned int h3 = 0xa54ff53a;
	unsigned int h4 = 0x510e527f;
	unsigned int h5 = 0x9b05688c;
	unsigned int h6 = 0x1f83d9ab;
	unsigned int h7 = 0x5be0cd19;

//	printf("h0 %u\n", h0);
//	printf("h1 %u\n", h1);
//	printf("h2 %u\n", h2);
//	printf("h3 %u\n", h3);
//	printf("h4 %u\n", h4);
//	printf("h5 %u\n", h5);
//	printf("h6 %u\n", h6);
//	printf("h7 %u\n", h7);
	
	unsigned int k[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	
	int i;
//	for (i = 0; i < 64; i++) {
//		printf("k[%d] = %u\n", i, k[i]);
//	}

	unsigned char mesg[64] = "";
	sprintf(mesg, "contrasena");

	int length = strlen(mesg);
	printf("len of string: %d\n", length);
	
	mesg[length+1] = 1;
	mesg[length+1] <<= 7;
	
	for (i = length+2; i < 56; i++) {
		mesg[i] = 0;
	}
//	printf("char appended first: 0x%x\n", (unsigned int)mesg[length+1]);
//	printf("char appended first: %u\n", (unsigned int)mesg[length+1]);

	unsigned long long len64 = (unsigned long long) length;
	printf("64 bit length: 0x%llx\n", len64);
	
	for (i = 56; i < 64; i++) {
		int step = 8 * (64-i -1);
		mesg[i] = (len64 << step) & 0xff;
//		printf("i: mesg[%d] = %u\n", i, (unsigned int)mesg[i]);
	}

	unsigned int w[64];
	
	for (i=0; i < 16; i++) {
		unsigned char *pcw = (unsigned char *)&w[i];
		int j;
		for (j = 0; j < 4; j++) {
			pcw[j] = mesg[i*4 + (3 - j)];
		//	printf("pcw[%d] = %x\n", (i*4)+j, pcw[j]);
		}
//		printf("w[%d] = %x\n", i, w[i]);

	}
	
	for (i = 16; i <64; i++) {
		unsigned int s0 = (w[i-15] >> 7) ^ (w[i-15] >> 18) ^ (w[i-15] >> 3);
		unsigned int s1 = (w[i-2] >> 17) ^ (w[i-2] >> 19) ^ (w[i-2] >> 10);
		w[i] = w[i-16] + s0 + w[i-7] + s1;
//		printf("w[%d] = %x\n", i, w[i]);
	}

	unsigned int a = h0;
	unsigned int b = h1;
	unsigned int c = h2;
	unsigned int d = h3;
	unsigned int e = h4;
	unsigned int f = h5;
	unsigned int g = h6;
	unsigned int h = h7;

	for (i = 0; i < 64; i++) {
		unsigned int S1 = (e >> 6) ^ (e >> 11) ^ (e >> 25);
		unsigned int ch = (e & f) ^ ((~e) & g);
		unsigned int temp1 = h + S1 + ch + k[i] + w[i];
		unsigned int S0 = (a >> 2) ^ (a >> 13) ^ (a >> 22);
		unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
		unsigned int temp2 = S0 + maj;
		
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}

	
	h0 += a;
	h1 += b;
	h2 += c;
	h3 += d;
	h4 += e;
	h5 += f;
	h6 += g;
	h7 += h;

	printf("h0 %x\n", h0);
	printf("h1 %x\n", h1);
	printf("h2 %x\n", h2);
	printf("h3 %x\n", h3);
	printf("h4 %x\n", h4);
	printf("h5 %x\n", h5);
	printf("h6 %x\n", h6);
	printf("h7 %x\n", h7);
	printf("final hash: 0x%x", h0);
	printf("%x", h1);
	printf("%x", h2);
	printf("%x", h3);
	printf("%x", h4);
	printf("%x", h5);
	printf("%x", h6);
	printf("%x", h7);
	printf("\n");

	return 0;
}
