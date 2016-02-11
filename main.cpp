#include <iostream>
#include "util.h"
#include "omp.h"

using namespace std;

int main(int argc, char** argv){
	if (argc < 3+1){
		cerr << "./compute [n] [k] [nr_samples]" << endl;
		exit(0);
	}
	int n = atoi(argv[1]);
	int k = atoi(argv[2]);
	int nr_samples = atoi(argv[3]);
	double overall_time = -omp_get_wtime();
	double w = width(n, k, nr_samples);
	overall_time += omp_get_wtime();
	double r = (n * 1.0)/k;
	cout << "n=" << n << ", k=" << k << ", width=" << w << ", ratio=" << w*w / (k*log(r) + k);
	cout << ", overal_time=" << overall_time;
	cout << endl;
}
