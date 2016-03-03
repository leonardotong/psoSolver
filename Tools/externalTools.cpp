//#define ToolsDLL
# include "externalTools.h"

/*
/* By Weiyang Tong
/* Modified on Feb 19, 2016
/* nondominant comparison of two vectors 
/* 0 - nondominant
/* 1 - f1 is better (f1 < f2)
/* 2 - f2 is better (f2 < f1)
/* weakly dominance is applied by default
*/

int TOOLS::vectorcmp(vector<double> f1, vector<double> f2)
{
	if (f1.empty() || f2.empty()) {
		cerr << "vectorcmp: input cannot be empty." << endl;
		exit(1);
	}
	else if (f1.size() != f2.size()) {
		cerr << "vectorcmp: two inputs must be in the same dimension." << endl;
		exit(1);
	}

	bool WeaklyDominantFlag = true;

	int flag = -1;
	int dim = (int)f1.size();
	bool LessThan = true;
	int lt = 0, gt = 0, eq = 0;
	for (int d = 0; d < dim; ++d) {
		if (f1[d] > f2[d])
			gt++;
		else if (f1[d] < f2[d])
			lt++;
		else
			eq++;
	}
	// flag = 0: non-dominated
	// flag = 1: f1 is dominating, i.e., f1 < f2
	// flag = 2: f2 is dominating, i.e., f1 > f2
	if (WeaklyDominantFlag) {

		if (lt == dim && gt == 0 && (eq >= 0 && eq < dim))
			flag = 1;
		else if (lt == 0 && gt == dim && eq >= 0)
			flag = 2;
		else if (lt == 0 && gt == 0 && eq >= 0) {
			if (eq < dim) {
				cout << "vectorcmp: debug: wrong logic." << endl;
				system("pause");
				exit(1);
			}
			flag = 2;
		}
		else if (lt >= 0 && gt >= 0 && eq < dim)
			flag = 0;
	}
	else {
		if (lt == dim && gt == 0 && eq == 0)
			flag = 1;
		else if (lt == 0 && gt == dim && eq == 0)
			flag = 2;
		else if (lt == 0 && gt == 0 && eq > 0)
			flag = 2;
		else if (lt >= 0 && gt >= 0 && eq >= 0)
			flag = 0;
	}
	return flag;
}

/*
/* By Weiyang Tong
/* Modified on Feb 19, 2016
/* nondominant comparison of two vectors
/* add a flag
/* true: weakly dominance, i.e., at least one of f1's values is better
/* false: strictly dominance, all f1's values must be better
*/
int TOOLS::vectorcmp(vector<double> f1, vector<double> f2, bool WeaklyDominantFlag)
{
	if (f1.empty() || f2.empty()) {
		cerr << "vectorcmp: input cannot be empty." << endl;
		exit(1);
	}
	else if (f1.size() != f2.size()) {
		cerr << "vectorcmp: two inputs must be in the same dimension." << endl;
		exit(1);
	}
	int flag = -1;
	int dim = (int)f1.size();
	bool LessThan = true;
	int lt = 0, gt = 0, eq = 0;
	for (int d = 0; d < dim; ++d) {
		if (f1[d] > f2[d])
			gt++;
		else if (f1[d] < f2[d])
			lt++;
		else
			eq++;
	}
	if (WeaklyDominantFlag) {
		// eq can be greater than zero
		// but not equal to dim (at least one f1's value is better)
		if (lt == dim && gt == 0 && (eq >= 0 && eq < dim))
			flag = 1;
		else if (lt == 0 && gt == dim && eq >= 0)
			flag = 2;
		else if (lt == 0 && gt == 0 && eq >= 0) {
			if (eq < dim) {
				cout << "vectorcmp: debug: wrong logic." << endl;
				system("pause");
				exit(1);
			}
			flag = 2;
		}
		else if (lt >= 0 && gt >= 0 && eq < dim)
			flag = 0;
	}
	else {
		// eq MUST be ZERO
		if (lt == dim && gt == 0 && eq == 0)
			flag = 1;
		else if (lt == 0 && gt == dim && eq == 0)
			flag = 2;
		else if (lt == 0 && gt == 0 && eq > 0)
			flag = 2;
		else if (lt >= 0 && gt >= 0 && eq >= 0)
			flag = 0;
	}
	return flag;
}

