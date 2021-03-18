#ifndef _SOURCE_H_
#define _SOURCE_H_

#include"Element.h"

using namespace std;

class Source :public Element
{
public:

	Source(int id, elem_type type);
	~Source();

	Source(const Source& other) = delete;
	Source(Source&& other) = delete;

private:
};

class Periodic :public Source
{
public:
	Periodic(int id, elem_type type, double frequency);
	~Periodic();

	Periodic(const Periodic& other) = delete;
	Periodic(Periodic&& other) = delete;

	double getFrequency();

	virtual void processLogic(double time) override;

private:
	double frequency_;
};

class Aperiodic :public Source
{
public:
	Aperiodic(int id, elem_type type, int num_of_toggles, vector<double> & moments_of_toggles);
	~Aperiodic();

	int getNumOfToggles();
	vector<double>& getMomentsOfToggles();

	Aperiodic(const Aperiodic& other) = delete;
	Aperiodic(Aperiodic&& other) = delete;

	virtual void processLogic(double time) override;

private:
	int num_of_toggles_;
	vector<double> moments_of_toggles_;
};

#endif // !_SOURCE_H_

