/**
 * @file ctr.c
 * @author Ted Krovetz, completed by Daniil Kistanov
 * @date 3 October 2018
 * @brief Encrypts or decrypts stdin to stdout
 * assignment for a cryptography class
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NONCE_LEN 12
#define BLK_LEN 48
#define KEY_LEN 48

void P52(unsigned s[12]);

static void xorArray(unsigned char arrOne[BLK_LEN], unsigned char arrTwo[BLK_LEN]) {
	for(int i = 0; i < BLK_LEN; i++) 
		arrOne[i] = arrOne[i] ^ arrTwo[i];
}

static void P52_BC(unsigned char blk[BLK_LEN], unsigned char key[BLK_LEN]) {
	xorArray(blk, key);
	P52((unsigned *)blk);
	xorArray(blk, key);
}

static void incrementBlock(unsigned char blk[BLK_LEN]) {
	for(int i = 47; i > 12; i--) {
		blk[i] += 1;
		if(blk[i] != 0)
			break;
	}
}


int main(int argc, const char* argv[]) {
	size_t bytes_read;
	unsigned char key[KEY_LEN] = {0};  /* Auto init to all zeros */
	unsigned char nonce[NONCE_LEN];
	unsigned char blk[BLK_LEN];
	unsigned char counter[BLK_LEN] = {0};
	unsigned char temp_blk[BLK_LEN];

	/* Setup key and nonce. Report command line errors */
	if ((argc == 3) && (strlen(argv[1]) == 1) && (strlen(argv[2]) <= KEY_LEN) && ((argv[1][0] == 'e') || (argv[1][0] == 'd'))) {
		if (argv[1][0] == 'e') {
			// part where you read in random nonce
			FILE *rand = fopen("/dev/urandom", "rb");
			fread(nonce, 1, NONCE_LEN, rand);
			fclose(rand);
			fwrite(nonce, 1, NONCE_LEN, stdout);
		} else {
			fread(nonce, 1, NONCE_LEN, stdin);
		}
		//put the nonce and key into blocks
		memcpy(counter, nonce, NONCE_LEN);
		memcpy(key, argv[2], strlen(argv[2]));
	} else {
		fprintf(stderr, "usage: %s d|e <pwd up to 48 chars>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	do {
		bytes_read = fread(blk, 1, BLK_LEN, stdin);  
		if (bytes_read > 0) {
			
			// copy counter into temp value
			memcpy(temp_blk, counter, BLK_LEN);
			
			// run the block through P52-BC
			P52_BC(temp_blk, key);
			
			// xor the result with plaintext
			xorArray(temp_blk, blk);
			
			// increment the counter			
			incrementBlock(counter);
			
			// write the block to output
			fwrite(temp_blk, 1, bytes_read, stdout);	
		}
	} while (bytes_read == BLK_LEN);
	
	return EXIT_SUCCESS;
}
