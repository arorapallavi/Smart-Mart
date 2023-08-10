
//Necessary Inclusions
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <iomanip>
using namespace std;


// 		********* PRODUCT CLASS *********
class Product {
    public :
        string productname;
        int productqty;
        double productprice;


        Product() : productqty(0), productprice(0.0) {}
        Product(const string& name, int quantity, double price)
        : productname(name), productqty(quantity), productprice(price) {}
    
        static vector<Product> products; // Static vector to store all products

    static void DisplayAvlbProducts() {
        ifstream productFile("products.txt");
        if (productFile.is_open()) {
            products.clear();
            string line;
            while (getline(productFile, line)) {
                cout<<line<<endl;
            }
            productFile.close();
        } else {
            cerr << "Error: Unable to open products file." << endl;
        }
    }

    static void LoadProductsFromFile() {
        ifstream productFile("products.txt");
        if (productFile.is_open()) {
            products.clear();
            string line;
            while (getline(productFile, line)) {
                string productName;
                int quantity;
                double price;
                istringstream iss(line);
                iss >> productName >> quantity >> price;
                products.emplace_back(productName, quantity, price);
            }
            productFile.close();
        } else {
            cerr << "Error: Unable to open products file." << endl;
        }
    }

    static void SaveProductsToFile() {
        ofstream productFile("products.txt");
        if (productFile.is_open()) {
            for (const auto& product : products) {
                productFile << product.productname << " " << product.productqty << " " << product.productprice << endl;
            }
            productFile.close();
        } else {
            cerr << "Error: Unable to open products file." << endl;
        }
    }

    static pair<int, double> getQuantityAndPrice(const string& productName) {
        for (const auto& product : products) {
            if (product.productname == productName) {
                return make_pair(product.productqty, product.productprice);
            }
        }
        return make_pair(0, 0.0); // Return 0 quantity and price if the product is not found
    }

    static void AddProduct() {
        Product product;
        cout << "Enter Product Name: ";
        cin >> product.productname;
        cout << "Enter Product Quantity: ";
        cin >> product.productqty;
        cout << "Enter Product Price: ";
        cin >> product.productprice;
        products.push_back(product);

        SaveProductsToFile();
        cout << "New Product Added Successfully." << endl;
    }

    static void RemoveProduct() {
        string pname;
        cout << "Enter ProductName to Remove: ";
        cin >> pname;

        auto it = remove_if(products.begin(), products.end(),
            [&pname](const Product& product) { return product.productname == pname; }
        );

        if (it != products.end()) {
            products.erase(it, products.end());
            SaveProductsToFile();
            cout << "Product Deleted Successfully." << endl;
        } else {
            cout << "Product not found." << endl;
        }
    }

    static void ModifyProduct(string productName, int newQuantityValue) {
        for (auto& product : products) {
            if (product.productname == productName) {
                product.productqty = newQuantityValue;
            }
        }
        SaveProductsToFile();
    }

    static void ModifyProduct(string productName, double newPriceValue) {
        for (auto& product : products) {
            if (product.productname == productName) {
                product.productprice = newPriceValue;
            }
        }
        SaveProductsToFile();
    }

    static void ModifyProduct(string productName, int newQuantityValue, double newPriceValue) {
        for (auto& product : products) {
            if (product.productname == productName) {
                product.productqty = newQuantityValue;
                product.productprice = newPriceValue;
            }
        }
        SaveProductsToFile();
    }
    // Function to decrement the quantity of a product
    static void decrementQuantity(const string& productName, int quantity) {
        for (auto& product : products) {
            if (product.productname == productName) {
                product.productqty -= quantity;
                Product::SaveProductsToFile(); // Save the updated product information to the file
                return;
            }
        }
        cerr << "Product " << productName << " not found." << endl;
    }
};

vector<Product> Product::products; // Initialize the static member




// *********  CART CLASS	********

class Cart {
private:
    

    struct CartItem {
        int quantity;
        double individualPrice;

        CartItem() : quantity(0), individualPrice(0.0) {}
        CartItem(int quantity, double price)
            : quantity(quantity), individualPrice(price) {}
    };
    static map<string, CartItem> cartItems; // Static map to store product name and cart item details

public:

