#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include <sys/stat.h>


// Computes the Shannon entropy of a file given the unnormalized distribution over ngrams
double H(unsigned int* un_P_X, unsigned int n, double array_sum) {
    int i;
    double P_X_i = 0.;
    double delta = 0.;
    double entropy = 0.;     

    if (array_sum == 0.) {
        return 0.; // there was no variation
    }
    // H = - SUM_i_1-n (P(X_i)*log_2(P(X_i)))
    for (i = 0; i < n; i++) {
        if (!un_P_X[i]) {
            continue;
        }
        P_X_i = un_P_X[i] / array_sum;
        delta = P_X_i * log2(P_X_i);
        entropy -= delta;
    }
    return entropy;
}


int main(int argc, char** argv) {
    char* file_path; // path to file to be measured 
    FILE* file_fs; // file struct of file to be measured
    int current_byte; // current byte in file
    struct stat file_stat;
    unsigned int P_X[256]; // unnormalized probability mass distribution 
    memset(P_X, 0, sizeof(P_X));

    // validate command-line arguments
    if (argc != 2){
        printf("Usage: %s <file-path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    file_path = argv[1];

    // open file and get file stat
    if (!(file_fs = fopen(file_path, "r"))) {
        fprintf(stderr,"[!] Error, could not open file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
   
    if (fstat(fileno(file_fs), &file_stat) == -1) {
        fprintf(stderr,"[!] Error, could not get file size: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // read file and populate probability mass array
    while((current_byte = fgetc(file_fs)) != EOF) {
        P_X[current_byte]++;
    }

    // compute entropy and print
    printf("%f\n", H(P_X, 256, (double) file_stat.st_size));
     
    exit(EXIT_SUCCESS);
}


