#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <string>
using namespace std;

double train[10000][400], test[10000][400], result[10000][2];

void read(double (*p)[400], string filename)
{
	ifstream fin(filename);
	string line;
	int x = 0;
	int y = 0;
	while(getline(fin, line))
	{
		istringstream sin(line);
		string field;
		y = 0;
		while(getline(sin, field, ','))
		{
			if (x == 0)
				break;
			p[x-1][y] = atof(field.c_str());
			y++;
		}
		x++;
	}
}

void initTheta(double *p)
{
	for (int i = 0; i < 385; i++)
	{
		p[i] = 0;
	}
}

void trainning(double (*p_train)[400], double *p_theta, double alpha)
{
	int n = 10000, m = 385, times = 0;
	double H[10000], t_theta[385], J;
	double t, J_theta = 0, J_theta_former = -1;
	while (J_theta_former == -1 || !(abs(J_theta - J_theta_former) < 0.0001))
	{
		for (int i = 0; i < 10000; i++)
		{
			H[i] = p_theta[0];
			for (int z = 1; z < 385; z++)
			{
				H[i] += (p_theta[z] * p_train[i][z+1]);
			}
			H[i] -= p_train[i][1];
		}
		
		for (int j = 0; j < 385; j++)
		{
			J = 0;
			for (int i = 0; i < 10000; i++)
			{
				if (j != 0)
				{

					t = H[i] * p_train[i][j+1];
					J += t;
				}
				else
				{
					J += H[i];
				}
			}
			t_theta[j] = p_theta[j] - alpha / 10000 * J;
		}

		J_theta_former = J_theta;
		J_theta = 0;
		for (int i = 0; i < 10000; i++)
		{
			t = H[i];
			t *= t;
			J_theta += t;
		}
		J_theta /= 20000;
		std::cout << J_theta << " " << times << endl;

		for (int i = 0; i < 385; i++)
		{
			p_theta[i] = t_theta[i];
		}
		times++;
		if (times > 500)
			alpha = 0.005;
		else if (times > 200)
			alpha = 0.05;
	}
}

void prediction(double (*p_test)[400], double *p_theta, double (*p_result)[2])
{
	for (int i = 0; i < 10000; i++)
	{
		p_result[i][0] = 10001 + i;
		p_result[i][1] = p_theta[0];
		for (int j = 1; j < 385; j++)
		{
			p_result[i][1] += (p_theta[j] * p_test[i][j]);
		}
	}
}

void write(double (*p_result)[2], string filename)
{
	ofstream fout(filename);
	fout << "id,reference\n";
	for (int i = 0; i < 10000; i++)
	{
		fout << p_result[i][0] << "," << p_result[i][1] << endl;
	}
}

int main()
{
	double theta[385], alpha = 0.1;

	read(train, "train.csv");
	read(test, "test.csv");

	initTheta(theta);

	trainning(train, theta, alpha);
	prediction(test, theta, result);

	write(result, "result.csv");

//	system("pause");
	return 0;
}
