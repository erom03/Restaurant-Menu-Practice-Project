//Enrique Romero
//Practice Project

//Feel free to update the menu.csv file with your own menu options, the program should work with whatever is put in
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

void mainMenu();    //Prompts the user to pick what they'd like to do
void hold();        //Holds the user until they press enter
void menuCheck(vector<string> &itemName);   //Checks if a menu has been loaded
void importMenu(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach); //Imports a menu from a csv file
void addBill(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach);    //Adds items from menu to bill
void removeBill(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach); //Removes items currently in bill
void calcTotal(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach);  //Calculates the subtotal, tax, and prompts for tip. Then calculates total
void printBill(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach);  //Prints bill to a txt file

struct {
    double subtotal = 0.0;
    double tax = 0.0;
    double tip = 0.0;
    double total = 0.0;
} bill;                         //Used when calculating the bill

int main()
{
    mainMenu();
    hold();
    return 0;
}

void mainMenu()
{
    char option; //user will input choice here
    vector<string> itemName; //used to save list of names
    vector<double> itemCost; //used to save list of prices
    vector<int> totalOfEach; //used to save how many of each item there are

    do {
        system("cls"); //clear screen for menu

        cout << "\n\n\tRestaurant Bill Menu"    //Print options for user
             << "\na.) Import menu"
             << "\nb.) Add items to the bill"
             << "\nc.) Remove items from the bill"
             << "\nd.) Calculate total"
             << "\ne.) Print the bill"
             << "\nf.) Exit"
             << "\n\nPlease select an option: ";

        cin >> option; //Prompt user for input

        switch (tolower(option)) {  //Check what user picked and navigate accordingly
            case 'a':
                importMenu(itemName, itemCost, totalOfEach);
                break;
            case 'b':
                addBill(itemName, itemCost, totalOfEach);
                break;
            case 'c':
                removeBill(itemName, itemCost, totalOfEach);
                break;
            case 'd':
                calcTotal(itemName, itemCost, totalOfEach);
                break;
            case 'e':
                printBill(itemName, itemCost, totalOfEach);
                break;
            case 'f':
                system("cls");
                cout << "\nExiting ...";
                break;
            default:                            //If user picks incorrectly, prompt them to collect again
                system("cls");
                cout << "\nPlease select a valid option";
                hold();
        }
    } while (tolower(option) != 'f');   //Condition needed to exit loop
}

void hold()
{
    cout << "\n\nPlease press enter to continue...";
    cin.get();
    cin.ignore(256, '\n');
}

void importMenu(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach)
{
    system("cls");  //Clear users screen

    string name = " ";
    string priceAsString = " ";
    double price; //These will be used to store the names and prices of items

    ifstream menu;
    menu.open("menu.csv");  //Open up the file "menu.csv" and assign it to menu

    while(!menu.eof())
    {
        getline(menu, name, ',');               // Grab the name of the dish
        getline(menu, priceAsString, '\n');     // Grab the price of the dish as a string

        price = stod(priceAsString);    // Convert the price from a string to a double

        cout << name << endl;           //Print the name of the item
        cout << price << endl << endl;  //Print the price of the item

        itemName.push_back(name);
        itemPrice.push_back(price);
        totalOfEach.push_back(0);       //Store the info in their corresponding arrays
    }

    menu.close();   //Close the file

    cout << "\nMenu successfully updated";
    hold();
}

void menuCheck(vector<string> &itemName)
{
    system("cls"); //Clear screen

    if(itemName.empty()) //Check if vectors are empty
    {
        cout << "\nPlease import a menu through option a.)";    //If they are, prompt the user to import a menu
        hold();
        mainMenu();
    }
}

void addBill(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach)
{
    menuCheck(itemName);

    int itemChoice = 0;
    char repeatChoice;
    bool repeat;

    do {                                //repeat until the user doesn't want to add anymore
        system("cls");

        cout << "\n\tAdd Items to Bill";
        for (int i = 0; i < itemName.size(); ++i) {
            cout << left
                 << "\n" << i + 1 << ".)"
                 << " " << itemName.at(i);
            cout << right << setw(10)
                 << "\t" << itemPrice.at(i);        //Lists all the menu options with their prices in a numbered list
        }
        cout << "\nPlease select an item";
        cout <<"\nEnter 0 to cancel: ";
        cin >> itemChoice;                      //Prompt the user for a choice

        if(itemChoice < 0 || itemChoice > itemName.size() + 1)
        {
            system("cls");
            cout << "\nPlease select a valid option";
            hold();                                         //Check if the user inputted an invalid option
        } else if(itemChoice == 0)                          //Check if user wants to cancel adding an item
            repeat = false;
        else {
            bill.subtotal += itemPrice.at(itemChoice - 1);  //Adds items price to subtotal
            totalOfEach.at(itemChoice - 1) += 1;            //Adds one to the count of that item

            do {
                system("cls");
                cout << "\nItem added, would you like to add something else?"
                     << "\n\ta.) Yes"
                     << "\n\tb.) No"
                     << "\nPlease select an option: ";
                cin >> repeatChoice;                        //asks the user if they'd like to repeat

                switch (repeatChoice) {
                    case 'a':
                        repeat = true;
                        break;
                    case 'b':
                        repeat = false;
                        break;
                    default:
                        system("cls");
                        cout << "\nPlease select a valid option";
                        hold();
                }
            } while(tolower(repeatChoice) != 'b' && tolower(repeatChoice) != 'a');
        }
    } while(repeat);

    system("cls");
    cout << "\n\nReturning to main menu...";
    hold();
}

