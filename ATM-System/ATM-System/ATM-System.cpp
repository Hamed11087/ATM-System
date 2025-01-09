#include <iostream>
#include <string>
#include<fstream>
#include<iomanip>
#include<vector>
using namespace std;

const string ClientFileName = "Clients.txt";
void Login();
void ShowATMMainMenue();
void GoBackToATMMainMenue();

enum enATMMainMenue {
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3,
    eShowBalance = 4, eLogOut = 5
};

struct sClient
{
    string AccoutNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

sClient CurrentClient;

vector<string>SplitString(string S1, string Delim)
{
    vector<string>vString;
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

sClient ConverLineToRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string>vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccoutNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConverRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";

    stClientRecord += Client.AccoutNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccoutNumber, string FileName)
{
    vector<sClient>vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConverLineToRecord(Line);
            if (Client.AccoutNumber == AccoutNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);

        }
        MyFile.close();
    }

    return false;
}

vector<sClient>LoadClientsDataFromFile(string FileName)
{
    vector<sClient>vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConverLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient>vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccoutNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }

    return false;
}

bool FindClientByAccountNumberAndPidCode(string AccountNumber, string PinCode, sClient& Client)
{
    vector<sClient>vClients = LoadClientsDataFromFile(ClientFileName);
    for (sClient C : vClients)
    {
        if (C.AccoutNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }

    return false;

}

vector<sClient>SaveClientsDataToFile(string FileName, vector<sClient>vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
            if (C.MarkForDelete == false)
            {
                DataLine = ConverRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vClients;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

bool DepositBalanceTOClientByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClient)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n? ";
    cin >> Answer;

    if (Answer == 'Y' || Answer == 'y')
    {
        for (sClient& C : vClient)
        {
            if (C.AccoutNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientFileName, vClient);
                cout << "\n\nDone Successfully. New balance is :" << C.AccountBalance;

                return true;
            }

        }
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;

    return AccountNumber;
}

short ReadQucikWithDraw()
{
    short Choise = 0;
    cout << "Choose what to withdraw form[1] to [8]? ";
    cin >> Choise;

    return (Choise == 1 ? 20 : (Choise == 2) ? 50 : (Choise == 3) ? 100 : (Choise == 4) ? 200 : (Choise == 5) ? 400 : (Choise == 6) ? 600
        : (Choise == 7) ? 800 : (Choise == 8) ? 1000 : 9);
}

void ShowEndScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tProgram Ends :-)";
    cout << "\n-----------------------------------\n";
}

void ShowDepositScreen()
{
    cout << "===========================================\n";
    cout << "\tDeposit Screen\n";
    cout << "===========================================\n";


    vector <sClient> vClients = LoadClientsDataFromFile(ClientFileName);
    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;
    DepositBalanceTOClientByAccountNumber(CurrentClient.AccoutNumber, Amount, vClients);
}

void ShowQuickWithDrawScreen()
{
    cout << "===========================================\n";
    cout << "\t\tQucik Withdraw\n";
    cout << "===========================================\n";
    cout << "\t[1] 20 \t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";

    short Amount;
    Amount = ReadQucikWithDraw();

    vector<sClient>vClient = LoadClientsDataFromFile(ClientFileName);
    FindClientByAccountNumber(CurrentClient.AccoutNumber, vClient, CurrentClient);

    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << CurrentClient.AccountBalance << endl;
        cout << "Please enter another amount? ";
        Amount = ReadQucikWithDraw();
    }

    if (Amount == 9)
        return;

    DepositBalanceTOClientByAccountNumber(CurrentClient.AccoutNumber, Amount * -1, vClient);
}

void ShowNormalWithDrawScreen()
{
    short Amount = 0;
    cout << "===========================================\n";
    cout << "\t\tNormal Withdraw Screen\n";
    cout << "===========================================\n";
    do
    {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;
    } while (Amount % 5 != 0);

    vector<sClient>vClient = LoadClientsDataFromFile(ClientFileName);
    FindClientByAccountNumber(CurrentClient.AccoutNumber, vClient, CurrentClient);

    while (Amount > CurrentClient.AccountBalance || (Amount % 5 != 0))
    {
        cout << "\nThe Amount Exceeds the balance, make another choice : ";
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> Amount;
    }



    DepositBalanceTOClientByAccountNumber(CurrentClient.AccoutNumber, Amount * -1, vClient);
}

void ShowBalanesScreen()
{
    cout << "===========================================\n";
    cout << "\t\tCheck Balance Screen\n";
    cout << "===========================================\n";
    vector<sClient>vClient = LoadClientsDataFromFile(ClientFileName);
    FindClientByAccountNumber(CurrentClient.AccoutNumber, vClient, CurrentClient);
    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;
}

void GoBackToATMMainMenue()
{
    cout << "\n\nPress any key to go back to ATM Main Menue...";
    system("pause>0");
    ShowATMMainMenue();
}

short ReadATMMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice = 0;
    cin >> Choice;
    return Choice;
}

void PerfromATMMainMenueOption(enATMMainMenue ATMMainMenue)
{
    switch (ATMMainMenue)
    {
    case enATMMainMenue::eQuickWithdraw:
    {
        system("cls");
        ShowQuickWithDrawScreen();
        GoBackToATMMainMenue();
        break;
    }
    case enATMMainMenue::eNormalWithdraw:
    {
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToATMMainMenue();
        break;
    }
    case enATMMainMenue::eDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToATMMainMenue();
        break;
    }
    case enATMMainMenue::eShowBalance:
    {
        system("cls");
        ShowBalanesScreen();
        GoBackToATMMainMenue();

    }
    case enATMMainMenue::eLogOut:
    {
        Login();
    }
    }
}

void ShowATMMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Main Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balances.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromATMMainMenueOption((enATMMainMenue)ReadATMMenueOption());
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindClientByAccountNumberAndPidCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;

    string AccountNumber, PidCode;
    do
    {
        system("cls");
        cout << "-------------------------------------------\n";
        cout << "\t\tLogin Screen\n";
        cout << "-------------------------------------------\n";


        if (LoginFaild)
        {
            cout << "Invlaid AccountNumber/PinCode!\n";
        }

        cout << "Enter Account Number? ";
        cin >> AccountNumber;
        cout << "Enter Password? ";
        cin >> PidCode;

        LoginFaild = !LoadClientInfo(AccountNumber, PidCode);
    } while (LoginFaild);
    ShowATMMainMenue();


}

int main()
{
    Login();
    system("pause>0");
    return 0;
}
