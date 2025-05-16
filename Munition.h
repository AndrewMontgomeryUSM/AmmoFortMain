#include <string>
#include <iostream>

#ifndef MUNITION_H
#define MUNITION_H

class Munition
{
    private:
        std::string caliber;
        int quantOnHand;
        double costPerRound;
    
    public:
        Munition():
        caliber("N/A"), quantOnHand(0), costPerRound(0.0)
        {}
        void setCal(std::string);
        void setQuant(int);
        void setCost(double);
        std::string getCal()const;
        int getQuant()const;
        double getCost()const;
        double getTotalCost();
        ~Munition()
        {}
};

#endif 