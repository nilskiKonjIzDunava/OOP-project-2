#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include<iostream>
#include<vector>

using namespace std;

class Element
{
public:
	typedef enum state { INACTIVE, ACTIVE } state;
	typedef enum elem_type { SCOPE, PERIODIC, APERIODIC, NOT, AND, OR } elem_type;

	Element(int id, elem_type type);
	virtual ~Element();

	Element(const Element& other) = delete;
	Element(Element&& other) = delete;

	virtual void processLogic(double time) = 0;

	state getCurrState() const;
	int getId() const;
	elem_type getType() const;
	int getNumOfInputs() const;
	vector<Element*>& getInputs();

protected:
	// sve ovo hocemo da nasledi i objekat izvedenih klasa
	// i da imaju pristip tim poljima roditeljskog objekta u sebi
	int id_;
	elem_type type_;
	int num_of_inputs_ = 1; // podrazumevano 2 za kombinacione, sem za not 
	state curr_state_ = INACTIVE; //podrazumevano 0
	// pokazuju na ulaze datog elementa
	vector<Element*> inputs_;
};
#endif // !_ELEMENT_H_