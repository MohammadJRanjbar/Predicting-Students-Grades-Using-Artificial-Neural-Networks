//
// Created by mohammadhossein on 1/30/20.
//

#ifndef AP2020HW1_APHW1_H
#define AP2020HW1_APHW1_H
#include <vector>
#include <iostream>

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

#endif //AP2020HW1_APHW1_H
