//TODO:
// when non terminal symbols accepted
// print out token and lexeme

#include "Lexer.h"
#include "Syntax.h"
#include "Pair.h"
#include "Helpers.h"
#include <iostream>

int line_count = 1;

void printHelper::write(std::string message) {
	std::fstream fileWriter;
	fileWriter.open(this->filename, std::ios::out | std::ios::app);
	fileWriter << message;
	fileWriter.close();
}

void retError(std::list<Pair>& lexemes) {
	std::cout << "\n\n\nError on line: " << line_count << std::endl;
}

/* Gets token at the top of the list */
std::string getCurrentToken(std::list<Pair>& lexemes) {
	if (lexemes.size() == 0) return "";
	std::string token = lexemes.front().getToken();
	while (token == "\\n" || token == "\\t")
	{
		if (token == "\\n") { line_count++; }
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
	while (token == "\\n" || token == "\\t")
	{
		lexemes.pop_front();
		token = lexemes.front().getToken();
	}
	token = lexemes.front().getToken();
	std::string type = lexemes.front().getType();
	lexemes.pop_front();
	return Pair(token, type);
}


void showTop(std::string fn, std::list<Pair>& lexemes) {
	std::cout << fn << " " << lexemes.front().getToken() << std::endl;
}

bool functionA(std::list<Pair> lexemes, printHelper printer) {
	printer.write("<Rat17F>\n");
	std::string token;
	Pair temp = getPair(lexemes);
	if (printer.print) {
		printer.write("A-> B %% CD\n");
	}

	if (functionB(lexemes, printer)) {
		token = getCurrentToken(lexemes);
		if (token == "%%") {
			printer.write("Token: " + temp.getType() + "\n");
			printer.write("Lexeme: " + temp.getToken() + "\n");
			if (functionC(lexemes, printer)) {
				if (functionD(lexemes, printer)) {
					std::cout << "\n\nCorrectly parsed!\n";
				}
				else { retError(lexemes); return false; }
			}
			else { retError(lexemes); return false; }
		}
		else {
			lexemes.push_front(temp);
			retError(lexemes); return false;
		}
	}
	else { retError(lexemes); return false; }
	return true;
}

//TRYING SOMETHING OUT
/*bool functionB(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("B-> E | E'\n");
	}
	if (functionE(lexemes, printer)) {
		//printer.write("Created Optional Function Definition\n\n\n");
		return true;
	}
	else if (functionEprime(printer)) {
		//printer.write("Created Optional Function Definition\n\n\n");
		return true;
	}
	return false;
}*/

bool functionB(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("B-> E | E'\n");
	}
	if (functionE(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
	return false;
}

//TRYING SOMETHING OUT
/*bool functionE(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) { printer.write("E-> F | FE \n"); }
	if (functionF(lexemes, printer)) {
		if (functionE(lexemes, printer)) {
			return true;
		}
		return true;
	}
	else {
		return false;
	}
}*/

bool functionE(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("E-> F | FE \n");
	}
	if (functionF(lexemes, printer)) {
		if (functionE2(lexemes, printer)) {
			return true;
		}
	}
	else {
		return false;
	}
}


bool functionE2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("E2 -> E' | E");
	}
	if (functionE(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
	else return false;
}


/*OLD FUNCTION
bool functionF(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("F-> @ G (H) C I\n");
	}
	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	if (token == "@") {
		if (functionG(lexemes, printer)) {
			token = getCurrentToken(lexemes);
			if (token == "(") {
				if (functionH(lexemes, printer)) {
					token = getCurrentToken(lexemes);
					if (token == ")") {
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
	// F did not follow @ G (H) C I pattern
	return false;
}*/

bool functionF(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("F-> @ G (H) C I\n");
	}
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

/*OLD FUNCTION
bool functionH(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) { printer.write("H-> J | E' \n"); }
	if (functionJ(lexemes, printer)) { return true; }
	// chanhged to return functionEprime from just calling it
	else { return functionEprime(printer); }
}
*/

bool functionH(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("H-> J | E' \n");
	}
	if (functionJ(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
	else return false;
}

bool functionJ(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) { printer.write("J-> K | K, J \n"); }
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
/*
bool functionJ(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("J -> KJ2\n");
	}
	if (functionK(lexemes, printer)) {
		if (functionJ2(lexemes, printer)) {
			return true;
		}
		else return false;
	}
	else return false;
}

bool functionJ2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("J2 -> E' | , J\n");
	}
	std::string token = getCurrentToken(lexemes);
	if (token == ",") {
		if (functionJ(lexemes, printer)) {
			return true;
		}
		return true;
	}
	return true;
}*/


bool functionK(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("K-> L:M\n");
	}
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
	if (printer.print) {
		printer.write("M-> integer | boolean | floating\n");
	}
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
	if (printer.print) {
		printer.write("I->{ D }\n");
	}
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

/*OLD FUNCTION
bool functionC(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) { printer.write("C->N | E' \n"); }
	if (functionN(lexemes, printer)) {
		//printer.write("returning from N is true " << std::endl;
		return true;
	}
	else {
		//printer.write("N did not pass in C " << std::endl;
		return functionEprime(printer);
	}
}
*/

bool functionC(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("C-> N | E\n");
	}
	if (functionN(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
}

/*OLD FUNCTION
bool functionN(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("N-> O; | O;N\n");
	}
	Pair temp = getPair(lexemes);
	if (functionO(lexemes, printer)) {
		std::string token = getCurrentToken(lexemes);
		if (token == ");") {
			if (functionN(lexemes, printer)) { return true; }
			else { return true; }
		}
		else {
			lexemes.push_front(temp);
			return false;
		}
	}
	return false;
}
*/
bool functionN(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("N-> O\n");
	}
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
	if (printer.print) {
		printer.write("N | E'\n");
	}
	if (functionN(lexmes, printer) || functionEprime(printer)) {
		return true;
	}
}

bool functionO(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("0-> ML\n");
	}
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
	if (printer.print) {
		printer.write("L->G | G, L\n");
	}
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

/*
bool functionL(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("L-> G | G, L\n");
	}
	if (functionG(lexemes, printer)) {
		if (functionL2(lexemes, printer)) {
			return true;
		}
	}
	else return true;
}


bool functionL2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("L -> G , L2\n");
	}
	std::string token = getCurrentToken(lexemes);
	Pair temp = getPair(lexemes);
	if (token == ",") {
		if (functionL(lexemes, printer)) {
			return true;
		}
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
	return true;
}*/

/*OLD FUNCTION
bool functionD(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("D-> P | P D\n");
	}
	if (functionP(lexemes, printer)) {
		//showTop("D", lexemes);
		if (functionD(lexemes, printer)) {
			return true;
		}
		return true;
	}
	else {
		return false;
	}

}*/

bool functionD(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("D-> P D2\n");
	}
	if (functionP(lexemes, printer)) {
		if (functionD2(lexemes, printer)) {
			return true;
		}
		else return false;
	}
	else return false;
}

bool functionD2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("D2-> D | E\n");
	}
	if (functionD(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
}

bool functionP(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("P-> Q | R | S | T | U | V | W\n");
	}
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
	if (printer.print) {
		printer.write("Q-> { D }\n");
	}
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
	if (printer.print) {
		printer.write("R->G := X\n");
	}
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
	if (printer.print) {
		printer.write("S-> if ( Y ) P S'\n");
	}
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
	if (printer.print) { printer.write("Sprime -> fi | else P fi\n"); }
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
	if (printer.print) {
		printer.write("T-> return T'\n");
	}
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
	if (printer.print) { printer.write("Tprime-> ; | X;\n"); }
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
	if (printer.print) { printer.write("U-> write ( X );\n"); }
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
	if (printer.print) { printer.write("V-> read ( L ); \n"); }
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
	if (printer.print) { printer.write("W-> while ( Y ) P\n"); }
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
	if (printer.print) { printer.write("Y-> X Z X\n"); }
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
	if (printer.print) { 
		printer.write("Z-> = | /= | > | < | => | <=\n"); 
	}
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
/*OLD FUNCTION
bool functionX(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) { printer.write("X-> A' | A'X'\n"); }
	if (functionAprime(lexemes, printer)) {
		if (functionXprime(lexemes, printer)) {
			return true;
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}*/

bool functionX(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) { printer.write("X-> A' | A'X'\n"); }
	if (functionAprime(lexemes, printer)) {
		if (functionX2(lexemes, printer)) {
			return true;
		}
		else return false;
	}
	else return false;
}

