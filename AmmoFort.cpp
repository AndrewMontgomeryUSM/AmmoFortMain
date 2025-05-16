/*
----------------------------------------------------------------------------- 
**NOTE**
Segmentation Fault when running option 2


*/

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
void shoppingList(int&, Munition*&);
//bool inList(int, Munition*, std::string);
//void editQuantity(int, Munition*);
//void editPrice(int, Munition*);
//void receiveSupply(int&, Munition*&);

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
            shoppingList(totalBins, stock);
            break;
        /*case 3: "Search for item\n"; 
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
        */
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
    std::ifstream inFile("AmmoFort.csv");

    if (!inFile)
    {
        std::cerr << "Error opening AmmoFort.csv!\n";
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

/*
-----------------------------------------------------------------------------
Counts how many items are being held in the inventory .csv file for use in 
dynamic array creation later in the program
-----------------------------------------------------------------------------
*/
int binCounter()
{
    int totalBins = 0 ; // How many Provision objects are held in inventory
    
    std::string line;

    std::ifstream inFile("AmmoFort.csv");

    if (!inFile) 
    {
        std::cerr << "Error reading file!\n";
    }

    // Determines how long the dynamic array needs to be

    while (std::getline(inFile, line)) 
    {
        if (!line.empty())
            ++totalBins;
    }
    return totalBins;
}

/*
-----------------------------------------------------------------------------
Creates a shopping list based on minimum inventory and generates an estimate
for the cost of the total order.
-----------------------------------------------------------------------------
*/

void shoppingList(int& totalBins, Munition*& stockList)
{
    std::ofstream outFile("shopping_list.txt");

    // Gets current time for grocery list print out
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    
    if (!outFile) 
    {
        std::cerr << "Error creating file!\n";
    }
    
    Munition* shoppingList = new Munition[totalBins]; 

    int onHand, shoppingCount = 0; 

    for (int count = 0; count < totalBins; count++)
    {
        onHand = stockList[count].getQuant();

        
        if (onHand < MINIMUM_INVENTORY)
        {
            shoppingList[shoppingCount].setCal(stockList[count].getCal());
            shoppingList[shoppingCount].setQuant(stockList[count].getQuant());
            shoppingList[shoppingCount].setCost(stockList[count].getCost());
            shoppingCount++;
        }

    }

    // Calculates the estimated total from previously recorded costs for items in inventory.
    double estimatedCost = 0.0;
    for (int count = 0; count < shoppingCount; count++)
    {
        estimatedCost += shoppingList[count].getCost();
    }

    // Formatting for the .txt file for easier readability
    outFile << "**********Shopping List**********\n";
    outFile << std::put_time(std::localtime(&now_time), "%A, %B %d, %Y %I:%M %p") << "\n";
    outFile << "*********************************\n\n";
    for (int count = 0; count < shoppingCount; count++)
    {
        outFile << shoppingList[count].getCal() << "\n";
    }
    outFile << "\n";
    outFile << "*********************************\n";
    outFile << "Estimated Price for Stock: \n" << setw(12) << right
        << setprecision(2) << showpoint << fixed << "$" <<(estimatedCost * MINIMUM_INVENTORY) << endl;

    cout << "Stock List Created as shopping_list.txt!" << endl;

    delete[] shoppingList;
    shoppingList = nullptr;
    outFile.close();
}