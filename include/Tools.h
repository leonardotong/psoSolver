#pragma once
# ifndef _TOOLS_H_
# define _TOOLS_H_
/*
#ifdef ToolsDLL_EXPORTS  
#define ToolsAPI __declspec(dllexport)  
#else  
#define ToolsAPI __declspec(dllimport)  
#endif*/

# include <algorithm>
# include <conio.h>
# include <fstream>
# include <iostream>
# include <iomanip>
# include <math.h>
# include <vector>
# include <stdlib.h>
using namespace std;

// pre-define symbols
# define EPS 1.e-6
# define INF 1.e30
# define PI acos(-1.)
# define TINY 1.e-30

namespace TOOLS{
	// compare two vectors (weakly dominated by default)
	int vectorcmp(vector<double> f1, vector<double> f2);
	/* 
	 * compare two vectors by given flag
	 * true: weakly dominated
	 * false: strongly dominated
	 */
	int vectorcmp(vector<double> f1, vector<double> f2, bool _weaklyDominated);

	// rounding up to the nearest integer
	int nearestVertex(double v);
	// randomly rounding up to ceil or floor integer
	int nearestVertex(double v, double r);

	// return the ranked index (smallest to largest) of a given vector
	vector<int> ranking(vector<double> f);
	// update the ranked index(smallest to largest) of a given vector
	void ranking(vector<double> f, vector<int> &r);

	// compute the euclidean distance of two vectors
	double euclideanDistance(vector<double> f1, vector<double> f2);

}; // end of namespace TOOLS
# endif