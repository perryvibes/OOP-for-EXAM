#include <iostream>
#include <string>
#include <fstream>

using namespace std;

/*////////////////////////////////////////////////// TASKS ///////////////////////////////////////////

1. CLASSES X
...
2. Getters and Setters (access methods) X
...
3. Friend Attribute X
...
4. Operators Overloading (+=,++,!,[],>=,<= etc.) X
...
5. Files
...
6. Inheritance and Inclusion (IS A and HAS A relationships)
...
7. Virtual Functions
...
8. Abstract Classes X
...
9. Templates
...
10. STL
...

*////////////////////////////////////////////////////////////////////////////////////////////////////


// 1. CLASSES


class Employee {

	// YOU CAN DECLARE PUBLIC, PRIVATE (DEFAULT) and PROTECTED(for IS A relationship)
	
private: // only accesed in the class or using 'friend' functions

	// CONST variable that can be assigned once
	// - you can crate another function as an IDs generator that increments id everytime an Employee object is created
	const int id = 0;
	
	// STATIC variable will be mentioned only in the class, it will be assigned with a GENERAL value associated to CLASS Employee outside the class.
	// You can give/modify values to it without creating an Employee Object.
	static double salary;

	// other attributes
	// NOTE - initiate and assign with values here and Default Constructor will be only Employee(){}
	string name = "n/a";
	int age = 0;
	
	// Dynamic allocation
	int nrDiscounts = 0; // number of discounts and the size of 'discounts' vector
	float* discounts = nullptr; // 'nullptr' cause it's a pointer DON'T USE [0,0.0," ", etc.]

public: // PUBLIC to everyone, can be accesed from everywhere. PROTECTED is used when we want to access from derived classes.

	// CONSTRUCTORS (NOTE THAT I WILL ADD CONDITIONS ONLY WHERE IT MATTERS, checking name length and other is OPTIONAL to you)
	
	// Default
	Employee(){}

	// With ID only
	Employee(int _id):id(_id){
		// DON'T FORGET ABOUT OTHER ATTRIBUTES!!
		this->name = "n/a";
		this->age = 0;
		this->nrDiscounts = 0;
		this->discounts = nullptr;
	}

	// With all parameters (There are few ways to create it)
	 
	// 1. By using :id(_id),name(name)... NOT FOR DINAMICALLY ALLOCATION!
	// 2. By using :id(_id) { this->name = _name; this->nrDiscounts = _nrDiscounts... + allocation etc. }

	// Why always :id(_id) and NOT this->id = _id?
	/* 
	Because id is a CONST variable and when you are doing : id(_id) it gets the value right when the OBJECT is created.
	Instead when doing this->id = _id , the id is already initiated with '0' from class attributes and the program tries to assign the _id value to the const variable and THIS IS NOT POSSIBLE!	
	*/
	Employee(int _id, string _name, int _nrDiscounts, float* _discounts):id(_id),name(_name),nrDiscounts(_nrDiscounts) { // attributes are already assigned with _... values

		if (this->nrDiscounts > 0) { // checking if we have a vector pozitive size
			// allocating memory NEW 'type'[SIZE OF ALLOCATED MEMORY]
			this->discounts = new float[this->nrDiscounts]; // NOTE!! when allocating for char* type always add + 1!! EX: [SIZE + 1] That's for '\0'
			for (int i = 0; i < this->nrDiscounts; i++) {
				this->discounts[i] = _discounts[i];
			}
		}
	}
	// GOOD PRACTICE - when you allocated memory in the Constructor create the Destructor right after.
	~Employee() { // DESTRUCTOR
		if (this->discounts != nullptr) { // checking if we have memory allocated
			delete[] this->discounts; // deleting memory, [] it's because discounts is a vector
			this->discounts = nullptr; // NO DANGLING pointers *google it for more
		}
	}

	// COPY CONSTRUCTOR (SHALLOW COPY)

