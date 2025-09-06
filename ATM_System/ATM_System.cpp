#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMainMenuOptions {
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3,
    eCheckBalance = 4, eLogout = 5
};

enum enQuickWithdraw {
    enTwenty = 1, enFifty = 2, enHundred = 3, enTwoHundreds = 4,
    enFourHundreds = 5, enSixHundreds = 6, enEightHundreds = 7, 
    enThousand = 8, enExit = 9
};

void Login();
void ShowMainMenu();
void PerformMainMenuOption(enMainMenuOptions MenuOptions);
void GoBackToMainMenu();

struct stClientInfo {
    string AccountNumber;
    string PinCode;
    string ClientName = "";
    string ClientPhone = "";
    int AccountBalance = 0;
};

stClientInfo CurrentClient;

vector <string> SplitString(string S1, string Delim){
    vector <string> vString;

    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);  
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length()); 
    }

    if (S1 != "")
    {
        vString.push_back(S1);
    }

    return vString;

}

stClientInfo ConvertClientLineToRecord(string Line, string Seperator = "#//#"){
    stClientInfo ClientInfo;
    vector <string> vClientData;

    vClientData = SplitString(Line, Seperator);

    ClientInfo.AccountNumber = vClientData[0];
    ClientInfo.PinCode = vClientData[1];
    ClientInfo.ClientName = vClientData[2];
    ClientInfo.ClientPhone = vClientData[3];
    ClientInfo.AccountBalance = stoi(vClientData[4]);

    return ClientInfo;

}

vector <stClientInfo> LoadClientsDataFromFile(string FileName){

    vector <stClientInfo> vClients;

    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        string Line;
        stClientInfo ClientInfo;

        while (getline(MyFile, Line)) {
            ClientInfo = ConvertClientLineToRecord(Line);
            vClients.push_back(ClientInfo);
        }
        MyFile.close();
    }

    return vClients;

}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClientInfo &ClientInfo){
    
    vector <stClientInfo> vClients = LoadClientsDataFromFile(ClientsFileName);
    for (stClientInfo C : vClients) {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode) {
            ClientInfo = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode){
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

short GetBalanceToWithdraw(enQuickWithdraw QuickWithdraw){
    short Balance = 0;
    if (QuickWithdraw == enQuickWithdraw::enExit) {
        GoBackToMainMenu();
        return 0;
    }
    switch (QuickWithdraw)
    {
    case enQuickWithdraw::enTwenty:
        Balance = 20;
        break;
    case enQuickWithdraw::enFifty:
        Balance = 50;
        break;
    case enQuickWithdraw::enHundred:
        Balance = 100;
        break;
    case enQuickWithdraw::enTwoHundreds:
        Balance = 200;
        break;
    case enQuickWithdraw::enFourHundreds:
        Balance = 400;
        break;
    case enQuickWithdraw::enSixHundreds:
        Balance = 600;
        break;
    case enQuickWithdraw::enEightHundreds:
        Balance = 800;
        break;
    case enQuickWithdraw::enThousand:
        Balance = 1000;
        break;
    }
    return Balance;
}

enQuickWithdraw ReadBalanceWithdrawChoice() {
    int Choice = 0;
    while (true) {
    cout << "Choose what to withdraw from [1] to [8] ? ";
    cin >> Choice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        system("Color 4F");
        cout << "\nInvalid input. Numbers only, please.\n";
        continue;
    }
    if (Choice < 1 || Choice > 9) {
        system("Color 4F");
        cout << "\nInvalid choice. Please enter a number between 1 and 9.\n";
        continue;
    }
    break;
}
    return (enQuickWithdraw)Choice;
}

enMainMenuOptions ReadMainMenuOption() {
    short Choice = 0;
    while(true){
    cout << "Choose what do you want to do ? [1 to 5] ? ";
    cin >> Choice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        system("Color 4F");
        cout << "\nInvalid input. Numbers only, please.\n";
        continue;
    }
    if (Choice < 1 || Choice > 5) {
        system("Color 4F");
        cout << "\nInvalid choice. Please enter a number between 1 and 5.\n";
        continue;
    }
    break;
    }
    return (enMainMenuOptions)Choice;
}

void DisplayBalanceScreen() {
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";

    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

}

string ConvertClientRecordToLine(stClientInfo ClientInfo, string Seperator = "#//#") {
    string stClientRecord = "";
    stClientRecord += ClientInfo.AccountNumber + Seperator;
    stClientRecord += ClientInfo.PinCode + Seperator;
    stClientRecord += ClientInfo.ClientName + Seperator;
    stClientRecord += ClientInfo.ClientPhone + Seperator;
    stClientRecord += to_string(ClientInfo.AccountBalance);

    return stClientRecord;
}

vector <stClientInfo> SaveClientsDataToFile(string FileName, vector <stClientInfo> vClients) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open()) {
        for (stClientInfo C : vClients) {
            DataLine = ConvertClientRecordToLine(C);
            MyFile << DataLine << endl;
        }
        MyFile.close();
    }
    return vClients;
}

