#include "Particle.h"

// ======================== Declaration for Particle ======================== //
// empty particle by default
PARTICLES::CParticle::CParticle()
	: dimension(NULL)
	, nobj(NULL)
	, crow_dist(NULL)
	, violation(NULL)
	, LocalFlag(false)
	, GlobalFlag(false)
{
	position.resize(dimension);
	bestpos.resize(dimension);
	velocity.resize(dimension);
	fitness.resize(nobj);
	bestfit.resize(nobj);
}

PARTICLES::CParticle::CParticle(int dim, int obj)
	: dimension(dim)
	, nobj(obj)
	, crow_dist(NULL)
	, violation(NULL)
	, LocalFlag(false)
	, GlobalFlag(false)
{
	position.resize(dimension);
	bestpos.resize(dimension);
	velocity.resize(dimension);
	fitness.resize(nobj);
	bestfit.resize(nobj);
}

PARTICLES::CParticle::~CParticle()
{
	if ( dimension ) {
		position.clear();
		bestpos.clear();
		velocity.clear();
		position.shrink_to_fit();
		bestpos.shrink_to_fit();
		velocity.shrink_to_fit();
	}

	if ( nobj ) {
		fitness.clear();
		bestfit.clear();
		fitness.shrink_to_fit();
		bestfit.shrink_to_fit();
	}

	crow_dist = 0;
	violation = 0;
	LocalFlag = false;
	GlobalFlag = false;
}

// use dimension and 
// number of objectives to
// initialize particles
void PARTICLES::CParticle::set_dimension(int dim, int obj)
{
	dimension = dim;
	nobj = obj;

	position.resize(dimension);
	bestpos.resize(dimension);
	velocity.resize(dimension);
	fitness.resize(nobj);
	bestfit.resize(nobj);

	crow_dist = 0;
	violation = 0;
	LocalFlag = false;
	GlobalFlag = false;
}


// ======================== Declaration for Particle Sets ======================== //
PARTICLES::Pset::Pset()
{
	this->capacity = 0;
	this->mincrwd = 0.;
	this->indexMinCrwd = -1;
	hparticles.clear();
	span.clear();
	crwdDist.clear();
}


PARTICLES::Pset::~Pset()
{
	if (this->capacity) {
		this->mincrwd = 0.;
		this->indexMinCrwd = -1;
		hparticles.clear();
		span.clear();
		crwdDist.clear();
	}
}

void PARTICLES::Pset::SetPset(int cap)
{
	this->capacity = cap;
	this->mincrwd = 0.;
	this->indexMinCrwd = -1;
	hparticles.reserve(cap);
	span.clear();
	crwdDist.clear();
}

void PARTICLES::Pset::append(CParticle p)
{
	int len = (int)hparticles.size();
	int maxlen = this->capacity;
	if (len < 0) {
		cerr << "invalid size found in Pset.append( )" << endl;
		_getch();
		exit(1);
	}
	if (len < maxlen)
		hparticles.push_back(p);
	else {
		cerr << "set exceeds the capacity in Pset.append( )" << endl;
		_getch();
		exit(1);
	}
}

void PARTICLES::Pset::insert(CParticle p)
{
	int len = (int)hparticles.size();
	int maxlen = this->capacity;
	if (len < 0) {
		cerr << "invalid size found in Pset.insert( )" << endl;
		_getch();
		exit(1);
	}
	if (len < maxlen)
		hparticles.push_back(p);
	else
	{
		this->update();
		hparticles[this->indexMinCrwd] = p;
	}
}


void PARTICLES::Pset::remove(int k)
{
	if (hparticles.empty()) {
		cerr << "empty local/global set in Pset.remove( )" << k << endl;
		_getch();
		return;
	}
	int len = (int)hparticles.size();
	if (k > len) {
		cerr << "specified index exceedes the current local/global set size in Pset.remove( )" << endl;
		_getch();
		exit(1);
	}
	else if (len == 1)
		hparticles.clear();
	else
		hparticles.erase(hparticles.begin() + k);
}

void PARTICLES::Pset::update()
{
	if (hparticles.empty()) {
		cerr << "empty local/global set in Pset.update()" << endl;
		_getch();
		return;
	}

	int len = (int)hparticles.size();
	if (!crwdDist.empty())
		crwdDist.clear();
	crwdDist.resize(len);
	int nobj = hparticles[0].nobj;
	if (!span.empty())
		span.clear();
	span.resize(nobj);

	// for each obj
	for (int n = 0; n < nobj; ++n) {
		// f_l^n
		vector<double> f;
		f.resize(len);
		for (int l = 0; l < len; ++l)
			f[l] = hparticles[l].fitness[n];
		// sort f for the nth obj
		vector<int> rank;
		TOOLS::ranking(f, rank);
		// current spread at the nth obj space
		span[n] = f[rank[len - 1]] - f[rank[0]];
		// set anchor points' crwdDist to INFINITY
		crwdDist[rank[0]] = INF*span[n];
		crwdDist[rank[len - 1]] = INF*span[n];
		// sum up for all obj space
		for (int k = 1; k < len - 1; ++k)
			crwdDist[rank[k]] += (f[rank[k + 1]] - f[rank[k - 1]]) / span[n];
		f.clear();
		rank.clear();
		f.shrink_to_fit();
		rank.shrink_to_fit();
	}
	// find the most crowded solu
	this->mincrwd = INF;
	for (int l = 0; l < len; ++l) {
		if (crwdDist[l] < this->mincrwd) {

			this->mincrwd = crwdDist[l];

			this->indexMinCrwd = l;
		}
	}
}
