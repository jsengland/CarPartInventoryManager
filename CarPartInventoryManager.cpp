//============================================================================
// Name        : Homework4JasonEngland.cpp
// Author      : Jason England
// Date        : 11/3/2021
// Version     :
// Copyright   : 
// Description : This program will act as a user interface for a user to
//interact with an inventory of parts, customers, and customer orders
//differently based on the role they select.
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Part{
private:
	string _partnum;
	string _desc;
	int _stockqty;
public:
	Part(){}
	Part(string p, string d, int s): _partnum(p), _desc(d), _stockqty(s){}
	void setPartNum(string p){_partnum = p;}
	string getPartNum() const {return _partnum;}
	void setDesc(string d){_desc = d;}
	string getDesc(){return _desc;}
	void setStock(int s){_stockqty = s;}
	int getStock() const {return _stockqty;}
};

class Inventory{
private:
	vector<Part*> _inv;
public:
	Inventory(){}
	Inventory(const Inventory& b);
	~Inventory();
	const Inventory& operator=(const Inventory& b);
	void addPart(string p, string d, int s);
	bool deletePart(string);
	bool addStock(string partNum, int addQty);
	void printInventory(ostream&) const;
};

Inventory::Inventory(const Inventory& b){
	for (unsigned int i=0; i<b._inv.size(); i++){
		_inv.push_back(new Part(*(b._inv[i])));
	//	_inv.push_back(new Part(b._inv[i]->getPartNum(), b._inv[i]->getStock()));
	}
}
Inventory::~Inventory(){
	for(unsigned int i=0; i<_inv.size(); i++){
		delete _inv[i];
	}
}
const Inventory& Inventory::operator=(const Inventory& b){
	for (unsigned int i=0; i<b._inv.size(); i++){
		_inv.push_back(new Part(*(b._inv[i])));
	}
	return b;
}

void Inventory::addPart(string p, string d, int s){
	Part* ptr = new Part(p, d, s);
	_inv.push_back(ptr);
}

bool Inventory::deletePart(string p){
	bool found = false;
	for (unsigned int i=0; (i < _inv.size() && !found); i++){
		if (_inv[i]->getPartNum() == p){
			found = true;
			delete _inv[i];
			_inv.erase(_inv.begin()+i);
		}
	}
	return found;
}

bool Inventory::addStock(string partNum, int addQty){
	bool found = false;
	for (unsigned int i=0; (i < _inv.size() && !found); i++){
		if (_inv[i]->getPartNum() == partNum){
			found = true;
			_inv[i]->setStock(_inv[i]->getStock() + addQty);
		}
	}
	return found;
}

void Inventory::printInventory(ostream& os) const {
	os << "\nCurrent Inventory:" << endl;
	//os << _inv.size() << endl;
	for (unsigned int i = 0; i < _inv.size(); i++){
		os << "Part: " << _inv[i]->getPartNum() <<
			"  Stock Qty: " << _inv[i]->getStock() <<
			"  Part Description: " << _inv[i]->getDesc() << endl;
		//os << &(*_inv[i]) << endl;  -or-
		//os << _inv[i] << endl;
		//os << &(_inv[i]) << endl;
	}
}

class Order{
private:
	int _ordnum;
	string _cname;
	vector<string> _parts;
	vector<int> _qtys;
	bool _shipped;
public:
	Order();
	Order(int ordNum, string custName): _ordnum(ordNum), _cname(custName), _shipped(false){}
	void setOrdnum(int o){_ordnum = o;}
	void setName(string c){_cname = c;}
	int getOrdnum(){return _ordnum;}
	string getName(){return _cname;}
	bool getShipped(){return _shipped;}
	void setShipped(bool stat){_shipped = stat;}
	void shippingAffect(Inventory& inv);
	void addPart(string p, int q){
		_parts.push_back(p);
		_qtys.push_back(q);
	}
	void print(ostream&);
};
void Order::print(ostream& os){
	os << "\nOrder Number: " << getOrdnum()<< endl;
	os << "Customer Name: " << getName() << endl;
	os << "Shipped Status (0 -> Not Shipped, 1 -> Shipped): " << getShipped() << endl;
	for (unsigned int i = 0; i < _parts.size(); i++){
		os << "Part: " << _parts[i] << "  Part Quantity: " << _qtys[i] << endl;
	}
}
void Order::shippingAffect(Inventory& inv){
	for (unsigned int i = 0; i < _parts.size(); i++){
		inv.addStock(_parts[i], (0 - _qtys[i]));
	}
}


