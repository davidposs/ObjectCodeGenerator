/******************************************************************************
* File:		Syntax.cpp
* Authors:	David Poss, Douglas Galm
*
* Usage:	implements methods defined in Syntax.h
*
******************************************************************************/

//TODO:
// when non terminal symbols accepted
// print out token and lexeme

#include "Lexer.h"
#include "Syntax.h"
#include "Pair.h"
#include "Helpers.h"
#include "globals.h"
#include "OCGenerator.h"
#include <iostream>
#include <fstream>

int line_count = 1;

/* Writes to results file. Probably could be implemented better if time permits */
void printHelper::write(std::string message) {
	std::cout << "Printing to " << this->filename << "\n\n";
	if (!message.empty()) {
		std::fstream fileWriter;
		fileWriter.open(this->filename, std::ios::out | std::ios::app);
		fileWriter << message;
		fileWriter.close();
	}
}

void generateInstruction(std::string inst, printHelper printer) {
	static std::string filename = "object-code.txt";
	printer.filename = filename;
	return;
}


/* Prints out which line an error occurred on, if any */
void retError() {
	std::cout << "\n\n\nError on line: " << line_count << std::endl;
}

/* Gets token at the top of the list */
std::string getCurrentToken(std::list<Pair>& lexemes) {
	if (lexemes.size() == 0) return "";

	std::string token = lexemes.front().getToken();
	while (token == "\\n" || token == "\\t") {
		if (token == "\\n") line_count++;

		lexemes.pop_front();
		token = lexemes.front().getToken();
	}
	token = lexemes.front().getToken();
	std::string tokenType = lexemes.front().getType();
	//printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
	lexemes.pop_front();

	return token;
}

/* Gets the token+type pair at the top of the list */
Pair getPair(std::list<Pair> lexemes) {
	if (lexemes.size() == 0) return Pair("", "");

	std::string token = lexemes.front().getToken();
	while (token == "\\n" || token == "\\t") {
		lexemes.pop_front();
		token = lexemes.front().getToken();
	}
	token = lexemes.front().getToken();
	std::string type = lexemes.front().getType();
	lexemes.pop_front();
	return Pair(token, type);
}

/* Shows front of the lexemes list, used for debugging */
void showTop(std::string fn, std::list<Pair>& lexemes) {
	std::cout << fn << " " << lexemes.front().getToken() << std::endl;
}

bool functionA(std::list<Pair> lexemes, printHelper printer) {
	printer.write("<Rat17F>\n");
	std::string token;
	Pair temp = getPair(lexemes);
	if (printer.print) printer.write("A-> B %% CD\n");
	std::cout << "test";

	if (functionB(lexemes, printer)) {
		token = getCurrentToken(lexemes);
		if (token == "%%") {
			printer.write("Token: " + temp.getType() + "\n");
			printer.write("Lexeme: " + temp.getToken() + "\n");
			if (functionC(lexemes, printer)) {
				if (functionD(lexemes, printer)) {
					std::cout << "\n\nCorrectly parsed!\n";
				}
				else { retError(); return false; }
			}
			else { retError(); return false; }
		}
		else {
			lexemes.push_front(temp);
			retError(); return false;
		}
	}
	else { retError(); return false; }
	return true;
}

bool functionB(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("B-> E | E'\n");

	return (functionE(lexemes, printer) || functionEprime(printer));
}

bool functionE(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("E-> F | FE \n");
	
	if (functionF(lexemes, printer)) {
		if (functionE2(lexemes, printer)) return true;
		else return true;
	}
	else return false;
}

bool functionE2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("E2 -> E' | E");

	return (functionE(lexemes, printer) || functionEprime(printer));
}

bool functionF(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("F-> @ G (H) C I\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "@") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " 
			+ temp.getToken() + "\n");
		if (functionG(lexemes, printer)) {
			temp = getPair(lexemes);
			token = getCurrentToken(lexemes);
			if (token == "(") {
				printer.write("Token: " + temp.getType() + "\t Lexeme: " 
					+ temp.getToken() + "\n");
				if (functionH(lexemes, printer)) {
					temp = getPair(lexemes);
					token = getCurrentToken(lexemes);
					if (token == ")") {
						printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
						if (functionC(lexemes, printer)) {
							return functionI(lexemes, printer);
						}
					}
					else {
						lexemes.push_front(temp);
					}
				}
			}
			else {
				lexemes.push_front(temp);
			}
		}
	}
	else {
		lexemes.push_front(temp);
	}
	return false;
}

