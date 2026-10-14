
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
using namespace std;

// ======================================
// 1. DURATION TYPE
// ======================================

enum DurationType {
    HOURS,
    DAYS
};

// ======================================
// 2. PARKING FEE STRATEGY PATTERN
// ======================================

class ParkingFeeStrategy {
public:
    virtual double calculateFee(string vehicleType,
                                int duration,
                                DurationType durationType) = 0;

    virtual ~ParkingFeeStrategy() {}
};

class BasicFeeStrategy : public ParkingFeeStrategy {
public:
    double calculateFee(string vehicleType,
                        int duration,
                        DurationType durationType) {
        double rate = 10;

        if (vehicleType == "Car") {
            rate = 20;
        }
        else if (vehicleType == "Bike") {
            rate = 10;
        }

        if (durationType == DAYS) {
            duration = duration * 24;
        }

        return rate * duration;
    }
};

// ======================================
// 3. VEHICLE FACTORY PATTERN
// ======================================

class Vehicle {
private:
    string licensePlate;
    string vehicleType;
    ParkingFeeStrategy* feeStrategy;

protected:
    Vehicle(string licensePlate, string vehicleType,
            ParkingFeeStrategy* feeStrategy) {
        this->licensePlate = licensePlate;
        this->vehicleType = vehicleType;
        this->feeStrategy = feeStrategy;
    }

public:
    virtual ~Vehicle() {}

    string getVehicleType() {
        return vehicleType;
    }

    string getLicensePlate() {
        return licensePlate;
    }

    double calculateFee(int duration,
                        DurationType durationType) {
        return feeStrategy->calculateFee(
            vehicleType, duration, durationType
        );
    }
};

class CarVehicle : public Vehicle {
public:
    CarVehicle(string licensePlate, string vehicleType,
               ParkingFeeStrategy* feeStrategy)
        : Vehicle(licensePlate, vehicleType, feeStrategy) {
    }
};

class BikeVehicle : public Vehicle {
public:
    BikeVehicle(string licensePlate, string vehicleType,
                ParkingFeeStrategy* feeStrategy)
        : Vehicle(licensePlate, vehicleType, feeStrategy) {
    }
};

class OtherVehicle : public Vehicle {
public:
    OtherVehicle(string licensePlate, string vehicleType,
                 ParkingFeeStrategy* feeStrategy)
        : Vehicle(licensePlate, vehicleType, feeStrategy) {
    }
};

class VehicleFactory {
public:
    static Vehicle* createVehicle(
        string vehicleType,
        string licensePlate,
        ParkingFeeStrategy* feeStrategy) {

        if (vehicleType == "Car") {
            return new CarVehicle(
                licensePlate, vehicleType, feeStrategy
            );
        }
        else if (vehicleType == "Bike") {
            return new BikeVehicle(
                licensePlate, vehicleType, feeStrategy
            );
        }

        return new OtherVehicle(
            licensePlate, vehicleType, feeStrategy
        );
    }
};

// ======================================
// 4. PAYMENT STRATEGY PATTERN
// ======================================

class PaymentStrategy {
public:
    virtual void processPayment(double amount) = 0;
    virtual ~PaymentStrategy() {}
};

class CashPayment : public PaymentStrategy {
public:
    CashPayment(double fee) {
        // Same constructor as Java
    }

    void processPayment(double amount) {
        cout << "Processing cash payment of $"
             << amount << endl;
    }
};

class UPIPayment : public PaymentStrategy {
public:
    void processPayment(double amount) {
        cout << "Processing UPI payment of $"
             << amount << endl;
    }
};

class Payment {
private:
    double amount;
    PaymentStrategy* paymentStrategy;

public:
    Payment(double amount, PaymentStrategy* paymentStrategy) {
        this->amount = amount;
        this->paymentStrategy = paymentStrategy;
    }

    void processPayment() {
        if (amount > 0) {
            paymentStrategy->processPayment(amount);
        }
        else {
            cout << "Invalid payment amount." << endl;
        }
    }
};

// ======================================
// 5. PARKING SPOT ABSTRACT CLASS
// ======================================

class ParkingSpot {
private:
    int spotNumber;
    bool occupied;
    Vehicle* vehicle;
    string spotType;

public:
    ParkingSpot(int spotNumber, string spotType) {
        this->spotNumber = spotNumber;
        this->spotType = spotType;
        this->occupied = false;
        this->vehicle = NULL;
    }

    virtual ~ParkingSpot() {}

    bool isOccupied() {
        return occupied;
    }

    virtual bool canParkVehicle(Vehicle* vehicle) = 0;

    void parkVehicle(Vehicle* vehicle) {
        if (occupied) {
            throw runtime_error("Spot is already occupied");
        }

        if (!canParkVehicle(vehicle)) {
            throw invalid_argument("Spot is not suitable");
        }

        this->vehicle = vehicle;
        this->occupied = true;
    }

