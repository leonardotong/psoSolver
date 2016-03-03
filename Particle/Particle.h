#pragma once
#ifndef _PARTICLE_H
#define _PARTICLE_H
/*
#ifdef ParticleDLL_EXPORTS  
#define ParticleAPI __declspec(dllexport)  
#else  
#define ParticleAPI  __declspec(dllimport)  
#endif  */

# include "..\include\Tools.h"
#pragma comment(lib,"Tools.lib")
namespace PARTICLES {

	// define class for basic particles
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

		std::vector<double>
			position
			, bestpos
			, fitness
			, bestfit
			, velocity;

		bool
			LocalFlag, GlobalFlag;

		void set_dimension(int dim, int obj);
	};

	// define class for particle sets
	class Pset
	{
	public:
		// properties of Pset
		vector<CParticle> hparticles;
		vector<double> span;
		int capacity;
		int indexMinCrwd;	// index[0] of the most crowded solu
		double mincrwd; //  the most crowded solu

		vector<double> crwdDist;	// crowding distance

									// functions/methods for Pset

									// adding a new particle to local/global set
									// cannot exceed capacity
		void append(CParticle p);

		// inserting a new particle to local/global set
		// replace the worst solu 
		// when the capacity is reached
		void insert(CParticle p);

		// removing a specified particle from local/global set
		void remove(int k);

		// return the size of local/global set
		int setSize() { return (int)this->hparticles.size(); };

		// options
		void set_capacity(int cap) { this->capacity = cap; };
		void update();
		Pset();
		~Pset();
		void SetPset(int cap);
	};
}
#endif