#include <iostream>
#include <string>
using namespace std;

// Interface: the common contract.
class Notification {
public:
    virtual void send(const string& message) = 0;
    virtual ~Notification() = default;
};

// Concrete strategy 1.
class EmailNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Email: " << message << '\n';
    }
};

// Concrete strategy 2.
class SMSNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "SMS: " << message << '\n';
    }
};

// Concrete strategy 3.
class PushNotification : public Notification {
public:
    void send(const string& message) override {
        cout << "Push: " << message << '\n';
    }
};

// Client.
int main() {
    EmailNotification email;
    SMSNotification sms;
    PushNotification push;

    // Select Email as the strategy.
    Notification* strategy = &email;
    strategy->send("Your order is confirmed!");

    // Switch to SMS.
    strategy = &sms;
    strategy->send("Your order is confirmed!");

    // Switch to Push.
    strategy = &push;
    strategy->send("Your order is confirmed!");

    return 0;
}