class AllOrders{
private:
	vector<Order*> _orders;
public:
	AllOrders(){}
	AllOrders(const AllOrders& b);
	~AllOrders();
	const AllOrders& operator=(const AllOrders& b);
	void addOrder(int ordNum, string custName);
	bool addPartToOrder(int ordNum, string part, int qty);
	bool deleteOrder(int ordNum);
	bool shipOrder(int ordNum, Inventory& inv);
	void printAllOrders(ostream&) const;
};

AllOrders:: AllOrders(const AllOrders& b){
	for (unsigned int i=0; i<b._orders.size(); i++){
		_orders.push_back(new Order(*(b._orders[i])));
	}
}

AllOrders::~AllOrders(){
	for(unsigned int i=0; i<_orders.size(); i++){
		delete _orders[i];
	}
}

const AllOrders& AllOrders::operator=(const AllOrders& b){
	for (unsigned int i=0; i<b._orders.size(); i++){
		_orders.push_back(new Order(*(b._orders[i])));
	}
	return b;
}

void AllOrders::addOrder(int ordNum, string custName){
	Order* o = new Order(ordNum, custName);
	_orders.push_back(o);
}

bool AllOrders::addPartToOrder(int ordNum, string part, int qty){
	bool found = false;
	for (unsigned int i=0; (i < _orders.size() && !found); i++){
		if (_orders[i]->getOrdnum() == ordNum){
			found = true;
			_orders[i]->addPart(part, qty);
		}
	}
	return found;
}

bool AllOrders::deleteOrder(int ordNum){
	bool found = false;
	for (unsigned int i=0; (i < _orders.size() && !found); i++){
		if (_orders[i]->getOrdnum() == ordNum){
			found = true;
			delete _orders[i];
			_orders.erase(_orders.begin()+i);
		}
	}
	return found;
}

bool AllOrders::shipOrder(int ordNum, Inventory& inv){
	bool found = false;
	for (unsigned int i=0; (i < _orders.size() && !found); i++){
		if (_orders[i]->getOrdnum() == ordNum){
			found = true;
			_orders[i]->setShipped(true);
			_orders[i]->shippingAffect(inv);
		}
	}
	return found;
}

void AllOrders::printAllOrders(ostream& os) const{
	os << "\nAll Orders:" << endl;
	for (unsigned int i = 0; i < _orders.size(); i++){
		_orders[i]->print(os);
	}
}

class Address{
private:
	string _street;
	string _city;
	string _state;
	string _zip;
public:
	Address(){}
	Address(string street, string city, string state, string zip): _street(street), _city(city), _state(state), _zip(zip){}
	void setStreet(string street){_street = street;}
	string getStreet(){return _street;}
	void setCity(string city){_city = city;}
	string getCity(){return _city;}
	void setState(string state){_state = state;}
	string getState(){return _state;}
	void setZip(string zip){_zip = zip;}
	string getZip(){return _zip;}
};

class Customer{
private:
	string _customerName;
	Address _custAddress;
public:
	Customer(){}
	Customer(string custName, string custStreet, string custCity, string custState, string custZip): _customerName(custName){
		_custAddress.setStreet(custStreet);
		_custAddress.setCity(custCity);
		_custAddress.setState(custState);
		_custAddress.setZip(custZip);
	}
	void setCustomerName(string custName){_customerName = custName;}
	string getCustomerName(){return _customerName;}
	void printCustomer(ostream& os);
};

void Customer::printCustomer(ostream& os){
	os << "\nCustomer Name: " << _customerName << endl << "Customer Address: "
			<< _custAddress.getStreet() << " " << _custAddress.getCity()
			<< ", " << _custAddress.getState() << " " << _custAddress.getZip() << endl;
}

class AllCustomers{
private:
	vector<Customer*> _customers;
public:
	AllCustomers(){}
	AllCustomers(const AllCustomers& b);
	~AllCustomers();
	const AllCustomers& operator=(const AllCustomers& b);
	void addCustomer(string custName, string custStreet, string custCity, string custState, string custZip);
	bool deleteCustomer(string custName);
	void printAllCustomers(ostream& os) const;
};

AllCustomers:: AllCustomers(const AllCustomers& b){
	for (unsigned int i=0; i<b._customers.size(); i++){
		_customers.push_back(new Customer(*(b._customers[i])));
	}
}