void UpdateClientInFile(stClientInfo UpdateClient) {
    vector <stClientInfo> vClients = LoadClientsDataFromFile(ClientsFileName);

    for (stClientInfo& C : vClients) {
        if (C.AccountNumber == UpdateClient.AccountNumber)
            C = UpdateClient;
    }
    SaveClientsDataToFile(ClientsFileName, vClients);
}

void PerformBalanceWithdraw(enQuickWithdraw QuickWithdraw) {
    short Balance = GetBalanceToWithdraw(QuickWithdraw);
    vector <stClientInfo> vClients = LoadClientsDataFromFile(ClientsFileName);
    char Choice = 'n';
    while (CurrentClient.AccountBalance < Balance) {
        cout << "\nThe amount exceeds your account balance, make another choice\n\n";
        QuickWithdraw = ReadBalanceWithdrawChoice();
        Balance = GetBalanceToWithdraw(QuickWithdraw);
    }
    cout << "\nAre you sure you want to perform this transaction ? (Y/N) ? ";
    cin >> Choice;
    if (Choice == 'y' || Choice == 'Y') {
        CurrentClient.AccountBalance -= Balance;
        cout << "\nDone Successfully, new balance is " << CurrentClient.AccountBalance << endl;
        UpdateClientInFile(CurrentClient);
    }
}

void DisplayQuickWithdrawScreen() {
    cout << "===========================================\n";
    cout << "\t\tQuick Withdraw Screen\n";
    cout << "===========================================\n";
    cout << "\t [1] 20\t\t [2] 50\n";
    cout << "\t [3] 100\t [4] 200\n";
    cout << "\t [5] 400\t [6] 600\n";
    cout << "\t [7] 800\t [8] 1000\n";
    cout << "\t [9] Exit\n";
    cout << "===========================================\n";

    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

    PerformBalanceWithdraw(ReadBalanceWithdrawChoice());
}

void DisplayNormalWithdrawScreen() {
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";
}

int ReadNormalWithdraw() {
    int WithdrawNumber = 0;
    while (true) {
        cout << "Enter an amount multiple of 5's ? ";
        cin >> WithdrawNumber;
        if (cin.fail() || WithdrawNumber <= 0 || WithdrawNumber % 5 != 0 ){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nInvalid input! Please enter a positive number.\n\n";
            continue;
        }
        break;
    }
    return WithdrawNumber;
}

void PerformWithdrawAction() {
    char Choice = 'n';
    DisplayNormalWithdrawScreen();
    int WithdrawNumber = ReadNormalWithdraw();
    while (CurrentClient.AccountBalance < WithdrawNumber) {
        cout << "\nThe amount exceeds your account balance, make another choice\n\n";
        WithdrawNumber = ReadNormalWithdraw();
    }
    cout << "Are you sure you want to perform this transaction ? (Y/N) ? ";
    cin >> Choice;
    if (Choice == 'y' || Choice == 'Y') {
        CurrentClient.AccountBalance -= WithdrawNumber;
        cout << "\nDone Successfully, new balance is " << CurrentClient.AccountBalance << endl;
        UpdateClientInFile(CurrentClient);
    }
}

void DisplayDepositScreen() {
    cout << "===========================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "===========================================\n";
}

int ReadDepositAmount() {
    int DepositAmount = 0;
    while (true) {
        cout << "Enter a positive deposit amount ? ";
        cin >> DepositAmount;
        if (cin.fail() || DepositAmount <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a positive number.\n";
            continue;
        }
        break;
    }
    return DepositAmount;
}

void PerformDepositAmount() {
    DisplayDepositScreen();
    int DepositAmount = ReadDepositAmount();
    char Answer = 'n';
    cout << "\nAre you sure you want to perform this transaction ? (Y/N) ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y') {
        CurrentClient.AccountBalance += DepositAmount;
        cout << "\nDone Successfully. New balance is: " << CurrentClient.AccountBalance << endl;
        UpdateClientInFile(CurrentClient);
    }
}

void PerformMainMenuOption(enMainMenuOptions MainMenuOptions){
    switch (MainMenuOptions)
    {
    case enMainMenuOptions::eQuickWithdraw:

        system("cls");
        DisplayQuickWithdrawScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eNormalWithdraw:
        system("cls");
        PerformWithdrawAction();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eDeposit:
        system("cls");
        PerformDepositAmount();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eCheckBalance:
        system("cls");
        DisplayBalanceScreen();
        GoBackToMainMenu();
        break;
    case enMainMenuOptions::eLogout:
        system("cls");
        Login();
        break;
    }
}

void ShowMainMenu() {
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";

    PerformMainMenuOption(ReadMainMenuOption());
}

void Login(){
    bool LoginFailed = false;
    string AccountNumber, PinCode;

    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";
        if (LoginFailed)
            cout << "Invalid AccountNumber/PinCode! \n";

        cout << "Enter Account Number ? ";
        cin >> AccountNumber;
        cout << "Enter PinCode ? ";
        cin >> PinCode;

        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);
    } while (LoginFailed);

    ShowMainMenu();
}

void GoBackToMainMenu() {
    system("Color 0F");
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenu();
}

int main()
{
    Login();
}

