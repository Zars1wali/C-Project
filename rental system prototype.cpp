#include <iostream>
#include <string>
using namespace std;

// Maximum number of vehicles that can be added
const int MAX_VEHICLES = 10;

// Vehicle class
class Vehicle {
protected:
    string brand, model;
    double pricePerDay;
    bool isAvailable;

public:
    // Constructor to initialize the vehicle
    Vehicle(string b = "", string m = "", double price = 0) 
        : brand(b), model(m), pricePerDay(price), isAvailable(true) {}

    // Method to display vehicle information
    void displayInfo() {
        cout << "  " << brand << " " << model << " - $" << pricePerDay << " per day";
        cout << (isAvailable ? " (Available)" : " (Rented)") << endl;
    }

    // Getters and Setters
    bool getAvailability() { return isAvailable; }
    void rentVehicle() { isAvailable = false; }
    void returnVehicle() { isAvailable = true; }

    double getPricePerDay() { return pricePerDay; }
};

// User class
class User {
public:
    string name, username, password;
    string bookingHistory[MAX_VEHICLES];
    int bookingCount;

    // Constructor to initialize user
    User(string n = "", string u = "", string p = "") : name(n), username(u), password(p), bookingCount(0) {}

    // Method to view booking history
    void viewBookingHistory() {
        if (bookingCount == 0) {
            cout << "No booking history available." << endl;
        } else {
            for (int i = 0; i < bookingCount; i++) {
                cout << bookingHistory[i] << endl;
            }
        }
    }

    // Method to add booking to history
    void addBookingToHistory(string booking) {
        if (bookingCount < MAX_VEHICLES) {
            bookingHistory[bookingCount++] = booking;
        }
    }
};

// CarRentalSystem class
class CarRentalSystem {
private:
    Vehicle vehicles[MAX_VEHICLES];  // List of vehicles
    int vehicleCount;
    User currentUser;

public:
    // Constructor to initialize the system
    CarRentalSystem() : vehicleCount(0) {}

    // Method to register a new user
    void registerUser(string name, string username, string password) {
        currentUser = User(name, username, password);
        cout << "User registered successfully!" << endl;
    }

    // Method to log in a user
    bool loginUser(string username, string password) {
        if (currentUser.username == username && currentUser.password == password) {
            cout << "Login successful!" << endl;
            return true;
        }
        cout << "Invalid credentials!" << endl;
        return false;
    }

    // Method to add a vehicle to the system
    void addVehicle(string brand, string model, double price) {
        if (vehicleCount < MAX_VEHICLES) {
            vehicles[vehicleCount++] = Vehicle(brand, model, price);
            cout << "Vehicle added successfully!" << endl;
        } else {
            cout << "Cannot add more vehicles, limit reached!" << endl;
        }
    }

    // Method to display available vehicles
    void displayAvailableVehicles() {
        cout << "\n========================================" << endl;
        cout << "        🚗 Available Vehicles" << endl;
        cout << "========================================" << endl;

        if (vehicleCount == 0) {
            cout << "❗ No vehicles available at the moment!" << endl;
            return;
        }

        for (int i = 0; i < vehicleCount; i++) {
            cout << "  " << i + 1 << ". ";
            vehicles[i].displayInfo();
        }
        cout << "========================================" << endl;
    }

    // Method to rent a vehicle
    void rentVehicle(int index) {
        if (index >= 0 && index < vehicleCount && vehicles[index].getAvailability()) {
            vehicles[index].rentVehicle();
            cout << "✅ Vehicle rented successfully!" << endl;
            currentUser.addBookingToHistory("Rented: $" + to_string(vehicles[index].getPricePerDay()));
        } else {
            cout << "❌ Invalid choice or vehicle already rented!" << endl;
        }
    }

    // Method to return a rented vehicle
    void returnVehicle(int index) {
        if (index >= 0 && index < vehicleCount && !vehicles[index].getAvailability()) {
            vehicles[index].returnVehicle();
            cout << "✅ Vehicle returned successfully!" << endl;
        } else {
            cout << "❌ Invalid choice or vehicle not rented!" << endl;
        }
    }

    // Method to give feedback
    void giveFeedback(string feedback) {
        cout << "✅ Feedback: " << feedback << " submitted!" << endl;
    }

    // Method to view booking history
    void viewBookingHistory() {
        if (!currentUser.username.empty()) {
            currentUser.viewBookingHistory();
        } else {
            cout << "❌ Please log in first!" << endl;
        }
    }

    // Admin panel for adding vehicles
    void adminPanel() {
        cout << "\n*** Admin Panel ***" << endl;
        cout << "1. Add Vehicle" << endl;
        cout << "2. View All Vehicles" << endl;
        cout << "Enter choice: ";
        int choice;
        cin >> choice;
        if (choice == 1) {
            string brand, model;
            double price;
            cout << "Enter brand: ";
            cin >> brand;
            cout << "Enter model: ";
            cin >> model;
            cout << "Enter price per day: ";
            cin >> price;
            addVehicle(brand, model, price);
        }
        else if (choice == 2) {
            displayAvailableVehicles();
        }
    }
};

// Main function
int main() {
    CarRentalSystem system;
    system.registerUser("John Doe", "john", "1234");

    // Add some sample vehicles
    system.addVehicle("Toyota", "Camry", 40);
    system.addVehicle("Ford", "Explorer", 60);
    system.addVehicle("Chevrolet", "Silverado", 80);

    int choice;
    while (true) {
        cout << "\n===============================" << endl;
        cout << "        🚗 Car Rental System" << endl;
        cout << "===============================" << endl;
        cout << "1. Register User" << endl;
        cout << "2. Login" << endl;
        cout << "3. View Available Vehicles" << endl;
        cout << "4. Rent a Vehicle" << endl;
        cout << "5. Return a Vehicle" << endl;
        cout << "6. View Booking History" << endl;
        cout << "7. Give Feedback" << endl;
        cout << "8. Admin Panel" << endl;
        cout << "9. Exit" << endl;
        cout << "===============================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                {
                    string name, username, password;
                    cout << "Enter name: ";
                    cin >> name;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    system.registerUser(name, username, password);
                    break;
                }
            case 2:
                {
                    string username, password;
                    cout << "Enter username: ";
                    cin >> username;
                    cout << "Enter password: ";
                    cin >> password;
                    system.loginUser(username, password);
                    break;
                }
            case 3:
                system.displayAvailableVehicles();
                break;
            case 4:
                {
                    int index;
                    cout << "Enter vehicle number to rent: ";
                    cin >> index;
                    system.rentVehicle(index - 1);
                    break;
                }
            case 5:
                {
                    int index;
                    cout << "Enter vehicle number to return: ";
                    cin >> index;
                    system.returnVehicle(index - 1);
                    break;
                }
            case 6:
                system.viewBookingHistory();
                break;
            case 7:
                {
                    string feedback;
                    cout << "Enter feedback: ";
                    cin.ignore();
                    getline(cin, feedback);
                    system.giveFeedback(feedback);
                    break;
                }
            case 8:
                system.adminPanel();
                break;
            case 9:
                cout << "\n👋 Exiting... Have a great day!" << endl;
                return 0;
            default:
                cout << "❌ Invalid choice! Try again." << endl;
        }
    }
}