     friend class Product; // Friend Class

    // Function to add a product to the cart
    void addProductToCart(const string& productName, int quantity) {
        pair<int, double> productInfo = Product::getQuantityAndPrice(productName);
        int availableQuantity = productInfo.first;
        double individualPrice = productInfo.second;
        
         if (availableQuantity > 0) {
            cartItems[productName].quantity += min(quantity, availableQuantity);
            cartItems[productName].individualPrice = individualPrice;
            // Decrement the product quantity in the Product class
            Product::decrementQuantity(productName, cartItems[productName].quantity);
            cout << "Product " << productName << " added to cart." << endl;
        }
         else {
        cout << "Product " << productName << " is out of stock." << endl;
        }

    }

            // Function to empty the cart items
    void emptyCart() {
        cartItems.clear();
        cout << "Cart items cleared." << endl;
    }
    // Function to display cart items with details
    void displayCartItems() {
        cout << "Cart Items:" << endl;
        double netTotal = 0.0;

        for (const auto& entry : cartItems) {
            const string& productName = entry.first;
            const CartItem& cartItem = entry.second;
            int quantity = cartItem.quantity;
            double individualPrice = cartItem.individualPrice;
            double itemTotal = quantity * individualPrice;
            netTotal += itemTotal;

            cout << "Product: " << productName << ", "
                 << "Quantity: " << quantity << ", "
                 << "Individual Price: " << individualPrice << ", "
                 << "Net Price: " << itemTotal << endl;
        }

        cout << "Net Total: " << netTotal << endl;
    }

    // Function to confirm cart items and save them to the user's file and purchases file
    void confirmCartItems(const string& username) {
        ofstream userFile(username + ".txt", ios::app);
        ofstream purchasesFile("purchases.txt", ios::app);

        if (userFile.is_open() && purchasesFile.is_open()) {
            userFile << "Cart Items:" << endl;
            double netTotal = 0.0;

            for (const auto& entry : cartItems) {
                const string& productName = entry.first;
                const CartItem& cartItem = entry.second;
                int quantity = cartItem.quantity;
                double individualPrice = cartItem.individualPrice;
                double itemTotal = quantity * individualPrice;
                netTotal += itemTotal;

                userFile << "Product: " << productName << ", "
                         << "Quantity: " << quantity << ", "
                         << "Individual Price: " << individualPrice << ", "
                         << "Net Price: " << itemTotal << endl;

                purchasesFile << productName << endl;
            }

            userFile << "Net Total: " << netTotal << endl;

            userFile.close();
            purchasesFile.close();

            cout << "Cart items confirmed and saved for " << username << "." << endl;
        } else {
            cerr << "Error: Unable to open files." << endl;
        }
    }

    // Function to get cart items 
    const static map<string, CartItem>& getCartItems() {
        return cartItems;
    }
};

map<string, Cart::CartItem> Cart::cartItems; // Initialize the static member


// 		********   APRIORI CLASS     ********

class Apriori {
private:
    vector<set<string>> transactions;
    map<set<string>, int> supportCount;
    vector<set<string>> frequentItemSets;

    void readTransactions(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open the file.\n";
            exit(1);
        }

        string line;
        while (getline(file, line)) {
            set<string> items;
            stringstream ss(line);
            string item;
            while (ss >> item) {
                items.insert(item);
            }
            transactions.push_back(items);
        }

