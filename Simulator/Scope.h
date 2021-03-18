#ifndef _SCOPE_H_
#define _SCOPE_H_

#include"Element.h"

using namespace std;

class Scope :public Element
{
public:
	Scope(int id, elem_type type, int num_of_inputs);
	~Scope();

	Scope(const Scope& other) = delete;
	Scope(Scope&& other) = delete;

	virtual void processLogic(double time) override;
};


#endif // !_SCOPE_H_