AllCustomers::~AllCustomers(){
	for(unsigned int i=0; i<_customers.size(); i++){
		delete _customers[i];
	}
}

const AllCustomers& AllCustomers::operator=(const AllCustomers& b){
	for (unsigned int i=0; i<b._customers.size(); i++){
		_customers.push_back(new Customer(*(b._customers[i])));
	}
	return b;
}

void AllCustomers::addCustomer(string custName, string custStreet, string custCity, string custState, string custZip){
	Customer* c = new Customer(custName, custStreet, custCity, custState, custZip);
	_customers.push_back(c);
}

bool AllCustomers::deleteCustomer(string custName){
	bool found = false;
	for(unsigned int i=0; (i<_customers.size() && !found); i++){
		if(_customers[i]->getCustomerName() == custName){
			found = true;
			delete _customers[i];
			_customers.erase(_customers.begin()+i);
		}
	}
	return found;
}

void AllCustomers::printAllCustomers(ostream& os) const{
	os << "\nAll Customers:" << endl;
	for (unsigned int i = 0; i < _customers.size(); i++){
		_customers[i]->printCustomer(os);
	}
}

//Pre: 1st param is the ifstream object constructed in the main class. It will contain all input starting data. Assume data is in "dataFile.txt"
//     2nd param is inventory object user wants to populate. 3rd param is AllCustomers object user wants to populate.
//     4th param is AllOrders object user wants to populate.
//Post: This function will open up the file and construct Inventory, AllCustomers, and AllOrders based on the given data.
void loadData(ifstream& inFile, Inventory* inv, AllCustomers* allCust, AllOrders* allOrds){
	inFile.open("dataFile.txt");

	string partNum;
	string partDesc;
	string qtyStr;
	int qtyVal;
	bool valid = true;
	while(!inFile.eof() && valid){
		inFile >> partNum;
		partNum = partNum.substr(0, partNum.length()-1);

		inFile >> partDesc;
		partDesc = partDesc.substr(0, partDesc.length()-1);

		inFile >> qtyStr;
		qtyVal = stoi(qtyStr);

		valid = !(partNum == "0" && partDesc == "0" && qtyVal == 0);
		if(valid){inv->addPart(partNum, partDesc, qtyVal);}
	}

	string custName;
	string custStreet;
	string custCity;
	string custState;
	string custZip;
	valid = true;
	while(!inFile.eof() && valid){
		inFile >> custName;
		custName = custName.substr(0, custName.length()-1);

		inFile >> custStreet;
		custStreet = custStreet.substr(0, custStreet.length()-1);

		inFile >> custCity;
		custCity = custCity.substr(0, custCity.length()-1);

		inFile >> custState;
		custState = custState.substr(0, custState.length()-1);

		inFile >> custZip;

		valid = !(custName == "0" && custStreet == "0" && custCity == "0" && custState == "0" && custZip == "0");
		if(valid){allCust->addCustomer(custName, custStreet, custCity, custState, custZip);}
	}

	string orderNumStr;
	string custName2;
	int orderNumVal;
	valid = true;
	while(!inFile.eof() && valid){
		inFile >> orderNumStr;
		orderNumStr = orderNumStr.substr(0, orderNumStr.length()-1);
		orderNumVal = stoi(orderNumStr);

		inFile >> custName2;

		valid = !(orderNumVal == 0 && custName2 == "0");
		if(valid){allOrds->addOrder(orderNumVal, custName2);}

		string ordPartNum;
		string ordQtyStr;
		int ordQtyVal;
		bool valid2 = true;
		while(!inFile.eof() && valid2){
			inFile >> ordPartNum;
			ordPartNum = ordPartNum.substr(0, ordPartNum.length()-1);

			inFile >> ordQtyStr;
			ordQtyVal = stoi(ordQtyStr);

			valid2 = !(ordPartNum == "0" && ordQtyVal == 0);
			if(valid2){allOrds->addPartToOrder(orderNumVal, ordPartNum, ordQtyVal);}
		}
	}



}


