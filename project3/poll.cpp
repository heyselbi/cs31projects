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

	if (upperPollData.size() < 1)
		return true;

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


/*1- Check proper syntax of pollData
2- Proper syntax of party code
3- Capitalize both party and pollData so it is consistent to work with
4- Slice the pollData into state forecasts separated by comma
5- If state forecast has no forecasts, skip; If it does, analyze chars after state code.
6- While scanning forecasts, if you bump into party code of interest, copy the digits before it. They are copied in reverse order
7- Convert digit (string type) into char
8- Convert char into int
9- Sum up int (ie. 5 + 4*10 + 1*100) depending on their relative position index
10- Repeat for all state forecasts and sum up all at the end*/
int tallySeats(string pollData, char party, int& seatTally)
{
	//1- Check proper syntax of pollData
	if (!hasProperSyntax(pollData))
		return 1;
	//2- Proper syntax of party code
	if (!isalpha(party))
		return 2;

	//3- Capitalize both party and pollData so it is consistent to work with
	string upperParty = "";
	upperParty += toupper(party);

	string upperPollData;
	for (string::size_type k = 0; k != pollData.size(); k++)
		upperPollData += toupper(pollData[k]);

	int pollSum = 0;
	upperPollData += ","; //so last stateForecast can be analyzed too
	string stateForecast = "";

	//4- Slice the pollData into state forecasts separated by comma
	for (string::size_type j = 0; j != upperPollData.size(); j++)
	{
		stateForecast += upperPollData[j];
		//if you bump into comma, your stateForecast is ready to be analyzed
		if (upperPollData[j] == ',')
		{
			/*//if state forecast is empty, move to next one ==> has been clarified by professor
			if (stateForecast == ",")
			{
				cout << "This is line 147, stateForecast empty " << stateForecast << endl;
				stateForecast = ""; continue;
			}*/

			//5- If state forecast has no forecasts, skip
			if (stateForecast.size() < 4)
			{
				stateForecast = "";
				continue;
			}

			//Let's analyze only those ones that have forecasts (ie. analyze if CT8R, and not analyze if just VT)
			if (stateForecast.size() > 3) //three because comma is included (ie. VT,)
			{
				//Starting at index 2 (statecode is index 0 and 1) analyze stateForecast
				int stateForecastSum = 0;
				for (string::size_type i = 2; i != stateForecast.size(); i++) //stateForecast analysis (ie. NY12R110D)
				{	
					//6- While scanning forecasts, if you bump into party code of interest, copy the digits before it. 
					//They are copied in reverse order
					if (stateForecast[i] == upperParty[0]) 
					{
						string stateForecastNumber = ""; //string for number of party votes

						//go in reverse and copy the chars
						for (string::size_type m = (i-1); m > 1; m--)
						{
							if (isalpha(stateForecast[m]))
								break;
							//7- Convert string type party vote number into char in reverse order since we are going backwards
							stateForecastNumber += stateForecast[m]; //stateForecastNumber = 011 in reverse (ie. real is 110)
							
						}
						
						int conv;
						//8- Convert char into int
						for (string::size_type n = 0; n != stateForecastNumber.size(); n++) 
						{
							conv = stateForecastNumber[n];
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
							//9- Sum up int (ie. 5 + 4*10 + 1*100) depending on their relative position index
							if (n == 0)
								stateForecastSum += conv;
							else
							{
								int power = pow(10.0, n);
								stateForecastSum += (conv * power);
							}
						}
					}
				}
				//10- Repeat for all state forecasts and sum up all at the end
				pollSum += stateForecastSum;
			}
			
			stateForecast = "";
		} //reset stateForecast to empty string for a new analysis
		
	}
	//assign total to seatTally that is returned
	seatTally = pollSum;
	return 0;
}

int main()
{	
	for (;;)
	{
		cout << "Enter poll data string: ";
		string pollData;
		getline(cin, pollData);

		cout << hasProperSyntax(pollData) << endl;

		int s;
		cout << tallySeats(pollData, 'r', s) << endl;
		cout << s << endl;

		if (pollData == "quit")
			break;
	}
}
