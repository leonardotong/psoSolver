#include "Swarm.h"

SWARM::CSwarm::CSwarm()
	: Dimension(NULL)
	, nObj(NULL)
	, Population(NULL)
	, Iteration(NULL)
	, MaxEval(NULL)
{
	gBest.resize(Population);
	vartype.resize(Dimension);
	LB.resize(Dimension);
	UB.resize(Dimension);
}

SWARM::CSwarm::CSwarm(int dim, int obj, int pop, int iter)
	: Dimension(dim)
	, nObj(obj)
	, Population(pop)
	, Iteration(iter)
	, MaxEval(pop*iter)
{
	gBest.resize(Population);
	vartype.resize(Dimension);
	LB.resize(Dimension);
	UB.resize(Dimension);
}


SWARM::CSwarm::~CSwarm()
{
	if (Dimension) {
		gBest.clear();
		vartype.clear();
		LB.clear();
		UB.clear();
		gBest.shrink_to_fit();
		vartype.shrink_to_fit();
		LB.shrink_to_fit();
		UB.shrink_to_fit();
	}
	
	if (Population) {
		Population = NULL;
		Iteration = NULL;
		MaxEval = NULL;
	}
}

// uni-type
void SWARM::CSwarm::set_vartype(int t)
{
	if (!Dimension)
		return;

	this->vartype.resize(Dimension, t);
}
// multi-type
void SWARM::CSwarm::set_vartype(vector<int> t)
{
	if (!Dimension)
		return;

	try {
		vartype.resize(Dimension);
		if (t.size() == Dimension) {
			for (int i = 0; i < Dimension; ++i)
				vartype[i] = t[i];
		}
	}
	catch (double) {
		cerr << "set_vartype: input dimension does not match." << endl;
		_getche();
		exit(1);
	}
}

// uni-bound
void SWARM::CSwarm::set_bounds(double lb, double ub)
{
	if (!Dimension)
		return;
	if (lb > ub) {
		cerr << "set_bounds: low bound should not be greater than upper bound." << endl;
		_getche();
		exit(1);
	}
	LB.resize(Dimension, lb);
	UB.resize(Dimension, ub);
}
// multi-bound
void SWARM::CSwarm::set_bounds(vector<double> lb, vector<double> ub)
{
	if (!Dimension)
		return;
	int lenlb = (int)lb.size();
	int lenub = (int)ub.size();

	if (lenlb != Dimension) {
		cerr << "set_bounds: lower bound has wrong dimension." << endl;
		_getche();
		exit(1);
	} else if (lenub != Dimension) {
		cerr << "set_bounds: upper bound has wrong dimension." << endl;
		_getche();
		exit(1);
	}
	for (int d = 0; d < Dimension; ++d) {
		if (lb[d] > ub[d]) {
			cerr << "set_bounds: low bound should not be greater than upper bound." << endl;
			_getche();
			exit(1);
		}
	}

	LB.resize(Dimension);
	UB.resize(Dimension);

	for (int i = 0; i < Dimension; ++i) {
		LB[i] = lb[i];
		UB[i] = ub[i];
	}
		
}

int SWARM::CSwarm::integer_handler(double p)
{
	int res = TOOLS::nearestVertex(p);
	return res;
}
int SWARM::CSwarm::integer_handler(double p, double r)
{
	int res = TOOLS::nearestVertex(p, r);
	return res;
}

void SWARM::CSwarm::display(void *p)
{
	// report 
	// iter # | gbest index | bestfit | net_vio
	if (nObj < 2)
		rep = (bool(*)(int, int, double, double))p;
	else
		mrep = (bool(*)(int, int))p;
}