bool functionH(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("H-> J | E' \n");

	if (functionJ(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
	else return false;
}

bool functionJ(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("J-> K | K, J \n");

	if (functionK(lexemes, printer)) {
		Pair temp = getPair(lexemes);
		std::string token = getCurrentToken(lexemes);
		if (token == ",") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionJ(lexemes, printer)) { return true; }
		}
		else {
			lexemes.push_front(temp);
			return true;
		}
	}
	return false;
}

bool functionK(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("K-> L:M\n");

	Pair temp = getPair(lexemes);
	if (functionL(lexemes, printer)) {
		std::string token = getCurrentToken(lexemes);
		if (token == ":") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionM(lexemes, printer)) {
				return true;
			}
			else return false;
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
		return true;
	}
	else {
		return false;
	}
}

bool functionM(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("M-> integer | boolean | floating\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if ((token == "integer")
		|| (token == "boolean")
		|| (token == "real")) {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionI(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("I->{ D }\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "{") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionD(lexemes, printer)) {
			temp = getPair(lexemes);
			token = getCurrentToken(lexemes);
			if (token == "}") {
				printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
				return true;
			}
			else {
				lexemes.push_front(temp);
			}
		}
		else {
			return false;
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionC(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("C-> N | E\n");

	if (functionN(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
}

bool functionN(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("N-> O\n");

	if (functionO(lexemes, printer)) {
		Pair temp = getPair(lexemes);
		std::string token = getCurrentToken(lexemes);
		if (token == ";") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionN2(lexemes, printer)) {
				return true;
			}
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
	}
	return false;
}

bool functionN2(std::list<Pair>& lexmes, printHelper printer) {
	if (printer.print) printer.write("N | E'\n");

	if (functionN(lexmes, printer) || functionEprime(printer)) {
		return true;
	}
}

bool functionO(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("0-> ML\n");

	if (functionM(lexemes, printer)) {
		if (functionL(lexemes, printer)) {
			return true;
		}
	}
	else {
		return false;
	}
}

bool functionL(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("L->G | G, L\n");

	std::string token;
	if (functionG(lexemes, printer)) {
		Pair temp = getPair(lexemes);
		token = getCurrentToken(lexemes);
		if (token == ",") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionL(lexemes, printer)) { return true; }
			else {
				return false;
			}
		}
		else {
			lexemes.push_front(temp);
			return true;
		}
	}
	else {
		return false;
	}
	return true;
}

bool functionD(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("D-> P D2\n");

	if (functionP(lexemes, printer)) {
		if (functionD2(lexemes, printer)) {
			return true;
		}
		else return false;
	}
	else return false;
}

bool functionD2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("D2-> D | E\n");

	if (functionD(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
}

bool functionP(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("P-> Q | R | S | T | U | V | W\n");

	if (functionQ(lexemes, printer)) { return true; }
	if (functionR(lexemes, printer)) { return true; }
	if (functionS(lexemes, printer)) { return true; }
	if (functionT(lexemes, printer)) { return true; }
	if (functionU(lexemes, printer)) { return true; }
	if (functionV(lexemes, printer)) { return true; }
	if (functionW(lexemes, printer)) { return true; }
	return false;
}

bool functionQ(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("Q-> { D }\n");

	std::string token;
	Pair temp = getPair(lexemes);
	token = getCurrentToken(lexemes);
	if (token == "{") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionD(lexemes, printer)) {
			temp = getPair(lexemes);
			token = getCurrentToken(lexemes);
			if (token == "}") {
				printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
				return true;
			}
			else {
				lexemes.push_front(temp);
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionR(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("R->G := X\n");

	std::string token;
	Pair temp = getPair(lexemes);
	if (functionG(lexemes, printer)) {
		token = getCurrentToken(lexemes);
		if (token == ":=") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionX(lexemes, printer)) {
				temp = getPair(lexemes);
				token = getCurrentToken(lexemes);
				if (token == ";") {
					printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
					return true;
				}
				else {
					lexemes.push_front(temp);
					return false;
				}
			}
			else { return false; }
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
	}
	else return false;
}

bool functionS(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("S-> if ( Y ) P S'\n");

	std::string token;
	Pair temp = getPair(lexemes);
	token = getCurrentToken(lexemes);
	if (token == "if") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		temp = getPair(lexemes);
		token = getCurrentToken(lexemes);
		if (token == "(") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionY(lexemes, printer)) {
				temp = getPair(lexemes);
				token = getCurrentToken(lexemes);
				if (token == ")") {
					printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
					if (functionP(lexemes, printer)) {
						if (functionSprime(lexemes, printer)) {
							return true;
						}
					}
				}
				else {
					lexemes.push_front(temp);
					return false;
				}
			}
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionSprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("Sprime -> fi | else P fi\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "fi") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		return true;
	}
	else if (token == "else") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionP(lexemes, printer)) {
			temp = getPair(lexemes);
			token = getCurrentToken(lexemes);
			if (token == "fi") {
				printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
				return true;
			}
			else {
				lexemes.push_front(temp);
				return false;
			}
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
	return false;
}

bool functionT(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("T-> return T'\n");

	std::string token;
	Pair temp = getPair(lexemes);
	token = getCurrentToken(lexemes);
	if (token == "return") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionTprime(lexemes, printer)) {
			return true;
		}
		else return false;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
	return false;
}

bool functionTprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("Tprime-> ; | X;\n"); 

	Pair temp = getPair(lexemes);
	if (functionX(lexemes, printer)) {
		std::string token = getCurrentToken(lexemes);
		if (token == ";") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			return true;
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
	}
	temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == ";") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionU(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("U-> write ( X );\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "write") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		temp = getPair(lexemes);
		token = getCurrentToken(lexemes);
		if (token == "(") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionX(lexemes, printer)) {
				temp = getPair(lexemes);
				token = getCurrentToken(lexemes);
				if (token == ")") {
					printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
					temp = getPair(lexemes);
					token = getCurrentToken(lexemes);
					if (token == ";") {
						printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
						return true;
					}
					else {
						lexemes.push_front(temp);
						return false;
					}
				}
				else {
					lexemes.push_front(temp);
					return false;
				}
			}
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
	return false;
}

bool functionV(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("V-> read ( L ); \n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "read") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		temp = getPair(lexemes);
		token = getCurrentToken(lexemes);
		if (token == "(") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionL(lexemes, printer)) {
				temp = getPair(lexemes);
				token = getCurrentToken(lexemes);
				if (token == ")") {
					printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
					temp = getPair(lexemes);
					token = getCurrentToken(lexemes);
					if (token == ";") {
						printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
						return true;
					}
					else {
						lexemes.push_front(temp);
						return false;
					}
				}
				else {
					lexemes.push_front(temp);
					return false;
				}
			}
			else return false;
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
	return false;
}

bool functionW(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("W-> while ( Y ) P\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "while") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		temp = getPair(lexemes);
		token = getCurrentToken(lexemes);
		if (token == "(") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionY(lexemes, printer)) {
				temp = getPair(lexemes);
				token = getCurrentToken(lexemes);
				if (token == ")") {
					printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
					if (functionP(lexemes, printer)) {
						return true;
					}
				}
				else {
					lexemes.push_front(temp);
					return false;
				}
			}
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
	return false;
}

bool functionY(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("Y-> X Z X\n");

	if (functionX(lexemes, printer)) {
		if (functionZ(lexemes, printer)) {
			if (functionX(lexemes, printer)) { 
				return true; 
			}
			else return false; 
		}
		else return false; 
	}
	else return false; 
}

bool functionZ(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("Z-> = | /= | > | < | => | <=\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);

	if ((token == "=") || (token == "/=") || (token == ">")
		|| (token == "<") || (token == "=>") || (token == "<=")) {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionX(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("X-> A' | A'X'\n");

	if (functionAprime(lexemes, printer)) {
		if (functionX2(lexemes, printer)) {
			return true;
		}
		else return false;
	}
	else return false;
}

bool functionX2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("X2 --> X' | E'\n");

	if (functionXprime(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
}


bool functionXprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("Xprime-> +A'X' | -A'X' | E'\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "+") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionAprime(lexemes, printer)) {
			if (functionXprime(lexemes, printer)) { 
				return true; 
			}
			else return false;
		}
		else return false;
	}
	else if (token == "-") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionAprime(lexemes, printer)) {
			if (functionXprime(lexemes, printer)) { 
				return true; 
			}
			else return false;
		}
		else return false; 
	}
	else {
		lexemes.push_front(temp);
		return functionEprime(printer);
	}
}

bool functionAprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("A' -> B'A'2\n");

	if (functionBprime(lexemes, printer)) {
		if (functionAprime2(lexemes, printer)) {
			return true;
		}
		else return false;
	}
	else return false;
}

bool functionAprime2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("A'2 --> A2 | E");

	if (functionA2(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
}


bool functionA2(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("A2-> *B'A2 | /B'A2 | E'\n");

	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "*") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionBprime(lexemes, printer)) {
			if (functionA2(lexemes, printer)) { return true; }
			else { return false; }
		}
		else { return false; }
	}
	else if (token == "/") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionBprime(lexemes, printer)) {
			if (functionA2(lexemes, printer)) { return true; }
			else { return false; }
		}
		else { return false; }
	}
	else {
		lexemes.push_front(temp);
		return functionEprime(printer);
	}
}

