#include"Source.h"

using namespace std;

Source::Source(int id, elem_type type) :Element(id, type)
{

}

Source::~Source()
{
}

Periodic::Periodic(int id, elem_type type, double frequency) :Source(id, type), frequency_(frequency)
{
}

Periodic::~Periodic()
{
	frequency_ = 0;
}

double Periodic::getFrequency()
{
	return frequency_;
}

void Periodic::processLogic(double time)
{
	int cnt = 0;
	double half_period = 1 / (2 * frequency_), sum = 0;

	if (time == 0)
	{
		curr_state_ = INACTIVE;
		return;
	}

	while (sum <= time)
	{
		cnt++;
		sum += half_period;
	}

	if (cnt % 2 == 1)
		curr_state_ = INACTIVE;
	else
		curr_state_ = ACTIVE;
}

Aperiodic::Aperiodic(int id, elem_type type, int num_of_toggles, vector<double> & moments_of_toggles) :Source(id, type)
{
	num_of_toggles_ = num_of_toggles;
	moments_of_toggles_ = moments_of_toggles;
}

Aperiodic::~Aperiodic()
{

}

int Aperiodic::getNumOfToggles()
{
	return num_of_toggles_;
}

vector<double>& Aperiodic::getMomentsOfToggles()
{
	return moments_of_toggles_;
}

void Aperiodic::processLogic(double time)
{
	int cnt = 0;
	if (!num_of_toggles_)
	{
		curr_state_ = INACTIVE;
		return;
	}
	else
	{
		double sum = 0;
		for (auto moment : moments_of_toggles_)
		{
			sum += moment;
			if (sum <= time)
				cnt++;
			else
				break;
		}
	}
	if (cnt % 2 == 0)
		curr_state_ = INACTIVE;
	else
		curr_state_ = ACTIVE;
}
