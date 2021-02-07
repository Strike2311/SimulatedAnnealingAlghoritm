// PEA0.cpp : Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.
//
#include <chrono> 
#include <iostream>
#include <fstream>
#include <string>
#include<time.h>
#include<stdlib.h>
using namespace std;
using namespace std::chrono;


int** edgeMatrix(ifstream&, int, int**); // zwraca macierz krawêdzi w tabeli 2 wymiarowej
ifstream readFile(); // wczytuje plik
int* nearestNeighbour(int**, int, int); // wykonuje algorytm NN dla podanego elem. startowego oraz liczy PRD
double calculatePRD(int, int);
int calculateTour(int**, int, int*);
int* swap_random(int*, int);
void simulatedA(double, double, int**, int, int, double);


int main()
{
	srand(time(NULL));

	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	string data;

	ifstream file;

	int n, optimum;
	file = readFile(); //wczytanie wybranego pliku
	file >> data;     //wczytanie liczby miast z pliku

	n = stoi(data); // zmiana liczby miast ze string na int

	int** matrix = new int* [n];//tworzenie dynamicznej tablicy 2 wymiarowej o wymiarach n x n
	for (int i = 0; i < n; i++) { //do któej wprowadzimy macierz krawêdzi
		matrix[i] = new int[n];
	}
	matrix = edgeMatrix(file, n, matrix);// wczytanie macierzy z pliku
	system("PAUSE");

	file >> optimum;

	double init_temp, min_temp, coolingRate;

	cout << "Please enter following parameters:\n";
	cout << "Initial temperature: ";
	cin >> init_temp;
	cout << "\nMinimal temperature: ";
	cin >> min_temp;
	cout << "\nCooling rate: ";
	cin >> coolingRate;

	start = high_resolution_clock::now();
	simulatedA(init_temp, min_temp, matrix, optimum, n, coolingRate);
	stop = high_resolution_clock::now();

	duration = duration_cast<microseconds>(stop - start);

	cout << "Calculation time: " << (double)duration.count() / 1000 << "ms" << endl;






	file.close();
	system("PAUSE");
	delete matrix;
	return 0;
}

ifstream readFile() //wczytuje podany plik
{
	string name, data;

	cout << "Enter the name of the instance you wish to read:\n";
	cin >> name;

	ifstream file(name);

	file >> data; //wczytanie nazwy pliku zawartej w œrodku

	if (data != name) //sprawdza czy nazwa podana przez u¿ytkownika jest taka sama jak nazwa pliku który zosta³ wczytany
		cout << "Error: Couldn't read the file!\n";

	else
		cout << "File loaded succesfully!\n";

	return file;
}


int** edgeMatrix(ifstream& file, int n, int** matrix) {

	string data;

	for (int i = 0; i < n; i++) {      //wype³nienie tabeli 2D macierz¹ krawêdzi
		for (int j = 0; j < n; j++) {
			file >> data;
			matrix[i][j] = stoi(data);      //tworzenie macierzy, stoi(data)-konwersja string do int
		}
	}

	return matrix;
}



int* nearestNeighbour(int** matrix, int n, int optimum) {

	int* met = new int[n]; //dynamiczna tablica na ju¿ sprawdzone elementy
	int* toCheck = new int[n]; //dynamiczna tablica z elementami które jeszcze s¹ do sprawdzenia

	for (int i = 0; i < n; i++) {//wype³nienie tablicy 1 - ¿aden element nie zosta³ sprawony
		toCheck[i] = 1;			 //gdy sprawdzi to w miejsce 1 wstawia 0
	}

	int curr, min = 1000, minPos, hamilton = 0;
	curr = 0;				//element startowy
	met[0] = curr;				//elem startowy doodany do odwiedzonych
	toCheck[curr] = 0;			//elem startowy usuwany z tablicy do sprawdzenia
	for (int j = 1; j < n; j++) { //pêtla idzie po kolei przez macierz krawedzi i szuka min. dla kolejnych elementów

		min = INT_MAX;

		for (int i = 0; i < n; i++) {
			if (matrix[curr][i] < min && toCheck[i] != 0) {
				min = matrix[curr][i];
				minPos = i;

			}
		}
		toCheck[minPos] = 0;
		met[j] = minPos;
		hamilton += min;
		curr = minPos;
	}

	hamilton += matrix[met[0]][minPos];


	//	for (int i = 0; i < n; i++) {
		//	cout << met[i] << " ";
		//}
	cout << endl;

	//cout << "Calculated Hamilton value: " << hamilton << "   PRD: " << calculatePRD(hamilton, optimum) << "%\n";

	return met;
}


double calculatePRD(int hamilton, int optimum)
{
	double prd = 100 * (double)hamilton / (double)optimum;
	return prd;
}



int calculateTour(int** matrix, int n, int* result_matrix) {
	int tour = 0;
	for (int i = 1; i < n; i++) {
		tour += matrix[result_matrix[i - 1]][result_matrix[i]];
	}
	tour += matrix[result_matrix[n - 1]][result_matrix[0]];
	return tour;
}
int* swap_random(int* base, int n) {
	//generuje 2 ró¿ne liczby losowe
	int rand_x = rand() % n;
	int rand_y = rand() % n;
	int* temp = new int[n];


	//zapisuje wartosci z base do temp
	for (int i = 0; i < n; i++) {
		temp[i] = base[i];
	}
	//zamienia wartosci o 2 losowych indeksach w temp
	temp[rand_x] = base[rand_y];
	temp[rand_y] = base[rand_x];
	return temp;



}

