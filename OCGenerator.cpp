/******************************************************************************
* File:     OCGenerator.cpp
* Authors:	David Poss, Douglas Galm
*
* Usage:	implements methods defined in OCGenerator.h .. Basic stuff
*
******************************************************************************/

#include "OCGenerator.h"

/*****************************************************************************/
/******************************* ROW FUNCTIONS *******************************/
/*****************************************************************************/

/* Row constructor */
Row::Row(std::string lex, unsigned addr)
    :lexeme_(lex), address_(addr) {}

unsigned Row::getAddress() {
    return address_;
}

std::string Row::getLexeme() {
    return lexeme_;
}

void Row::setAddress(unsigned addr) {
    address_ = addr;
}

void Row::setLexeme(std::string lex) {
    lexeme_ = lex;
}

/*****************************************************************************/
/************************** Symbol Table Functions ***************************/
/*****************************************************************************/
SymbolTable::SymbolTable(): currentAddress_(10000) {}

void SymbolTable::addEntry(std::string lexeme) {
    table_.push_back(Row(lexeme, currentAddress_++));    
}

/* Finds the address of a lexeme if it is in the table, returns -1 otherwise */
unsigned SymbolTable::getAddress(std::string lexeme) {
    for (auto& it : table_) {
        if (it.getLexeme() == lexeme) {
            return it.getAddress();
        }
    }
    return -1;
}

/* Finds the lexeme at a given address, returns empty if not found */
std::string SymbolTable::getLexeme(unsigned addr) {
    for (auto& it : table_) {
        if (it.getAddress() == addr) {
            return it.getLexeme();
        }
    }
    return "";  
}

/* Search the table for a particular lexeme */
bool SymbolTable::inTable(std::string lexeme) {
    for (auto& it : table_) {
        if (it.getLexeme() == lexeme) {
            return true;
        }
    }
    return false;
}

/* Update the address of a given identifier/lexeme */
void SymbolTable::updateAddress(std::string lex, unsigned newAddr) {
    for (auto& it : table_) {
        if (it.getLexeme() == lex) {
            it.setAddress(newAddr);
        }
    }
}

/* Update the symbol at a given address (probably won't be used) */
void SymbolTable::updateSymbol(std::string lex, std::string newSymbol) {
    for (auto& it : table_) {
        if (it.getLexeme() == lex) {
            it.setLexeme(newSymbol);
        }
    }
}

/* Print all identifiers (lexemes) for a specified file */
void SymbolTable::printIdentifiers(std::string filename) {
    std::fstream output;
    output.open(filename, std::ios::out | std::ios::app);
    for (auto& it : table_) {
        output << it.getLexeme() << "\n";
    }
}


/*****************************************************************************/
/************************** Instructions Functions ***************************/
/*****************************************************************************/

Instructions::Instructions(): numInstructions_(0) {}

void Instructions::addInstruction(std::string instr) {
    instructions_.push_back(Pair(std::to_string(++numInstructions_), instr));
}

/* TODO: change Pair to be more ambiguous. Initially it was designed to hold
 * tokens and lexemes, but in this case I'm using it to hold the instruction
 * number and instruction, even though Pair's values are "Token" and "Lexeme" */
void Instructions::printInstructions(std::string filename) {
    std::fstream output;
    output.open(filename, std::ios::out | std::ios::app);
    for (auto& it : instructions_) {
        output << it.getType() << std::endl;
    }
}

/* Overload [] operator for array-like indexing of Instructions list */
std::string Instructions::operator[](unsigned index) {
    for (auto& it: instructions_) {
        if (it.getToken() == std::to_string(index)) {
            return it.getType(); // type refers to instruction
        }
    }
    return "";
}