        file.close();
    }

    void generateFrequentItemSets(int minSupport, const vector<string>& targetItems) {
        set<string> uniqueItems;
        for (const auto& trans : transactions) {
            for (const auto& item : trans) {
                uniqueItems.insert(item);
                supportCount[{item}]++; // Count support for individual items
            }
        }

        int k = 2; // Frequent item set size
        vector<set<string>> prevCandidates, candidates;
        while (!uniqueItems.empty()) {
            vector<set<string>> currentCandidates;
            if (k == 2) {
                // Generate pairs for the first iteration
                for (const auto& item : uniqueItems) {
                    set<string> pair = { item };
                    currentCandidates.push_back(pair);
                }
            } else {
                // Generate candidate item sets using the previous frequent item sets
                currentCandidates = generateCandidates(prevCandidates);
            }

            for (const auto& trans : transactions) {
                for (const auto& candidate : currentCandidates) {
                    if (includes(trans.begin(), trans.end(), candidate.begin(), candidate.end())) {
                        supportCount[candidate]++;
                    }
                }
            }

            prevCandidates = currentCandidates;
            candidates.clear();

            // Prune and filter based on minSupport
            for (const auto& candidate : prevCandidates) {
                if (supportCount[candidate] >= minSupport) {
                    candidates.push_back(candidate);
                    frequentItemSets.push_back(candidate);
                }
            }

            uniqueItems.clear();
            for (const auto& candidate : candidates) {
                for (const auto& item : candidate) {
                    uniqueItems.insert(item);
                }
            }

            k++;
        }

        // Filter frequent item sets based on target items and pair presence
        vector<set<string>> filteredItemSets;
        for (const auto& itemSet : frequentItemSets) {
            bool hasTargetItems = true;
            bool hasPair = false;

            for (const auto& target : targetItems) {
                if (itemSet.find(target) == itemSet.end()) {
                    hasTargetItems = false;
                    break;
                }
            }

            if (hasTargetItems) {
                for (size_t i = 0; i < targetItems.size() - 1; i++) {
                    for (size_t j = i + 1; j < targetItems.size(); j++) {
                        set<string> pair = { targetItems[i], targetItems[j] };
                        if (includes(itemSet.begin(), itemSet.end(), pair.begin(), pair.end())) {
                            hasPair = true;
                            break;
                        }
                    }
                    if (hasPair) {
                        break;
                    }
                }
            }

            if (hasTargetItems && !hasPair) {
                filteredItemSets.push_back(itemSet);
            }
        }

        frequentItemSets = filteredItemSets;
    }

    vector<set<string>> generateCandidates(const vector<set<string>>& prevCandidates) {
        vector<set<string>> candidates;

        int k = prevCandidates[0].size() + 1;
        for (size_t i = 0; i < prevCandidates.size(); i++) {
            for (size_t j = i + 1; j < prevCandidates.size(); j++) {
                vector<string> itemSet1(prevCandidates[i].begin(), prevCandidates[i].end());
                vector<string> itemSet2(prevCandidates[j].begin(), prevCandidates[j].end());

                // Check if the first k-2 elements are equal between both item sets
                bool canJoin = true;
                for (int x = 0; x < k - 2; x++) {
                    if (itemSet1[x] != itemSet2[x]) {
                        canJoin = false;
                        break;
                    }
                }

                if (canJoin) {
                    set<string> newCandidate(prevCandidates[i]);
                    newCandidate.insert(*prevCandidates[j].rbegin());
                    candidates.push_back(newCandidate);
                }
            }
        }

        return candidates;
    }

public:
    Apriori(const string& filename, int minSupport, const vector<string>& targetItems) {
        readTransactions(filename);
        generateFrequentItemSets(minSupport, targetItems);
    }

    void printFrequentItemSets() {
        for (const auto& itemSet : frequentItemSets) {
            for (const auto& item : itemSet) {
                cout << item << " ";
            }
            cout << endl;
        }
    }
};




// Base class User
class User {
public:
    virtual ~User() {
        cout<<"Base Class User Destroyed"<<endl;
    }
    virtual void showMenu() = 0; //Pure Virtual Function
};

// CUSTOMER - Derived Class
class Customer : public User {
        private:
        string username;
        string password;
        Cart cart;
        
public:
	// Function to show the Customer Login or Register Menu
    void showCustomerMenu() {
        cout << "Customer Menu:" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
    }
    void showMenu() {
        cout << "Customer Menu:" << endl;
        cout << "1. Display available products" << endl;
        cout << "2. Add product to cart" << endl;
        cout << "0. Logout" << endl;
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
         Product::DisplayAvlbProducts();
        // Display the list of products (assuming they are stored somewhere)
    }



