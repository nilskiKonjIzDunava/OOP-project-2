#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include"Circuit.h"
#include <string>
#include<vector>

using namespace std;

// sadrzi uneta digitalna kola, moguce je paralelno obradjivanje
class Simulator
{
public:
	Simulator();
	~Simulator();
	Simulator(const Simulator& other) = delete;
	Simulator(Simulator&& other) = delete;

	void loadCircuit(const string& filepath);
	void simulate(const string& filepath);

private:
	vector<Circuit*> networks_of_digital_circuits_;
};
#endif // !_SIMULATOR_H_
