// ===================================================================================== 
// Programmers: Alejandro Sanchez & Waymon Ho 
// Class: CPSC 323 
// FINAL PROJECT  
// Instructor: Ray Ahmadnia 
// 
// Description: given finalp1.txt:  
/* 
program a2015;
(* This program computers and prints the value
			of an expression*)
var
(* declare variables *)
	al,		ba,		c, ba12  :integer   ;
begin

		al		=3 ;
		ba=		4;
		 c		=5    ;
		 print( 'al=', al  )   ;
	print( ' value of c='   ,  c) ;
		(* compute the value of the expression *)
			ba12 = al	*  (ba + 2 *  c )	;
			print( 'bal2 is equal to ',		bal2 ) ;   (* print the value of bal2*)
			
	end. (*end of program*)		
*/ 
// This program removes comments from a text file then formats  
//and outputs the result to newdata.txt. 
/* 
NOTE*: Since data.txt was given with "nume2" as one of the delcared variables, 
this program will have a compiler error since "num2" was not declared. 
*/ 
// ===================================================================================== 
#include <iostream> 
#include <string> 
#include <ctime> 
#include <stack>
#include <sstream> 
#include <fstream> 
using namespace std; 

const int ROWS = 25;
const int COLS = 33;



void displayLocalTime(); 
void firstParseStringByChar (string subString,ostream &writeToFile, bool & firstInLiteral) ;
void secondParseStringByChar (string subString,ostream &writeToFile) ;
void classify(string s, ostream & writeFile);
bool isOperator(string s);
bool isReserved(string s);
bool isConstant(string s);
bool isSeparator(string s);
//table functions
bool testWord(string input,string table[ROWS][COLS],ostream & writeToFile);
string aquireTableEntry (stack<string> parseStack, string input,string table [ROWS][COLS],ostream & writeToFile);
bool isValidTableEntry(string state);
void handleTableEntry(stack <string> & parseStack,string state,ostream & writeToFile);
void handleTerminalFromStackTop(stack <string> &parseStack, int & index, string & token, bool & verified, string input, ostream & writeToFile);
void displayStack(stack <string> parseStack,ostream & writeToFile);
void writeRemainingOutput(int & count, string & token, string inputWord, ostream & writeToFile);


