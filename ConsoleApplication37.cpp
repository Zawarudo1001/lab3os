#include <iostream>
#include <mutex>
#include <thread>
#include <windows.h>
#include <queue>
#include <vector>
#include <random>

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> dist(1, 100);
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class ConsumerProducerModel {
public:
	mutex convair_lock;
	queue<int> products;

	bool StopConvair;
	bool ProduсerFlag;
	const int convair_size = 200;
	const int ProducerCount = 3;
	const int ConsumerCount = 2;

	vector<thread> Producers;
	vector<thread> Consumers;


	void produce() {
		while (!StopConvair) {
			if (ProduсerFlag == false) {
				unique_lock<mutex> ulock(convair_lock);
				int elem = dist(gen);
				products.push(elem);
				SetConsoleTextAttribute(hConsole, 10);
				cout << "На конвейер был добавлен элемент " << elem << " всего элементов: " << products.size() << endl;
				ulock.unlock();
				Sleep(500);
			}
		}
	}

	void consume() {
		while (products.size() > 0) {
			unique_lock<mutex> ulock(convair_lock);
			int elem = products.front();
			products.pop();
			SetConsoleTextAttribute(hConsole, 12);
			cout << "С конвейера был взят элемент: " << elem << " всего элементов: " << products.size() << endl;
			ulock.unlock();
			Sleep(500);
		}
	}
};

class Program : ConsumerProducerModel{
public:
	void startSimulation() {

		for (int i = 0; i < ProducerCount; i++) {
			Producers.push_back(thread([&]() {produce(); }));
		}

		for (int i = 0; i < ConsumerCount; i++) {
			Consumers.push_back(thread([&]() {consume(); }));
		}

		while (!GetAsyncKeyState('Q')) {
			(products.size() >= 100 and products.size() > 80) ? ProduсerFlag = true : ProduсerFlag = false;
		}

		StopConvair = true;

		for (int i = 0; i < ProducerCount; i++) {
			if (Producers.at(i).joinable()) Producers.at(i).join();
		}

		for (int i = 0; i < ConsumerCount; i++) {
			if (Consumers.at(i).joinable()) Consumers.at(i).join();
		}
	};
};

int main() {
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	Program MainProcess;

	MainProcess.startSimulation();

	SetConsoleTextAttribute(hConsole, 7);
	return 0;
}