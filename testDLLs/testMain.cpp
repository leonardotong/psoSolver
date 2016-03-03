# include "PSO.h"

vector<double> fun1(vector<double> x1)
{
	return x1;
}
vector<double> fun2(vector<double> x2)
{
	return x2;
}


int main()
{
	SWARM::objfun fun = fun1;
	SWARM::confun con = fun2;
	class mySolu : public SWARM::CSwarm
	{
	public:
		PARTICLES::CParticle p1, p2;
		mySolu(int dim, int obj, int pop, int iter) 
			: CSwarm(dim, obj, pop, iter) 
		{
			p1.set_dimension(dim, obj);
			p2.set_dimension(dim, obj);
		};
		~mySolu() { cout << "deleting mySolu ..." << endl; };
		void show(SWARM::objfun f, SWARM::confun g)
		{
			vector<double> x(10, (double)rand() / (double)RAND_MAX);
			
			update_swarm(f, g);
		}
	protected:
		void update_swarm(SWARM::objfun f, SWARM::confun g)
		{
			cout << "updating solutions in the swarm ..." << endl;
		};
	};

	mySolu *solu = NULL;
	solu = new mySolu(2, 1, 10, 1);
	solu->show(fun, con);
	vector<double> f1, f2;
	f1.push_back((double)rand() / (double)RAND_MAX);
	f1.push_back((double)rand() / (double)RAND_MAX);
	f2.push_back((double)rand() / (double)RAND_MAX);
	f2.push_back((double)rand() / (double)RAND_MAX);
	solu->p1.fitness = f1;
	solu->p2.fitness = f2;
	int flag = TOOLS::vectorcmp(solu->p1.fitness, solu->p2.fitness);
	cout << flag << endl;
	delete solu;

	return EXIT_SUCCESS;
	
}