#pragma region MAIN
int main(void) 
{ 

	// DECLARATIONS 
	stringstream testStringSt; 
	string temp; // holds string tokens from input line 
	string tempLine; // holds a line from text file
	int commentLoc = 0; //stores index of comment beginning 
	int endCommentLoc = 0; // index of comment end
	bool commentFlag = false;
	ifstream inFile1("finalp1.txt"); 
	ofstream outFile1("noComment.txt"); 
	bool firstInLiteral = true;
	string inputWord = "";


	// LL PARSE TABLE
	string predictiveParseTable [ROWS][COLS] = 
			     {
	/*O - row0*/{"program I var L begin R end.", "~" , "~" , "~" , "~" , "~", "~" ,"~","~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~"},
	/*I - row1*/{"~", "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "D Z", "D Z", "D Z", "D Z", "D Z", "~"},
	/*Z - row2*/{"~" , "#" , "~" , "~" , "~", "#" ,"#", "~", "~", "~", "#", "~", "#", "#", "#", "#", "#", "G Z", "G Z", "G Z", "G Z", "G Z", "G Z", "G Z", "G Z", "G Z", "G Z", "D Z", "D Z", "D Z", "D Z", "D Z", "~"},
	/*L - row3*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "C : Y ;", "C : Y ;", "C : Y ;", "C : Y ;", "C : Y ;", "~"},
	/*C - row4*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "I J", "I J", "I J", "I J", "I J", "~"},
	/*J - row5*/{"~" , "~" , "~" , "~" , "~", "#" ,", I", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~" },
	/*Y - row6*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "integer", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~"},
	/*R - row7*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "S Q", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "S Q", "S Q", "S Q", "S Q", "S Q", "~"},
	/*Q - row8*/{"~" , "~" , "~" , "~" , "#", "~" ,"~", "~", "R", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "R", "R", "R", "R", "R", "~"},
	/*S - row9*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "P", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "A", "A", "A", "A", "A", "~"},
	/*P - row10*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "print ( B );", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~"},
	/*B - row11*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "H V", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "I V", "I V", "I V", "I V", "I V", "~"},
	/*H - row12*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "literal", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~"},
	/*V - row13*/{"~" , "~" , "~" , "~" , "~", "~" ,", B", "~", "~", "~", "#", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~"},
	/*A - row14*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "I = E ;", "I = E ;", "I = E ;", "I = E ;", "I = E ;", "~"},
	/*E - row15*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "T W", "~", "~", "~", "T W", "T W", "~", "~", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "T W", "~"},
	/*W - row16*/{"~" , "#" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "#", "~", "~", "+ T W", "- T W", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~"},
	/*T - row17*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "F U", "~", "~", "~", "F U", "F U", "~", "~", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "F U", "~"},
	/*U - row18*/{"~" , "#" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "#", "~", "~", "#", "#", "* F U", "/ F U", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~"},
	/*F - row19*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "( E )", "~", "~", "~", "N", "N", "~", "~", "N", "N", "N", "N", "N", "N", "N", "N", "N", "N", "I", "I", "I", "I", "I", "~"},
	/*N - row20*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "~", "~", "M G X", "M G X", "~", "~", "M G X", "M G X", "M G X", "M G X", "M G X", "M G X", "M G X", "M G X", "M G X", "M G X", "~", "~", "~", "~", "~", "~"},
	/*X - row21*/{"~" , "#" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "#", "~", "~", "#", "#", "#", "#", "G X", "G X", "G X", "G X", "G X", "G X", "G X", "G X", "G X", "G X", "~", "~", "~", "~", "~", "~"},
	/*M - row22*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "~", "~", "+", "-", "~", "~", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "~", "~", "~", "~", "~", "~"},
	/*G - row23*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "~", "~", "~", "~", "~", "~"},
	/*D - row24*/{"~" , "~" , "~" , "~" , "~", "~" ,"~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "~", "a", "b", "c", "d", "e", "~"},
				};


	// ===================================================
	displayLocalTime(); 

	while (getline(inFile1, tempLine)) //grab each line from text file
	{ 
		testStringSt.clear(); 
		testStringSt.str(tempLine); 
		//if there is a new line character 
		if (tempLine.empty()) 
			testStringSt << temp;  
		else 
		{ 
			while (testStringSt >> temp)
				firstParseStringByChar(temp,outFile1,firstInLiteral);
		}

	}
	cout << endl; 
	inFile1.close();
	outFile1.close();

	ifstream inFile2("noComment.txt");
	ofstream outFile2("finalp2.txt");
	
	while (getline(inFile2, tempLine)) //grab each line from text file
	{ 
		testStringSt.clear(); 
		testStringSt.str(tempLine); 
		//if there is a new line character 
		if (tempLine.empty()) 
			testStringSt << temp;  
		else if ((tempLine.length() > 1) && ((tempLine[0] == '(') && (tempLine[1] == '*')))
			testStringSt << temp;
		else if ((tempLine.length() > 1) && (tempLine[0] == '\''))
		{
			testStringSt << temp;
			outFile2 << "\nliteral\n";
		}
		else 
		{ 
			while (testStringSt >> temp)
				secondParseStringByChar(temp,outFile2);
		}

	}
	inFile2.close();
	outFile2.close();

	ofstream outFile3("testword.txt");
	ifstream inFile3("finalp2.txt");
	tempLine = "";
	while (getline(inFile3, tempLine)) //grab each line from text file
	{ 
		testStringSt.clear(); 
		testStringSt.str(tempLine); 
		while(!testStringSt.eof())
		{
			string word;
			testStringSt >> word;
			classify(word,outFile3);
			cout << endl;
		}
	 
	}
	
	inFile3.close();
	outFile3.close(); 


	ifstream inFile4("testword.txt");
	ofstream outFile4("Test_Output.txt");

	getline(inFile4,inputWord);
	
	if (testWord(inputWord,predictiveParseTable,outFile4))
	{
		outFile4 << "\n\nProgram is accepted ==============================";

	}
	else
		outFile4 << "\n\n\Program not accepted =============================";

	return 0; 
} // end of main() 
#pragma endregion 
// _______________________________ 
// 
// FUNCTIONS 
//________________________________ 

# pragma region Clean Up Text File

// ======= firstParseStringByChar() ======================================================== 
// Prevalue: substring containing characters to parse 
// 
// Postvalue: parsed tokens are output to newdata.txt 
// 
// Description: This function increments through each index of the substring passed  
//to it. It will identify operators and separate them 
//from potential identifiers with a space. It then writes formatted data to  
//newdata.txt character by charater. 
//===================================================================================== 
void firstParseStringByChar (string subString,ostream &writeToFile, bool & firstInLiteral) 
{
	unsigned int count = 0; 
	
	// go through each token string and look for operators, semicolons and comments. 
	while (count < subString.length()) 
	{ 
		// handle arithmetic operators and commas 
		
		if (subString[count] == ';' ) // handle semicolons 
		{ 
			writeToFile << subString[count] << endl ; //append new line 
		} 
		
		else if (subString[count] == '\'')
		{
			if (firstInLiteral)
			{
				writeToFile << endl << subString[count];
				firstInLiteral = false;
			}
			else
			{
				writeToFile << subString[count] << endl;
				firstInLiteral = true;
			}
			//append new line
		}
		else if ((count == 0) &&((subString[count] == '(')&& (subString[count+1] == '*')))
		{
			writeToFile << endl << subString[count] << subString[count+1] << " " ; //append new line
			count += 2;
		}
		else if ((count == (subString.length()-2)) &&((subString[count] == '*')&& (subString[count+1] == ')')))
		{
			writeToFile << subString[count] << subString[count+1] <<  endl ; //append new line
			count += 2;
		}
		
		
		 else if ((subString.length() == 1) &&( (subString[count] == '+' )|| (subString[count]== '-' )||(subString[count] == '*' ) 
			|| (subString[count] == '/' ) || (subString[count] =='=')||(subString[count] == ',') ) )
		{ 
			writeToFile << subString[count] << " " ; 
		} 
	 
		//output character in substring[count]. Append a space if it is the last character 
		// in the substring or if it is followed by a non alphanumeric character. 
		else 
		{ 
			// append a space if current index is last character in substring or next character is not alphanumeric 
			if ((count == subString.length()-1) || (!isalnum(subString[count+1])))  
				writeToFile << subString[count] << " " ; 
			else 
				writeToFile << subString[count]; // output character of current index in substring 
		} 
		count ++; 
	} 
}//end of parseStringByChar() 


// ======= secondParseStringByChar() ======================================================== 
// Prevalue: substring containing characters to parse 
// 
// Postvalue: parsed tokens are output to newdata.txt 
// 
// Description: This function increments through each index of the substring passed  
//to it. It will identify operators and separate them 
//from potential identifiers with a space. It then writes formatted data to  
//newdata.txt character by charater. 
//===================================================================================== 
void secondParseStringByChar (string subString,ostream &writeToFile) 
{

	unsigned int count = 0; 
	// go through each token string and look for operators, semicolons and comments. 
	while (count < subString.length()) 
	{ 
		// handle arithmetic operators and commas 
		
		if (subString[count] == ';' ) // handle semicolons 
		{ 
			writeToFile << subString[count] << endl ; //append new line 

		} 
		
		else if ((subString[count] == '+' )|| (subString[count]== '-' )||(subString[count] == '*' ) 
			|| (subString[count] == '/' ) || (subString[count] == ':' ) || (subString[count] =='=')||(subString[count] == ',') || (subString[count] == '(') || (subString[count] == '\'')) 
		{ 
			writeToFile << subString[count] << " " ; 
		} 
		else if ((subString.length() == 4) &&((subString[count] == 'e') && (subString[count+1] == 'n') && (subString[count+2] == 'd') && (subString[count+3] == '.')))
		{
			writeToFile << subString[count] << subString[count+1] << subString[count +2] << subString[count+3] ; 
			count += 4;
		}
		
		else if ((subString.length() == 3)&&(subString[count] == 'v') && (subString[count+1] == 'a') && (subString[count+2] == 'r'))
		{
			writeToFile << subString[count] << subString[count+1] << subString[count +2] << " "; 
			count += 3;
		}
	
		else if ((subString.length() == 5)&&(subString[count] == 'b') && (subString[count+1] == 'e') && (subString[count+2] == 'g') && (subString[count+3] == 'i') && (subString[count+4] == 'n'))
		{
			writeToFile << subString[count] << subString[count+1] << subString[count +2] << subString[count+3] << subString[count+4] << " "; 
			count += 5;
		}
		
		
		else if ((subString.length() == 5)&&(subString[count] == 'p') && (subString[count+1] == 'r') && (subString[count+2] == 'i') && (subString[count+3] == 'n') && (subString[count+4] == 't'))
		{
			writeToFile << subString[count] << subString[count+1] << subString[count +2] << subString[count+3] << subString[count+4] << " "; 
			count += 5;
		}
	
	
		else if ((subString.length() == 7)&&(subString[count] == 'i') && (subString[count+1] == 'n') && (subString[count+2] == 't') && (subString[count+3] == 'e') && (subString[count+4] == 'g') && (subString[count+5] == 'e') && (subString[count+6] == 'r'))
		{
			writeToFile << subString[count] << subString[count+1] << subString[count +2] << subString[count+3] << subString[count+4] << subString[count+5] << subString[count +6] << " " ; 
			count += 7;
		}

		else if ((subString.length() == 7)&&(subString[count] == 'p') && (subString[count+1] == 'r') && (subString[count+2] == 'o') && (subString[count+3] == 'g') && (subString[count+4] == 'r') && (subString[count+5] == 'a') && (subString[count+6] == 'm'))
		{
			writeToFile << subString[count] << subString[count+1] << subString[count +2] << subString[count+3] << subString[count+4] << subString[count+5] << subString[count +6] <<  " " ; 
			count += 7;
		}
		//output character in substring[count]. Append a space if it is the last character 
		// in the substring or if it is followed by a non alphanumeric character. 
		else 
		{ 
			// append a space if current index is last character in substring or next character is not alphanumeric 
			if ((count == subString.length()-1) || (!isalnum(subString[count+1])))  
				writeToFile << subString[count] << " " ; 
			else 
				writeToFile << subString[count]; // output character of current index in substring 
		} 
		count ++; 
	} 
}//end of secondParseStringByChar() 
# pragma endregion

#pragma region Francis_Classifier

bool isOperator(string s)
{
	return s == "=" || s== "+" || s== "-" || s=="*" || s =="/";
}

bool isReserved(string s)
{
	return s == "end." || s== "begin" || s== "integer" || s=="print" || s =="program"|| s =="var"|| s =="literal";
}
bool isConstant(string s)
{
	int c = 0;
	for (int i = 0; i < s.length();++i)
	{
		if (isdigit(s[i]) || s[i] == '.')
			c++; //pun not intended
	}
	return c==s.length();
}
bool isSeparator(string s)
{
	return s == "(" || s== ")" || s== "{" || s=="}" || s ==","|| s== "," || s==";" || s ==":";
}
void classify(string s, ostream & writeFile)
{
	if (isOperator(s)) 
	{
			cout << s << "\toperator";
			writeFile << s << " ";
	}
	else if (isReserved(s)) 
	{
			cout << s << "\treserved word";
			writeFile << s << " ";
	}
	else if (isConstant(s)) 
	{
			cout << s << "\tconstant";
			writeFile << s<< " ";
	}
	else if (isSeparator(s)) 
	{
			cout << s << "\tseparator";
			writeFile << s << " ";
	}
	else // is identifier
	{
			cout << s << "\tidentifier";
			for (int i = 0; i < s.length(); i++)
			{
				writeFile << s[i] << " ";
			}
			
	}
}

#pragma endregion

#pragma region TEST WORD()
// ======= testWord() =================================================
// pre-value: string representing word to test, table representing FA.
// post-value: bool value representing if word is in language or not.
//
// description: This function accepts a string and a 2D int array
// representing the predictice parsing table generated for a given language.
// This function will return true or false depending on if
// the word is accepted by the grammar or not.
//====================================================================
bool testWord(string inputString,string table[ROWS][COLS],ostream & writeToFile)
{
	stringstream wordStream;
	bool verified = false; // word is verified or not
	stack <string> parseStack; // stack to push table entries
	int stringCount = 0;
	inputString += "$"; // concatenate '$' to word for testing
	string state = ""; // table entry result
	string token= "";
	int looper = 0;
	// Push end condition and first non-terminal

	string temp;
	wordStream.clear();
	wordStream.str(inputString);
	wordStream >> token;

	while ((stringCount < inputString.length()) ) // separate input into testable tokens
	{

		if (looper == 0)
		{
			parseStack.push("$");
			parseStack.push("O");

			writeToFile << "\nPush: $  Push: O\n";
			displayStack(parseStack,writeToFile);

			writeToFile << "\n\nNow Reading: " << token << endl;
	
			state =  aquireTableEntry(parseStack, token, table,writeToFile);
			/*stringCount += token.length();

			for (int i = stringCount; i< inputString.length();i++)
			{
				temp += inputString[i];
			}
	
			wordStream.clear();
			wordStream.str(temp);
	
			writeToFile <<"Remaining Input: " << wordStream.str() << endl;
			*/
		}
		else
			writeToFile << "\t\tcurrent Token before aquire is: " << token ;
			state =  aquireTableEntry(parseStack, token, table,writeToFile); 

		// Check table entry for lambda, non-terminal, or empty cell (~).
		if( !isValidTableEntry(state))
		{
			writeToFile << "\nEmpty table entry detected, invalid word. . .\n";
			//exit loop
			exit(1);
		}
		else // pop non-terminal from stack
		{
			handleTableEntry(parseStack,state,writeToFile);
			displayStack(parseStack,writeToFile);
			//check stack for terminal character and handle
			handleTerminalFromStackTop(parseStack, stringCount, token,verified,inputString,writeToFile);
			if ((parseStack.top() == token) && (parseStack.top() ==  "program"|| ";" ||"var" || "begin" || "end." ||":" || "," || "integer" ||
									"print" || "(" || ")" || "literal" || "=" || "+" ||  "-" || "*" || "/"||
									"0" ||"1" || "2" ||"3" ||"4" || "5" ||"6" ||"7" || "8" ||  "9" || "a"||
									 "b" || "c" || "d" || "e" ||"$"))
			{
				while ((parseStack.top() == token) && (parseStack.top() ==  "program"|| ";" ||"var" || "begin" || "end." ||":" || "," || "integer" ||
									"print" || "(" || ")" || "literal" || "=" || "+" ||  "-" || "*" || "/"||
									"0" ||"1" || "2" ||"3" ||"4" || "5" ||"6" ||"7" || "8" ||  "9" || "a"||
									 "b" || "c" || "d" || "e" ||"$"))
				{
					handleTerminalFromStackTop(parseStack, stringCount, token,verified,inputString,writeToFile);
				}
			}
		}
		looper++;
	}// end of while loop

	


	return verified;
} // end of bool testWord()
//====================================================================
#pragma endregion

#pragma region WriteRemainingOutput()
void writeRemainingOutput(int & count, string & token, string inputWord, ostream & writeToFile)
{
	string temp;
	stringstream tokenStream;
	writeToFile <<"Remaining Input: " ;
		
	count += token.length() +1;



	if (count  == inputWord.length()+1)
	{
		writeToFile << "NONE";
	}
	else
	{
		
		for (int i = count; i < inputWord.length();i++)
		{
			temp += inputWord[i];
		}
		writeToFile << temp;
	}
		writeToFile << endl;


		tokenStream.clear();
		tokenStream.str(temp);
		tokenStream >> token;
		writeToFile << "\n\nNow Reading: " << token << endl;
	//obtain next token
		/*stringCount += token.length();

			for (int i = stringCount; i< inputString.length();i++)
			{
				temp += inputString[i];
			}
	
			wordStream.clear();
			wordStream.str(temp);
	
			writeToFile <<"Remaining Input: " << wordStream.str() << endl;
			*/
}

#pragma endregion

#pragma region aquireTableEntry()
// ======= aquireTableEntry() ========================================
// Prevalue: 
//
// Postvalue: 
//
// Description: 
//====================================================================


string aquireTableEntry (stack<string> parseStack, string input, string table [ROWS][COLS], ostream & writeToFile)
{
	unsigned int row = NULL;
	unsigned int col = NULL; 
	string rowEntry = "";
	string colEntry = "";

	writeToFile << "\n\t\tTop of stack before aquire is: " << parseStack.top() << endl;

	if (input == "program")   {col = 0; colEntry = "program";}
	else if (input == ";")    {col = 1; colEntry = ";"; }
	else if (input == "var")  {col = 2; colEntry = "var";}
	else if (input == "begin"){ col = 3; colEntry = "begin";}
	else if (input == "end.") { col = 4; colEntry = "end.";}
	else if (input == ":")    {	col = 5; colEntry = ":"; }
	else if (input == ",")	  {col = 6; colEntry = ",";}
	else if (input == "integer"){col = 7; colEntry = "integer";}
	else if (input == "print"){col = 8; colEntry = "print";}
	else if (input == "("){col = 9; colEntry = "(";}
	else if (input == ")"){col = 10; colEntry = ")";}
	else if (input == "literal"){	col = 11; colEntry = "literal";}
	else if (input == "="){col = 12; colEntry = "=";}
	else if (input == "+"){	col = 13; colEntry = "+";}
	else if (input == "-"){col = 14; colEntry = "-";}
	else if (input == "*")	{col = 15; colEntry = "*";}
	else if (input == "/"){col = 16; colEntry = "/";}
	else if (input == "0"){col = 17; colEntry = "0";}
	else if (input == "1")	{col = 18; colEntry = "1";}
	else if (input == "2"){	col = 19; colEntry = "2";}
	else if (input == "3"){col = 20; colEntry = "3";}
	else if (input == "4"){col = 21; colEntry = "4";}
	else if (input == "5"){col = 22; colEntry = "5";}
	else if (input == "6"){col = 23; colEntry = "6";}
	else if (input == "7"){col = 24; colEntry = "7";}
	else if (input == "8"){col = 25; colEntry = "8";}
	else if (input == "9"){col = 26; colEntry = "9";}
	else if (input == "a"){col = 27; colEntry = "a";}
	else if (input == "b")	{col =  28 ; colEntry = "b";}
	else if (input == "c"){col = 29; colEntry = "c";}
	else if (input == "d")	{col = 30; colEntry = "d";}
	else if (input == "e"){col = 31; colEntry = "e";}
	else if (input == "$"){		col = 32; colEntry = "$";}


	if(parseStack.top() == "O" )	{row = 0; rowEntry = "O";}
	else if (parseStack.top() == "I" ){row = 1; rowEntry = "I";}
	else if (parseStack.top() == "Z" ){row = 2; rowEntry = "Z";}
	else if (parseStack.top() == "L" ){row = 3; rowEntry = "L";}
	else if (parseStack.top() == "C" ){	row = 4; rowEntry = "C";}
	else if (parseStack.top() == "J" ){row = 5; rowEntry = "J";}
	else if (parseStack.top() == "Y" ){row = 6; rowEntry = "Y";}
	else if (parseStack.top() == "R" ){row = 7; rowEntry = "R";}
	else if (parseStack.top() == "Q" ){row = 8; rowEntry = "Q";}
	else if (parseStack.top() == "S" ){row = 9; rowEntry = "S";}
	else if (parseStack.top() == "P" ){row = 10; rowEntry = "P";}
	else if (parseStack.top() == "B" ){row = 11; rowEntry = "B";}
	else if (parseStack.top() == "H" ){row = 12; rowEntry = "H";}
	else if (parseStack.top() == "V" ){row = 13; rowEntry = "V";}
	else if (parseStack.top() == "A" ){row = 14; rowEntry = "A";}
	else if (parseStack.top() == "E" ){row = 15; rowEntry = "E";}
	else if (parseStack.top() == "W" ){row = 16; rowEntry = "W";}
	else if (parseStack.top() == "T" ){row = 17; rowEntry = "T";}
	else if (parseStack.top() == "U" ){row = 18; rowEntry = "U";}
	else if (parseStack.top() == "F" ){row = 19; rowEntry = "F";}
	else if (parseStack.top() == "N" ){row = 20; rowEntry = "N";}
	else if (parseStack.top() == "X" ){row = 21; rowEntry = "X";}
	else if (parseStack.top() == "M" ){row = 22; rowEntry = "M";}
	else if (parseStack.top() == "G" ){row = 23; rowEntry = "G";}
	else if (parseStack.top() == "D" ){row = 24; rowEntry = "D";}


	writeToFile << "Pop: " << parseStack.top() << endl;
	writeToFile << "Table entry [" << rowEntry  << "," << colEntry << "] = "
		 << table[row][col] << endl;
	return table[row][col];
}// end of aquireTableEntry()
//====================================================================


#pragma endregion

#pragma region isValidEntry
// ======= isValidTableEntry() =============================
// Prevalue: string containing current table entry
//
// Postvalue: boolean value depending on if table entry is non-empty. 
//
// Description: 
//========================================================
bool isValidTableEntry(string state)
{
	if (state == "~")
		return false;
	else return true;
}// end of isValidTableEntry()
//====================================================================

#pragma endregion

#pragma region handleTableEntry()
// ======= handleTableEntry() ========================================================
// Prevalue: char stack object and a string containing the current table entry
//
// Postvalue: 
//
// Description: Handles non-Terminals
//=====================================================================================
void handleTableEntry(stack <string> & parseStack, string state,ostream & writeToFile)
{
	stringstream tableStream;
	tableStream.clear();
	tableStream.str(state);
	string streamState;
	int count=0;
	stack<string> tempStack;

	if ((state == "#") ) // table entry is lambda
	{
		writeToFile << "lambda detected, popping stack. . .\n";
		parseStack.pop();
		writeToFile<< "\nNew top of stack is: " << parseStack.top() << endl;
		
	}
	else // table entry is not lambda
	{
		while(!tempStack.empty())
			tempStack.pop();
		parseStack.pop();
		//push onto stack in reverse order
		while (tableStream>> streamState)
		{
			tempStack.push(streamState);
		}
		writeToFile << "PUSH: ";
		for (stack <string> temp = tempStack; !temp.empty(); temp.pop())
		{
			parseStack.push(temp.top());
			writeToFile << parseStack.top() << " ";
		}
		writeToFile << endl;
	}
}// end of handleTableEntry()
//====================================================================
#pragma endregion

#pragma region handleTerminalFromStack()

// ======= handleTerminalFromStackTop() =========================
// Prevalue: 
//
// Postvalue: 
//
// Description: 
//================================================================
void handleTerminalFromStackTop(stack <string> &parseStack, int & index, string & token, bool & verified, string input, ostream & writeToFile)
{
	// terminal character check
	if ((!parseStack.empty()) && (token == parseStack.top()) && (
		(parseStack.top() == "program") || (parseStack.top() == ";") ||
		(parseStack.top() == "var")		|| (parseStack.top() == "begin" )||
		(parseStack.top() == "integer") || (parseStack.top() == "end.") ||
		(parseStack.top() == ":")		|| (parseStack.top() == ",") || 
		(parseStack.top() == "print" )  || (parseStack.top() == "(" ) ||
		(parseStack.top() == ")" )		|| (parseStack.top() == "literal" ) ||
		(parseStack.top() == "=" )		|| (parseStack.top() == "+" ) ||
		(parseStack.top() == "-" )		|| (parseStack.top() == "*" ) ||
		(parseStack.top() == "/" )		|| (parseStack.top() == "0" ) ||
		(parseStack.top() == "1" )		|| (parseStack.top() == "2" ) ||
		(parseStack.top() == "3" )		|| (parseStack.top() == "4" ) ||
		(parseStack.top() == "5" )		|| (parseStack.top() == "6" ) ||
		(parseStack.top() == "7" )		|| (parseStack.top() == "8" ) ||
		(parseStack.top() == "9" )		|| (parseStack.top() == "a" ) ||
		(parseStack.top() == "b" )		|| (parseStack.top() == "c" ) ||
		(parseStack.top() == "d" )		|| (parseStack.top() == "e" ) ||
		(parseStack.top() == "$" ) ))
	{
		writeToFile<< "Input: " << token << " matches top of stack: " 
			<< parseStack.top() << ", popping and discarding . . .\n"; 
		parseStack.pop();
		displayStack(parseStack,writeToFile);
		writeRemainingOutput(index,token,input,writeToFile);

		
	} // end of terminal character check

	//end condition check
	if (parseStack.top() == "$") 
	{
		writeToFile << "\nEnd character detected, checking end condition. . .\n";
		if (index == (input.length() -1))
		{
			writeToFile<<input[index] << " is last character in input string. \n"
				<< "Input: " << token<< " matches top of stack: " 
				<< parseStack.top() << endl; 
			writeToFile<<  "End condition met!\n\n"; 
			writeToFile  << "Pop: " << parseStack.top() << endl;
			verified = true;
			parseStack.pop();
			displayStack(parseStack,writeToFile);
		}
		else
			writeToFile<< "\nInput: " << input [index] 
				<< " does not match top of stack: " 
				<< parseStack.top() << "\nInvalid word. . .\n";
	}// end of end condtion check
}//end of handleTerminalFromStackTop()
//===================================================================
#pragma endregion

#pragma region displayStack()
// ======= displayStack() ============================================================
// Prevalue: current state of string stack
//
// Postvalue: Contents of stack output to screen
//
// Description: Displays contents of stack passed as argument
//=====================================================================================
void displayStack(stack <string> myStack,ostream & writeToFile)
{
	writeToFile << "  Top of stack <-- ";
	if (!myStack.empty())
	{
		for (stack <string> temp = myStack; !temp.empty(); temp.pop())
			writeToFile << temp.top() <<  " " ;
		writeToFile << endl;
	}
	else
		writeToFile << "*Empty*\n";
}// end of displayStack

#pragma endregion 

#pragma region displayLocalTime()
// ======= displayLocalTime() ======================================================== 
// Prevalue: none 
// 
// Postvalue: displays local time 
// 
// Description: Uses strftime function contained in ctime library to display the local time 
//===================================================================================== 
void displayLocalTime() 
{ 
	const int BUFFSIZE=200; // buffer for tmdesc[] which is used to format the time 

	// set up to use strftime() 
	time_t myTime = time(0); //obtain the current time_t value 
	tm now= *localtime(& myTime); //convert to tm, a struct defined in ctime library 
	char tmdesc[BUFFSIZE] ={0}; //ensures finite characters for displaying time output 
	// output statement for tm struct 
	// strftime() token reference: http://en.cppreference.com/w/cpp/chrono/c/strftime 
	const char fmt[] = "Current local time and date:\n%A, %B %d %Y \n%I:%M:%S %p %Z"; 

	// Display today's date and time 
	if (strftime(tmdesc, sizeof(tmdesc)-1, fmt, &now) >0) 
		cout << tmdesc << endl <<endl ; 
	else 
		cerr<<"strftime failed. Errno code: " <<errno  << endl; 
}// end of displayLocalTime() 

#pragma endregion
//===================================================================================== 

