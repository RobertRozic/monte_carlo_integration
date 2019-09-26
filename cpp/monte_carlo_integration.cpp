#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <math.h>
#include "measure_time.h"

// Thread safe random generating
double getRandom(const double & min, const double & max) {
	static thread_local std::mt19937* generator = nullptr;
	if (!generator) generator = new std::mt19937(clock() + std::hash<std::thread::id>()(std::this_thread::get_id()));
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(*generator);
}

// Racunanje integrala - MC
double monteCarlo(double (*func)(double), double a, double b, int n) {

	long double integral = 0, randNum, rez;

	for (int i = 0; i < n; i++) {
 		randNum = getRandom(a, b);
		integral += func(randNum);
	}

	rez = ((b-a)/n)*integral;
	return rez;
}

// Racunanje broja Pi Monte Carlo metodom
long double calculatePi(int n) {
	int circleDarts = 0;
	long double randX, randY, pi;

	srand(time(NULL));

	for (int i = 0; i < n; ++i) {
 		randX = rand() / (double)RAND_MAX;
		randY = rand() / (double)RAND_MAX;
		if (1 > ((randX*randX) + (randY*randY))) ++circleDarts;
	}

	pi = 4 * ((double)circleDarts/(double)n);

	return pi;
}

// Funkcija od koje trazimo integral
double func (double x){
  return pow(x, 2)+7*cos(x);
}

int main() {
	int n = 0, th_num = 0;
	double start, end;

	std::cout << "Unesite velicinu uzorka: \n";
	while(n < 1)
		std::cin >> n;

	std::cout << "Unesite broj racunanja:\n";
	while(th_num <= 0 || th_num > n)
		std::cin >> th_num;

	double long pi = calculatePi(100000);


	double rezs[th_num];

	std::cout << "----------------------------" << std::endl;

	// Sekvencijalno
	start = get_wall_time();
	for(int i=0; i < th_num; i++) {
		rezs[i] = monteCarlo(func, 0, 10, n);
	}
	end = get_wall_time();
	std::cout << "Sekvencijalna izvedba: " << (end - start) << std::endl;

	// Paralelno, koristeci async
	std::future<double> f[th_num];
	start = get_wall_time();
	for(int i=0; i < th_num; i++) {
		f[i] = std::async(&monteCarlo, func, 0, 10, n);
	}

	for(int i=0; i<th_num; i++) {
		rezs[i] = f[i].get();
	}
	end = get_wall_time();
	std::cout << "Paralelna izvedba: " << (end - start) << std::endl;

	// Racunanje prosjecnog rezultata
	double sum = 0;
	for(int i=0; i<th_num; i++) {
		sum += rezs[i];
	}
	double avg = sum/th_num;
	printf("Rezultat: %lf \n", avg);
}
