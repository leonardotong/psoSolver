#include "PSO.h"
// ======================== Definitions for PSO ======================== //
CPSO::~CPSO()
{
	if (Population)
	{
		W = 0.;
		C1 = 0.;
		C2 = 0.;
		for (int i = 0; i < (int)particle.size(); ++i)
			particle[i].~CParticle();
		particle.clear();
		particle.shrink_to_fit();
	}
}

void CPSO::DebugPlotPopulation(char *filename)
{
	ofstream file(filename);
	int pop = Population;
	int dim = Dimension;
	int num = nObj;
	for (int i = 0; i < pop; ++i)
	{
		for (int n = 0; n < num; ++n)
			file << particle[i].fitness[n] << " \t";
		for (int n = 0; n < num; ++n)
			file << particle[i].bestfit[n] << " \t";
		for (int j = 0; j < dim; ++j)
			file << particle[i].position[j] << " \t";
		for (int j = 0; j < dim; ++j)
			file << particle[i].bestpos[j] << " \t";
		file << particle[i].violation<< endl;
		/*if (i == gBest[i])
			file << 1 << endl;
		else
			file << 0 << endl;*/
	}

	file.close();

}

/*
/* initialize the population
/* 1. random distribution over entire design space
/* 2. random initial velocity
/* 3. use initial solution as the pbest
*/
void CPSO::initialize(objfun f, confun g)
{
	if (particle.empty())
	{
		cerr << "Empty population found when initializing..." << endl;
		return;
	}
		
	// random seed
	static int kk = (unsigned)time(NULL);
	srand((unsigned)time(NULL) + kk++);

	int k = 0;	// set gbest index

	for (int i = 0; i < Population; ++i) 
	{
		for (int j = 0; j < Dimension; ++j) 
		{
			// random distribution over entire design space
			double r1 = rand() / (double)RAND_MAX;
			double r2 = rand() / (double)RAND_MAX;
			double p = r2 + 1.e-3*r1;
			p = p < 0. ? 0. : (p > 1. ? 1. : p);

			// use initial solution as the pbest
			particle[i].bestpos[j] = particle[i].position[j] = p*(UB[j] - LB[j]) + LB[j];		

			// randomly intialize particle velocities
			double r = rand() / (long double)RAND_MAX;
			double maxvel = UB[j] - LB[j];
			if (r > 0.5)
				particle[i].velocity[j] = rand() / (long double)RAND_MAX*maxvel - maxvel / 2.;	
			else
				particle[i].velocity[j] = -rand() / (long double)RAND_MAX*maxvel + maxvel / 2.;

		} // end for loop j

		// calculate initial objective fucntion values
		particle[i].fitness = calculate_fitness(f, particle[i].position);
		// use initial solution as the pbest
		particle[i].bestfit = particle[i].fitness;
		
		// checking constraint(s)
		if (g == NULL)
			particle[i].violation = 0.;
		else {
			vector<double> vio;
			vio = calculate_violation(g, particle[i].position);
			int lenvio = (int)vio.size();
			for (int l = 0; l < lenvio; ++l)
			{
				vio[l] = vio[l] < 0. ? 0. : vio[l];
				particle[i].violation += vio[l];
			}
		}

		// non-dominance based comparison
		// returns 1 - solu i is better, 2 - solu k is better, 0 - nondominant
		if (i != k)
			k = solucmp(particle[i], particle[k]) == 1 ? i : k;
		
	} // end for loop i

	// for single objective optimization problem
	gBest.resize(Population, k);

	// debug: initialize
	//DebugPlotPopulation("debug_initialize.dat");
}

/* 
/* Modified by Leonardo Tong on Feb 12, 2016
/* update pbest based on individual particle history 
/* simultanesouly update:
/*		1. objective function value
/*		2. net violation if applicable
/*		3. if better solution found:
/*			a. best position
/*			b. best fitness
*/
void CPSO::pbest_update(objfun f, confun g, CParticle &p)
{
	// candidate solution
	vector<double> x = p.position;

	// update fitness function
	p.fitness = calculate_fitness(f, x);

	// use ZERO if no constraints
	double v1 = 0.;
	// otherwise calculating constraints 
	// based on candidate solution
	if (g != NULL) {
		vector<double> v = calculate_violation(g, x);
		// number of constraints
		int lenvio = (int)v.size();
		for (int l = 0; l < lenvio; ++l)
		{
			v[l] = v[l] < 0. ? 0. : v[l];
			v1 += v[l];
		}
	}
	// assigning the recorded net violation
	// corresponding to the pbest
	double v2 = p.violation;
	
	// both infeasible
	if (v1 >= EPS && v2 >= EPS)
		p.bestpos = v1 < v2 ? p.position : p.bestpos;
	/*
	/* v1 infeasible, v2 feasible
	/* else if (v1 >= EPS && v2 < EPS)
		/* p.bestfit = p.bestfit;
	*/
	// v1 feasible, v2 infeasible
	else if (v1 < EPS && v2 >= EPS)
		p.bestpos = p.position;
	// both feasible
	else if (v1 < EPS && v2 < EPS) 
	{
		// f1 < f2
		if ( TOOLS::vectorcmp(p.fitness, p.bestfit, false) == 1 )
		{
			// smaller fitness
			p.bestpos = p.position;
			p.bestfit = p.fitness;
		}

	}

	// update current net_violation
	p.violation = v1;
}