// Integer Handles
// standard: rounding to the nearest integer
int TOOLS::nearestVertex(double v)
{
	int up = (int) ceil(v);
	int low = (int)floor(v);

	return (v - low) < (up - v) ? low : up;
}

// stochastically rounding to the nearest integer
// #random number based
int TOOLS::nearestVertex(double v, double r)
{
	int up = (int)ceil(v);
	int low = (int)floor(v);
	if (r > 0.5)
		return up;
	else
		return low;
}

void TOOLS::ranking(vector<double> f, vector<int> &r)
{
	if (f.empty())
		return;
	int len = (int)f.size();
	r.resize(len);
	for (int j = 0; j < len; ++j)
		r[j] = j;

	for (int i = 0; i < len-1; ++i) {
		for (int j = 1; j < len; ++j) {
			if (f[j] < f[i]) {
				int tmp = r[i];
				r[i] = r[j];
				r[j] = tmp;
				double tmpf = f[i];
				f[i] = f[j];
				f[j] = tmpf;
			}
		}
	}
}

vector<int> TOOLS::ranking(vector<double> f)
{
	if (f.empty())
	{
		cerr << "ranking: input vector is empty" << endl;
		exit(1);
	}
	int len = (int)f.size();
	vector<int> r;
	r.resize(len);
	for (int j = 0; j < len; ++j)
		r[j] = j;

	for (int i = 0; i < len - 1; ++i) {
		for (int j = 1; j < len; ++j) {
			if (f[j] < f[i]) {
				int tmp = r[i];
				r[i] = r[j];
				r[j] = tmp;
				double tmpf = f[i];
				f[i] = f[j];
				f[j] = tmpf;
			}
		}
	}
	return r;
}

// Calculate the Euclidean distance betwee two vectors
double TOOLS::euclideanDistance(vector<double> f1, vector<double> f2)
{
	if (f1.empty() || f2.empty()) {
		cerr << "euclideanDistanc: empty input identified" << endl;
		cerr << "Press any key to exit..." << endl;
		_getch();
		exit(1);
	}
	// checking equal dimension
	if (f1.size() != f2.size()) {
		cerr << "euclideanDistanc: input vectors must be the same size" << endl;
		cerr << "Press any key to exit..." << endl;
		_getch();
		exit(1);
	}
	
	int len = (int)f1.size();
	double dist = 0.;
	for (int l = 0; l < len; ++l)
		dist += pow(f1[l] - f2[l], 2.);

	return sqrt(dist);
}
/*
// general obj function
vector<double> generalObjFun(vector<double> x)
{

	ifstream file(objExeFile);
	if (!file)
	{
		cout << objExeFile << "is not found in the current directory..." << endl;
		_getch();
		exit();
	}
	file.close();

	vector<double> obj;
	if (!obj.empty())
		obj.clear();

	char *objOutputFile = "objOutput.out";
	bool done = false;
	done = system(objExeFile);
	if (done)
	{
		ofstream out(objOutputFile);

	}
	infile.close();
}
return obj;
}

// general constr function
vector<double> generalConFun(const char *executionFile)
{
	ifstream file(executionFile);
	if (!file)
	{
		cout << executionFile << "is not found in the current directory..." << endl;
		_getch();
		exit(1);
	}
	file.close();

	vector<double> con;
	if (!con.empty())
		con.clear();
	bool done = false;
	done = system(executionFile);
	if (done)
	{

		ifstream infile(outputFile);
		if (!infile)
		{
			cout << "output file " << outputFile << " is not found in the current directory..." << endl;
			_getch();
			exit(1);
		}
		while (!infile.eof())
		{
			double f;
			infile >> f;
			con.push_back(f);
		}
		infile.close();
	}
	return con;
}
*/