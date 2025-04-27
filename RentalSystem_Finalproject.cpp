// CarRentalSystem.cpp - A C++ console application for a car rental system
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Base class for vehicles (abstract representation)
class Vehicle {
public:
    string brand;
    string model;
    double pricePerDay;
    bool isAvailable;
public:
    // Constructor
    Vehicle(const string& b, const string& m, double price) 
        : brand(b), model(m), pricePerDay(price), isAvailable(true) {}

    // Virtual destructor for proper cleanup
    virtual ~Vehicle() {}

    // Display vehicle information (override in subclasses)
    virtual void displayInfo() {
        cout << brand << " " << model << " - $" << pricePerDay << " per day ";
        cout << (isAvailable ? "(Available)" : "(Rented)") << endl;
    }

    // Rent the vehicle (mark as not available)
    void rentVehicle() {
        isAvailable = false;
    }
    // Return the vehicle (mark as available)
    void returnVehicle() {
        isAvailable = true;
    }
    // Check availability
    bool getAvailability() const {
        return isAvailable;
    }
    // Get price per day
    double getPrice() const {
        return pricePerDay;
    }
};

// Sedan class (derived from Vehicle)
class Sedan : public Vehicle {
public:
    Sedan(const string& b, const string& m, double price)
        : Vehicle(b, m, price) {}
    // Display specific to Sedan
    void displayInfo() override {
        cout << "Sedan: ";
        Vehicle::displayInfo();
    }
};

// SUV class (derived from Vehicle)
class SUV : public Vehicle {
public:
    SUV(const string& b, const string& m, double price)
        : Vehicle(b, m, price) {}
    // Display specific to SUV
    void displayInfo() override {
        cout << "SUV:   ";
        Vehicle::displayInfo();
    }
};

// Truck class (derived from Vehicle)
class Truck : public Vehicle {
public:
    Truck(const string& b, const string& m, double price)
        : Vehicle(b, m, price) {}
    // Display specific to Truck
    void displayInfo() override {
        cout << "Truck: ";
        Vehicle::displayInfo();
    }
};

// Booking class to store rental details
class Booking {
private:
    Vehicle* vehicle;
    int days;
    double totalCost;
public:
    Booking(Vehicle* v, int d) : vehicle(v), days(d) {
        if (vehicle != nullptr) {
            totalCost = vehicle->getPrice() * d;
        } else {
            totalCost = 0;
        }
    }
    // Print bill receipt for this booking
    void printBill(const string& customerName) const {
        cout << "\n--------------- Bill Receipt ---------------\n";
        cout << "Customer: " << customerName << endl;
        cout << "Vehicle: ";
        // We want to display brand and model without showing "(Rented)" or "(Available)" in the receipt
        cout << vehicle->brand << " " << vehicle->model << "\n";
        cout << "Days: " << days << endl;
        cout << "Price per day: $" << vehicle->getPrice() << endl;
        cout << "Total Cost: $" << totalCost << endl;
        cout << "--------------------------------------------\n";
    }
};

// Feedback class to store customer feedback
class Feedback {
public:
    string username;
    string comment;
    Feedback(const string& user, const string& comm) : username(user), comment(comm) {}
};

// Customer class
class Customer {
private:
    string name;
    string username;
    string password;
    vector<Booking> bookingHistory;      // List of past bookings
    vector<Vehicle*> rentedVehicles;     // List of vehicles currently rented
public:
    Customer(const string& n = "", const string& u = "", const string& p = "")
        : name(n), username(u), password(p) {}

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getName() const { return name; }
    void setPassword(const string& p) { password = p; }

