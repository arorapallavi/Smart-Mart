#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Product {
    public :
        string productname;
        int productqty;
        double productprice;
    
    

        void AddProduct(){
            cout << "Enter Product Name: ";
            cin >> productname;
            cout << "Enter Product Qunatity: ";
            cin >> productqty;
            cout << "Enter Product Price: ";
            cin >> productprice;


            // Store the customer login info in customers.txt
            ofstream productFile("products.txt",ios::app);
            if (productFile.is_open()) {
                productFile << productname << " " << productqty <<" " << productprice << endl;
                productFile.close();

                

                cout << "New Product Added Successfully." << endl;
            } else {
                cerr << "Error: Unable to open products file." << endl;
            }
        }

        void RemoveProduct() {
             string pname;
             cout << "Enter ProductName to Remove: ";
             cin >> pname;

             ifstream productFile("products.txt");
             ofstream tempFile("temp_products.txt");
            if (productFile.is_open() && tempFile.is_open()) {
                 string line;
                bool found = false;

                while (getline(productFile, line)) {
                     
                    string storedPname = line.substr(0, line.find_first_of(' '));
                        if (pname == storedPname) {
                            found = true;
                            continue; // Skip writing this line to the temporary file
                        }
                    
                    tempFile << line <<  endl;
                }

                productFile.close();
                tempFile.close();

                if (found) {
                    // Remove the original file and rename the temporary file to the original file name
                    remove("products.txt");
                    rename("temp_products.txt", "products.txt");

                     cout << "Product Deleted Successfully" <<  endl;
                } else {
                     cout << "Product not found." <<  endl;
                    remove("temp_products.txt"); // Remove the temporary file
                }
            } else {
                 cerr << "Error: Unable to open files." <<  endl;
            }
        }

};



class Customer {
    private:
        string username;
        string password;
public:

	// Function to show the Customer Login or Register Menu
    void showCustomerMenu() {
        cout << "Customer Menu:" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
    }
	
	// Function to register a new customer
    void registerCustomer() {
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        // Store the customer login info in customers.txt
        ofstream customerFile("customers.txt",ios::app);
        if (customerFile.is_open()) {
            customerFile << username << " " << password << endl;
            customerFile.close();

            // Create a new file for the customer
            ofstream userFile(username + ".txt");
            if (userFile.is_open()) {
                userFile.close();
            } else {
                cerr << "Error: Unable to create user file." << endl;
            }

            cout << "Registration successful. You can now login." << endl;
        } else {
            cerr << "Error: Unable to open customers file." << endl;
        }
    }
	
	// Function to handle customer login
    void customerLogin() {
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        // Check if the customer login info exists in customers.txt
        ifstream customerFile("customers.txt");
        if (customerFile.is_open()) {
            string storedUsername, storedPassword;
            while (customerFile >> storedUsername >> storedPassword) {
                if (username == storedUsername && password == storedPassword) {
                    cout << "Login successful. Welcome, " << username << "!" << endl;
                    customerFile.close();
                    return;
                }
            }
            customerFile.close();
        } else {
            cerr << "Error: Unable to open customers file." << endl;
        }

        cerr << "Login failed. Invalid username or password." << endl;
    }
	
    void displayAvailableProducts() {
         cout << "Available Products:" <<  endl;
        // Display the list of products (assuming they are stored somewhere)
    }

    void addToCart() {
         string product;
         cout << "Enter the product name you want to add to your cart: ";
         cin >> product;
        // Add the product to the customer's cart (implementation not shown)
    }
};

class Employee {

    private:
        string username;
        string password;
public:

	// Function to handle employee login
    void EmployeeLogin() {
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        // Check if the employee login info exists in employees.txt

            ifstream employeeFile("employees.txt");
            if (employeeFile.is_open()) {
                string storedUsername, storedPassword;
                while (employeeFile >> storedUsername >> storedPassword) {
                    if (username == storedUsername && password == storedPassword) {
                        cout << "Login successful. Welcome, " << username << "!" << endl;
                        employeeFile.close();
                        return;
                    }
                }
                employeeFile.close();
            } else {
                cerr << "Error: Unable to open employees file." << endl;
            }
        

        cerr << "Login failed. Invalid username or password." << endl;
    }

    void increaseProductQuantity() {
         string product;
        int quantity;
         cout << "Enter the product name you want to update: ";
         cin >> product;
         cout << "Enter the quantity to add: ";
         cin >> quantity;
        // Increase the quantity of the existing product (implementation not shown)
    }
};

class Admin :private Product  {
    private:
        string username;
        string password;
    public:
        Product pobj;
	// Function to handle admin login
    void AdminLogin() {
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;

        // Check if the provided username and password match the pre-known admin credentials
        if (username == "admin" && password == "admin123") {
            cout << "Login successful. Welcome, Admin!" << endl;
            return;
        }
        cerr << "Login failed. Invalid username or password." << endl;
    }

