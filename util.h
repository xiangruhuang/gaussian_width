#include <iostream>
#include <vector>
#include <random>
//#include <map>
//#include <string>
//#include <iomanip>
#include <time.h>
#include <cassert>
#include <algorithm>

using namespace std;

const double INFI = 1e300;

static default_random_engine generator(time(0));

inline vector<double> randn(int n, double mean, double sigma){

	normal_distribution<double> dist(mean, sigma);

	vector<double> randn;
	for (int i = 0; i < n; ++i) {
		randn.push_back(dist(generator));
	}

	return randn;
}

inline double max(vector<double>& vec){
	double max_val = -INFI;
	for (vector<double>::iterator it = vec.begin(); it != vec.end(); it++){
		if ( (*it) > max_val){
			max_val = *it;
		}
	}
	return max_val;
}

inline double min(vector<double>& vec){
	double min_val = INFI;
	for (vector<double>::iterator it = vec.begin(); it != vec.end(); it++){
		if ((*it) < min_val){
			min_val = *it;
		}
	}
	return min_val;
}

// return the best x that maximize g^T x, s.t. x \in T(n, k) and \|x\|_2 = 1
inline vector<double> oracle(vector<double>& g, int k){
	int n = g.size();
	assert(n >= k && n > 0 && k > 0);
	double l2_norm = 0.0;
	double* sum = new double[n];
	vector<double> x;
	for (int i = 0; i < n; i++){
		if (i >= k)
			g[i] = fabs(g[i]);
		l2_norm += g[i] * g[i];
	}

	l2_norm = sqrt(l2_norm);

	sort(g.begin()+k, g.end(), greater<double>() );
	
	sum[0] = g[0];
	for (int i = 0; i < n; i++){
		if (i > 0)
			sum[i] = sum[i-1] + g[i];
	}
	if (sum[n-1] <= 0.0){
		//x = g / \|g\|_2
		for (vector<double>::iterator it_g = g.begin(); it_g != g.end(); it_g++){
			x.push_back(*it_g / l2_norm);
		}
		return x;
	}
	double current_sum = sum[k-1];
	int t = k;
	for (int i = k; i < n; i++){
		current_sum += g[i];
		double b = -current_sum/t;
		if (b + g[i] >= 0)
			t = (i+1);
		else
			break;
	}
	double b = -sum[t-1]/t;
	double l2_norm_x = 0.0;
	for (int i = 0; i < t; i++){
		x.push_back(g[i] + b);
		l2_norm_x += x.back() * x.back();
	}
	for (int i = 0; i < n-t; i++)
		x.push_back(0.0);
	
	l2_norm_x = sqrt(l2_norm_x);

	if (fabs(l2_norm_x) >= 1e-12){
		for (int i = 0; i < t; i++){
			x[i] /= l2_norm_x;
		}
	}

	delete[] sum;
	
	return x;
}

double dot(vector<double>& a, vector<double>& b){
	double prod = 0.0;
	assert(a.size() == b.size());
	vector<double>::iterator it_a = a.begin(), it_b = b.begin();
	for (; it_a != a.end() && it_b != b.end(); it_a++, it_b++){
		prod += (*it_a) * (*it_b);
	}
	return prod;
}

double width(int n, int k, int nr_samples){
	double avg = 0.0;
	for (int i = 0; i < nr_samples; i++){
		vector<double> g = randn(n, 0.0, 1.0);
		vector<double> x = oracle(g, k);
		avg += dot(g, x);
	}
	return avg/nr_samples;
}
