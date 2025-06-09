
// Generated from /home/code/exp/exp04-minic-expr/frontend/antlr4/MiniC.g4 by ANTLR 4.12.0

#pragma once


#include "antlr4-runtime.h"




class  MiniCLexer : public antlr4::Lexer {
public:
  enum {
    T_L_PAREN = 1, T_R_PAREN = 2, T_SEMICOLON = 3, T_L_BRACE = 4, T_R_BRACE = 5, 
    T_L_BRACKET = 6, T_R_BRACKET = 7, T_ASSIGN = 8, T_EQ = 9, T_NE = 10, 
    T_LE = 11, T_LT = 12, T_GE = 13, T_GT = 14, T_AND = 15, T_OR = 16, T_NOT = 17, 
    T_COMMA = 18, T_ADD = 19, T_SUB = 20, T_MUL = 21, T_DIV = 22, T_MOD = 23, 
    T_RETURN = 24, T_INT = 25, T_VOID = 26, T_IF = 27, T_ELSE = 28, T_WHILE = 29, 
    T_BREAK = 30, T_CONTINUE = 31, T_ID = 32, T_DIGIT = 33, SL_COMMENT = 34, 
    ML_COMMENT = 35, WS = 36
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