    void addEmployee() {
         string username, password;
         cout << "Enter the new employee's username: ";
         cin >> username;
         cout << "Enter the new employee's password: ";
         cin >> password;
        // Add the new employee to the employees.txt file
        ofstream employeeFile("employees.txt",ios::app);
        if (employeeFile.is_open()) {
            employeeFile << username << " " << password << endl;
            employeeFile.close();
            cout << "New Employee Registered "<< endl;
        } 
		else {
            cerr << "Error: Unable to open employees file." << endl;
        }
    }

    void AdminAddProduct() {
        //pobj.AddProduct();
         /*string product;
        int quantity;
         cout << "Enter the new product name: ";
         cin >> product;
         cout << "Enter the initial quantity: ";
         cin >> quantity;
        // Add the new product to the product list (implementation not shown)
        */
    }
};



// MAIN PROJECT CLASS
class SmartMart {

   
    // Function to check if a file exists
    bool fileExists(const  string& fileName) {
        ifstream file(fileName);
        return file.good();
    }

public:
    enum class UserType { CUSTOMER, EMPLOYEE, ADMIN };
     UserType userType;
    // Constructor
    SmartMart() : userType(UserType::CUSTOMER) {}

    // Function to set the user type
    void setUserType(UserType type) {
        userType = type;
    }

        // Function to create objects based on the user type
    void startSession() {
        switch (userType) {
            case UserType::CUSTOMER: {
                Customer customerobj;
				int loginOrRegister;
				customerobj.showCustomerMenu();
				cout << "Enter your choice: ";
				cin >> loginOrRegister;
				switch (loginOrRegister) {
					case 1:
						customerobj.customerLogin();
						break;
					case 2:
						customerobj.registerCustomer();
						break;
					default:
						cerr << "Invalid choice." << endl;
						return ;
				}
				
                int choice;
                do {
                     cout << "Customer Menu:" <<  endl;
                     cout << "1. Display available products" <<  endl;
                     cout << "2. Add product to cart" <<  endl;
                     cout << "0. Logout" <<  endl;
                     cout << "Enter your choice: ";
                     cin >> choice;
                    switch (choice) {
                        case 1:
                            customerobj.displayAvailableProducts();
                            break;
                        case 2:
                            customerobj.addToCart();
                            break;
                        case 0:
                             cout << "Logged out as Customer." <<  endl;
                            break;
                        default:
                             cout << "Invalid choice." <<  endl;
                    }
                } while (choice != 0);
                break;
            }
            case UserType::EMPLOYEE: {
                Employee employee;
				employee.EmployeeLogin();
				
                int choice;
                do {
                     cout << "Employee Menu:" <<  endl;
                     cout << "1. Increase product quantity" <<  endl;
                     cout << "0. Logout" <<  endl;
                     cout << "Enter your choice: ";
                     cin >> choice;
                    switch (choice) {
                        case 1:
                            employee.increaseProductQuantity();
                            break;
                        case 0:
                             cout << "Logged out as Employee." <<  endl;
                            break;
                        default:
                             cout << "Invalid choice." <<  endl;
                    }
                } while (choice != 0);
                break;
            }
            case UserType::ADMIN: {
                Admin admin;
				Product pobj;
				admin.AdminLogin();
				
                int choice;
                do {
                     cout << "Admin Menu:" <<  endl;
                     cout << "1. Add new employee" <<  endl;
                     cout << "2. Add new product" <<  endl;
                     cout << "3. Remove Product" << endl;
                     cout << "0. Logout" <<  endl;
                     cout << "Enter your choice: ";
                     cin >> choice;
                    switch (choice) {
                        case 1:
                            admin.addEmployee();
                            break;
                        case 2:
                            pobj.AddProduct();
                            break;
                        case 3:
                            pobj.RemoveProduct();
                            break;
                        case 0:
                             cout << "Logged out as Admin." <<  endl;
                            break;
                        default:
                             cout << "Invalid choice." <<  endl;
                    }
                } while (choice != 0);
                break;
            }
        }
    }

};





int main() {

    
    SmartMart user;


    int userTypeChoice;
    cout << "Choose who you are:" << endl;
    cout << "1. Customer" << endl;
    cout << "2. Employee" << endl;
    cout << "3. Admin" << endl;
    cout << "Enter your choice: ";
    cin >> userTypeChoice;



    switch (userTypeChoice) {
        case 1:
            user.setUserType(SmartMart::UserType::CUSTOMER);
            break;
        case 2:
            user.setUserType(SmartMart::UserType::EMPLOYEE);
            break;
        case 3:
            user.setUserType(SmartMart::UserType::ADMIN);
            break;
        default:
            cerr << "Invalid choice. Exiting." << endl;
            return 1;
    }

   

    user.startSession();

    return 0;
}
