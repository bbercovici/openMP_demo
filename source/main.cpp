
#include <iostream>
#include <chrono>
#include <cassert>


void demo_omp(double * average_run_time, int runs) {


	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds ;
	unsigned int N = 10000000;

	// Array assignment without OMP
	int * array = new int[N];

	start = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < N; ++i) {
		array[i] = i;
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	average_run_time[0] += elapsed_seconds.count() / runs;


	// Array assignment with OMP
	int * array_omp = new int[N];
	start = std::chrono::system_clock::now();
	#pragma omp parallel for
	for (unsigned int i = 0; i < N; ++i) {
		array_omp[i] = i;
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	average_run_time[1] += elapsed_seconds.count() / runs;


	// Array summation without OMP
	int sum = 0;
	start = std::chrono::system_clock::now();
	for (unsigned int i = 0; i < N; ++i) {
		sum += array[i];
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	average_run_time[2] += elapsed_seconds.count() / runs;

	// Array summation without OMP
	int sum_omp = 0;
	start = std::chrono::system_clock::now();
	#pragma omp parallel for reduction(+:sum_omp)
	for (unsigned int i = 0; i < N; ++i) {
		sum_omp += array_omp[i];
	}
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	average_run_time[3] += elapsed_seconds.count() / runs;
	assert(sum_omp == sum);


	delete[](array);
	delete[](array_omp);

}





int main() {



# ifdef _OPENMP
	std::cout << "Compiled by an OpenMP-compliant implementation.\n";
# endif

	int runs = 1000;
	double average_run_time[4] = {0, 0, 0, 0};

	for (int i = 0; i < runs; ++i ) {
		demo_omp(average_run_time, runs);
	}


	std::cout << "Average assignement times: " << std::endl;
	std::cout << "\t Without OMP: " << average_run_time[0] << " s" <<  std::endl;
	std::cout << "\t With OMP: " << average_run_time[1] << " s" <<  std::endl;
	if (average_run_time[1] < average_run_time[0]) {
		std::cout << "\t Absolute difference: " << average_run_time[0] - average_run_time[1] << " s in favor of OMP" <<  std::endl;
		std::cout << "\t Relative difference: OMP is " << 100 * abs(average_run_time[1] - average_run_time[0])/average_run_time[0] << " % faster" <<  std::endl;

	}
	std::cout << "Average summation times: " << std::endl;
	std::cout << "\t Without OMP: " << average_run_time[2] << " s" <<  std::endl;
	std::cout << "\t With OMP: " << average_run_time[3] << " s" <<  std::endl;
	if (average_run_time[3] < average_run_time[2]) {
		std::cout << "\t Absolute difference: " << average_run_time[2] - average_run_time[3] << " s in favor of OMP" <<  std::endl;
		std::cout << "\t Relative difference: OMP is " << 100 * abs(average_run_time[3] - average_run_time[2])/average_run_time[2] << " % faster" <<  std::endl;
		
	}




	return 0;
}