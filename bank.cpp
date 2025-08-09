#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<iomanip>
#include<ctime>

using namespace std;

// COLOURS
void Red()     { cout << "\033[31m"; }
void Green()   { cout << "\033[32m"; }
void Yellow()  { cout << "\033[33m"; }
void Blue()    { cout << "\033[34m"; }
void Magenta() { cout << "\033[35m"; }
void Cyan()    { cout << "\033[36m"; }
void White()   { cout << "\033[37m"; }
void Reset()   { cout << "\033[0m"; }

class BankAccount{
private:
    string bankName = "BOI"; 
    int bankCode;     
    string accountHolderName;
    string accountNumber;
    string accountPassword;
    string adharNumber;
    string email;
    string phoneNumber; 
    double balance;
    vector<string> transactions; // Renamed from transactionHistory to avoid conflict
    
public:
    void saveToFile();
    void loadFromFile();
    void mainMenu();
    void createAccount();
    void loginAccount();
    void depositMoney();
    void withdrawMoney();
    void checkBalance();
    void accountDetails();
    void showTransactionHistory(); // Renamed from allTransactionHistroy
};

// Save data to the file
void BankAccount::saveToFile(){
    ofstream file(accountNumber + ".txt");
    if(file.is_open()){
        file << accountHolderName << endl;
        file << accountNumber << endl;
        file << accountPassword << endl;
        file << adharNumber << endl;
        file << email << endl;
        file << phoneNumber << endl;
        file << balance << endl;
        for(const auto& transaction : transactions){
            file << transaction << endl;  
        }
        file.close();
    } else {
        cout << "Error opening file!";
    }
}

// Read data from the file
void BankAccount::loadFromFile(){
    transactions.clear(); // avoid duplicates when loading multiple times

    ifstream file(accountNumber + ".txt");
    if(file.is_open()){
        getline(file, accountHolderName);
        getline(file, accountNumber);
        getline(file, accountPassword);
        getline(file, adharNumber);
        getline(file, email);
        getline(file, phoneNumber);

        // read balance as a line and convert - more robust than mixing >> and getline
        string balanceLine;
        if(getline(file, balanceLine)){
            try {
                balance = stod(balanceLine);
            } catch (const exception&){
                balance = 0.0;
            }
        } else {
            balance = 0.0;
        }

        string transaction;
        while(getline(file, transaction)){
            if(!transaction.empty())
                transactions.push_back(transaction);
        }
        file.close();
    } else {
        cout << "Error opening file!" << endl; 
    }
}

// Main Menu
void BankAccount::mainMenu(){
    int choice;
    do{
        cout << "-------------------------------------" << endl;
        cout << "Enter 1 for Deposit Money" << endl;
        cout << "Enter 2 for Withdraw Money" << endl;
        cout << "Enter 3 for Check Balance" << endl;
        cout << "Enter 4 for Account Details" << endl;
        cout << "Enter 5 for Transaction History" << endl;
        cout << "Enter 6 for Logout" << endl;
        cout << "-------------------------------------" << endl;
        cout << "Enter your Choice: ";
        cin >> choice;
        cin.ignore();
        
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        switch(choice){
            case 1:
                depositMoney();
                break;
            case 2:
                withdrawMoney();
                break;
            case 3:
                checkBalance();
                break;
            case 4:
                accountDetails();
                break;
            case 5:
                showTransactionHistory();
                break;
            case 6:
                cout << "Logging Out..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again" << endl;
                break;
        }
    } while(choice != 6);
}

// Create Account
void BankAccount::createAccount(){
    Green();
    cout << "        Create Account     " << endl;
    cout << "-------------------------------------" << endl;
    cout << "Enter Your Full Name: ";
    getline(cin, accountHolderName);
    cout << "Enter Email Id: ";
    getline(cin, email);
    cout << "Enter Phone Number: ";
    getline(cin, phoneNumber);
    cout << "Enter Adhar Number: ";
    getline(cin, adharNumber);
    cout << "Enter Account Password: ";
    getline(cin, accountPassword);
    cout << "-------------------------------------" << endl;
    Reset();
    balance = 500.00;

    ifstream file("Bank-Of-India.txt");
    if(file.is_open()){
        file >> bankCode;
        file.close();

        ofstream output("Bank-Of-India.txt");
        if(output.is_open()){
            output << bankCode + 1;
            output.close();
        } else {
            cout << "Error opening file!" << endl;
        }
    } else {
        // initialize if missing
        bankCode = 10000;
        ofstream init("Bank-Of-India.txt");
        init << bankCode + 1;
        init.close();
    }

    accountNumber = bankName + to_string(bankCode);
    transactions.push_back("Deposit: " + to_string(balance));

    Yellow();
    cout << "-------------------------------------" << endl;
    cout << "Account Created Successfully!" << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Account Password: " << accountPassword << endl;
    cout << "Minimum Balance: 500.00" << endl;
    cout << "-------------------------------------" << endl;
    Reset();
    saveToFile();
}