	Employee(const Employee& a):id(a.id) {  // 'const Employee& a' because we DON'T want to modify any data from the Object named "a"
		this->name = a.name;
		if (a.nrDiscounts > 0 && a.discounts != nullptr) {
			this->nrDiscounts = a.nrDiscounts;
			this->discounts = new float[this->nrDiscounts];
			for (int i = 0; i < this->nrDiscounts; i++) {
				this->discounts[i] = a.discounts[i];
			}
		}
	}
	// main function assignments: Employee a1 = a2, Employee a1(a2)

	// OPERATOR= (DEEP COPY)

	Employee& operator=(const Employee& a) { // imagine this like | a1 = a2 | where | a1 is "Employee&" | = is "operator=" | a2 is "const Employee a" | 
		if (this != &a) { // checking if a1 is not the same as a2
			// 1. ADD the DESTRUCTOR (just copy it if you have done it) (only if in the class is dinamically memory allocated)
			if (this->discounts != nullptr) { // checking if we have memory allocated
				delete[] this->discounts; // deleting memory, [] it's because discounts is a vector
				this->discounts = nullptr; // NO DANGLING pointers *google it for more
			}
			// 2. ADD the COPY CONSTRUCTOR
			this->name = a.name;
			if (a.nrDiscounts > 0 && a.discounts != nullptr) {
				this->nrDiscounts = a.nrDiscounts;
				this->discounts = new float[this->nrDiscounts];
				for (int i = 0; i < this->nrDiscounts; i++) {
					this->discounts[i] = a.discounts[i];
				}
			}
		}
		return *this; // Don't forget to return *this !!!! 
	}
	//*google differences between shallow copy and deep copy

	// 2. GETTERS AND SETTERS (ACCESS METHODS) 

	int getAge() const { //adding CONST because we want only to return the value and not modify it
		return this->age;
	}
	void setAge(int _age) {
		this->age = _age;
	}
	string getName() const {
		return this->name;
	}
	void setName(string _name) {
		this->name = _name;
	}
	// IMPORTANT! Here is the getter and setter for dinamically allocated memory, precise 'discounts' pointer.
	const float* getDiscounts() {
		return this->discounts; // you can use CONST returning the pointer and it can't be modified in theory (not 100% safe) (memory efficient)
	}
	float* getDiscount() {
		float* copy;
		copy = new float[this->nrDiscounts];
		for (int i = 0; i < this->nrDiscounts; i++) {   // THIS IS INCAPSULATION !!!!!!!! WE ARE RETURNING A COPY AND NOT THE REAL ADDRESS OF MEMORY!
			copy[i] = this->discounts[i]; // !! 100% SAFE !!!!(NOT MEMORY EFFICIENT ON BIGGER DATA)
		}
		return copy;
	}

	// 3. Friend Attribute (gives access to a class or a function to everything in the class)

	friend int RandomCalculation(Employee& a); // WE CAN ACCESS PRIVATE ATTRIBUTES OUTSIDE THE CLASS!! Not recommended! CHECK -> (3)

	// used with ostream and istream operators for inputs and outputs

	// 4. OPERATORS OVERLOADING
	
	// STREAMING OPERATORS
	friend ostream& operator<<(ostream& out, const Employee& a) {
		out << "Name: " << a.name << endl;
		out << "Age: " << a.age << endl;
		out << "Number of DISCOUNTS: " << a.nrDiscounts << endl;
		out << "Discounts: ";
		if (a.nrDiscounts > 0 && a.discounts != nullptr) {
			for (int i = 0; i < a.nrDiscounts; i++) {
				out << a.discounts[i] << " ";
			}
		}
		out << endl;
		return out; // DON'T FORGET TO RETURN 'out'!
	}
	friend istream& operator>>(istream& in, Employee& a) { // NO CONST for istream!!
		cout << "Type your name: " << endl;
		in >> a.name;
		cout << "\n" << "Type your age: " << endl;
		in >> a.age;
		cout << "\n" << "Type your number of DISCOUNTS: " << endl;
		in >> a.nrDiscounts;
		cout << "\n" << "Type discount value: " << endl;
		if (a.nrDiscounts > 0) {
			a.discounts = new float[a.nrDiscounts];
			for (int i = 0; i < a.nrDiscounts; i++) {
				in >> a.discounts[i];
			}
		}
		return in; // DON'T FORGET TO RETURN 'in'!
	}