    void addToCart() {
           string productName;
        int quantity;

        while (true) {
            cout << "Enter product name to add to the cart (or 'done' to finish): ";
            cin >> productName;
            if (productName == "done") {
                break;
            }

            cout << "Enter quantity: ";
            cin >> quantity;

            // Add the product to the cart with the given quantity
            cart.addProductToCart(productName, quantity);
        }

        // Display cart items with details
        cart.displayCartItems();

        const string filename = "purchases.txt";
        int minSupport = 3;
        vector<string> targetItems ={"default"};
        cout<<" OUR Reccomendations Based on Products in Your Cart"<<endl;
        for (const auto& entry : cart.getCartItems()) {
            const string& productName = entry.first;
            cout << "Recommending Below Items As You have "  << productName <<" in your cart"<< endl;
            targetItems[0]=productName;
            Apriori apriori(filename, minSupport, targetItems);
            apriori.printFrequentItemSets();

        }

             
        
        Apriori apriori(filename, minSupport, targetItems);
        apriori.printFrequentItemSets();
        int finalcart;
        do{
        cout<<"Menu"<<endl;
        cout<<"1. Add more Products"<<endl;
        cout<<"2. Confirm Checkout with Finalized Items in Cart "<<endl;
        cout<<"3. Empty Cart and Exit"<<endl;
        cin >> finalcart ;
        if(finalcart==1){
            addToCart();
        }
        else if(finalcart==3){
            cart.emptyCart();
            cout<<"All Items in Cart Cleared"<<endl;
            cout<<"Logged out "<<endl;
            return;
        }
        }
        while(finalcart!=2);
        // Confirm the cart items and save them to the respective files
        cart.confirmCartItems(username); // 'username' is the customer's username
    }
};

// EMPLOYEE - Derived Class
class Employee : public User {
        private:
        string username;
        string password;
public:
    void showMenu()  {
        cout << "Employee Menu:" << endl;
        cout << "1. Display Available Products" << endl;
        cout << "2. Increase product quantity" << endl;
        cout << "0. Logout" << endl;
    }
    
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

     void displayAvailableProducts() {
         cout << "Available Products:" <<  endl;
         Product::DisplayAvlbProducts();
        // Display the list of products (assuming they are stored somewhere)
    }

    void increaseProductQuantity() {
         string pname;
         int pqty;
         cout << "Enter the product name you want to update: ";
         cin >> pname;
         cout << "Enter the quantity to add: ";
         cin >> pqty;
      
        Product::ModifyProduct(pname,pqty);
    }
};

// ADMIN - Derived Class
class Admin : public User {
        private:
        string username;
        string password;
public:
    void showMenu() {
        cout << "Admin Menu:" << endl;
        cout << "1. Add new employee" << endl;
        cout << "2. Add new product" << endl;
        cout << "3. Remove Product" << endl;
        cout << "4. Modify Existing Product" << endl;
        cout << "0. Logout" << endl;
    }
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
};


//			******* 	MAIN CLASS : SMART MART 	**********

class SmartMart {
private:
    User* currentUser;

public:
    enum class UserType { CUSTOMER, EMPLOYEE, ADMIN };
    UserType userType; // STORES CURRENT USER TYPE

    // CONSTRUCTOR
    SmartMart() : userType(UserType::CUSTOMER), currentUser(nullptr) {}

    // Function to SET the USER TYPE
    void setUserType(UserType type) {
        userType = type;
    }