bool functionX2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) {
		printer.write("X2 --> X' | E'\n");
	}
	if (functionXprime(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
}


bool functionXprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) { printer.write("Xprime-> +A'X' | -A'X' | E'\n"); }
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

/*OLD FUNCTION
bool functionAprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) { printer.write("Aprime-> B' | B'A2\n"); }
	if (functionBprime(lexemes, printer)) {
		if (functionA2(lexemes, printer)) { return true; }
		else { return true; }
	}
	return false;
}*/

bool functionAprime(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) { printer.write("A' -> B'A'2\n"); }
	if (functionBprime(lexemes, printer)) {
		if (functionAprime2(lexemes, printer)) {
			return true;
		}
		else return false;
	}
	else return false;
}

bool functionAprime2(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) { printer.write("A'2 --> A2 | E"); }
	if (functionA2(lexemes, printer) || functionEprime(printer)) {
		return true;
	}
}


bool functionA2(std::list<Pair>&lexemes, printHelper printer) {
	if (printer.print) { printer.write("A2-> *B'A2 | /B'A2 | E'\n"); }
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
	if (printer.print) { printer.write("Bprime -> -C' | C'\n"); }
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
	if (printer.print) { 
		printer.write("Cprime-> G | D' | G[L] | (X) | F' | true | false\n"); 
	}
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
	if (printer.print) { 
		printer.write("Eprime -> epsilon\n"); 
	}
	return true;
}

bool functionDprime(std::list<Pair>& lexemes, printHelper printer) {
	if (printer.print) { printer.write("Dprime -> integer\n"); }

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
	if (printer.print) { printer.write("Fprime-> real\n"); }
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
	if (printer.print) { printer.write("Gprime-> identifier\n"); }
	Pair temp = getPair(lexemes);
	std::string token = getCurrentToken(lexemes);
	std::string currType = temp.getType();
	if (currType == "identifier") {
		return true;
	}
	else {
		lexemes.push_front(temp);
		return false;
	}
}