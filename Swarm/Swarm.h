# pragma once

# ifndef _SWARM_H_
# define _SWARM_H_
/*
#	ifdef SwarmDLL_EXPORTS  
#	  define SwarmAPI __declspec(dllexport)  
#	else  
#	  define SwarmAPI __declspec(dllimport)  
#	endif  */

# include "..\include\Tools.h"
#pragma comment(lib,"Tools.lib")

namespace SWARM {

	typedef vector<double>(*objfun) (vector<double> x);
	typedef vector<double>(*confun) (vector<double> x);

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

#endif