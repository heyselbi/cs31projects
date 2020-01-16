#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cassert>
#include <cctype>
#include <cstring>
using namespace std;

const int MAX_WORD_LENGTH = 20;
const int MAX_DOC_LENGTH = 250;

void shiftToLeft(char a1[][MAX_WORD_LENGTH + 1], char a2[][MAX_WORD_LENGTH + 1], int a3[], int startIndex, int arrayLength)
//shift all array elements after startIndex to the left by one index
{
	for (int k = startIndex; k < (arrayLength-1); k++)
			{
				strcpy(a1[k], a1[k + 1]);
				strcpy(a2[k], a2[k + 1]);
				a3[k] = a3[k + 1];
			}
}

//
void clean(const char origDoc[], char cleanDoc[])
{
  int j = 0, origDocLength = strlen(origDoc);
	for (int i = 0; i < origDocLength; i++)
	{
		if ( isalpha(origDoc[i]) || isspace(origDoc[i]) )
		{
      cleanDoc[j] = tolower(origDoc[i]);
      j++;
		}
	}
  // C strings must have 0 at end of array, so strlen() knows the length
  cleanDoc[j] = 0;
}

int makeProper(char word1[][MAX_WORD_LENGTH + 1], char word2[][MAX_WORD_LENGTH + 1], int separation[], int nPatterns)
{
	//if nPatterns is negative, set it to zero

	if (nPatterns < 0)
	{
		nPatterns = 0;
		return nPatterns;
	}
	else if (nPatterns == 0)
	{
		return nPatterns;
	}

	for (int i = 0; i < nPatterns; i++)
	{
		if ((separation[i] < 0) || (strlen(word1[i]) < 1) || (strlen(word2[i])) < 1)
		{
			//if separation or word in an array is not proper, shift all words after it to the left by one index
			shiftToLeft(word1, word2, separation, i, nPatterns);
			nPatterns--;
            i--;
            continue;
		}

		//check properness of word1
        bool checkWord2 = true; //if no mistakes found in ith element of word1, check word2's ith element
		int wordLength = strlen(word1[i]);
		for (int m = 0; m < wordLength; m++)
		{
			if (!isalpha(word1[i][m]))
			{
				shiftToLeft(word1, word2, separation, i, nPatterns);
				nPatterns--;
                i--;
                checkWord2 = false;
                break;
			}
			word1[i][m] = tolower(word1[i][m]);
		}
        
        if(checkWord2 == false)
            continue;

		//check properness ofword2
        wordLength = strlen(word2[i]);
        for (int n = 0; n < wordLength; n++)
        {
            if (!isalpha(word2[i][n]))
            {
                shiftToLeft(word1, word2, separation, i, nPatterns);
                nPatterns--;
                i--;
                break;
            }
            word2[i][n] = tolower(word2[i][n]);
        }
	}

    /*
    printf("------\n");
    for(int i=0; i<nPatterns; i++)
        printf("%s\t%s\t%d\n", word1[i], word2[i], separation[i]);
    printf("------\n");
    */

	//check if w1 element matches w2 element in a pattern
	int index = 0;
	while (index < nPatterns)
	{
		int compIndex = index+1;
		while (compIndex < nPatterns)
		{
			if ( ( strcmp(word1[index], word1[compIndex]) == 0 && strcmp(word2[index], word2[compIndex]) == 0)
				|| 
                (strcmp(word1[index], word2[compIndex]) == 0 && strcmp(word2[index], word1[compIndex]) == 0) )
				{
                    // printf("%s,%s %d  %d\n", word1[index], word2[index], separation[index], separation[compIndex]);
					if (separation[index] < separation[compIndex])
					{
                        // printf("delete at index %d\n", index);
						shiftToLeft(word1, word2, separation, index, nPatterns);
						nPatterns--;
                        index--;
                        break;
					} else {
                        // printf("delete at compIndex %d\n", compIndex);
					    shiftToLeft(word1, word2, separation, compIndex, nPatterns);
					    nPatterns--;
                        compIndex--;
                    }
				} 
			compIndex++;
		}
        index++;
	}
	return nPatterns;
}