    // Add a booking record (when renting a vehicle)
    void addBooking(const Booking& b) {
        bookingHistory.push_back(b);
    }
    // View all past bookings
    void viewBookingHistory() const {
        if (bookingHistory.empty()) {
            cout << "No booking history available." << endl;
        } else {
            cout << "\n----- Booking History for " << name << " -----\n";
            for (const auto& b : bookingHistory) {
                b.printBill(name);
            }
            cout << "---------------------------------------\n";
        }
    }
    // Add a vehicle to currently rented list
    void addRentedVehicle(Vehicle* v) {
        rentedVehicles.push_back(v);
    }
    // Remove a vehicle from rented list
    bool removeRentedVehicle(Vehicle* v) {
        auto it = find(rentedVehicles.begin(), rentedVehicles.end(), v);
        if (it != rentedVehicles.end()) {
            rentedVehicles.erase(it);
            return true;
        }
        return false;
    }
    // Check if this customer has rented the given vehicle
    bool hasRented(Vehicle* v) const {
        return (find(rentedVehicles.begin(), rentedVehicles.end(), v) != rentedVehicles.end());
    }
};

// Main system class managing vehicles, customers, and operations
class CarRentalSystem {
private:
    vector<Vehicle*> vehicles;         // All vehicles in the system
    vector<Customer> customers;        // All registered customers
    vector<Feedback> feedbacks;        // All submitted feedbacks
    Customer* currentUser;            // Pointer to the logged-in customer
public:
    CarRentalSystem() : currentUser(nullptr) {
        // Initialize with some sample vehicles
        vehicles.push_back(new Sedan("Toyota", "Camry", 50.0));
        vehicles.push_back(new SUV("Honda", "CR-V", 65.0));
        vehicles.push_back(new Truck("Ford", "F-150", 80.0));
    }
    ~CarRentalSystem() {
        // Clean up dynamically allocated vehicle objects
        for (Vehicle* v : vehicles) {
            delete v;
        }
    }