	// UNARY OPERATORS 

	Employee& operator+=(int _value) { // for static allocated memory
		this->age += _value;
		return *this;
	}

	Employee& operator+=(float _value) { // adding a new element to dinamically allocated memory 'DISCOUNTS'
		float* newDiscounts = new float[this->nrDiscounts + 1];
		for (int i = 0; i < this->nrDiscounts; i++) {
			newDiscounts[i] = this->discounts[i];
		}
		newDiscounts[this->nrDiscounts] = _value; // here we are adding the new value on last position!
		this->nrDiscounts++; // incrementing the number of discount too
		if (this->discounts != nullptr) {
			delete[] this->discounts;  // deleteing the data from discounts pointer
		}
		this->discounts = newDiscounts; // giving new data from NEW DISCOUNTS to DISCOUNTS back by passing the POINTER!
		return *this;
	}
	// same for '-='

	bool operator!() {
		if (this->age == 0) {
			return true;
		}
		return false;
	}

	bool operator==(const Employee& a) const { // let's try with an object (EX: Employee A1,A2(3); A1 == A2 ??)
		bool result = true;
		if (this->age != a.age) result = false;
		if (this->name != a.name) result = false;
		if (this->nrDiscounts != a.nrDiscounts) {
			result = false;
		}
		else {
			for (int i = 0; i < this->nrDiscounts; i++) {
				if (this->discounts[i] != a.discounts[i]) result = false;
			}
		}
		return result;
	};

	float operator[](int _index) {
		return this->discounts[_index];
	}

	Employee& operator++() {   // OPERATOR++ PRE-INCREMENTED!! - it will change the value and return the value changed (ex: age = 1 ... operator++(pre) ... will return age = 2)
		this->age++;
		return *this;
	}
	Employee operator++(int) { // OPERATOR++ POST-INCREMENTED!! - it will change the value and it will return the value BEFORE incrementation!!
		Employee copy = *this; // (ex: age = 1 ... operator++(post) ... will return age = 1, BUT FOR NEXT OPERATIONS AGE IS 2!!)
		this->age++;
		return copy; // we will return a COPY of the object with value before the incrementation
	}
	// same for '--'

	Employee& operator+(const Employee& a) {   // this is for A1 = A2 + A3; 
		int value = this->age + a.age;		// IMAGINE LIKE: 'A1 =' is (operator=), 'A2 + A3' is (A2.operator+(A3) OR Employee&(A2) operator+(const Employee& a)(A3))
		this->age = value;
		return *this;
	}
	// Note that here we modify A2 !! (because we passed '&' and we don't return a copy but the object itself!)
	// or you can use Employee without & BEFORE operator+ and return a copy of the object with 'return this'

	Employee& operator+(int _value) {   
		int result = this->age + _value;  // THAT'S A1 = A2 + 10(value)
		this->age = result;
		return *this;
	}

	friend Employee operator+(int _value, Employee a) {  // this is A1 = VALUE(1,4,25,12 etc.) + A2;
		return a.age + _value;
	}
	// why friend? Because you give a foreign value (int _value) and the compiler has to know that the value isn't one of the object either way you get and ERROR!
	// same for '-'

	bool operator<=(const Employee& a) {
		return this->age <= a.age;
	}
	bool operator>=(const Employee& a) {
		return this->age >= a.age;
	}
	
	explicit operator float() {  // Using 'explicit' is safer, you have to call it manually;
		return this->age; // EX: Employee x(...); int y = 213; x = (float) y;
	}
	operator double() {
		return this->age; // Using default (implicit) could lose some information, not too safe to use it will automatically cast.
	} //EX: Employee x(...); int y = 24; x = y; 

