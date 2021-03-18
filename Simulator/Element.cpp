#include"Element.h"

using namespace std;

Element::Element(int id, elem_type type) :id_(id), type_(type)
{
}

Element::~Element()
{
	for (auto e : inputs_)
		e = nullptr;
}

Element::state Element::getCurrState() const
{
	return this->curr_state_;
}

int Element::getId() const
{
	return id_;
}

Element::elem_type Element::getType() const
{
	return type_;
}

int Element::getNumOfInputs() const
{
	return num_of_inputs_;
}


vector<Element*>& Element::getInputs()
{
	return inputs_;
}