    // Register a new customer
    bool registerCustomer(const string& name, const string& username, const string& password) {
        // Check for duplicate username
        for (const Customer& c : customers) {
            if (c.getUsername() == username) {
                cout << "❌ Username already taken. Please choose a different username." << endl;
                return false;
            }
        }
        customers.push_back(Customer(name, username, password));
        cout << "✅ User registered successfully!" << endl;
        return true;
    }
    // Log in a customer
    bool loginCustomer(const string& username, const string& password) {
        for (auto& c : customers) {
            if (c.getUsername() == username && c.getPassword() == password) {
                currentUser = &c;
                cout << "✅ Login successful! Welcome, " << c.getName() << "." << endl;
                return true;
            }
        }
        cout << "❌ Invalid username or password!" << endl;
        return false;
    }
    // Reset password for a customer
    bool resetPassword(const string& username, const string& newPassword) {
        for (auto& c : customers) {
            if (c.getUsername() == username) {
                c.setPassword(newPassword);
                cout << "✅ Password has been reset successfully for user " << username << "!" << endl;
                return true;
            }
        }
        cout << "❌ Username not found!" << endl;
        return false;
    }
    // Add a new vehicle to the system (admin function)
    void addVehicle(const string& type, const string& brand, const string& model, double price) {
        if (type == "Sedan") {
            vehicles.push_back(new Sedan(brand, model, price));
        } else if (type == "SUV") {
            vehicles.push_back(new SUV(brand, model, price));
        } else if (type == "Truck") {
            vehicles.push_back(new Truck(brand, model, price));
        }
        cout << "✅ " << type << " added successfully!" << endl;
    }
    // Display all vehicles (for admin or informational purposes)
    void displayAllVehicles() {
        cout << "\n----- All Vehicles -----\n";
        if (vehicles.empty()) {
            cout << "No vehicles have been added to the system yet." << endl;
        } else {
            int index = 1;
            for (Vehicle* v : vehicles) {
                cout << index << ". ";
                v->displayInfo();
                index++;
            }
        }
        cout << "------------------------" << endl;
    }
    // Display only available vehicles (customer use)
    void displayAvailableVehicles() {
        cout << "\n----- Available Vehicles -----\n";
        bool anyAvailable = false;
        int index = 1;
        for (Vehicle* v : vehicles) {
            if (v->getAvailability()) {
                cout << index << ". ";
                v->displayInfo();
                anyAvailable = true;
            }
            index++;
        }
        if (!anyAvailable) {
            cout << "❗ No vehicles available at the moment." << endl;
        }
        cout << "------------------------------" << endl;
    }
    // Rent a vehicle for the current user
    bool rentVehicle(int index, int days) {
        if (currentUser == nullptr) {
            cout << "❌ No user is currently logged in." << endl;
            return false;
        }
        if (index < 1 || index > (int)vehicles.size()) {
            cout << "❌ Invalid vehicle selection!" << endl;
            return false;
        }
        Vehicle* v = vehicles[index - 1];
        if (!v->getAvailability()) {
            cout << "❌ Vehicle is already rented!" << endl;
            return false;
        }
        if (days < 1 || days > 30) {
            cout << "❌ Invalid number of days! Please enter between 1 and 30." << endl;
            return false;
        }
        // Complete the rental
        v->rentVehicle();
        currentUser->addRentedVehicle(v);
        Booking b(v, days);
        currentUser->addBooking(b);
        cout << "✅ Vehicle rented successfully!" << endl;
        // Print bill receipt
        b.printBill(currentUser->getName());
        return true;
    }
    // Return a vehicle for the current user
    bool returnVehicle(int index) {
        if (currentUser == nullptr) {
            cout << "❌ No user is currently logged in." << endl;
            return false;
        }
        if (index < 1 || index > (int)vehicles.size()) {
            cout << "❌ Invalid vehicle selection!" << endl;
            return false;
        }
        Vehicle* v = vehicles[index - 1];
        if (v->getAvailability()) {
            cout << "❌ This vehicle is not currently rented." << endl;
            return false;
        }
        if (!currentUser->hasRented(v)) {
            cout << "❌ You have not rented this vehicle!" << endl;
            return false;
        }
        // Complete the return
        v->returnVehicle();
        currentUser->removeRentedVehicle(v);
        cout << "✅ Vehicle returned successfully!" << endl;
        return true;
    }
    // Submit feedback (by current user)
    void giveFeedback(const string& comment) {
        if (currentUser != nullptr) {
            feedbacks.push_back(Feedback(currentUser->getUsername(), comment));
            cout << "✅ Feedback submitted. Thank you!" << endl;
        } else {
            cout << "❌ Please login as customer to give feedback." << endl;
        }
    }
    // Admin views all feedback
    void viewFeedbacks() {
        cout << "\n----- Customer Feedback -----\n";
        if (feedbacks.empty()) {
            cout << "No feedback has been submitted yet." << endl;
        } else {
            for (const auto& fb : feedbacks) {
                cout << fb.username << ": " << fb.comment << endl;
            }
        }
        cout << "-----------------------------" << endl;
    }
    // Get the pointer to current user
    Customer* getCurrentUser() {
        return currentUser;
    }
};