    void vacate() {
        if (!occupied) {
            throw runtime_error("Spot is already vacant");
        }

        this->vehicle = NULL;
        this->occupied = false;
    }

    int getSpotNumber() {
        return spotNumber;
    }

    Vehicle* getVehicle() {
        return vehicle;
    }

    string getSpotType() {
        return spotType;
    }
};

// ======================================
// 6. CONCRETE PARKING SPOTS
// ======================================

class CarParkingSpot : public ParkingSpot {
public:
    CarParkingSpot(int number)
        : ParkingSpot(number, "Car") {
    }

    bool canParkVehicle(Vehicle* vehicle) {
        return vehicle != NULL &&
               vehicle->getVehicleType() == "Car";
    }
};

class BikeParkingSpot : public ParkingSpot {
public:
    BikeParkingSpot(int number)
        : ParkingSpot(number, "Bike") {
    }

    bool canParkVehicle(Vehicle* vehicle) {
        return vehicle != NULL &&
               vehicle->getVehicleType() == "Bike";
    }
};

// ======================================
// 7. PARKING LOT CONTROLLER
// ======================================

class ParkingLot {
private:
    vector<ParkingSpot*> parkingSpots;

public:
    ParkingLot(vector<ParkingSpot*> parkingSpots) {
        this->parkingSpots = parkingSpots;
    }

    ParkingSpot* findAvailableSpot(string vehicleType) {
        for (int i = 0; i < parkingSpots.size(); i++) {
            ParkingSpot* spot = parkingSpots[i];

            if (!spot->isOccupied() &&
                spot->getSpotType() == vehicleType) {
                return spot;
            }
        }

        return NULL;
    }

    ParkingSpot* parkVehicle(Vehicle* vehicle) {
        ParkingSpot* spot =
            findAvailableSpot(vehicle->getVehicleType());

        if (spot != NULL) {
            spot->parkVehicle(vehicle);

            cout << "Vehicle parked successfully in spot: "
                 << spot->getSpotNumber() << endl;

            return spot;
        }

        cout << "No parking spots available for "
             << vehicle->getVehicleType() << "!" << endl;

        return NULL;
    }

    void vacateSpot(ParkingSpot* spot, Vehicle* vehicle) {
        if (spot != NULL &&
            spot->isOccupied() &&
            spot->getVehicle() == vehicle) {

            spot->vacate();

            cout << vehicle->getVehicleType()
                 << " vacated the spot: "
                 << spot->getSpotNumber() << endl;
        }
        else {
            cout << "Invalid operation!" << endl;
        }
    }

    ParkingSpot* getSpotByNumber(int spotNumber) {
        for (int i = 0; i < parkingSpots.size(); i++) {
            if (parkingSpots[i]->getSpotNumber() == spotNumber) {
                return parkingSpots[i];
            }
        }

        return NULL;
    }

    vector<ParkingSpot*> getParkingSpots() {
        return parkingSpots;
    }
};

// ======================================
// 8. MAIN FUNCTION
// ======================================

int main() {

    // Create fee strategy
    BasicFeeStrategy feeStrategy;

    // Create parking spots
    CarParkingSpot carSpot1(101);
    CarParkingSpot carSpot2(102);
    BikeParkingSpot bikeSpot1(201);

    // Add spots to parking lot
    vector<ParkingSpot*> spots;

    spots.push_back(&carSpot1);
    spots.push_back(&carSpot2);
    spots.push_back(&bikeSpot1);

    // Constructor dependency injection
    ParkingLot parkingLot(spots);

    // Create vehicles using Factory Pattern
    Vehicle* car = VehicleFactory::createVehicle(
        "Car", "DL01AB1234", &feeStrategy
    );

    Vehicle* bike = VehicleFactory::createVehicle(
        "Bike", "DL02XY5678", &feeStrategy
    );

    // Park vehicles
    ParkingSpot* assignedCarSpot =
        parkingLot.parkVehicle(car);

    ParkingSpot* assignedBikeSpot =
        parkingLot.parkVehicle(bike);

    // Calculate fee and pay
    if (assignedCarSpot != NULL) {
        double fee = car->calculateFee(2, HOURS);

        cout << "Car parking fee: $" << fee << endl;

        CashPayment cash(fee);
        Payment payment(fee, &cash);

        payment.processPayment();

        parkingLot.vacateSpot(assignedCarSpot, car);
    }

    if (assignedBikeSpot != NULL) {
        double fee = bike->calculateFee(3, HOURS);

        cout << "Bike parking fee: $" << fee << endl;

        UPIPayment upi;
        Payment payment(fee, &upi);

        payment.processPayment();

        parkingLot.vacateSpot(assignedBikeSpot, bike);
    }

    delete car;
    delete bike;

    return 0;
}
