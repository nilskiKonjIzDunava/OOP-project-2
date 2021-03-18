#ifndef _COMBINATIONAL_ELEMENT_
#define _COMBINATIONAL_ELEMENT_

#include"Element.h"


using namespace std;

class CombinationalElement : public Element
{
public:
	CombinationalElement(int id, elem_type type, int num_of_inputs);
	~CombinationalElement(); // ovo ce biti virtuelno jer je virtuelan destruktor osnovne klase, pa ce se isradvo obrisati objekar koji je kreiran na ovaj nacin:
							 // Element* ptr=new CombinationalElement
							 // delete ptr;

	CombinationalElement(const CombinationalElement& other) = delete;
	CombinationalElement(CombinationalElement&& other) = delete;

protected:

};

class AndGate :public CombinationalElement
{
public:
	AndGate(int id, elem_type type, int num_of_inputs);
	~AndGate();

	AndGate(const AndGate& other) = delete;
	AndGate(AndGate&& other) = delete;

	virtual void processLogic(double time) override;
};

class OrGate :public CombinationalElement
{
public:
	OrGate(int id, elem_type type, int num_of_inputs);
	~OrGate();

	OrGate(const OrGate& other) = delete;
	OrGate(OrGate&& other) = delete;

	virtual void processLogic(double time) override;

};

class NotGate :public CombinationalElement
{
public:
	NotGate(int id, elem_type type, int num_of_inputs);
	~NotGate();

	NotGate(const NotGate& other) = delete;
	NotGate(NotGate&& other) = delete;

	virtual void processLogic(double time) override;

protected:

};
#endif // !_COMBINATIONAL_ELEMENT_