// Entry point
int main() {
    CarRentalSystem system;
    int choice;
    while (true) {
        // Main Menu
        cout << "\n========================================" << endl;
        cout << "       🚗 Car Rental System Main Menu 🚗" << endl;
        cout << "========================================" << endl;
        cout << "1. Customer Registration" << endl;
        cout << "2. Customer Login" << endl;
        cout << "3. Admin Login" << endl;
        cout << "4. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            // Customer Registration
            string name, username, password;
            cout << "Enter Name: ";
            cin >> ws; 
            getline(cin, name);
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;
            system.registerCustomer(name, username, password);
        }
        else if (choice == 2) {
            // Customer Login
            string username, password;
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;
            if (system.loginCustomer(username, password)) {
                // Customer menu after successful login
                int custChoice;
                while (true) {
                    cout << "\n--------- Customer Menu ---------" << endl;
                    cout << "1. View Available Vehicles" << endl;
                    cout << "2. Rent Vehicle" << endl;
                    cout << "3. Return Vehicle" << endl;
                    cout << "4. View Booking History" << endl;
                    cout << "5. Submit Feedback" << endl;
                    cout << "6. Logout" << endl;
                    cout << "---------------------------------" << endl;
                    cout << "Enter your choice: ";
                    cin >> custChoice;
                    if (custChoice == 1) {
                        system.displayAvailableVehicles();
                    }
                    else if (custChoice == 2) {
                        int vehNo, days;
                        system.displayAvailableVehicles();
                        cout << "Enter vehicle number to rent: ";
                        cin >> vehNo;
                        cout << "Enter number of days to rent (max 30): ";
                        cin >> days;
                        system.rentVehicle(vehNo, days);
                    }
                    else if (custChoice == 3) {
                        int vehNo;
                        cout << "Enter vehicle number to return: ";
                        cin >> vehNo;
                        system.returnVehicle(vehNo);
                    }
                    else if (custChoice == 4) {
                        Customer* curr = system.getCurrentUser();
                        if (curr != nullptr) {
                            curr->viewBookingHistory();
                        }
                    }
                    else if (custChoice == 5) {
                        string feedback;
                        cout << "Enter feedback: ";
                        cin.ignore();
                        getline(cin, feedback);
                        system.giveFeedback(feedback);
                    }
                    else if (custChoice == 6) {
                        cout << "👋 Logging out...\n";
                        break;
                    }
                    else {
                        cout << "❌ Invalid choice! Please try again." << endl;
                    }
                }
            } else {
                // Offer password reset on failed login
                char forgotOption;
                cout << "Forgot password? (y/n): ";
                cin >> forgotOption;
                if (forgotOption == 'y' || forgotOption == 'Y') {
                    string uname, newpass;
                    cout << "Enter your username: ";
                    cin >> uname;
                    cout << "Enter new password: ";
                    cin >> newpass;
                    system.resetPassword(uname, newpass);
                }
            }
        }
        else if (choice == 3) {
            // Admin Login
            string adminUser, adminPass;
            cout << "Enter Admin username: ";
            cin >> adminUser;
            cout << "Enter Admin password: ";
            cin >> adminPass;
            if (adminUser == "admin" && adminPass == "admin123") {
                cout << "✅ Admin login successful!" << endl;
                // Admin menu
                int adminChoice;
                while (true) {
                    cout << "\n-------- Admin Menu --------" << endl;
                    cout << "1. Add New Vehicle" << endl;
                    cout << "2. View All Vehicles" << endl;
                    cout << "3. View All Feedback" << endl;
                    cout << "4. Logout" << endl;
                    cout << "----------------------------" << endl;
                    cout << "Enter your choice: ";
                    cin >> adminChoice;
                    if (adminChoice == 1) {
                        int typeChoice;
                        string brand, model;
                        double price;
                        cout << "Select vehicle type (1: Sedan, 2: SUV, 3: Truck): ";
                        cin >> typeChoice;
                        cout << "Enter brand: ";
                        cin >> brand;
                        cout << "Enter model: ";
                        cin >> model;
                        cout << "Enter price per day: ";
                        cin >> price;
                        if (typeChoice == 1) {
                            system.addVehicle("Sedan", brand, model, price);
                        } else if (typeChoice == 2) {
                            system.addVehicle("SUV", brand, model, price);
                        } else if (typeChoice == 3) {
                            system.addVehicle("Truck", brand, model, price);
                        } else {
                            cout << "❌ Invalid vehicle type!" << endl;
                        }
                    }
                    else if (adminChoice == 2) {
                        system.displayAllVehicles();
                    }
                    else if (adminChoice == 3) {
                        system.viewFeedbacks();
                    }
                    else if (adminChoice == 4) {
                        cout << "👋 Admin logged out.\n";
                        break;
                    }
                    else {
                        cout << "❌ Invalid choice! Please try again." << endl;
                    }
                }
            } else {
                cout << "❌ Invalid admin credentials!" << endl;
            }
        }
        else if (choice == 4) {
            cout << "\n👋 Thank you for using the Car Rental System. Goodbye!" << endl;
            break;  // Exit the program
        }
        else {
            cout << "❌ Invalid choice! Please try again." << endl;
        }
    }
    return 0;
}
