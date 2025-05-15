
// Generated from MiniC.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  MiniCLexer : public antlr4::Lexer {
public:
  enum {
    T_L_PAREN = 1, T_R_PAREN = 2, T_SEMICOLON = 3, T_L_BRACE = 4, T_R_BRACE = 5, 
    T_ASSIGN = 6, T_EQ = 7, T_NE = 8, T_LE = 9, T_LT = 10, T_GE = 11, T_GT = 12, 
    T_AND = 13, T_OR = 14, T_NOT = 15, T_COMMA = 16, T_ADD = 17, T_SUB = 18, 
    T_MUL = 19, T_DIV = 20, T_MOD = 21, T_RETURN = 22, T_INT = 23, T_VOID = 24, 
    T_IF = 25, T_ELSE = 26, T_WHILE = 27, T_ID = 28, T_DIGIT = 29, SL_COMMENT = 30, 
    ML_COMMENT = 31, WS = 32
  };

  explicit MiniCLexer(antlr4::CharStream *input);

  ~MiniCLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

