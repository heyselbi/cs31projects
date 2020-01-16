#include <iostream>
#include <string> //so we could work with strings
using namespace std;

int main()
{
	//Let's first introduce all integers and strings
	int odometerStart, odometerEnd, days, month;
	string name, luxCar;

	//Let's get information on rental
	cout << "Odometer at start: ";
	cin >> odometerStart;

	cout << "Odometer at end: ";
	cin >> odometerEnd;

	cout << "Rental days: ";
	cin >> days;
	cin.ignore(10000, '\n');  //to clean out newline

	cout << "Customer name: ";
	getline(cin, name);			//no need for cin.ignore because getline already gets rid off newline

	cout << "Luxury car? (y/n): ";
	getline(cin, luxCar);

	cout << "Month (1=Jan, 2=Feb, etc.): ";
	cin >> month;

	cout << "---" << endl;

	//After all inputs are given, now time to check if there are any errors in inputs
	if ((odometerStart) < 0) {						 //user entered negative integer
		cout << "The starting odometer reading must be nonnegative." << endl;
		return 1;
	} else if (odometerEnd < odometerStart) {		//user entered larger value for starting odometer than the ending one
		cout << "The final odometer reading must be at least as large as the starting reading." << endl;
		return 1;
	} else if (days < 1) {							//user entered a non-positive number of days
		cout << "The number of rental days must be positive." << endl;
		return 1;
	} else if (name == "") {                       //user did not enter his/her name
		cout << "You must enter a customer name." << endl;
		return 1;
	} else if ((luxCar != "n") && (luxCar != "y")) { //user types something else but y or n
		cout << "You must enter y or n." << endl;
		return 1;
	} else if ((month < 1) || (month > 12)) {         //user typed out of range month
		cout << "The month number must be in the range 1 through 12." << endl;
		return 1;
	}

	//Let's set the base rates
	double baseRate;
	if (luxCar == "n")
		baseRate = 33.0;  //base rate of non-luxury car
	else
		baseRate = 61.0;  //base rate of a luxury car

	//Now let's charge according to miles driven
	double milesDriven = (odometerEnd - odometerStart);  //calculate total miles driven

	//Because I will be using only one formula at the end, I start *Mile with zero and assign values
	//to miles traveled depending on amount of milesDriven
	//totalCharge is defined at the end with a formula and *Rate are defined within if/else statements
	//depending on milesDriven
	//low* is when milesDriven is less than 100, mid* when between 100 and 400 
	//and high* when more than 400 miles
	double totalCharge, midRate, highRate, lowMile = 0, midMile = 0, highMile = 0;

	//if milesDriven are less than 100, we have only lowMile and no mid or high miles
	if (milesDriven < 101) {
		midRate = 0;
		highRate = 0;
		lowMile = milesDriven;

	}
	else if ((milesDriven > 100) && (milesDriven < 401)) { //if milesDriven between 100 and 400, we have low and mid miles and rates
		highRate = 0;
		lowMile = 100;
		midMile = (milesDriven - 100);
		{
			if ((month < 12) || (month > 3)) //if month is between April and November
				midRate = 0.21;
			else //if month is between December and February
				midRate = 0.27;
		}
	}
	else { //if milesDriven is more than 400 miles
		highRate = 0.19;
		lowMile = 100;
		midMile = 300;
		highMile = (milesDriven - 400);
		if ((month < 4) || (month == 12)) //same logic as above
			midRate = 0.27;
		else //same logic as above
			midRate = 0.21;
	}

	//now let's calculate final total charge based on miles traveled
	totalCharge = ((baseRate*days) + (lowMile*0.27) + (midMile*midRate) + (highMile*highRate));

	//here we set precision to two decimal points
	cout.setf(ios::fixed);
	cout.precision(2);
	cout << "The rental charge for " << name << " is $" << totalCharge << endl;

}
