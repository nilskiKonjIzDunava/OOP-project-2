#include"CombinationalElement.h"

using namespace std;

CombinationalElement::CombinationalElement(int id, elem_type type, int num_of_inputs) :Element(id, type)
{
	num_of_inputs_ = num_of_inputs;
	inputs_.resize(num_of_inputs_);
}

CombinationalElement::~CombinationalElement()
{
	// implicitno se poziva ~Element()
}

AndGate::AndGate(int id, elem_type type, int num_of_inputs = 2) :CombinationalElement(id, type, num_of_inputs)
{
	num_of_inputs_ = num_of_inputs;
}

AndGate::~AndGate()
{
}

void AndGate::processLogic(double time)
{
	int cnt = 0;
	for (int i = 0; i < num_of_inputs_; i++) if (inputs_[i]->getCurrState() == ACTIVE) cnt++;
	if (cnt == num_of_inputs_) curr_state_ = ACTIVE;
	else curr_state_ = INACTIVE;
}

OrGate::OrGate(int id, elem_type type, int num_of_inputs = 2) :CombinationalElement(id, type, num_of_inputs)
{
}

OrGate::~OrGate()
{
}

void OrGate::processLogic(double time)
{
	int cnt1 = 0;
	for (int i = 0; i < num_of_inputs_; i++)
	{
		if (inputs_[i]->getCurrState() == ACTIVE)
		{
			cnt1++;
		}
		
	}
	if (cnt1) 
		curr_state_ = ACTIVE;
	else curr_state_ = INACTIVE;
	
}

NotGate::NotGate(int id, elem_type type, int num_of_inputs = 1) :CombinationalElement(id, type, num_of_inputs)
{
}

NotGate::~NotGate()
{
}

void NotGate::processLogic(double time)
{
	if (inputs_[0]->getCurrState() == ACTIVE)
		curr_state_ = INACTIVE;
	else
		curr_state_ = ACTIVE;
}