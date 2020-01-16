#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
#include <cmath>
using namespace std;

// Return true if the argument is a two-uppercase-letter state code, or
// false otherwise.
bool isValidUppercaseStateCode(string stateCode)
{
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY."
		"LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND."
		"OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos  &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

/*returns true if its parameter is a proper poll data string
1- Test if any blanks in pollData
2- Since isValidUppercaseStateCode works only with capital letters, capitalize all letters
3- Slice pollData into separate state forecasts for analysis
4- Check if the first two chars of state forecast are proper state names
5- If state forecast has not forecasts, do not analyze further
But if a state has forecasts, analyze those
6- Check if it is a digit (party vote number) after state code (ie. NY1... --> 1)
7- Except for the last two characters, check if party code is only one letter 
by making sure there are digits on both sides of the party code
8 - Check if last char in state forecast is a letter (ie. party code)
9 - Check if a character right before last char (ie. party code) is a digit, making sure party code is single-letter
10 -If have passed all above tests, return true*/
bool hasProperSyntax(string pollData)
{
	string upperPollData;
	for (string::size_type k = 0; k != pollData.size(); k++)
	{
		//Step 1: Test if any blanks in pollData; If any empty spaces, say false
		if (pollData[k] == ' ')
			return false;

		//Step 2: let's capitalize all letters so it is easier to work with it
		upperPollData += toupper(pollData[k]);
	}	

	//Step 3: now we are going to analyze each state forecast one by one
	//by slicing upperPollData into separate state forecasts
	string stateForecast = "";
	upperPollData += ","; //so last stateForecast can be analyzed too

	for (string::size_type j = 0; j != upperPollData.size(); j++)
	{	
		stateForecast += upperPollData[j];

		//if you bump into comma, your stateForecast is ready to be analyzed
		if (upperPollData[j] == ',')
		{
			/*//let's make sure that empty forecast is seen as valid; if it is, skip all and continue with for loop
			if (stateForecast == ",")
			{
				stateForecast = "";
				continue;
			}*/

			//let's slice first two chars for state correctness check-up
			string corrState = stateForecast.substr(0, 2); 

			//Step 4: Check if the first two chars of state forecast are proper state names
			if (!isValidUppercaseStateCode(corrState))
				return false;
			
			//Step 5: now let's analyze party letters and make sure there are no consecutive letters
			//and we will analyze only those ones that have forecasts (ie. analyze if CT8R, and not analyze if just VT)
			if (stateForecast.size() > 3)
			{
				//Step 6: Check if it is a digit (party vote number) after state code (ie. NY1... --> 1)
				if (!isdigit(stateForecast[2]))
					return false;

				//Step 7: Except for the last two characters, check if party code is only one letter 
				//Starting at index 2 (statecode is index 0 and 1) analyze stateForecast up until
				//two last chars, which will be tested after
				for (string::size_type i = 2; i != (stateForecast.size() - 2); i++) 
				{	//find party letter and make sure it is a digit right before it and digit right after it
					if (isalpha(stateForecast[i]))
					{
						if ((!isdigit(stateForecast[i + 1])) || (!isdigit(stateForecast[i - 1])))
							return false;
					}
					//let's also make sure there are no other chars than digits and letters
					else if ( !isdigit(stateForecast[i]) )
						return false;
				}

				//Step 8: now let's analyze last two chars since last char, which is a party code
				//does not have a digit after, but has a comma
				//make sure last char is a letter (ie. party letter); if not, say false
				if (!isalpha(upperPollData[j - 1]))
					return false;

				//Step 9: make sure last char is a letter (ie. party letter); if not, say false
				if (!isdigit(upperPollData[j - 2]))
					return false;
			}
			//reset stateForecast to empty string for a new analysis
			stateForecast = "";
		}	
	}
	//Step 10: if all above tests have been passed, then it is correct syntax
	return true;
}

int tallySeats(string pollData, char party, int& seatTally)
{
	if (!hasProperSyntax(pollData))
		return 1;
	if (!isalpha(party))
		return 2;

	//let's capitalize party character so it is easier to work with and consistent
	string upperParty = "";
	upperParty += toupper(party);
	cout << "This is line 123, upperParty = " << upperParty << endl;

	//let's capitalize pollData like we did in earlier function so it is all consistent
	string upperPollData;
	for (string::size_type k = 0; k != pollData.size(); k++)
		upperPollData += toupper(pollData[k]);

	int pollSum = 0;
	upperPollData += ","; //so last stateForecast can be analyzed too
	string stateForecast = "";

	cout << "This is line 135, upperPollData = " << upperPollData << endl;

	for (string::size_type j = 0; j != upperPollData.size(); j++)
	{
		stateForecast += upperPollData[j];
		cout << "This is line 140, stateForecast = " << stateForecast << endl;
		//if you bump into comma, your stateForecast is ready to be analyzed
		if (upperPollData[j] == ',')
		{
			/*//if state forecast is empty, move to next one
			if (stateForecast == ",")
			{
				cout << "This is line 147, stateForecast empty " << stateForecast << endl;
				stateForecast = ""; continue;
			}*/

			cout << "This is line 151, final stateForecast = " << stateForecast << endl;

			//if state forecast is just VT, (three chars), move to next state forecast
			if (stateForecast.size() < 4)
			{
				stateForecast = "";
				continue;
			}

			//Let's analyze only those ones that have forecasts (ie. analyze if CT8R, and not analyze if just VT)
			if (stateForecast.size() > 3) //three because comma is included (ie. VT,)
			{
				cout << "This is line 163, stateForecast analysis begin with " << stateForecast << endl;
				//Starting at index 2 (statecode is index 0 and 1) analyze stateForecast
				int stateForecastSum = 0;
				for (string::size_type i = 2; i != stateForecast.size(); i++) //stateForecast analysis (ie. NY12R110D)
				{	
					cout << "This is line 168, party is " << party << endl;
					if (stateForecast[i] == upperParty[0]) //D is at position 8; i =8
					{
						cout << "This is line 170, location of R (i) is = " << i << endl;
						string stateForecastNumber = ""; //slicing the number

						for (string::size_type m = (i-1); m > 1; m--) //i=8, m=7
						{
							if (isalpha(stateForecast[m]))
								break;
							cout << "This is line 175, i and m are " << i << " " << m << endl;
							stateForecastNumber += stateForecast[m]; //stateForecastNumber = 011 (ie. 110)
							cout << "This is line 177, stateForecastNumber = " << stateForecastNumber << endl;
							
						}
						
						int conv;
						//stateForecastNumber = 011 (ie. 110)
						cout << "This is line 184, final stateForecastNumber = " << stateForecastNumber << endl;
						for (string::size_type n = 0; n != stateForecastNumber.size(); n++) 
						{
							conv = stateForecastNumber[n]; //n=0, Number = 48
							cout << "This is line 188, conv = " << conv << endl;
							cout << "Character number " << n << " of stateForecastNumber is " << conv << endl;
							switch (conv)
							{
							case 48: //ASCII
							case 240: //EBCDIC
								conv = 0;
								break;
							case 49:
							case 241:
								conv = 1;
								break;
							case 50:
							case 242:
								conv = 2;
								break;
							case 51:
							case 243:
								conv = 3;
								break;
							case 52:
							case 244:
								conv = 4;
								break;
							case 53:
							case 245:
								conv = 5;
								break;
							case 54:
							case 246:
								conv = 6;
								break;
							case 55:
							case 247:
								conv = 7;
								break;
							case 56:
							case 248:
								conv = 8;
								break;
							case 57:
							case 249:
								conv = 9;
								break;
							}
							cout << "This is line 233, converted conv = " << conv << endl;
							cout << "This is line 234, n is = " << n << endl;
							if (n == 0)
								stateForecastSum += conv;
							else
							{
								int power = pow(10.0, n);
								stateForecastSum += (conv * power);
							}
							cout << "This is line 236, stateForecastSum = " << stateForecastSum << endl;
						}
					}
				}
				cout << "This is line 240, pollSum before = " << pollSum << endl;
				pollSum += stateForecastSum;
				cout << "This is line 242, pollSum after = " << pollSum << endl;
			}
			
			stateForecast = "";
		} //reset stateForecast to empty string for a new analysis
		
	}
	
	cout << "This is line 249, pollSum final = " << pollSum << endl;
	seatTally = pollSum;
	return 0;
}

int main()
{	
	for (;;)
	{
		cout << "Enter poll data string: ";
		string pollData;
		//pollData = "NY1I,NC10D,CT9R7D,VT,CA7D8I";
		getline(cin, pollData);
		cout << hasProperSyntax(pollData) << endl;
		int s;
		cout << tallySeats(pollData, 'r', s) << endl;
		cout << s << endl;

		//string pds;
		//getline(cin, pds);
		if (pollData == "quit")
			break;
		
		//cout << "hasProperSyntax returns ";
		//assert(hasProperSyntax(pds));
		//assert(!hasProperSyntax(pds));
		//cout << endl;
	}
}
