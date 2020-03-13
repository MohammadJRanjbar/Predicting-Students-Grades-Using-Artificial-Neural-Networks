
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>      
#include <random>
std::vector < std::vector <double> > getData(const char*);
std::vector < std::vector <double> > Normalize(std::vector < std::vector <double> >);
void displayDataset(std::vector <std::vector <double >>);
double grade(std::vector <double > w, std::vector <double > x);
std::vector<double> WeightInitializer(std::vector <double > w);
double J(std::vector<double>, std::vector<std::vector<double>>);
std::vector<double> train(std::vector<std::vector<double>>, std::vector<double>, double, size_t, double, bool);
void displayOutput(std::vector <std::vector <double >>, std::vector <double>);
void predict(std::vector<double>);
void save(std::vector<double>, const char*);
std::vector<double> load(const char*);

int Hw1()
{
	std::vector<std::vector<double>> M = getData("AP-Data.csv");
	size_t cols { M[0].size()};
	std::vector <double> W(cols - 1, 1);
	std::vector <double> x(cols - 1, 1);
	bool train_flag=true;
	bool predict_flag;
	displayDataset(M);
	W = WeightInitializer(W);
	std::ifstream Wfile("weights.csv");

	if (Wfile.good())
	{
		W= load("weights.csv");
		std::cout << "Network is trained and cost is : " << J(W, M) << std::endl;
		std::cout << "Do you want to train more ? (1 for yes or 0 for no) : ";
		std::cin >> train_flag;
		std::cin.ignore();
	}

	double lr {0.01};
	size_t epochs {500};
	double min_cost{0.01};
	bool verbose=false;

	if (train_flag == true)
	{
		std::string input;
		std::cout << "learning rate (only press enter for the default value = 0.01)  = ";
		std::getline(std::cin, input);

		if (!input.empty())
		{
			lr = std::stod(input);
		}

		std::cout << "epochs (only press enter for the default value = 500)  = ";

		std::getline(std::cin, input);

		if (!input.empty())
		{
			epochs = std::stol(input);
		}


		std::cout << "min cost (only press enter for the default value = 0.01)  =";
		std::getline(std::cin, input);
		if (!input.empty())
		{
			min_cost = std::stod(input);
		}

		std::cout << "do you want to see the cost per epoch press 1 for yes or 0 for no? (only press enter for the default value = 0 )  =";

		std::getline(std::cin, input);

			if (input == "1")
				verbose = 1;
			else
				verbose = 0;

		W = train(M, W, lr, epochs, min_cost, verbose);

		save(W, "weights.csv");
	}
	displayOutput(M, W);

	std::cout << "Do you want to predict your grade ? (1 for yes or 0 for no) : ";
	std::cin >> predict_flag;
	if(predict_flag==true)
	predict(W);
	return 0;

}
std::vector < std::vector <double> > getData(const char* FileAddress)
{
	std::ifstream Grades;
	Grades.open(FileAddress);
	size_t rows {0};
	size_t cols {0};
	std::string line;
	while (std::getline(Grades, line))
		rows++;
	Grades.clear();
	Grades.seekg(0, std::ios_base::beg);
	while (getline(Grades, line, ','))
		cols++;
	cols = (cols - 1) / rows + 1;
	std::vector<std::vector<double>> M(rows, std::vector<double>(cols + 1));
	std::string A{};
	std::string B{};
	Grades.close();
	Grades.open(FileAddress);
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 1; j < cols; j++)
		{
			M[i][0] = 1;
			getline(Grades, A, ',');

			M[i][j] = std::stod(A);
		}
		getline(Grades, B, '\n');
		M[i][cols] = std::stod(B);

	}
	Grades.close();
	M = Normalize(M);
	return M;
}
void displayDataset(std::vector <std::vector <double >> M)
{
	size_t rows {size(M)};
	size_t cols {M[0].size()};
	std::cout << std::left << std::setw(15) << "Bias";
	std::cout << std::left << std::setw(15) << "Class";
	std::cout << std::left << std::setw(15) << "TA";
	std::cout << std::left << std::setw(15) << "Coding";
	std::cout << std::left << std::setw(15) << "studying" << std::setw(15);
	std::cout << std::left << std::setw(15) << "Bckgraound" << std::setw(15);
	std::cout << std::left << std::setw(15) << "Mind" << std::setw(15);
	std::cout << std::left << std::setw(15) << "Grade" << std::endl;

	for (size_t temp = 0; temp < 110; temp++)
		std::cout << "*";
	std::cout << std::endl;

	for (size_t T = 0; T < rows; T++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			if ( j == 3 || j == 4)
			{
				M[T][j] = M[T][j] * 100;
			}
			std::cout << std::left << std::setw(15) << M[T][j];
		}
		std::cout << std::endl;
	}
}
std::vector < std::vector <double> > Normalize(std::vector < std::vector <double> > M)
{
	size_t rows {size(M)};
	for (size_t i = 0; i < rows; i++)
	{
		M[i][3] = M[i][3] / static_cast <double>(100);
		M[i][4] = M[i][4] / static_cast <double>(100);
		//M[i][7] = M[i][7] / double(20);
	}
	return M;
}
std::vector<double> WeightInitializer(std::vector <double > w)
{
	size_t rows {size(w)};
	srand(static_cast <size_t> (time(0)));
	for (size_t i = 0; i < rows; i++)
	{

		w[i] = (rand() % 100 + 1) / static_cast <double>(100);
	}
	return w;
}
double grade(std::vector <double > w, std::vector <double > x)
{
	double GradeO {0};
	size_t cols  {w.size()};
	for (size_t k = 0; k < cols; k++)
	{
		GradeO = GradeO + x[k] * w[k];
	}
	return GradeO;
}
double J(std::vector<double> W, std::vector<std::vector<double>> M)
{
	size_t rows  {size(M)};
	size_t cols  {M[0].size()};
	double Costf {0};
	std::vector <double> x(cols - 1, 1);
	std::vector <double> G(rows, 1);

	for (size_t j = 0; j < rows; j++)
	{
		for (size_t i = 0; i < cols - 1; i++)
		{
			x[i] = M[j][i];

		}
		G[j] = grade(W, x);

	}

	for (size_t j = 0; j < rows; j++)
	{
		Costf = (G[j] - M[j][cols - 1])*(G[j] - M[j][cols - 1]) + Costf;
	}

	Costf = Costf / (double(2 * rows));
	return Costf;
}
std::vector<double> train(std::vector<std::vector<double>> M, std::vector<double>W, double alpha, size_t epoch, double mincost, bool verbose)
{
	size_t cols {M[0].size()};
	size_t rows  {size(M)};
	double cost{2000};
	size_t A{0};
	std::vector <double> X(cols - 1, 1);
	std::vector <double> backP(cols - 1, 1);
	std::vector <double> x(cols - 1, 1);
	std::vector <double> GradeOut(rows, 1);
	while (cost>mincost)
	{
		for (size_t j = 0; j < rows; j++)
		{
			for (size_t i = 0; i < cols - 1; i++)
			{
				x[i] = M[j][i];

			}
			GradeOut[j] = grade(W, x);

		}
		cost = J(W, M);
		if (verbose == 1)
			std::cout << "cost value for epoch number " << A + 1 << " is " << cost << std::endl;
		for (size_t j = 0; j < rows; j++)
		{
			double error = (GradeOut[j] - M[j][cols - 1]);

			for (size_t i = 0; i < cols - 1; i++)
			{
				backP[i] = M[j][i] * error;
			}
			for (size_t i = 0; i < cols - 1; i++)
			{
				X[i] = X[i] + backP[i];
			}
		}

		for (size_t i = 0; i < cols - 1; i++)
		{
			X[i] = X[i] / (static_cast <double>(rows));
		}
		for (size_t i = 0; i < cols - 1; i++)
		{
			W[i] = W[i] - X[i] * alpha;
		}

		A++;

		if (A == epoch)
			break;
	}
	return W;
}
void save(std::vector<double>W, const char* Address)
{
	std::fstream weight;
	size_t cols {W.size()};
	weight.open(Address, std::ofstream::out | std::ofstream::trunc);
	for (size_t j = 0; j < cols; j++)
	{
		weight << W[j] << std::endl;
	}
}
std::vector<double> load(const char*Address)
{
	std::ifstream weight;
	weight.open(Address);
	size_t rows{ 0 };
	size_t cols{ 0 };
	std::string line;
	while (std::getline(weight, line))
		rows++;
	weight.clear();
	weight.seekg(0, std::ios_base::beg);
	while (getline(weight, line, ','))
		cols++;
	cols = (cols - 1) / rows + 1;
	std::vector<double> W(rows, 1);
	std::string A;
	weight.close();
	weight.open(Address);
	for (size_t i = 0; i < rows; i++)
	{
		std::getline(weight, A, '\n');
		W[i] = std::stod(A);
	}
	weight.close();
	return W;
}
void displayOutput(std::vector <std::vector <double >>M, std::vector <double>W)
{
	size_t rows {size(M)};
	size_t cols {M[0].size()};
	std::vector <double> x(cols - 1, 1);
	std::vector <double> GradeOut(rows, 1);
	std::vector <double> Numbers(rows, 1);
	for (size_t j = 0; j < rows; j++)
	{
		for (size_t i = 0; i < cols - 1; i++)
		{
			x[i] = M[j][i];
		}
		GradeOut[j] = grade(W, x);
		if (GradeOut[j] > 20)
			GradeOut[j] = 20;
		if (GradeOut[j] < 0)
			GradeOut[j] = 0;

		Numbers[j] = j + 1;
	}
	std::cout << std::left << std::setw(15) << "No";
	std::cout << std::left << std::setw(15) << "Real Grade";
	std::cout << std::left << std::setw(15) << "Estimated Grade" << std::endl;
	for (size_t temp = 0; temp < 45; temp++)
		std::cout << "*";
	std::cout << std::endl;

	for (size_t T = 0; T < rows; T++)
	{
		std::cout << std::left << std::setw(15) << Numbers[T];
		std::cout << std::left << std::setw(15) << M[T][cols - 1];
		std::cout << std::left << std::setw(15) << GradeOut[T];
		std::cout << std::endl;
	}
}
void predict(std::vector<double>w)
{
	size_t cols {w.size()};
	std::vector <double> x(cols, 1);
	std::cout << "How much attentiaon did he/she had in the class?" << std::endl;
	std::cin >> x[1];
	std::cout << "How much attentiaon did he/she had in the Ta class?" << std::endl;
	std::cin >> x[2];
	std::cout << "How many hours of coding and practicing does he/she had in the week?" << std::endl;
	std::cin >> x[3];
	std::cout << "How many hours of studing and reading books does he/she had in the week?" << std::endl;
	std::cin >> x[4];
	std::cout << "How much Previous background did he/she had ?" << std::endl;
	std::cin >> x[5];
	std::cout << "how much Talent did he/she had?" << std::endl;
	std::cin >> x[6];
	x[3] = x[3] / static_cast <double>(100);
	x[4] = x[4] / static_cast <double>(100);

	double GradeO = grade(w, x);
	if (GradeO > 20)
		GradeO = 20;
	if (GradeO < 0)
		GradeO = 0;
	std::cout << GradeO << std::endl;

}