// Login Account
void BankAccount::loginAccount(){
    string enteredAccountNumber;
    string enteredPassword;
    Blue();
    cout << "     Login Account     " << endl;
    cout << "______________________" << endl;
    cout << "Enter Account Number: ";
    getline(cin, enteredAccountNumber);

    accountNumber = enteredAccountNumber; 
    ifstream file(enteredAccountNumber + ".txt");
    if(file.is_open()){
        file.close();
        loadFromFile();
        cout << "Enter Account Password: ";
        getline(cin, enteredPassword);
        if(accountPassword == enteredPassword){
            cout << "Login Successful!" << endl;
            mainMenu();
        } else {
            cout << "Invalid Password!" << endl;
        }
    } else {
        cout << "Account not found!" << endl;
    }
}

// Deposit Money
void BankAccount::depositMoney() {
    double amount;
    cout << "Enter Amount to Deposit: ";
    cin >> amount;
    cin.ignore();

    if (amount > 0) {
        balance += amount;
        time_t now = time(nullptr);
        transactions.push_back("Deposit: " + to_string(amount) + " at " + string(ctime(&now)));
        saveToFile();
        cout << "Deposit Successful!" << endl;
        cout << "New Balance: " << balance << endl;
    } else {
        cout << "Invalid Amount!" << endl;
    }
}

// Withdraw Money
void BankAccount::withdrawMoney() {
    double amount;
    cout << "Enter amount to withdraw: ";
    cin >> amount;
    cin.ignore();

    if (amount > 0 && amount <= balance) {
        balance -= amount;
        time_t now = time(nullptr);
        transactions.push_back("Withdraw: " + to_string(amount) + " at " + string(ctime(&now)));
        saveToFile();
        cout << "Withdraw Successful!" << endl;
        cout << "New Balance: " << balance << endl;
    } else {
        cout << "Invalid Amount!" << endl;
    }
}

// Check Balance
void BankAccount::checkBalance(){
    cout << "Current Balance: " << balance << endl;
    cout << "-------------------------------" << endl;
}

// Account details
void BankAccount::accountDetails(){
    Magenta();
    cout << "|----------------------------------------|" << endl;
    cout << "|              Bank of India             |" << endl;
    cout << "|----------------------------------------|" << endl;

    Blue();
    cout << "Account Holder Name: " << accountHolderName << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Aadhar Number: " << adharNumber << endl;
    cout << "Email ID: " << email << endl;
    cout << "Phone Number: " << phoneNumber << endl;
    cout << "Current Balance: " << balance << endl;
    cout << "------------------------------------------" << endl;

    Reset();
}

// View all transaction history
void BankAccount::showTransactionHistory(){
    cout << "Transaction History:" << endl;

    Yellow();
    cout << "------------------------------------------" << endl;
    cout << "Account Holder Name: " << accountHolderName << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "------------------------------------------" << endl;

    for(const auto& transaction : transactions){
        cout << transaction << endl;
    }

    cout << "------------------------------------------" << endl;
    Reset();
}

// Main function
int main(){
    int choice;
    BankAccount account;

    do {
        cout << "==========================================" << endl;
        cout << "    Welcome to THE Bank of India          " << endl;
        cout << "==========================================" << endl;
        cout << "Enter 1. to Create Account" << endl;
        cout << "Enter 2. to Login" << endl;
        cout << "Enter 3. to Exit" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        switch(choice){
            case 1:
                account.createAccount();
                break;
            case 2:
                account.loginAccount();
                break;
            case 3:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 3);

    return 0;
}