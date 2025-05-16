#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <limits>
#include <ctime>
#include <chrono>
#include "Munition.h"
using namespace std;

// This is the minimum threshold before an item is moved
// To the purchase list. 
const int MINIMUM_INVENTORY = 500;

//Function prototypes
void fillInventory();
Munition* loadInventory(int&);
int binCounter();
void shoppingList(int, Munition*);
bool inList(int, Munition*, std::string);
void editQuantity(int, Munition*);
void editPrice(int, Munition*);
void receiveSupply(int&, Munition*&);

int main()
{
    int totalBins = binCounter();
    Munition* stock = loadInventory(totalBins);

    int choice;
    std::string grocery;   

    // Menu options; added as functions are added.
    cout << "*****Inventory Management System********\n";
    cout << "________________________________________\n";
    cout << "*****Enter 1 to Fill Inventory**********\n";
    cout << "*****Enter 2 to Make a Shopping List****\n";
    cout << "*****Enter 3 to Search for an Item******\n";
    cout << "*****Enter 4 to Adjust Actual Quantity**\n";
    cout << "*****Enter 5 to Adjust Actual Price*****\n";
    cout << "*****Enter 6 to Receive a Supply Order**\n";
    cin >> choice;

    switch(choice)
    {
        case 1: "Making New Inventory:\n";
            fillInventory();
            break;
        case 2: "Completing Shopping List:\n";
            groceryList(totalBins, stock);
            break;
        case 3: "Search for item\n"; 
            cout << "Item Name: ";
                cin.ignore();
                getline(cin, grocery);
            if (inList(totalBins, stock, grocery) == true)
                cout << "Item in stock:\n";
            else
                cout << "Item Not Found\n";
            break;
        case 4: "Adjusting Quantity on Hand:\n";
            editQuantity(totalBins, stock);
            shoppingList(totalBins, stock);
            break;
        case 5: "Adjusting Unit Cost Per Round:\n";
            editPrice(totalBins, stock);
            break;
        case 6: "Receive Order:\n";
            receiveSupply(totalBins, stock);
            shoppingList(totalBins, stock);
            break;
        default:
            cout << "Invalid Selection!\n";
    }

    delete [] stock;
    stock = nullptr;
    return 0;
}

/*
-----------------------------------------------------------------------------
From a blank inventory: Asks generally, how many bins to fill then
takes a single item name, the quantity on hand for that item, and the cost 
per unit. It then creates a .csv file and writes the contents of the array to 
the file.
----------------------------------------------------------------------------- 
*/
void fillInventory() 
{
    int totalBins; // How many Provision objects are held in inventory
    cout << "How many types of munitions are being stored? ";
    cin >> totalBins;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush buffer
    std::string tempName;
    int tempQuant;
    double tempCost;

    std::ofstream outFile("AmmoFort.csv");
    
    if (!outFile) 
    {
        std::cerr << "Error creating file!\n";
        return;
    }

    Munition* aFort = new Munition[totalBins];

    // Steps through user inputs for item, quantity, and cost
    for (int count = 0; count < totalBins; count++)
    {
        cout << "\nCaliber, Projectile, and Manufacturer: ";
        cin.ignore();
        getline(cin, tempName);
        aFort[count].setCal(tempName);
        cout << "\nQuantity on Hand: ";
        cin >> tempQuant;
        aFort[count].setQuant(tempQuant);
        cout << "\nCost per round: ";
        cin >> tempCost;
        aFort[count].setCost(tempCost);
    }

    // Saves dynamic array to a .csv file for permanence
    for (int count = 0; count < totalBins; count++)
    {
        outFile << aFort[count].getCal() << ","
        << aFort[count].getQuant() << ","
        << aFort[count].getCost() << "\n";
    }



    delete [] aFort;
    aFort = nullptr;
    outFile.close();

    cout << "Ammunition Fort Inventory has been saved to <AmmoFort.csv>!\n";
}

/*
-----------------------------------------------------------------------------
Loads inventory from the .csv file into an array for handling withing the 
program. 
----------------------------------------------------------------------------- 
*/

Munition* loadInventory(int& totalBins)
{   
    totalBins = binCounter();
    std::string line;

    // Creates the file if it doesn't exist, replaces it if it does.
    std::ifstream inFile("pantry.csv");

    if (!inFile)
    {
        std::cerr << "Error opening pantry.csv!\n";
        return nullptr;
    }

    inFile.clear();                 // clear EOF flag
    inFile.seekg(0);                // rewind to beginning

    Munition* stock = new Munition[totalBins];

    // I had an easier time going into tempVariables than straight into the object
    std::string tempName;
    int tempQuant;
    double tempCost;

    for (int count = 0; count < totalBins; count++)
    {
        getline(inFile, tempName, ',');
        inFile >> tempQuant;
        inFile.ignore(); // skip comma
        inFile >> tempCost;
        inFile.ignore(); // skip newline

        stock[count].setCal(tempName);
        stock[count].setQuant(tempQuant);
        stock[count].setCost(tempCost);
    }

    inFile.close();
    return stock; 
}