	// 5. FILES 
	friend ofstream& operator<<(ofstream& fout, const Employee& a) {
		// same as ostream //
		fout << "Name: " << a.name << endl;
		fout << "Age: " << a.age << endl;
		fout << "Number of DISCOUNTS: " << a.nrDiscounts << endl;
		fout << "Discounts: ";
		if (a.nrDiscounts > 0 && a.discounts != nullptr) {
			for (int i = 0; i < a.nrDiscounts; i++) {
				fout << a.discounts[i] << " ";
			}
		}
		fout << endl;
		return fout; // DON'T FORGET TO RETURN 'fout'!
	}
	friend ifstream& operator>>(ifstream& fin, Employee& a) {
		// same as istream but with some minor modifications //
		// NOTE: for char* you have to create a buffer with a certain size!!
		// NOTE: You have to use 'ws' and getline() !! * google it for more!
		// IF YOU ARE USING STRINGS you don't have to use these.
		cout << "Type your name: " << endl;
		fin >> a.name;
		cout << "\n" << "Type your age: " << endl;
		fin >> a.age;
		cout << "\n" << "Type your number of DISCOUNTS: " << endl;
		fin >> a.nrDiscounts;
		cout << "\n" << "Type discount value: " << endl;
		if (a.nrDiscounts > 0) {
			a.discounts = new float[a.nrDiscounts];
			for (int i = 0; i < a.nrDiscounts; i++) {
				fin >> a.discounts[i];
			}
		}
		return fin; // DON'T FORGET TO RETURN 'in'!
	}
};

double Employee::salary = 2904.42; // STATIC ASSIGNMENT FROM EMPLOYEE (1)

int RandomCalculation (Employee& a) { // PROOF (3)
	int number = a.age + a.nrDiscounts; // We accessed private attributes and used them.
	return number;
}


// 8. ABSTRACT CLASS

// A class only with pure virtual methods is called Abstract Class.
// Virtually Pure Method is a virtual method assigned with 0 (ex: virtual int SumOfTwo() = 0)
// Virtual Method != Pure Virtual Method!!

class Abstract {
public:
	virtual void Testing() = 0;
	virtual int Calculator() = 0;
};


int main() { 

	// 1. CLASSES

	int numberOfDiscounts = 3;
	float discounts[]{ 25,12.5,45.1 };
	Employee p1(), p2(1), p3(2, "Jhon", 3, discounts);
	//copy constructor
	Employee p4 = p3;
	Employee p5(p3);
	//operator=
	p5 = p2;

	// 2. GETTERS AND SETTERS (ACCESS METHODS)

	p3.getAge();
	p4.setName("Michael");
	p5.getDiscounts();

	// 3. FRIEND ATTRIBUTE

	RandomCalculation(p3); // accessing everything in p3 through the function.

	// 4. OPERATORS OVERLOADING

	// istream + ostream
	cout << p2 << endl;
	cin >> p2;
	cout << endl;
	cout << p2;

	// += for static and dinamic
	cout << p3 << endl;
	p3 += 2;
	cout << p3 << endl;
	cout << p2 << endl;
	p2 += float(492.2); // adding one more discount
	cout << p2;
	
	// ==, !, [], <=, >=
	p2 == p3 ? cout << "Equal" : cout << "Not equal"; // same as if(p1==p3) {cout << "Equal";} else {cout << "Not equal";} [CONDITIONAL OPERATOR]
	
	!p2 ? cout << "Not a valid age!" : cout << "Valid age!";
	
	cout << p3[1] << endl;
	
	p2 >= p3 ? cout << "p2 is greater/equal p3" : cout << "NOT!";
	p2 <= p3 ? cout << "p2 is less/equal p3" : cout << "NOT";

	// ++
	p2++;
	cout << p2 << endl;
	
	// +
	
	p3 = p5 + p2;
	p2 = 10 + p3;
	p2 = p3 + 10;

	// casting int -> float

	int y = 242;
	p3 = (float)y; // explicit
	p3 = y; // implicit

	// 5. FILES

	// output to file in TEXT FORMAT
	// let's say we already have p3
	ofstream fileOut("file_output_name.txt", ios::out);
	fileOut << p3;

	// input from file in TEXT FORMAT
	Employee p6(10);
	ifstream fileIn("file_name.txt", ios::in);
	fileIn >> p6;
	cout << p6;

	return 0; 
}