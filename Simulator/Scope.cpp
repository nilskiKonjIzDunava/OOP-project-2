#include"Scope.h"

using namespace std;

Scope::Scope(int id, elem_type type, int num_of_inputs = 1) :Element(id, type)
{
	inputs_.resize(1);
}

Scope::~Scope()
{
}

void Scope::processLogic(double time)
{
	if (inputs_[0]->getCurrState() == ACTIVE)
		curr_state_ = ACTIVE;
	else
		curr_state_ = INACTIVE;
}