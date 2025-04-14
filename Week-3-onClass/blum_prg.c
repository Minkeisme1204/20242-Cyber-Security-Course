#include <stdio.h>
#include <math.h>

#define PRIME_P 23
#define PRIME_Q 7
#define MAXN 1000000

void blum_random(int seed) {
    int p = PRIME_P;
    int q = PRIME_Q;
    int n = p * q;
    int state = (seed * seed) % n;
    FILE *file = fopen("random.txt", "w");
    char char_i = 0;   
    for (int i = 0; i < MAXN; i++) {
        int bit = state % 2;

        char data = bit * pow(2, 8 - char_i); 
        char_i++;
	
        state = (state*state) % n; 

        if (char_i == 8) {
            fprintf(file, "%c", data);
            printf ("written data: %d", data);
            data = 0; 
            char_i = 0;
        }
    }
}

int main(int argc, char **argv) {
    blum_random(121204);
    return 0; 
}