int* invert_random(int* base, int n) {
	int rand_x = rand() % n;
	int rand_y = rand() % n;
	int* temp = new int[n];

	//zapisuje wartosci z base do temp
	for (int i = 0; i < n; i++) {
		temp[i] = base[i];
	}
	//jesli Y wieksze od X, to iteruje od X do Y jednoczeœniej id¹c od koñca indeksem j
	if (rand_y > rand_x) {
		int j = rand_y;
		for (int i = rand_x; i <= rand_y; i++) {
			temp[i] = base[j];
			j--;
		}
	}
	//jeœli X > Y to iteruje od Y do X jednoczeœnie id¹c od koñca indeksem j
	else {
		int j = rand_x;
		for (int i = rand_y; i <= rand_x; i++) {
			temp[i] = base[j];
			j--;
		}
	}

	return temp;
}

int* insert_random(int* base, int n) {
	int rand_x = rand() % n;
	int rand_y = rand() % n;
	int* temp = new int[n];

	//zapisuje wartosci z base do temp
	for (int i = 0; i < n; i++) {
		temp[i] = base[i];
	}

	int index = temp[rand_x];
	if (rand_y > rand_x) {
		for (int i = rand_x; i < rand_y; i++) {
			temp[i] = temp[i + 1];
		}
		temp[rand_y] = index;
	}
	else {
		for (int i = rand_x; i > rand_y; i--) {
			temp[i] = temp[i - 1];
		}
		temp[rand_y] = index;
	}

	return temp;
}

int* neighbour(int* base, int n, int** matrix) {
	int* swap = new int[n];
	int* invert = new int[n];
	int* insert = new int[n];
	int swap_tour, invert_tour, insert_tour;
	swap = swap_random(base, n);
	invert = invert_random(base, n);
	insert = insert_random(base, n);
	swap_tour = calculateTour(matrix, n, swap);
	invert_tour = calculateTour(matrix, n, invert);
	insert_tour = calculateTour(matrix, n, insert);
	if (swap_tour < invert_tour && swap_tour < insert_tour) { //swap ma najmniejszy koszt
		return swap;
	}
	else if (invert_tour < swap_tour && invert_tour < insert_tour) { //invert ma najmniejszy koszt
		return invert;
	}
	else if (insert_tour < swap_tour && insert_tour < invert_tour) {//insert ma najmniejszy koszt
		return insert;
	}

	else if (insert_tour == swap_tour && insert_tour < invert_tour) { //insert i swap maja najmniejszy, równy koszt
		return insert;
	}
	else if (invert_tour == swap_tour && invert_tour < insert_tour) {// invert i swap maja najmniejszy, równy koszt
		return invert;
	}
	else if (invert_tour == insert_tour && insert_tour < swap_tour) { //invert i insert maj¹ najmniejszy, równy koszt
		return insert;
	}
	else return swap;
}

void simulatedA(double init_temp, double min_temp, int** matrix, int optimal, int n, double coolingRate) {
	int iteration = 0;
	int* finalTour = new int[n]; //ostateczna trasa
	int* bestTour = new int[n];
	for (int i = 0; i < n; i++) {
		bestTour[i] = 0;
	}
	bestTour = finalTour = nearestNeighbour(matrix, n, optimal); //wygenerowanie startowego wyniku za pomoc¹ NN

	//liczenie pocz¹tkowej wartoœci trasy
	int bestCost = calculateTour(matrix, n, bestTour);
	double temp = init_temp;
	int* newTour = new int[n];
	int deltaCost;
	double p, c;

	int finalCost = bestCost; //wartoœæ ostatecznej trasy
	cout << "Start " << "  " << finalCost << " (" << calculatePRD(bestCost, optimal) << "%)\n";

	while (temp > min_temp) {
		iteration++;

		newTour = neighbour(bestTour, n, matrix);
		deltaCost = calculateTour(matrix, n, newTour) - calculateTour(matrix, n, bestTour);

		if (deltaCost < 0) {
			for (int i = 0; i < n; i++) {
				bestTour[i] = newTour[i];
			}
		}
		else if (deltaCost >= 0) {
			p = exp(-deltaCost / temp);
			c = (double)rand() / (double)RAND_MAX;
			if (c < p) {
				for (int i = 0; i < n; i++) {
					bestTour[i] = newTour[i];
				}
			}

		}
		bestCost = calculateTour(matrix, n, bestTour);
		if (bestCost < finalCost) {
			finalCost = bestCost;
			for (int i = 0; i < n; i++) {
				finalTour[i] = bestTour[i];
			}
			cout << iteration << "  " << finalCost << " (" << calculatePRD(finalCost, optimal) << "%)\n";
		}
		temp = temp * coolingRate;
	}

	cout << "Best route: " << finalTour[0];

	for (int i = 1; i < n; i++) {
		cout << "->" << finalTour[i];
	}
	cout << endl;
}
