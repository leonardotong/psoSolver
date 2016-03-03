#pragma once
#ifndef _CPSO_H_
#define _CPSO_H_

#ifdef PSODLL  
#define PSOAPI _declspec(dllexport)  
#else  
#define PSOAPI _declspec(dllimport)  
#endif

# include "..\include\Tools.h"

// ======================== Declaration for Particle ======================== //
// define class for particles
class CParticle
{
public:
	CParticle();	// default constructor
	CParticle(int dim, int obj);	// constructor
	~CParticle();

	int
		dimension, nobj;

	double
		crow_dist, violation;

	vector<double>
		position
		, bestpos
		, fitness
		, bestfit
		, velocity;

	bool
		LocalFlag, GlobalFlag;

	void set_dimension(int dim, int obj);
};

// ======================== Declaration for CSwarm ======================== //

typedef vector<double>(*objfun) (vector<double> x);
typedef vector<double>(*confun) (vector<double> x);

// declare namespace for abstract class: CSwarm
namespace SWARM {
	
	// define variable types
	enum var_type {
		continuous,
		integer,
		discrete
	};

	// Base class for swarm-based optimization solvers
	class CSwarm
	{
	protected:	// private attributes

		int
			Dimension		// dimension
			, nObj			// # of objective
			, Population	// popsize
			, Iteration		// # of iteration
			, MaxEval;		// max # of function evaluation

		vector<int>  gBest;		// index of the global best solution(s)
		vector<int>  vartype;	// variable type
		vector<double> LB, UB;	// lower & upper bounds

		// private methods:
		/* private constructor
		* prevent generating instance from
		* abstract class
		*/
		CSwarm();
		CSwarm(int dim, int obj, int pop, int iter);

		// optimization proceeding
		virtual void update_swarm(objfun f, confun g) = 0;

		// ======= this is to be defined in another class =======
		int integer_handler(double p);
		int integer_handler(double p, double r);
		// ================================================


		vector<double> calculate_fitness(objfun f, vector<double> x) {
			return f(x);
		};

		vector<double> calculate_violation(confun g, vector<double> x) {
			return g(x);
		};

		// to display iteration details
		bool(*rep) (int, int, double, double);	// for single obj
		bool(*mrep) (int, int);					// for multi obj


	public:	// shared methods
		virtual ~CSwarm();
		//virtual void setoptions(string str, double val);

		// specify variable types
		void set_vartype(int t);	// for identical variable type
		void set_vartype(vector<int> t);	// for multiple variable types

		// specify lower & upper bounds
		void set_bounds(double lb, double ub);	// for identical bounds
		void set_bounds(vector<double> lb, vector<double> ub);	// for multiple bounds

		// specify popsize
		void set_population(int pop) { Population = pop; };

		// specify number of iterations
		void set_iteration(int iter) { Iteration = iter; };

		// specify the maximum allowed function evaluations
		void set_max_eval(int eval) { MaxEval = eval; };

		void display(void *p);

	};

}; // namespace of SWARM


class CPSO :
	public SWARM::CSwarm
{
			/* Inheritted attributes from Swarm:
				Dimension		// dimension
				, nObj			// # of objective
				, Population	// popsize
				, Iteration		// # of iteration
				, MaxEval;		// max # of function evaluation

				vector<int> gBest;	// index of the global best solution(s)
				vector<int> vartype;	// variable type
				vector<double> LB, UB;	// lower & upper bounds
				*/
protected:
	
	vector<CParticle> particle;	// basic PSO attributes
		
	// basic user-define parameters for PSO	
	double
		W		// inertia weight
		, C1	// cognitive parameter
		, C2;	// social parameter

	/* methods in basic PSO, which are to be inheritted
	 * inheritted methods from Swarm.h:
			
		virtual ~CSwarm();

		 // specify variable types
		 void set_vartype(int t);	// for identical variable type
		 void set_vartype(vector<int> t);	// for multiple variable types

		 // specify lower & upper bounds
		 void set_bounds(double lb, double ub);	// for identical bounds
		 void set_bounds(vector<double> lb, vector<double> ub);	// for multiple bounds

		 // specify popsize
		 void set_population(int pop) { Population = pop; };

		 // specify number of iterations
		 void set_iteration(int iter) { Iteration = iter; };

		 // specify the maximum allowed function evaluations
			void set_max_eval(int eval) { MaxEval = eval; };
			void display(void *p);
		*/

	// report summary of the current solver
	virtual void solver_summary(confun g);
		
	// initialize the swarm
	virtual void initialize(objfun f, confun g);

	// update the swarm status: position & velocity
	virtual void update_swarm(objfun f, confun g);

	// update pbest (individual history) 
	virtual void pbest_update(objfun f, confun g, CParticle &_particle);
		
	// update gbest (global best solution)
	virtual void gbest_update(void);

	// plot for debugging
	void DebugPlotPopulation(char*);

public:
	// methods that can be shared among solvers
	

	
	// default constructor
	CPSO( ) : CSwarm() {
		// using default values for W, C1, and C2
		W  = 0.;
		C1 = 0.;
		C2 = 0.;
	};

	// initialize constructor for CPSO
	CPSO(int dim, int obj, int pop, int iter) : CSwarm(dim, obj, pop, iter) {
		// using default values for W, C1, and C2
		W = 1.;
		C1 = 2.;
		C2 = 2.;
		
		/* create initial population
		 * based on given popsize, dim, and nobj
		 */
		particle.resize(Population);
		for (int i = 0; i < Population; ++i)
			particle[i].set_dimension(Dimension, nObj);
	};
	// destructor
	virtual ~CPSO();
	// options to modify private variables
	void set_inertia_weight(double w) { W = w; };
	void set_cognitive(double c1) { C1 = c1; };
	void set_social(double c2) { C2 = c2; };

	// perform iteration
	virtual bool& evolution(objfun f, confun g);
	
	// plot the global best solution
	virtual void plotGlobalBestSolu(vector<vector<double>> &fopt, vector<vector<double>> &xopt);

	// non-dominance based solution comparison
	virtual int solucmp(CParticle p1, CParticle p2);

};

/* iteration details
 * displaying:
 * iter# | solution index | gbest | net violation
 */
bool pso_report(int iter, int ind, double bestfit, double vio);

#endif