//
int rate(const char document[], const char word1[][MAX_WORD_LENGTH + 1], const char word2[][MAX_WORD_LENGTH + 1], const int separation[],
	int nPatterns)
{
	if (nPatterns < 0)
	{
		nPatterns = 0;
		return nPatterns;
	}
	else if (nPatterns == 0)
	{
		return nPatterns;
	}

	
	char workingDoc[MAX_DOC_LENGTH + 1];
  clean(document, workingDoc);

  // printf("\n\n\ncleaned workingDoc is '%s'\n", workingDoc);
  
 
  char arrayDoc[MAX_DOC_LENGTH+1][MAX_DOC_LENGTH+1]; 

	int arrayRow = 0;
  int workingDocLength = strlen(workingDoc);
  for (int d = 0; d < (workingDocLength );)
	{
		int charIndex = 0;
    if (isalpha(workingDoc[d]))
		{
      while (workingDoc[d] != ' ' && d<(workingDocLength))
      {
        arrayDoc[arrayRow][charIndex] = workingDoc[d];
        d++;
        charIndex++;
      }
      arrayDoc[arrayRow][charIndex] = '\0';
      arrayRow++;
		}
    d++;
	}

  //now we have an array of words
  //arrayDoc[..][..] = {docword0,docword1,docword2..}

  int rating = 0;
  
  for (int p = 0; p < nPatterns; p++)
  {
    // printf("CHecking for patter %d - %s %s\n", p, word1[p], word2[p]);
    bool patternFound = false;

    //word1[0], word2[0], separation[0]
    for (int a = 0; a < arrayRow; a++) //let's go through the arrayDoc
    {
      if (strcmp(arrayDoc[a],word1[p]) == 0) //if we find match to word1
      {
        // printf("Word1 matches at index %d, sep is %d\n", a, separation[p]);
        for (int searchPos = a - separation[p] - 1; searchPos <= a + separation[p] + 1; searchPos++) //search right side
        {
          if (searchPos < 0) 
            continue;
          if (searchPos >= arrayRow) 
            continue;
            
          // printf("Looking for word2 %d %s\n", searchPos, arrayDoc[searchPos]);
          if (strcmp(arrayDoc[searchPos], word2[p]) == 0)
          {
            // printf("Found!\n");
            rating++;
            patternFound = true;
            break;
          }

        }
        
        // if pattern is found, break from outer for loop too.
        if(patternFound)
          break;

      }

    }

  }
  // printf("rating of document is %d\n", rating);
	return rating;
}

int main()
{
    /*
	char w1[16][MAX_WORD_LENGTH + 1] = { "zero", "one",     "two",  "one",      "zero", "five", "six",      "five", "eight",    "hello",    "ten", "eleven", "mad", "blackt", "wh!!te", "bbb" };
	char w2[16][MAX_WORD_LENGTH + 1] = { "five", "eleven",  "one",  "three",    "five", "zero", "world",    "hi",   "",         "zero",     "ten", "one", "sc##", "hat", "h$t", "aaa" };
	int sep[16] = {                     5,          1,         2,   3,          4,      0,      6,          7,      8,          9,          -10,     11, 2, -100, 3, 33 };
	int numberElements = 16;

	int result = makeProper(w1, w2, sep, numberElements);

	//makeProper(w1, w2, sep, numberElements);

	cout << "return value is " << result << endl;
	cout << "numberElements is " << numberElements << endl;

	cout << "w1 is {";
	for (int a = 0; a < result; a++)
	{
		cout << w1[a] << ", ";
	}
	cout << "}" << endl;

	cout << "w2 is {";
	for (int b = 0; b < result; b++)
	{
		cout << w2[b] << ", ";
	}
	cout << "}" << endl;

	cout << "separations are {";
	for (int b = 0; b < result; b++)
	{
		cout << sep[b] << ", ";
	}
	cout << "}" << endl;
    */


  /*
	const char doc[] = { "5ty6" };
	int length = strlen(doc);
	cout << "length is " << length << endl;
	for (int b = 0; b < (length); b++)
	{
		cout << doc[b];
	}
	cout << endl;
	cout << "this is it." << endl;
  */

	/*clean(doc, length);

	for (int b = 0; b < (length); b++)
	{
		cout << doc[b];
	}
	cout << endl;
	cout << "this is it." << endl;*/

	
	const int TEST1_NRULES = 4;
	char test1w1[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
		"mad",       "deranged", "nefarious", "have"
	};
	char test1w2[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
		"scientist", "robot",    "plot",      "mad"
	};
	int test1dist[TEST1_NRULES] = {
		1,           3,          0,           12
	};
  
  
	assert(rate("The mad UCLA scientist unleashed a deranged evil giant robot.",
		test1w1, test1w2, test1dist, TEST1_NRULES) == 2);

	assert(rate("The mad UCLA scientist unleashed    a deranged robot.",
		test1w1, test1w2, test1dist, TEST1_NRULES) == 2);
	assert(rate("**** 2018 ****",
		test1w1, test1w2, test1dist, TEST1_NRULES) == 0);
	assert(rate("  That plot: NEFARIOUS!",
		test1w1, test1w2, test1dist, TEST1_NRULES) == 1);
	assert(rate("deranged deranged robot deranged robot robot",
		test1w1, test1w2, test1dist, TEST1_NRULES) == 1);
	

  assert(rate("That scientist said two mad scientists suffer from deranged-robot fever.",
		test1w1, test1w2, test1dist, TEST1_NRULES) == 0);


	cout << "All tests succeeded" << endl;
}

