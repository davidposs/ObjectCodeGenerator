/******************************************************************************
* File:     OCGenerator.h
* Authors:	David Poss, Douglas Galm
*
* Usage:	Defines symbol table, functions to translate code into assembly,
*           and functions to print output to a file
*
******************************************************************************/
#pragma once
#ifndef __OCGENERATOR_H__
#define __OCGENERATOR_H__ 
#include "Syntax.h"
#include <vector>


/* If time permits, refactor Pair so it can replace Row */
class Row {
public:
    Row(Pair, unsigned);
    unsigned getAddress();
    std::string getType();
    std::string getLexeme();
    void setAddress(unsigned);
    void setLexeme(std::string);

private:
    Pair tokenPair_;
    unsigned address_;
};

class SymbolTable {
public:
    SymbolTable();

    /* Adds adn entry to the table */
    void addEntry(Pair);

    /* Finds address of given lexeme, returns -1 if it could not be found */
    unsigned getAddress(std::string);

    /* Finds identifier by address, returns "" if not found (bad I know) */
    std::string getLexeme(unsigned);

    /* Updates address of a given symbol, most likely won't be used */
    void updateAddress(std::string, unsigned);

    /* Updates a symbol in the table */
    void updateSymbol(std::string, std::string);

    /* Searches Symbol Table for an identifier, returns false if not found. 
       Useful for checking if one has been usd without declaring it */
    bool inTable(std::string);

    /* Prints out all identifiers to a specified filename */
    void printIdentifiers(std::string);

private:
    unsigned currentAddress_;
    std::vector<Row> table_;
};

class Instructions {
public:
    Instructions();

    /* Adds an instruction to list */
    void addInstruction(std::string);

    /* Prints all instructions without number to a specified file */
    void printInstructions(std::string filename);

    /* Used to allow array-like indexing to retrieve an instruction */
    std::string operator[](unsigned index);

private:
    /* Current count of how many instructions there are. Incremented every time
       addInstruction is called */
    unsigned numInstructions_;

    /* Pair should be refactored to be more general, but it'll fo for now */
    std::vector<Pair> instructions_;
};

#endif