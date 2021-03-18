#include"Simulator.h"

using namespace std;

Simulator::Simulator()
{
}

Simulator::~Simulator()
{
	for (auto dc : networks_of_digital_circuits_)
	{
		delete dc;
		dc = nullptr;
	}

	networks_of_digital_circuits_.clear();

}

void Simulator::loadCircuit(const string & filepath)
{
	// ako je prethodno ucitana neka mreza, brisu se sve
	if (networks_of_digital_circuits_.size())
	{
		for (Circuit* cir : networks_of_digital_circuits_)
			delete cir;
		networks_of_digital_circuits_.clear();
	}

	Circuit* new_circuit = new Circuit();
	try
	{
		//Circuit* new_circuit = new Circuit();   
		new_circuit->loadOneCircuit(filepath);
		networks_of_digital_circuits_.push_back(new_circuit);
	}

	
	catch (NotUniqueIdException* e)
	{
		delete new_circuit;
		delete e;
	}

	catch (NegativeNumberOfElementsException* e)
	{
		delete new_circuit;
		delete e;
	}
	catch (NegativeFrequencyException* e)
	{
		delete new_circuit;
		delete e;
	}
	catch (NotConnectedPorts* e)
	{
		delete new_circuit;
		delete e;
	}
	catch (AlreadyConnectedPortException* e)
	{
		delete new_circuit;
		delete e;
	}
}

void Simulator::simulate(const string & filepath)
{
	for (auto cir : networks_of_digital_circuits_)
		cir->simulateCircuit(filepath);
}