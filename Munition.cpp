#include "Munition.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

void Munition::setCal(std::string n)
{ caliber = n; }

void Munition::setQuant(int q)
{ quantOnHand = q; }

void Munition::setCost(double c)
{ costPerRound = c; }

std::string Munition::getCal()const
{ return caliber;}

int Munition::getQuant()const
{ return quantOnHand;}

double Munition::getCost()const
{ return costPerRound; }

double Munition::getTotalCost()
{ return quantOnHand * costPerRound;}