// update gbest based on individual particle history
void CPSO::gbest_update(void)
{
	if (!Population)
		return;

	int k = 0;

	for (int i = 0; i < Population; ++i) 
	{
		if (i == k)
			continue;
		// non-dominance based comparison
		k = solucmp(particle[i], particle[k]) == 1 ? i : k;
	}

	for (int i = 0; i < Population; ++i)
		gBest[i] = k;

}

// update swarm: velocity & position updates
void CPSO::update_swarm(objfun f, confun g)
{
	if (particle.empty())
		return;

	// random seed
	static int kk = (unsigned)time(NULL);
	srand((unsigned)time(NULL) + kk++);

	for (int i = 0; i < Population; ++i) {
		
		long double vio = 0.;	// net violation
		// velocity update for each particle-i
		for (int j = 0; j < Dimension; ++j) {

			double t1 = W*particle[i].velocity[j];
			double r1 = rand() / (double)RAND_MAX;
			double t2 = r1*C1*(particle[i].bestpos[j] - particle[i].position[j]);
			double r2 = rand() / (double)RAND_MAX;
			double t3 = r2*C2*(particle[gBest[0]].bestpos[j] - particle[i].position[j]);
			particle[i].velocity[j] = t1 + t2 + t3;
			// position update at each direction-j
			particle[i].position[j] += particle[i].velocity[j];
			// applying boundary constraints
			// to avoid over-fly
			particle[i].position[j] = 
				particle[i].position[j] > UB[j] ? 
				UB[j] : (particle[i].position[j] < LB[j] ? LB[j] : particle[i].position[j]);

		}
		// for each particle-i
		pbest_update(f, g, particle[i]);
	}

	gbest_update( );
}


bool &CPSO::evolution(objfun f, confun g)
{
	solver_summary(g);
	
	int iter = Iteration;

	initialize(f,g);
	
	bool report = false;
	
	for (int i = 0; i < Iteration; ++i) 
	{
		update_swarm(f, g);

		report = rep(i + 1, gBest[0], particle[gBest[0]].bestfit[0], particle[gBest[0]].violation);
		if (!report)
			break;
	}

	return report;
}

void CPSO::solver_summary(confun g) {

	cout << "# of objective = " << nObj << endl;
	cout << "# of decision variables = " << Dimension << endl;
	cout << "Constrained: ";
	if (g == NULL)
		cout << "No" << endl;
	else
		cout << "Yes" << endl;

	cout << "Preparing Single-Objective Particle Swarm Optimization Solver" << endl;
	cout << "Popsize = " << Population << endl;
	cout << "C1 = " << C1 << ", C2 = " << C2 << endl;
}

void CPSO::plotGlobalBestSolu(vector<vector<double>> &fopt, vector<vector<double>> &xopt)
{
	if (!Population)
		return;
	int dim = Dimension;
	int nobj = nObj;
	// single-objective
	int nsolu = 0;
	if (nobj == 1)
		nsolu = 1;
	fopt.resize(nsolu);
	xopt.resize(nsolu);
	
	for (int n = 0; n < nsolu; ++n) 
	{
		fopt[n].resize(nobj);
		xopt[n].resize(dim);
	}
	
	if (nsolu == 1) {
		int best_indx = gBest[0];
		fopt[0] = particle[best_indx].bestfit;
		xopt[0] = particle[best_indx].bestpos;
	}
	else 
	{
		cerr << "there cannot be more than 1 solution for single-objective optimization" << endl;
		exit(1);
	}

}


int CPSO::solucmp(CParticle p1, CParticle p2)
{
	int flag = -1;
	
	int nobj = p1.nobj;
	
	if (p2.nobj != nobj) {
		cerr << "unmatched number of objectives found in solucmp" << endl;
		exit(1);
	}
	double v1 = p1.violation, v2 = p2.violation;
	

	if (v1 >= EPS && v2 >= EPS) // both infeasible
		flag = v1 < v2 ? 1 : 2;
	else if (v1 >= EPS && v2 < EPS)
		flag = 2;
	else if (v1 < EPS && v2 >= EPS)
		flag = 1;
	else if (v1 < EPS && v2 < EPS) {
		vector<double> f1, f2;
		f1.resize(nobj);
		f2.resize(nobj);
		f1 = p1.bestfit, f2 = p2.bestfit;
		// 0 - non-dominated
		// 1 - f1 is better
		// 2 - f2 is better
		flag = TOOLS::vectorcmp(f1, f2);
	}

	if (flag == -1) {
		cerr << "return value has not been assigned" << endl;
		exit(1);
	}

	return flag;

}


bool pso_report(int iter, int ind, double bestfit, double vio)
{
	cout << "#" << iter << ", gbest: " << ind << ", bestfit: ";
	cout << bestfit << " net_violation is: " << vio << endl;
	return true;
}