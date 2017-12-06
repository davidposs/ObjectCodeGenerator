/******************************************************************************
* File:		Syntax.h
* Authors:	David Poss, Douglas Galm
*
* Usage:	defines the parsing functions needed for syntax analysis, as well
*           as a struct printHelper to help with printing out to a file
*
******************************************************************************/

#ifndef _SYNTAX_H_
#define _SYNTAX_H_
#pragma once

#include <list>
#include "Pair.h"

/* Handles whether or not the functions will print their results to a file
   as well as the file they get printed to */
struct printHelper {
    printHelper(): print(true), filename("results.txt") {}
	bool print;
	std::string filename;
    void write(std::string);
};

/* Prototypes */
void retError();
bool functionEprime(printHelper);
bool functionA(std::list<Pair>, printHelper);
bool functionB(std::list<Pair>&, printHelper);
bool functionC(std::list<Pair>&, printHelper);
bool functionD(std::list<Pair>&, printHelper);
bool functionE(std::list<Pair>&, printHelper);
bool functionF(std::list<Pair>&, printHelper);
bool functionG(std::list<Pair>&, printHelper);
bool functionH(std::list<Pair>&, printHelper);
bool functionI(std::list<Pair>&, printHelper);
bool functionJ(std::list<Pair>&, printHelper);
bool functionK(std::list<Pair>&, printHelper);
bool functionL(std::list<Pair>&, printHelper);
bool functionM(std::list<Pair>&, printHelper);
bool functionN(std::list<Pair>&, printHelper);
bool functionO(std::list<Pair>&, printHelper);
bool functionP(std::list<Pair>&, printHelper);
bool functionQ(std::list<Pair>&, printHelper);
bool functionR(std::list<Pair>&, printHelper);
bool functionS(std::list<Pair>&, printHelper);
bool functionT(std::list<Pair>&, printHelper);
bool functionU(std::list<Pair>&, printHelper);
bool functionV(std::list<Pair>&, printHelper);
bool functionW(std::list<Pair>&, printHelper);
bool functionX(std::list<Pair>&, printHelper);
bool functionY(std::list<Pair>&, printHelper);
bool functionZ(std::list<Pair>&, printHelper);
bool functionA2(std::list<Pair>&, printHelper);
bool functionD2(std::list<Pair>&, printHelper);
bool functionE2(std::list<Pair>&, printHelper);
bool functionJ2(std::list<Pair>&, printHelper);
bool functionL2(std::list<Pair>&, printHelper);
bool functionN2(std::list<Pair>&, printHelper);
bool functionX2(std::list<Pair>&, printHelper);
bool functionAprime(std::list<Pair>&, printHelper);
bool functionBprime(std::list<Pair>&, printHelper);
bool functionCprime(std::list<Pair>&, printHelper);
bool functionDprime(std::list<Pair>&, printHelper);
bool functionFprime(std::list<Pair>&, printHelper);
bool functionSprime(std::list<Pair>&, printHelper);
bool functionTprime(std::list<Pair>&, printHelper);
bool functionXprime(std::list<Pair>&, printHelper);
bool functionAprime2(std::list<Pair>&, printHelper);

#endif