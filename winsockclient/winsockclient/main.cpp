//Shanel Elmaliach  213734890
//Introduction to computer since
//Exersice  Question\
 
#include<iostream>
#include<cstring>
#include<string>
#pragma warning (disable:4996)
using namespace std;
char* searchStr(char** lexicon, int size, char word[])
{
	for (int i = 0; i < size; i++)
	{
		if (strcmp(*(lexicon + i), word) == 0)
			return *(lexicon + i);

	}
	return NULL;
}

void newStr(char** lexicon, int& size, char neword[])
{
	if (searchStr(lexicon, size, neword) == NULL)
	{
		lexicon[size] = new char[strlen(neword) + 1];
		strcpy(lexicon[size], neword);
		int j = 0;
		for (int i = 0; i < size - 1; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				if (strcmp(*(lexicon + i), neword) > 0 && lexicon[j] != NULL)
				{
					char arr[81] = { '\0' };
					strcpy(arr, lexicon[i]);
					strcpy(lexicon[i], lexicon[j]);
					strcpy(lexicon[j], arr);

				}
			}
		}
		size++;
	}

}

void delStr(char* lexicon[], int& size, char word[])
{
	char* del = searchStr(lexicon, size, word);
	if (del != NULL)
		*del = NULL;
}

void printAll(char* lexicon[], int size)
{
	for (int t = 0; t < size; t++)
		cout << (*(lexicon + t)) << endl;
}

void printChar(char* lexicon[], int size, char tav)
{
	char** first = &lexicon[0], ** last = &lexicon[size - 1];	//'first' and 'last' represent the first tavs of the first and last word.
	for (int i = 0; i < size && first <= last; i++)	//Condition to not search past allocated memory.
	{
		while (**first == tav && first < last)	//While the first tav of a certain word in the array is the tav we're looking for.
		{
			cout << *first << " ";	//Printing word.
			first++;
		}
		first++;
	}
}


int main()
{
	enum numbers { zero = 0, one, last, three, four, five };
	char** lexicon = new char*;
	*lexicon = new char;
	int x, choice = 0, size = 0;
	char neword[81], word[81], tav;
	while (choice != 5)
	{
		cout << "Enter 0-5:" << endl;
		cin >> choice;
		switch (choice)
		{
		case zero:
			cout << "Enter the word:" << endl;
			cin >> neword;
			newStr(lexicon, size, neword);
			printAll(lexicon, size);
			break;
		case one:
			cout << "Enter the word to delete:" << endl;
			cin >> word;
			delStr(lexicon, size, word);
			printAll(lexicon, size);
			break;
		case last:
			cout << "Enter the word to search for:" << endl;
			cin >> word;
			if (searchStr(lexicon, size, word) == NULL)
				cout << "Not found" << endl;
			else
				cout << "Found" << endl;
			break;
		case three:
			cout << "Enter the char:" << endl;
			cin >> tav;
			printChar(lexicon, size, tav);
			break;
		case four:
			printAll(lexicon, size);
			break;
		default:
			cout << "ERROR" << endl;
			break;
		}
	}
	return 0;
}