void removeBill(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach)
{
    int itemChoice = 0;
    char repeatChoice;
    bool repeat = true;

    do {            //repeat until the user doesn't want to add anymore
        menuCheck(itemName);
        system("cls");

        cout << "\n\tRemove Items From Bill";
        for (int i = 0; i < itemName.size(); ++i) {
            cout << left
                 << "\n" << i + 1 << ".)"
                 << " " << itemName.at(i);
            cout << right << setw(10)
                 << "\t" << itemPrice.at(i);                //Lists all the menu options with their prices in a numbered list
        }
        cout << "\nPlease select an item";
        cout <<"\nEnter 0 to cancel: ";
        cin >> itemChoice;                      //Prompt the user for a choice

        if(itemChoice < 0 || itemChoice > itemName.size() + 1)
        {
            system("cls");
            cout << "\nPlease select a valid option";
            hold();                                     //Check if the user inputted an invalid option
        } else {
            if(itemChoice == 0)                         //Check if user wants to cancel adding an item
                repeat = false;
            else if(totalOfEach.at(itemChoice - 1) < 1)     //Checks that the item is actually being ordered
            {
                system("cls");
                cout << "\nPlease select an option in the bill";
                hold();
            }
            else {
                bill.subtotal -= itemPrice.at(itemChoice - 1);  //Subtract the cost of the item from the bill
                totalOfEach.at(itemChoice - 1) -= 1;            //Subtract one from the amount of that item being ordered

                do {
                    system("cls");
                    cout << "\nItem removed, would you like to remove something else?"
                         << "\n\ta.) Yes"
                         << "\n\tb.) No"
                         << "\nPlease select an option: ";
                    cin >> repeatChoice;                        //Prompt the user if they'd like to delete more

                    switch (repeatChoice) {
                        case 'a':
                            repeat = true;
                            break;
                        case 'b':
                            repeat = false;
                            break;
                        default:
                            system("cls");
                            cout << "\nPlease select a valid option";
                            hold();
                    }
                } while (tolower(repeatChoice) != 'b' && tolower(repeatChoice) != 'a');
            }
        }
    } while(repeat);

    system("cls");
    cout << "\n\nReturning to main menu...";
    hold();
}

void calcTotal(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach)
{
    system("cls");

    menuCheck(itemName);

    cout << "\nItem\t\t\tQuantity\tPrice";
    for(int i = 0; i < itemName.size(); ++i) {
        cout << "\n" << itemName.at(i)
             << "\t\tx" << totalOfEach.at(i)
             << "\t\t" << itemPrice.at(i) * totalOfEach.at(i);      //Prints each item name, quantity ordered, and price
    }
    cout << "\n\nSubtotal: " << bill.subtotal;  //Displays subtotal

    cout << "\n\nHow much would you like to tip?"
         << "\n(20% is " << setprecision(2) << fixed << bill.subtotal * .2 << "): ";    //Prompts user for tip, suggests a 20% tip
    cin >> bill.tip;

    bill.tax = bill.subtotal * 0.07;
    bill.total = bill.subtotal + bill.tax + bill.tip;       //Calculate totals

    system("cls");
    cout << "\nItem\t\t\tQuantity\tPrice";
    for(int i = 0; i < itemName.size(); ++i) {
        cout << "\n" << itemName.at(i)
             << "\t\tx" << totalOfEach.at(i)
             << "\t\t" << itemPrice.at(i) * totalOfEach.at(i);      //Prints each item name, quantity ordered, and price
    }

    cout << setprecision(2) << fixed <<"\n\nSubtotal: " << bill.subtotal
         << "\nTax: " << bill.tax
         << "\nTip: " << bill.tip
         << "\n\nTotal: " << bill.total;    //Displays new totals

    hold();
}

void printBill(vector<string> &itemName, vector<double> &itemPrice, vector<int> &totalOfEach)
{
    system("cls");

    menuCheck(itemName);

    ofstream finalBill("bill.txt");             //Creates new text file "bill.txt"

    finalBill << "\nItem\t\t\tQuantity\tPrice";
    for(int i = 0; i < itemName.size(); ++i) {
        finalBill << "\n" << itemName.at(i)
             << "\t\tx" << totalOfEach.at(i)
             << "\t\t" << itemPrice.at(i) * totalOfEach.at(i);  //Prints each item name, quantity ordered, and price
    }

    finalBill << setprecision(2) << fixed <<"\n\nSubtotal: " << bill.subtotal
         << "\nTax: " << bill.tax
         << "\nTip: " << bill.tip
         << "\n\nTotal: " << bill.total;        //Prints final new totals

    finalBill.close();

    cout << "\nBill created";       //Notifies user that bill has been created
    hold();
}