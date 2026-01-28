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
4. Operators Overloading (+=,++,!,[],>=,<= etc.)
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
	static float salary;

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
		this->discounts == nullptr;
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

	friend int RandomCalculation(Employee& a) {}; // WE CAN ACCESS PRIVATE ATTRIBUTES OUTSIDE THE CLASS!! Not recommended! CHECK -> (3)

	// used with ostream and istream operators for inputs and outputs

};

float Employee::salary = 2904.42; // STATIC ASSIGNMENT FROM EMPLOYEE (1)

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

	return 0; 
}