#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Account {
protected:
    string owner;
    double balance;
public:
    Account(string name, double bal) : owner(name), balance(bal) {}
    virtual ~Account() {}
    bool deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid amount.\n";
            return false;
        }
        balance += amount;
        cout << "Deposited $" << amount << " | Balance: $" << balance << "\n";
        return true;
    }
    virtual bool withdraw(double amount) = 0;
    virtual void display() const = 0;
    double getBalance() const { return balance; }
    string getOwner() const { return owner; }
};

class SavingsAccount : public Account {
    double minBalance;
public:
    SavingsAccount(string name, double bal) : Account(name, bal), minBalance(500) {}
    bool withdraw(double amount) override {
        if (amount <= 0) { cout << "Invalid amount.\n"; return false; }
        if (balance - amount < minBalance) {
            cout << "Can't withdraw. Minimum balance $" << minBalance << " required.\n";
            return false;
        }
        balance -= amount;
        cout << "Withdrew $" << amount << " | Balance: $" << balance << "\n";
        return true;
    }
    void display() const override {
        cout << "[Savings]  Owner: " << owner
             << " | Balance: $" << balance
             << " | Min Balance: $" << minBalance << "\n";
    }
};

class CurrentAccount : public Account {
    double overdraftLimit;
public:
    CurrentAccount(string name, double bal) : Account(name, bal), overdraftLimit(1000) {}
    bool withdraw(double amount) override {
        if (amount <= 0) { cout << "Invalid amount.\n"; return false; }
        if (balance - amount < -overdraftLimit) {
            cout << "Overdraft limit of $" << overdraftLimit << " exceeded.\n";
            return false;
        }
        balance -= amount;
        cout << "Withdrew $" << amount << " | Balance: $" << balance << "\n";
        return true;
    }
    void display() const override {
        cout << "[Current]  Owner: " << owner
             << " | Balance: $" << balance
             << " | Overdraft Limit: $" << overdraftLimit << "\n";
    }
};

void showMenu() {
    cout << "\nBANK MENU\n";
    cout << "1. Add Savings Account\n";
    cout << "2. Add Current Account\n";
    cout << "3. Display All Accounts\n";
    cout << "4. Deposit\n";
    cout << "5. Withdraw\n";
    cout << "6. Exit\n";
    cout << "Choice: ";
}

int selectAccount(vector<Account*>& accounts) {
    if (accounts.empty()) {
        cout << "No accounts found.\n";
        return -1;
    }
    cout << "\nSelect account:\n";
    for (int i = 0; i < (int)accounts.size(); i++) {
        cout << i + 1 << ". ";
        accounts[i]->display();
    }
    int idx;
    cout << "Choice: ";
    if (!(cin >> idx)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input.\n";
        return -1;
    }
    if (idx < 1 || idx > (int)accounts.size()) {
        cout << "Invalid choice.\n";
        return -1;
    }
    return idx - 1;
}

int main() {
    vector<Account*> accounts;
    int choice;

    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Enter a number.\n";
            continue;
        }

        if (choice == 1) {
            string name; double bal;
            cout << "Owner name: "; cin >> name;
            cout << "Initial balance: $"; cin >> bal;
            accounts.push_back(new SavingsAccount(name, bal));
            cout << "Savings account created.\n";

        } else if (choice == 2) {
            string name; double bal;
            cout << "Owner name: "; cin >> name;
            cout << "Initial balance: $"; cin >> bal;
            accounts.push_back(new CurrentAccount(name, bal));
            cout << "Current account created.\n";

        } else if (choice == 3) {
            if (accounts.empty()) { cout << "No accounts.\n"; continue; }
            cout << "\n";
            for (auto acc : accounts) acc->display();

        } else if (choice == 4) {
            int idx = selectAccount(accounts);
            if (idx == -1) continue;
            double amount;
            cout << "Amount to deposit: $"; cin >> amount;
            accounts[idx]->deposit(amount);

        } else if (choice == 5) {
            int idx = selectAccount(accounts);
            if (idx == -1) continue;
            double amount;
            cout << "Amount to withdraw: $"; cin >> amount;
            accounts[idx]->withdraw(amount);

        } else if (choice == 6) {
            cout << "Goodbye!\n";
        } else {
            cout << "Invalid choice.\n";
        }

    } while (choice != 6);

    for (auto acc : accounts) delete acc;
    return 0;
}