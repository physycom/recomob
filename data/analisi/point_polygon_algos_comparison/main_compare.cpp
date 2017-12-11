#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <omp.h>
#include <cstring>
#include "pep.h"


using namespace std;
using namespace std::chrono;

#ifdef _WIN32
#define DIR_SEPARATOR '\\'
#else
#define DIR_SEPARATOR '/'
#endif



void usage(char * progname)
{
	vector<string> tokens = split(string(progname), DIR_SEPARATOR);
	cout << "Usage : " << tokens.back() << " n_points min max" << endl;
	cout << "or" << endl;
	cout << "Usage : " << tokens.back() << " n_points min max radius_max" << endl;
	exit(111);
}



int main()
{

	int step = 25000, n = 10;
	double min = 4., max = 47.;
	
	string filename_polygon = "sample_polygon.txt";
	polygon<double> poly(filename_polygon);
	
	ofstream myfile("performances_n" + std::to_string(n) + "_step" + std::to_string(step) + ".txt");

	std::mt19937 gen(123); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<double> dis(min + FLT_MIN, max - FLT_MIN);
	points<double> point;
	myfile << "#Points\tSF\tRC\tWN" << endl;
	
#pragma omp parallel for shared(poly) private(point)
	for (int i = 0; i < n; ++i) 
	{
		double tim1 = 0.;
		double tim2 = 0.;
		double tim3 = 0.;
		point.n_punti = (i+1)*step;
		point.lat = new double[point.n_punti];
		point.lon = new double[point.n_punti];
		for(int j = 0; j < point.n_punti; ++j)
		{
			point.lat[j] = dis(gen);
			point.lon[j] = dis(gen);
		}
		for(int j = 0; j < 100; ++j)
		{
			double start = omp_get_wtime();
					poly.sommafasi(point);
			tim1 += omp_get_wtime() - start;

			start = omp_get_wtime();
					poly.raycast(point); 
			tim2 +=  omp_get_wtime() - start;
 			
 			start = omp_get_wtime();
					poly.wn_PnPoly(point); 
			tim3 +=  omp_get_wtime() - start;
		}
		
		point.n_punti = 0;
		delete[] point.lat; delete[] point.lon;
		#pragma omp critical
		{
			std::cout << (i+1) << std::endl;
			myfile << (i+1)*step << "\t" << tim1/100 << "\t" << tim2/100 << "\t" << tim3/100 << std::endl;
		}
	}
	
	return 0;
}