    // Function to create objects based on the user type
    void startSession() {
        switch (userType) {
            /* CUSTOMER OBJECT */
            case UserType::CUSTOMER: {
                int loginOrRegister;
                currentUser = new Customer();
				dynamic_cast<Customer*>(currentUser)->showCustomerMenu();
				cout << "Enter your choice: ";
				cin >> loginOrRegister;
				switch (loginOrRegister) {
					case 1:
						dynamic_cast<Customer*>(currentUser)->customerLogin();
						break;
					case 2:
						dynamic_cast<Customer*>(currentUser)->registerCustomer();
						break;
					default:
						cerr << "Invalid choice." << endl;
						return ;
				}
                break;
            }

            /* EMPLOYEE OBJECT */
            case UserType::EMPLOYEE: {
                currentUser = new Employee();
				dynamic_cast<Employee*>(currentUser)->EmployeeLogin();
                break;
            }

            /* ADMIN OBJECT */
            case UserType::ADMIN: {
                currentUser = new Admin();
				dynamic_cast<Admin*>(currentUser)->AdminLogin();
                break;
            }
			default:{}
                cerr << "Invalid user type. Exiting" << endl;
                currentUser = nullptr;
				exit(1);
                
			}
        

           int choice;
                do {
                currentUser->showMenu();
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
					
				case 1:{
						
                // Checking the type of currentUser using typeid
                    if (typeid(*currentUser) == typeid(Customer)) {

                        
                        dynamic_cast<Customer*>(currentUser)->displayAvailableProducts();

                    }
                    else if (typeid(*currentUser) == typeid(Employee)) {

                        
                        dynamic_cast<Employee*>(currentUser)->displayAvailableProducts();
                    }
                    else { //admin
                        
                        
                        dynamic_cast<Admin*>(currentUser)->addEmployee();
                    }
                
               break;
                }

                case 2:{
                                    // Checking the type of currentUser using typeid
                    if (typeid(*currentUser) == typeid(Customer)) {

                        
                        dynamic_cast<Customer*>(currentUser)->addToCart();

                    }
                    else if (typeid(*currentUser) == typeid(Employee)) {

                        
                        dynamic_cast<Employee*>(currentUser)->increaseProductQuantity();
                    }
                    else { //admin
                        
                       
                        Product::AddProduct();
                    }

                    break;
                }
                case 3:{
                    // Checking the type of currentUser using typeid
                    try{
                    if ((typeid(*currentUser) == typeid(Customer)) || (typeid(*currentUser) == typeid(Employee)) ) {

                        throw "choice 3 invalid for current user role";

                    }
 
                    
                    else { //admin
                        
                    
                        Product::RemoveProduct();
                    }
                }
                catch(...)
                {
                    cout<<"Invalid Choice Caught in Catch"<<endl;
                }

                    break;
                }
                case 4:{
                    // Checking the type of currentUser using typeid
                    try{
                        if ( (typeid(*currentUser) == typeid(Customer)) || typeid(*currentUser) == typeid(Employee)){

                            throw "error";

                        }

                        else { //admin
                            
                                int prodmodify;
                                string pname;
                                int pqty;
                                double pprice;
                                cout << "Choose from below : "<<endl;
                                cout <<"1. Modify Quantity" << endl;
                                cout <<"2. Modify Price" <<endl;
                                cout <<"3. Modify Quantity & Price"<<endl;
                                cin >> prodmodify ;
                                switch(prodmodify)
                                {
                                    case 1 :
                                        
                                        cout << "Enter the product name you want to update: ";
                                        cin >> pname;
                                        cout << "Enter the new quantity : ";
                                        cin >> pqty;
                                    
                                        Product::ModifyProduct(pname,pqty);
                                        break;
                                    case 2 :
                                        
                                        cout << "Enter the product name you want to update: ";
                                        cin >> pname;
                                        cout << "Enter the updated price: ";
                                        cin >> pprice;
                                    
                                    Product::ModifyProduct(pname,pprice);
                                        break;                                    
                                    case 3 :

                                        cout << "Enter the product name you want to update: ";
                                        cin >> pname;
                                        cout << "Enter the new quantity : ";
                                        cin >> pqty;                                    
                                        cout << "Enter the updated price: ";
                                        cin >> pprice;
                                        
                                        Product::ModifyProduct(pname,pqty,pprice);
                                        break; 
                                    default:
                                        cout << "Invalid choice." <<  endl;
                                }
                        }
                    }
                    catch(...){
                        cout<<"Invalid Choice caught in catch."<<endl;
                    }


                    break;
                }

                }
            } while (choice != 0);
        
    }

    // Destructor to clean up the dynamically allocated object
    ~SmartMart() {
        cout << " Deleting Current User by freeing dynamically allocated memory. Exited" <<endl;
        delete currentUser;
    }
};

int main() {
	
    SmartMart user;
	Product::LoadProductsFromFile();
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