int main() {
	ifstream f;
	Inventory *inv = new Inventory();
	AllCustomers *allCust = new AllCustomers();
	AllOrders *allOrds = new AllOrders();
	loadData(f, inv, allCust, allOrds);
	//User choice chars and repeat bools
	char userChoice;
	char userChoiceCSR;
	char userChoiceWR;
	char userChoiceS;
	bool repeat = true;
	bool repeatCSR;
	bool repeatWR;
	bool repeatS;
	//Data user can pass in
	int ordNum;
	string cName;
	string pNum;
	string desc;
	int qtyNum;

	//User Interface using nested switches
	while(repeat){
		repeatCSR = true;
		repeatWR = true;
		repeatS = true;

		cout << "\nPlease choose an option:" << "\n1. Customer Service Rep (Login)" << "\n2. Warehouse Receiver (Login)" <<
				"\n3. Warehouse Shipper (Login)" << "\n4. Dump Data" << "\n5. Exit" << endl;
		cin >> userChoice;
		switch(userChoice){
		case '1':
			cout << "Login as Customer Service Rep Successful" << endl;
			while(repeatCSR){
				cout << "\nPlease Choose an action:" << "\n1. Add an Order" <<
						"\n2. Delete an Order" << "\n3. Modify an Order" << "\n4. Back to Main Menu" << endl;
				cin >> userChoiceCSR;
				switch(userChoiceCSR){
				case '1':
					cout << "Please enter an order number:" << endl;
					cin >> ordNum;
					cout << "Please enter the customer name:" << endl;
					cin >> cName;
					allOrds->addOrder(ordNum, cName);
					cout << "Order added..." << endl;
					break;
				case '2':
					cout << "Please enter an order number to delete:" << endl;
					cin >> ordNum;
					allOrds->deleteOrder(ordNum);
					cout << "Order deleted..." << endl;
					break;
				case '3':
					cout << "Please enter an order number to add parts to:" << endl;
					cin >> ordNum;
					cout << "Please enter the part num" << endl;
					cin >> pNum;
					cout << "Please enter the desired amount to be ordered:" << endl;
					cin >> qtyNum;
					allOrds->addPartToOrder(ordNum, pNum, qtyNum);
					cout << "Added part to order!" << endl;
					break;
				case '4':
					cout << "\nReturning to main menu..." << endl;
					repeatCSR = false;
					break;
				default:
					cout << "ERROR please try again" << endl;
					break;
				}
			}
			break;
		case '2':

			cout << "Login as Warehouse Receiver Successful" << endl;
			while(repeatWR){
				cout << "\nPlease Choose an action:" << "\n1. Add a new part to inventory" <<
						"\n2. Add stock to existing part" << "\n3. Back to Main Menu" << endl;
				cin >> userChoiceWR;
				switch(userChoiceWR){
				case '1':
					cout << "Please enter a part number:" << endl;
					cin >> pNum;
					cout << "Please enter a name/desc. for the part:" << endl;
					cin >> desc;
					cout << "Please enter a quantity amount:" << endl;
					cin >> qtyNum;
					inv->addPart(pNum, desc, qtyNum);
					cout << "Part Added" << endl;
					break;
				case '2':
					cout << "Please enter a part number:" << endl;
					cin >> pNum;
					cout << "Please enter the amount you want to add to the current quantity" << endl;
					cin >> qtyNum;
					inv->addStock(pNum, qtyNum);
					break;
				case '3':
					cout << "\nReturning to main menu..." << endl;
					repeatWR = false;
					break;
				default:
					cout << "ERROR please try again" << endl;
					break;
				}
			}

			break;
		case '3':
			cout << "Login as Warehouse Shipper Successful" << endl;
			while(repeatS){
				cout << "\nPlease Choose an action:" << "\n1. Ship Order" << "\n2. Back to Main Menu" << endl;
				cin >> userChoiceS;
				switch(userChoiceS){
				case '1':
					cout << "Please enter an order number to ship:" << endl;
					cin >> ordNum;
					allOrds->shipOrder(ordNum, *inv);
					cout << "Order has been shipped" << endl;
					break;
				case '2':
					cout << "\nReturning to main menu..." << endl;
					repeatS = false;
					break;
				default:
					cout << "ERROR please try again" << endl;
					break;
				}
			}
			break;
		case '4':
			cout << "You chose to Dump Data (Found Below)" << endl;
			inv->printInventory(cout);
			allCust->printAllCustomers(cout);
			allOrds->printAllOrders(cout);
			cout << "\n***END OF DATA DUMP***" << endl;
			break;
		case '5':
			cout << "You chose to Exit...\nThank you for using our program!" << endl;
			repeat = false;
			break;
		default:
			cout << "ERROR please try again" << endl;
			break;
		}
	}
	return 0;
}
