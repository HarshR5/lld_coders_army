
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

// ========== USER ==========

class User {
public:
    int id;
    string name;

    User(int id, string name) {
        this->id = id;
        this->name = name;
    }
};

// ========== STRATEGY PATTERN ==========

class SplitStrategy {
public:
    virtual vector<double> split(
        double amount, int n,
        vector<double> values
    ) = 0;

    virtual ~SplitStrategy() {}
};

// Equal Split
class EqualSplit : public SplitStrategy {
public:
    vector<double> split(
        double amount, int n,
        vector<double> values
    ) {
        vector<double> result(n, amount / n);
        return result;
    }
};

// Exact Split
class ExactSplit : public SplitStrategy {
public:
    vector<double> split(
        double amount, int n,
        vector<double> values
    ) {
        return values;
    }
};

// Percentage Split
class PercentSplit : public SplitStrategy {
public:
    vector<double> split(
        double amount, int n,
        vector<double> values
    ) {
        vector<double> result;

        for (int i = 0; i < n; i++) {
            result.push_back(amount * values[i] / 100);
        }

        return result;
    }
};

// ========== EXPENSE ==========

class Expense {
public:
    User* paidBy;
    double amount;
    vector<User*> participants;
    vector<double> shares;

    Expense(User* payer, double amount,
            vector<User*> users,
            vector<double> shares) {
        this->paidBy = payer;
        this->amount = amount;
        this->participants = users;
        this->shares = shares;
    }
};

// ========== SPLITWISE ==========

class Splitwise {
private:
    vector<Expense*> expenses;

    // balance[A][B] = A owes B
    map<int, map<int, double> > balance;

public:

    void addExpense(
        User* paidBy,
        double amount,
        vector<User*> participants,
        SplitStrategy* strategy,
        vector<double> values
    ) {
        int n = participants.size();

        vector<double> shares =
            strategy->split(amount, n, values);

        Expense* expense = new Expense(
            paidBy, amount, participants, shares
        );

        expenses.push_back(expense);

        for (int i = 0; i < n; i++) {
            User* user = participants[i];

            if (user->id == paidBy->id) {
                continue;
            }

            balance[user->id][paidBy->id] += shares[i];
        }

        cout << "Expense Added Successfully!\n";
    }

    void showBalance(User* u1, User* u2) {
        double amount =
            balance[u1->id][u2->id] -
            balance[u2->id][u1->id];

        if (amount > 0) {
            cout << u1->name << " owes "
                 << u2->name << " Rs " << amount << endl;
        }
        else if (amount < 0) {
            cout << u2->name << " owes "
                 << u1->name << " Rs " << -amount << endl;
        }
        else {
            cout << "No pending balance\n";
        }
    }

    void settle(User* from, User* to, double amount) {
        // Simple demo: assumes valid payment
        balance[from->id][to->id] -= amount;
        cout << "Payment settled!\n";
    }
};

// ========== MAIN ==========

int main() {

    User* aman = new User(1, "Aman");
    User* rahul = new User(2, "Rahul");
    User* priya = new User(3, "Priya");

    Splitwise app;

    vector<User*> users;
    users.push_back(aman);
    users.push_back(rahul);
    users.push_back(priya);

    EqualSplit equal;
    ExactSplit exact;
    PercentSplit percent;

    // 1. Aman paid Rs 900, equally split
    vector<double> empty;

    app.addExpense(
        aman, 900, users, &equal, empty
    );

    app.showBalance(rahul, aman);
    app.showBalance(priya, aman);

    // 2. Rahul paid Rs 600, exact split
    vector<double> exactValues;
    exactValues.push_back(100);
    exactValues.push_back(200);
    exactValues.push_back(300);

    app.addExpense(
        rahul, 600, users, &exact, exactValues
    );

    app.showBalance(aman, rahul);

    // 3. Priya paid Rs 1000, percent split
    vector<double> percentages;
    percentages.push_back(50);
    percentages.push_back(30);
    percentages.push_back(20);

    app.addExpense(
        priya, 1000, users, &percent, percentages
    );

    app.showBalance(rahul, priya);

    // Settlement
    app.settle(rahul, aman, 100);

    return 0;
}
