#include <stdio.h>
#define KEY_LEN 4
#define MAXN 10

int key[KEY_LEN] = {1, 2, 3, 6}; // Secret key 

int S[256] = {0}; // Streaming key
int T[256] = {0}; // Temporary vector

void init_RC4() {
	for (int i = 0; i < MAXN; i++) {
		S[i] = i;
	       	T[i] = key[i % KEY_LEN];// Fill repeatedly secret key	
	}
	int j = 0;
        for (int i = 0; i < MAXN; i++) {
                j = (j + S[i] + T[i]) % MAXN;
                swap(S[i], S[j]); // Swap S[i] and S[j]
        }
	for (int i = 0; i < 16; i++) { printf("%d ", S[i]);}
}

void genKey_RC4() {
	int j = 0; 
	for (int i = 0; i <= 255; i++) {
    		j = (j + S[i] + T[i]) % 256;
    		swap(S[i], S[j]); // Swap S[i] and S[j]
	}



}

int main(int argc, char **argv) {
	init_RC4();
}

