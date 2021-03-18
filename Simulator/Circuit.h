#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

#include"CombinationalElement.h"
#include"Element.h"
#include"Exceptions.h"
#include"Scope.h"
#include"Source.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include<string>
#include<stack>
#include<queue>

using namespace std;

// predstavlja "n-arno stablo" widh benefits
// sadrzi sve elementne kola i kriticne trenutke - jednistvenu bazu za sve generatore, hronoloski poredjani
class Circuit
{
public:
	Circuit();
	~Circuit();

	Circuit(const Circuit& other);
	Circuit(Circuit&& other);

	void loadOneCircuit(const string& filepath);
	void simulateCircuit(const string& filepath);
	void writeToFile(const string& filepath, int scope_id, vector<Element::state>& output);

	vector<double>& getCriticalMoments();

private:
	vector<Element*> digital_circuit_;
	vector<double> critical_moments_;
	double duration_;
	int num_of_elem_;
};
#endif // !_CIRCUIT_H_