bool functionBprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("Bprime -> -C' | C'\n");
	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "-") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionCprime(lexemes, printer)) { return true; }
		else { return false; }
	}
	else {
		lexemes.push_front(temp);
		return functionCprime(lexemes, printer);
	}
}

bool functionCprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) printer.write("Cprime-> G | D' | G[L] | (X) | F' | true | false\n");

	Pair temp;
	std::string token;
	if (functionG(lexemes, printer)) {
		temp = getPair(lexemes);
		token = getCurrentToken(lexemes);
		if (token == "[") {
			printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
			if (functionL(lexemes, printer)) {
				temp = getPair(lexemes);
				token = getCurrentToken(lexemes);
				if (token == "]") {
					printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
					return true;
				}
				else {
					lexemes.push_front(temp);
				}
			}
		}
		else {
			lexemes.push_front(temp);
		}
		return true;
	}
	if (functionDprime(lexemes, printer)) {
		return true;
	}
	if (functionFprime(lexemes, printer)) {
		return true;
	}
	temp = getPair(lexemes);
	token = getCurrentToken(lexemes);
	if (token == "(") {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		if (functionX(lexemes, printer)) {
			temp = getPair(lexemes);
			token = getCurrentToken(lexemes);
			if (token == ")") {
				printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
				return true;
			}
			else {
				lexemes.push_front(temp);
				return false;
			}
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
	if ((token == "true") || (token == "false")) {
		printer.write("Token: " + temp.getType() + "\t Lexeme: " + temp.getToken() + "\n");
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}

}

bool functionEprime(printHelper printer) {
	if (printer.print) printer.write("Eprime -> epsilon\n"); 
	return true;
}

bool functionDprime(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("Dprime -> integer\n");

	Pair temp = getPair(lexemes);
	std::string currToken = getCurrentToken(lexemes);
	std::string currType = temp.getType();
	if (currType == "integer") {
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}

bool functionFprime(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("Fprime-> real\n");
	Pair temp = getPair(lexemes);
	std::string currToken = getCurrentToken(lexemes);
	std::string currType = temp.getType();
	if (currType == "real") {
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
	return true;
}

/* Returns true if the current token is an identifier */
bool functionG(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) printer.write("Gprime-> identifier\n");
	
	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	std::string currType = temp.getType();
	if (currType == "identifier") {
		symbolTable.addEntry(temp);
		instructions.addInstruction("POPM", symbolTable.getAddress(token));
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}
