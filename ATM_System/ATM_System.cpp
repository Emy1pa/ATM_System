#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

const string ClientsFileName = "Clients.txt";

void Login();
void ShowMainMenu();

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

int main()
{
    Login();
}

