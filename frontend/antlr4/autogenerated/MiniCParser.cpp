
// Generated from MiniC.g4 by ANTLR 4.13.2


#include "MiniCVisitor.h"

#include "MiniCParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct MiniCParserStaticData final {
  MiniCParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  MiniCParserStaticData(const MiniCParserStaticData&) = delete;
  MiniCParserStaticData(MiniCParserStaticData&&) = delete;
  MiniCParserStaticData& operator=(const MiniCParserStaticData&) = delete;
  MiniCParserStaticData& operator=(MiniCParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag minicParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<MiniCParserStaticData> minicParserStaticData = nullptr;

void minicParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (minicParserStaticData != nullptr) {
    return;
  }
#else
  assert(minicParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<MiniCParserStaticData>(
    std::vector<std::string>{
      "compileUnit", "formalParam", "formalParamList", "funcDef", "block", 
      "blockItemList", "blockItem", "varDecl", "basicType", "varDef", "arrayDimensions", 
      "arrayParamDimensions", "statement", "expr", "lorExp", "landExp", 
      "eqExp", "eqOp", "relExp", "relOp", "addExp", "addOp", "mulExp", "mulOp", 
      "unaryExp", "primaryExp", "realParamList", "lVal"
    },
    std::vector<std::string>{
      "", "'('", "')'", "';'", "'{'", "'}'", "'['", "']'", "'='", "'=='", 
      "'!='", "'<='", "'<'", "'>='", "'>'", "'&&'", "'||'", "'!'", "','", 
      "'+'", "'-'", "'*'", "'/'", "'%'", "'return'", "'int'", "'void'", 
      "'if'", "'else'", "'while'", "'break'", "'continue'"
    },
    std::vector<std::string>{
      "", "T_L_PAREN", "T_R_PAREN", "T_SEMICOLON", "T_L_BRACE", "T_R_BRACE", 
      "T_L_BRACKET", "T_R_BRACKET", "T_ASSIGN", "T_EQ", "T_NE", "T_LE", 
      "T_LT", "T_GE", "T_GT", "T_AND", "T_OR", "T_NOT", "T_COMMA", "T_ADD", 
      "T_SUB", "T_MUL", "T_DIV", "T_MOD", "T_RETURN", "T_INT", "T_VOID", 
      "T_IF", "T_ELSE", "T_WHILE", "T_BREAK", "T_CONTINUE", "T_ID", "T_DIGIT", 
      "SL_COMMENT", "ML_COMMENT", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,36,281,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,1,0,1,
  	0,5,0,59,8,0,10,0,12,0,62,9,0,1,0,1,0,1,1,1,1,1,1,3,1,69,8,1,1,2,1,2,
  	1,2,5,2,74,8,2,10,2,12,2,77,9,2,1,3,1,3,1,3,1,3,3,3,83,8,3,1,3,1,3,1,
  	3,1,4,1,4,3,4,90,8,4,1,4,1,4,1,5,4,5,95,8,5,11,5,12,5,96,1,6,1,6,3,6,
  	101,8,6,1,7,1,7,1,7,1,7,5,7,107,8,7,10,7,12,7,110,9,7,1,7,1,7,1,8,1,8,
  	1,9,1,9,3,9,118,8,9,1,9,1,9,3,9,122,8,9,1,10,1,10,1,10,1,10,4,10,128,
  	8,10,11,10,12,10,129,1,11,1,11,3,11,134,8,11,1,11,1,11,1,11,1,11,1,11,
  	1,11,5,11,142,8,11,10,11,12,11,145,9,11,1,12,1,12,1,12,1,12,1,12,1,12,
  	1,12,1,12,1,12,1,12,1,12,3,12,158,8,12,1,12,1,12,1,12,1,12,1,12,1,12,
  	1,12,1,12,3,12,168,8,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,
  	1,12,3,12,180,8,12,1,13,1,13,1,14,1,14,1,14,5,14,187,8,14,10,14,12,14,
  	190,9,14,1,15,1,15,1,15,5,15,195,8,15,10,15,12,15,198,9,15,1,16,1,16,
  	1,16,1,16,5,16,204,8,16,10,16,12,16,207,9,16,1,17,1,17,1,18,1,18,1,18,
  	1,18,5,18,215,8,18,10,18,12,18,218,9,18,1,19,1,19,1,20,1,20,1,20,1,20,
  	5,20,226,8,20,10,20,12,20,229,9,20,1,21,1,21,1,22,1,22,1,22,1,22,5,22,
  	237,8,22,10,22,12,22,240,9,22,1,23,1,23,1,24,1,24,1,24,1,24,1,24,1,24,
  	3,24,250,8,24,1,24,3,24,253,8,24,1,25,1,25,1,25,1,25,1,25,1,25,3,25,261,
  	8,25,1,26,1,26,1,26,5,26,266,8,26,10,26,12,26,269,9,26,1,27,1,27,1,27,
  	1,27,1,27,5,27,276,8,27,10,27,12,27,279,9,27,1,27,0,0,28,0,2,4,6,8,10,
  	12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,0,6,
  	1,0,25,26,1,0,9,10,1,0,11,14,1,0,19,20,1,0,21,23,2,0,17,17,20,20,288,
  	0,60,1,0,0,0,2,65,1,0,0,0,4,70,1,0,0,0,6,78,1,0,0,0,8,87,1,0,0,0,10,94,
  	1,0,0,0,12,100,1,0,0,0,14,102,1,0,0,0,16,113,1,0,0,0,18,115,1,0,0,0,20,
  	127,1,0,0,0,22,131,1,0,0,0,24,179,1,0,0,0,26,181,1,0,0,0,28,183,1,0,0,
  	0,30,191,1,0,0,0,32,199,1,0,0,0,34,208,1,0,0,0,36,210,1,0,0,0,38,219,
  	1,0,0,0,40,221,1,0,0,0,42,230,1,0,0,0,44,232,1,0,0,0,46,241,1,0,0,0,48,
  	252,1,0,0,0,50,260,1,0,0,0,52,262,1,0,0,0,54,270,1,0,0,0,56,59,3,6,3,
  	0,57,59,3,14,7,0,58,56,1,0,0,0,58,57,1,0,0,0,59,62,1,0,0,0,60,58,1,0,
  	0,0,60,61,1,0,0,0,61,63,1,0,0,0,62,60,1,0,0,0,63,64,5,0,0,1,64,1,1,0,
  	0,0,65,66,3,16,8,0,66,68,5,32,0,0,67,69,3,22,11,0,68,67,1,0,0,0,68,69,
  	1,0,0,0,69,3,1,0,0,0,70,75,3,2,1,0,71,72,5,18,0,0,72,74,3,2,1,0,73,71,
  	1,0,0,0,74,77,1,0,0,0,75,73,1,0,0,0,75,76,1,0,0,0,76,5,1,0,0,0,77,75,
  	1,0,0,0,78,79,7,0,0,0,79,80,5,32,0,0,80,82,5,1,0,0,81,83,3,4,2,0,82,81,
  	1,0,0,0,82,83,1,0,0,0,83,84,1,0,0,0,84,85,5,2,0,0,85,86,3,8,4,0,86,7,
  	1,0,0,0,87,89,5,4,0,0,88,90,3,10,5,0,89,88,1,0,0,0,89,90,1,0,0,0,90,91,
  	1,0,0,0,91,92,5,5,0,0,92,9,1,0,0,0,93,95,3,12,6,0,94,93,1,0,0,0,95,96,
  	1,0,0,0,96,94,1,0,0,0,96,97,1,0,0,0,97,11,1,0,0,0,98,101,3,24,12,0,99,
  	101,3,14,7,0,100,98,1,0,0,0,100,99,1,0,0,0,101,13,1,0,0,0,102,103,3,16,
  	8,0,103,108,3,18,9,0,104,105,5,18,0,0,105,107,3,18,9,0,106,104,1,0,0,
  	0,107,110,1,0,0,0,108,106,1,0,0,0,108,109,1,0,0,0,109,111,1,0,0,0,110,
  	108,1,0,0,0,111,112,5,3,0,0,112,15,1,0,0,0,113,114,5,25,0,0,114,17,1,
  	0,0,0,115,117,5,32,0,0,116,118,3,20,10,0,117,116,1,0,0,0,117,118,1,0,
  	0,0,118,121,1,0,0,0,119,120,5,8,0,0,120,122,3,26,13,0,121,119,1,0,0,0,
  	121,122,1,0,0,0,122,19,1,0,0,0,123,124,5,6,0,0,124,125,3,26,13,0,125,
  	126,5,7,0,0,126,128,1,0,0,0,127,123,1,0,0,0,128,129,1,0,0,0,129,127,1,
  	0,0,0,129,130,1,0,0,0,130,21,1,0,0,0,131,133,5,6,0,0,132,134,3,26,13,
  	0,133,132,1,0,0,0,133,134,1,0,0,0,134,135,1,0,0,0,135,136,5,7,0,0,136,
  	143,1,0,0,0,137,138,5,6,0,0,138,139,3,26,13,0,139,140,5,7,0,0,140,142,
  	1,0,0,0,141,137,1,0,0,0,142,145,1,0,0,0,143,141,1,0,0,0,143,144,1,0,0,
  	0,144,23,1,0,0,0,145,143,1,0,0,0,146,147,5,24,0,0,147,148,3,26,13,0,148,
  	149,5,3,0,0,149,180,1,0,0,0,150,151,3,54,27,0,151,152,5,8,0,0,152,153,
  	3,26,13,0,153,154,5,3,0,0,154,180,1,0,0,0,155,180,3,8,4,0,156,158,3,26,
  	13,0,157,156,1,0,0,0,157,158,1,0,0,0,158,159,1,0,0,0,159,180,5,3,0,0,
  	160,161,5,27,0,0,161,162,5,1,0,0,162,163,3,26,13,0,163,164,5,2,0,0,164,
  	167,3,24,12,0,165,166,5,28,0,0,166,168,3,24,12,0,167,165,1,0,0,0,167,
  	168,1,0,0,0,168,180,1,0,0,0,169,170,5,29,0,0,170,171,5,1,0,0,171,172,
  	3,26,13,0,172,173,5,2,0,0,173,174,3,24,12,0,174,180,1,0,0,0,175,176,5,
  	30,0,0,176,180,5,3,0,0,177,178,5,31,0,0,178,180,5,3,0,0,179,146,1,0,0,
  	0,179,150,1,0,0,0,179,155,1,0,0,0,179,157,1,0,0,0,179,160,1,0,0,0,179,
  	169,1,0,0,0,179,175,1,0,0,0,179,177,1,0,0,0,180,25,1,0,0,0,181,182,3,
  	28,14,0,182,27,1,0,0,0,183,188,3,30,15,0,184,185,5,16,0,0,185,187,3,30,
  	15,0,186,184,1,0,0,0,187,190,1,0,0,0,188,186,1,0,0,0,188,189,1,0,0,0,
  	189,29,1,0,0,0,190,188,1,0,0,0,191,196,3,32,16,0,192,193,5,15,0,0,193,
  	195,3,32,16,0,194,192,1,0,0,0,195,198,1,0,0,0,196,194,1,0,0,0,196,197,
  	1,0,0,0,197,31,1,0,0,0,198,196,1,0,0,0,199,205,3,36,18,0,200,201,3,34,
  	17,0,201,202,3,36,18,0,202,204,1,0,0,0,203,200,1,0,0,0,204,207,1,0,0,
  	0,205,203,1,0,0,0,205,206,1,0,0,0,206,33,1,0,0,0,207,205,1,0,0,0,208,
  	209,7,1,0,0,209,35,1,0,0,0,210,216,3,40,20,0,211,212,3,38,19,0,212,213,
  	3,40,20,0,213,215,1,0,0,0,214,211,1,0,0,0,215,218,1,0,0,0,216,214,1,0,
  	0,0,216,217,1,0,0,0,217,37,1,0,0,0,218,216,1,0,0,0,219,220,7,2,0,0,220,
  	39,1,0,0,0,221,227,3,44,22,0,222,223,3,42,21,0,223,224,3,44,22,0,224,
  	226,1,0,0,0,225,222,1,0,0,0,226,229,1,0,0,0,227,225,1,0,0,0,227,228,1,
  	0,0,0,228,41,1,0,0,0,229,227,1,0,0,0,230,231,7,3,0,0,231,43,1,0,0,0,232,
  	238,3,48,24,0,233,234,3,46,23,0,234,235,3,48,24,0,235,237,1,0,0,0,236,
  	233,1,0,0,0,237,240,1,0,0,0,238,236,1,0,0,0,238,239,1,0,0,0,239,45,1,
  	0,0,0,240,238,1,0,0,0,241,242,7,4,0,0,242,47,1,0,0,0,243,244,7,5,0,0,
  	244,253,3,48,24,0,245,253,3,50,25,0,246,247,5,32,0,0,247,249,5,1,0,0,
  	248,250,3,52,26,0,249,248,1,0,0,0,249,250,1,0,0,0,250,251,1,0,0,0,251,
  	253,5,2,0,0,252,243,1,0,0,0,252,245,1,0,0,0,252,246,1,0,0,0,253,49,1,
  	0,0,0,254,255,5,1,0,0,255,256,3,26,13,0,256,257,5,2,0,0,257,261,1,0,0,
  	0,258,261,5,33,0,0,259,261,3,54,27,0,260,254,1,0,0,0,260,258,1,0,0,0,
  	260,259,1,0,0,0,261,51,1,0,0,0,262,267,3,26,13,0,263,264,5,18,0,0,264,
  	266,3,26,13,0,265,263,1,0,0,0,266,269,1,0,0,0,267,265,1,0,0,0,267,268,
  	1,0,0,0,268,53,1,0,0,0,269,267,1,0,0,0,270,277,5,32,0,0,271,272,5,6,0,
  	0,272,273,3,26,13,0,273,274,5,7,0,0,274,276,1,0,0,0,275,271,1,0,0,0,276,
  	279,1,0,0,0,277,275,1,0,0,0,277,278,1,0,0,0,278,55,1,0,0,0,279,277,1,
  	0,0,0,28,58,60,68,75,82,89,96,100,108,117,121,129,133,143,157,167,179,
  	188,196,205,216,227,238,249,252,260,267,277
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  minicParserStaticData = std::move(staticData);
}

}

MiniCParser::MiniCParser(TokenStream *input) : MiniCParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

MiniCParser::MiniCParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  MiniCParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *minicParserStaticData->atn, minicParserStaticData->decisionToDFA, minicParserStaticData->sharedContextCache, options);
}

MiniCParser::~MiniCParser() {
  delete _interpreter;
}

const atn::ATN& MiniCParser::getATN() const {
  return *minicParserStaticData->atn;
}

std::string MiniCParser::getGrammarFileName() const {
  return "MiniC.g4";
}

const std::vector<std::string>& MiniCParser::getRuleNames() const {
  return minicParserStaticData->ruleNames;
}

const dfa::Vocabulary& MiniCParser::getVocabulary() const {
  return minicParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView MiniCParser::getSerializedATN() const {
  return minicParserStaticData->serializedATN;
}


//----------------- CompileUnitContext ------------------------------------------------------------------

MiniCParser::CompileUnitContext::CompileUnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::CompileUnitContext::EOF() {
  return getToken(MiniCParser::EOF, 0);
}

std::vector<MiniCParser::FuncDefContext *> MiniCParser::CompileUnitContext::funcDef() {
  return getRuleContexts<MiniCParser::FuncDefContext>();
}

MiniCParser::FuncDefContext* MiniCParser::CompileUnitContext::funcDef(size_t i) {
  return getRuleContext<MiniCParser::FuncDefContext>(i);
}

std::vector<MiniCParser::VarDeclContext *> MiniCParser::CompileUnitContext::varDecl() {
  return getRuleContexts<MiniCParser::VarDeclContext>();
}

MiniCParser::VarDeclContext* MiniCParser::CompileUnitContext::varDecl(size_t i) {
  return getRuleContext<MiniCParser::VarDeclContext>(i);
}


size_t MiniCParser::CompileUnitContext::getRuleIndex() const {
  return MiniCParser::RuleCompileUnit;
}


std::any MiniCParser::CompileUnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitCompileUnit(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::CompileUnitContext* MiniCParser::compileUnit() {
  CompileUnitContext *_localctx = _tracker.createInstance<CompileUnitContext>(_ctx, getState());
  enterRule(_localctx, 0, MiniCParser::RuleCompileUnit);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(60);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_INT

    || _la == MiniCParser::T_VOID) {
      setState(58);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(56);
        funcDef();
        break;
      }

      case 2: {
        setState(57);
        varDecl();
        break;
      }

      default:
        break;
      }
      setState(62);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(63);
    match(MiniCParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormalParamContext ------------------------------------------------------------------

MiniCParser::FormalParamContext::FormalParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::BasicTypeContext* MiniCParser::FormalParamContext::basicType() {
  return getRuleContext<MiniCParser::BasicTypeContext>(0);
}

tree::TerminalNode* MiniCParser::FormalParamContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

MiniCParser::ArrayParamDimensionsContext* MiniCParser::FormalParamContext::arrayParamDimensions() {
  return getRuleContext<MiniCParser::ArrayParamDimensionsContext>(0);
}


size_t MiniCParser::FormalParamContext::getRuleIndex() const {
  return MiniCParser::RuleFormalParam;
}


std::any MiniCParser::FormalParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFormalParam(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FormalParamContext* MiniCParser::formalParam() {
  FormalParamContext *_localctx = _tracker.createInstance<FormalParamContext>(_ctx, getState());
  enterRule(_localctx, 2, MiniCParser::RuleFormalParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(65);
    basicType();
    setState(66);
    match(MiniCParser::T_ID);
    setState(68);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_L_BRACKET) {
      setState(67);
      arrayParamDimensions();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormalParamListContext ------------------------------------------------------------------

MiniCParser::FormalParamListContext::FormalParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::FormalParamContext *> MiniCParser::FormalParamListContext::formalParam() {
  return getRuleContexts<MiniCParser::FormalParamContext>();
}

MiniCParser::FormalParamContext* MiniCParser::FormalParamListContext::formalParam(size_t i) {
  return getRuleContext<MiniCParser::FormalParamContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::FormalParamListContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::FormalParamListContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::FormalParamListContext::getRuleIndex() const {
  return MiniCParser::RuleFormalParamList;
}


std::any MiniCParser::FormalParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFormalParamList(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FormalParamListContext* MiniCParser::formalParamList() {
  FormalParamListContext *_localctx = _tracker.createInstance<FormalParamListContext>(_ctx, getState());
  enterRule(_localctx, 4, MiniCParser::RuleFormalParamList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(70);
    formalParam();
    setState(75);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(71);
      match(MiniCParser::T_COMMA);
      setState(72);
      formalParam();
      setState(77);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncDefContext ------------------------------------------------------------------

MiniCParser::FuncDefContext::FuncDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::BlockContext* MiniCParser::FuncDefContext::block() {
  return getRuleContext<MiniCParser::BlockContext>(0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_INT() {
  return getToken(MiniCParser::T_INT, 0);
}

tree::TerminalNode* MiniCParser::FuncDefContext::T_VOID() {
  return getToken(MiniCParser::T_VOID, 0);
}

MiniCParser::FormalParamListContext* MiniCParser::FuncDefContext::formalParamList() {
  return getRuleContext<MiniCParser::FormalParamListContext>(0);
}


size_t MiniCParser::FuncDefContext::getRuleIndex() const {
  return MiniCParser::RuleFuncDef;
}


std::any MiniCParser::FuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitFuncDef(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::FuncDefContext* MiniCParser::funcDef() {
  FuncDefContext *_localctx = _tracker.createInstance<FuncDefContext>(_ctx, getState());
  enterRule(_localctx, 6, MiniCParser::RuleFuncDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(78);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_INT

    || _la == MiniCParser::T_VOID)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(79);
    match(MiniCParser::T_ID);
    setState(80);
    match(MiniCParser::T_L_PAREN);
    setState(82);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_INT) {
      setState(81);
      formalParamList();
    }
    setState(84);
    match(MiniCParser::T_R_PAREN);
    setState(85);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

MiniCParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::BlockContext::T_L_BRACE() {
  return getToken(MiniCParser::T_L_BRACE, 0);
}

tree::TerminalNode* MiniCParser::BlockContext::T_R_BRACE() {
  return getToken(MiniCParser::T_R_BRACE, 0);
}

MiniCParser::BlockItemListContext* MiniCParser::BlockContext::blockItemList() {
  return getRuleContext<MiniCParser::BlockItemListContext>(0);
}


size_t MiniCParser::BlockContext::getRuleIndex() const {
  return MiniCParser::RuleBlock;
}


std::any MiniCParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockContext* MiniCParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 8, MiniCParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(87);
    match(MiniCParser::T_L_BRACE);
    setState(89);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16828727322) != 0)) {
      setState(88);
      blockItemList();
    }
    setState(91);
    match(MiniCParser::T_R_BRACE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemListContext ------------------------------------------------------------------

MiniCParser::BlockItemListContext::BlockItemListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::BlockItemContext *> MiniCParser::BlockItemListContext::blockItem() {
  return getRuleContexts<MiniCParser::BlockItemContext>();
}

MiniCParser::BlockItemContext* MiniCParser::BlockItemListContext::blockItem(size_t i) {
  return getRuleContext<MiniCParser::BlockItemContext>(i);
}


size_t MiniCParser::BlockItemListContext::getRuleIndex() const {
  return MiniCParser::RuleBlockItemList;
}


std::any MiniCParser::BlockItemListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockItemList(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockItemListContext* MiniCParser::blockItemList() {
  BlockItemListContext *_localctx = _tracker.createInstance<BlockItemListContext>(_ctx, getState());
  enterRule(_localctx, 10, MiniCParser::RuleBlockItemList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(94); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(93);
      blockItem();
      setState(96); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 16828727322) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemContext ------------------------------------------------------------------

MiniCParser::BlockItemContext::BlockItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::StatementContext* MiniCParser::BlockItemContext::statement() {
  return getRuleContext<MiniCParser::StatementContext>(0);
}

MiniCParser::VarDeclContext* MiniCParser::BlockItemContext::varDecl() {
  return getRuleContext<MiniCParser::VarDeclContext>(0);
}


size_t MiniCParser::BlockItemContext::getRuleIndex() const {
  return MiniCParser::RuleBlockItem;
}


std::any MiniCParser::BlockItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockItem(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BlockItemContext* MiniCParser::blockItem() {
  BlockItemContext *_localctx = _tracker.createInstance<BlockItemContext>(_ctx, getState());
  enterRule(_localctx, 12, MiniCParser::RuleBlockItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(100);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_L_PAREN:
      case MiniCParser::T_SEMICOLON:
      case MiniCParser::T_L_BRACE:
      case MiniCParser::T_NOT:
      case MiniCParser::T_SUB:
      case MiniCParser::T_RETURN:
      case MiniCParser::T_IF:
      case MiniCParser::T_WHILE:
      case MiniCParser::T_BREAK:
      case MiniCParser::T_CONTINUE:
      case MiniCParser::T_ID:
      case MiniCParser::T_DIGIT: {
        enterOuterAlt(_localctx, 1);
        setState(98);
        statement();
        break;
      }

      case MiniCParser::T_INT: {
        enterOuterAlt(_localctx, 2);
        setState(99);
        varDecl();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

MiniCParser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::BasicTypeContext* MiniCParser::VarDeclContext::basicType() {
  return getRuleContext<MiniCParser::BasicTypeContext>(0);
}

std::vector<MiniCParser::VarDefContext *> MiniCParser::VarDeclContext::varDef() {
  return getRuleContexts<MiniCParser::VarDefContext>();
}

MiniCParser::VarDefContext* MiniCParser::VarDeclContext::varDef(size_t i) {
  return getRuleContext<MiniCParser::VarDefContext>(i);
}

tree::TerminalNode* MiniCParser::VarDeclContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::VarDeclContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::VarDeclContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::VarDeclContext::getRuleIndex() const {
  return MiniCParser::RuleVarDecl;
}


std::any MiniCParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::VarDeclContext* MiniCParser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 14, MiniCParser::RuleVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(102);
    basicType();
    setState(103);
    varDef();
    setState(108);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(104);
      match(MiniCParser::T_COMMA);
      setState(105);
      varDef();
      setState(110);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(111);
    match(MiniCParser::T_SEMICOLON);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BasicTypeContext ------------------------------------------------------------------

MiniCParser::BasicTypeContext::BasicTypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::BasicTypeContext::T_INT() {
  return getToken(MiniCParser::T_INT, 0);
}


size_t MiniCParser::BasicTypeContext::getRuleIndex() const {
  return MiniCParser::RuleBasicType;
}


std::any MiniCParser::BasicTypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBasicType(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::BasicTypeContext* MiniCParser::basicType() {
  BasicTypeContext *_localctx = _tracker.createInstance<BasicTypeContext>(_ctx, getState());
  enterRule(_localctx, 16, MiniCParser::RuleBasicType);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(113);
    match(MiniCParser::T_INT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDefContext ------------------------------------------------------------------

MiniCParser::VarDefContext::VarDefContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::VarDefContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

MiniCParser::ArrayDimensionsContext* MiniCParser::VarDefContext::arrayDimensions() {
  return getRuleContext<MiniCParser::ArrayDimensionsContext>(0);
}

tree::TerminalNode* MiniCParser::VarDefContext::T_ASSIGN() {
  return getToken(MiniCParser::T_ASSIGN, 0);
}

MiniCParser::ExprContext* MiniCParser::VarDefContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}


size_t MiniCParser::VarDefContext::getRuleIndex() const {
  return MiniCParser::RuleVarDef;
}


std::any MiniCParser::VarDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitVarDef(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::VarDefContext* MiniCParser::varDef() {
  VarDefContext *_localctx = _tracker.createInstance<VarDefContext>(_ctx, getState());
  enterRule(_localctx, 18, MiniCParser::RuleVarDef);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(115);
    match(MiniCParser::T_ID);
    setState(117);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_L_BRACKET) {
      setState(116);
      arrayDimensions();
    }
    setState(121);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == MiniCParser::T_ASSIGN) {
      setState(119);
      match(MiniCParser::T_ASSIGN);
      setState(120);
      expr();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayDimensionsContext ------------------------------------------------------------------

MiniCParser::ArrayDimensionsContext::ArrayDimensionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> MiniCParser::ArrayDimensionsContext::T_L_BRACKET() {
  return getTokens(MiniCParser::T_L_BRACKET);
}

tree::TerminalNode* MiniCParser::ArrayDimensionsContext::T_L_BRACKET(size_t i) {
  return getToken(MiniCParser::T_L_BRACKET, i);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::ArrayDimensionsContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::ArrayDimensionsContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::ArrayDimensionsContext::T_R_BRACKET() {
  return getTokens(MiniCParser::T_R_BRACKET);
}

tree::TerminalNode* MiniCParser::ArrayDimensionsContext::T_R_BRACKET(size_t i) {
  return getToken(MiniCParser::T_R_BRACKET, i);
}


size_t MiniCParser::ArrayDimensionsContext::getRuleIndex() const {
  return MiniCParser::RuleArrayDimensions;
}


std::any MiniCParser::ArrayDimensionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitArrayDimensions(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ArrayDimensionsContext* MiniCParser::arrayDimensions() {
  ArrayDimensionsContext *_localctx = _tracker.createInstance<ArrayDimensionsContext>(_ctx, getState());
  enterRule(_localctx, 20, MiniCParser::RuleArrayDimensions);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(127); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(123);
      match(MiniCParser::T_L_BRACKET);
      setState(124);
      expr();
      setState(125);
      match(MiniCParser::T_R_BRACKET);
      setState(129); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == MiniCParser::T_L_BRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayParamDimensionsContext ------------------------------------------------------------------

MiniCParser::ArrayParamDimensionsContext::ArrayParamDimensionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> MiniCParser::ArrayParamDimensionsContext::T_L_BRACKET() {
  return getTokens(MiniCParser::T_L_BRACKET);
}

tree::TerminalNode* MiniCParser::ArrayParamDimensionsContext::T_L_BRACKET(size_t i) {
  return getToken(MiniCParser::T_L_BRACKET, i);
}

std::vector<tree::TerminalNode *> MiniCParser::ArrayParamDimensionsContext::T_R_BRACKET() {
  return getTokens(MiniCParser::T_R_BRACKET);
}

tree::TerminalNode* MiniCParser::ArrayParamDimensionsContext::T_R_BRACKET(size_t i) {
  return getToken(MiniCParser::T_R_BRACKET, i);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::ArrayParamDimensionsContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::ArrayParamDimensionsContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}


size_t MiniCParser::ArrayParamDimensionsContext::getRuleIndex() const {
  return MiniCParser::RuleArrayParamDimensions;
}


std::any MiniCParser::ArrayParamDimensionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitArrayParamDimensions(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ArrayParamDimensionsContext* MiniCParser::arrayParamDimensions() {
  ArrayParamDimensionsContext *_localctx = _tracker.createInstance<ArrayParamDimensionsContext>(_ctx, getState());
  enterRule(_localctx, 22, MiniCParser::RuleArrayParamDimensions);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(131);
    match(MiniCParser::T_L_BRACKET);
    setState(133);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 12886081538) != 0)) {
      setState(132);
      expr();
    }
    setState(135);
    match(MiniCParser::T_R_BRACKET);
    setState(143);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_BRACKET) {
      setState(137);
      match(MiniCParser::T_L_BRACKET);
      setState(138);
      expr();
      setState(139);
      match(MiniCParser::T_R_BRACKET);
      setState(145);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

MiniCParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t MiniCParser::StatementContext::getRuleIndex() const {
  return MiniCParser::RuleStatement;
}

void MiniCParser::StatementContext::copyFrom(StatementContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- WhileStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::WhileStatementContext::T_WHILE() {
  return getToken(MiniCParser::T_WHILE, 0);
}

tree::TerminalNode* MiniCParser::WhileStatementContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::ExprContext* MiniCParser::WhileStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::WhileStatementContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::StatementContext* MiniCParser::WhileStatementContext::statement() {
  return getRuleContext<MiniCParser::StatementContext>(0);
}

MiniCParser::WhileStatementContext::WhileStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::WhileStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitWhileStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BlockStatementContext ------------------------------------------------------------------

MiniCParser::BlockContext* MiniCParser::BlockStatementContext::block() {
  return getRuleContext<MiniCParser::BlockContext>(0);
}

MiniCParser::BlockStatementContext::BlockStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::BlockStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBlockStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AssignStatementContext ------------------------------------------------------------------

MiniCParser::LValContext* MiniCParser::AssignStatementContext::lVal() {
  return getRuleContext<MiniCParser::LValContext>(0);
}

tree::TerminalNode* MiniCParser::AssignStatementContext::T_ASSIGN() {
  return getToken(MiniCParser::T_ASSIGN, 0);
}

MiniCParser::ExprContext* MiniCParser::AssignStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::AssignStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::AssignStatementContext::AssignStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::AssignStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAssignStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BreakStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::BreakStatementContext::T_BREAK() {
  return getToken(MiniCParser::T_BREAK, 0);
}

tree::TerminalNode* MiniCParser::BreakStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::BreakStatementContext::BreakStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::BreakStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitBreakStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpressionStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ExpressionStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ExprContext* MiniCParser::ExpressionStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

MiniCParser::ExpressionStatementContext::ExpressionStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ExpressionStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitExpressionStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ContinueStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ContinueStatementContext::T_CONTINUE() {
  return getToken(MiniCParser::T_CONTINUE, 0);
}

tree::TerminalNode* MiniCParser::ContinueStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ContinueStatementContext::ContinueStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ContinueStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitContinueStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ReturnStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::ReturnStatementContext::T_RETURN() {
  return getToken(MiniCParser::T_RETURN, 0);
}

MiniCParser::ExprContext* MiniCParser::ReturnStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::ReturnStatementContext::T_SEMICOLON() {
  return getToken(MiniCParser::T_SEMICOLON, 0);
}

MiniCParser::ReturnStatementContext::ReturnStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::ReturnStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitReturnStatement(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfStatementContext ------------------------------------------------------------------

tree::TerminalNode* MiniCParser::IfStatementContext::T_IF() {
  return getToken(MiniCParser::T_IF, 0);
}

tree::TerminalNode* MiniCParser::IfStatementContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::ExprContext* MiniCParser::IfStatementContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::IfStatementContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

std::vector<MiniCParser::StatementContext *> MiniCParser::IfStatementContext::statement() {
  return getRuleContexts<MiniCParser::StatementContext>();
}

MiniCParser::StatementContext* MiniCParser::IfStatementContext::statement(size_t i) {
  return getRuleContext<MiniCParser::StatementContext>(i);
}

tree::TerminalNode* MiniCParser::IfStatementContext::T_ELSE() {
  return getToken(MiniCParser::T_ELSE, 0);
}

MiniCParser::IfStatementContext::IfStatementContext(StatementContext *ctx) { copyFrom(ctx); }


std::any MiniCParser::IfStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitIfStatement(this);
  else
    return visitor->visitChildren(this);
}
MiniCParser::StatementContext* MiniCParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 24, MiniCParser::RuleStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(179);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<MiniCParser::ReturnStatementContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(146);
      match(MiniCParser::T_RETURN);
      setState(147);
      expr();
      setState(148);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<MiniCParser::AssignStatementContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(150);
      lVal();
      setState(151);
      match(MiniCParser::T_ASSIGN);
      setState(152);
      expr();
      setState(153);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<MiniCParser::BlockStatementContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(155);
      block();
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<MiniCParser::ExpressionStatementContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(157);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 12886081538) != 0)) {
        setState(156);
        expr();
      }
      setState(159);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<MiniCParser::IfStatementContext>(_localctx);
      enterOuterAlt(_localctx, 5);
      setState(160);
      match(MiniCParser::T_IF);
      setState(161);
      match(MiniCParser::T_L_PAREN);
      setState(162);
      expr();
      setState(163);
      match(MiniCParser::T_R_PAREN);
      setState(164);
      statement();
      setState(167);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
      case 1: {
        setState(165);
        match(MiniCParser::T_ELSE);
        setState(166);
        statement();
        break;
      }

      default:
        break;
      }
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<MiniCParser::WhileStatementContext>(_localctx);
      enterOuterAlt(_localctx, 6);
      setState(169);
      match(MiniCParser::T_WHILE);
      setState(170);
      match(MiniCParser::T_L_PAREN);
      setState(171);
      expr();
      setState(172);
      match(MiniCParser::T_R_PAREN);
      setState(173);
      statement();
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<MiniCParser::BreakStatementContext>(_localctx);
      enterOuterAlt(_localctx, 7);
      setState(175);
      match(MiniCParser::T_BREAK);
      setState(176);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<MiniCParser::ContinueStatementContext>(_localctx);
      enterOuterAlt(_localctx, 8);
      setState(177);
      match(MiniCParser::T_CONTINUE);
      setState(178);
      match(MiniCParser::T_SEMICOLON);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

MiniCParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::LorExpContext* MiniCParser::ExprContext::lorExp() {
  return getRuleContext<MiniCParser::LorExpContext>(0);
}


size_t MiniCParser::ExprContext::getRuleIndex() const {
  return MiniCParser::RuleExpr;
}


std::any MiniCParser::ExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitExpr(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::ExprContext* MiniCParser::expr() {
  ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, getState());
  enterRule(_localctx, 26, MiniCParser::RuleExpr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(181);
    lorExp();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LorExpContext ------------------------------------------------------------------

MiniCParser::LorExpContext::LorExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::LandExpContext *> MiniCParser::LorExpContext::landExp() {
  return getRuleContexts<MiniCParser::LandExpContext>();
}

MiniCParser::LandExpContext* MiniCParser::LorExpContext::landExp(size_t i) {
  return getRuleContext<MiniCParser::LandExpContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::LorExpContext::T_OR() {
  return getTokens(MiniCParser::T_OR);
}

tree::TerminalNode* MiniCParser::LorExpContext::T_OR(size_t i) {
  return getToken(MiniCParser::T_OR, i);
}


size_t MiniCParser::LorExpContext::getRuleIndex() const {
  return MiniCParser::RuleLorExp;
}


std::any MiniCParser::LorExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitLorExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::LorExpContext* MiniCParser::lorExp() {
  LorExpContext *_localctx = _tracker.createInstance<LorExpContext>(_ctx, getState());
  enterRule(_localctx, 28, MiniCParser::RuleLorExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(183);
    landExp();
    setState(188);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_OR) {
      setState(184);
      match(MiniCParser::T_OR);
      setState(185);
      landExp();
      setState(190);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LandExpContext ------------------------------------------------------------------

MiniCParser::LandExpContext::LandExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::EqExpContext *> MiniCParser::LandExpContext::eqExp() {
  return getRuleContexts<MiniCParser::EqExpContext>();
}

MiniCParser::EqExpContext* MiniCParser::LandExpContext::eqExp(size_t i) {
  return getRuleContext<MiniCParser::EqExpContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::LandExpContext::T_AND() {
  return getTokens(MiniCParser::T_AND);
}

tree::TerminalNode* MiniCParser::LandExpContext::T_AND(size_t i) {
  return getToken(MiniCParser::T_AND, i);
}


size_t MiniCParser::LandExpContext::getRuleIndex() const {
  return MiniCParser::RuleLandExp;
}


std::any MiniCParser::LandExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitLandExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::LandExpContext* MiniCParser::landExp() {
  LandExpContext *_localctx = _tracker.createInstance<LandExpContext>(_ctx, getState());
  enterRule(_localctx, 30, MiniCParser::RuleLandExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(191);
    eqExp();
    setState(196);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_AND) {
      setState(192);
      match(MiniCParser::T_AND);
      setState(193);
      eqExp();
      setState(198);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqExpContext ------------------------------------------------------------------

MiniCParser::EqExpContext::EqExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::RelExpContext *> MiniCParser::EqExpContext::relExp() {
  return getRuleContexts<MiniCParser::RelExpContext>();
}

MiniCParser::RelExpContext* MiniCParser::EqExpContext::relExp(size_t i) {
  return getRuleContext<MiniCParser::RelExpContext>(i);
}

std::vector<MiniCParser::EqOpContext *> MiniCParser::EqExpContext::eqOp() {
  return getRuleContexts<MiniCParser::EqOpContext>();
}

MiniCParser::EqOpContext* MiniCParser::EqExpContext::eqOp(size_t i) {
  return getRuleContext<MiniCParser::EqOpContext>(i);
}


size_t MiniCParser::EqExpContext::getRuleIndex() const {
  return MiniCParser::RuleEqExp;
}


std::any MiniCParser::EqExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitEqExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::EqExpContext* MiniCParser::eqExp() {
  EqExpContext *_localctx = _tracker.createInstance<EqExpContext>(_ctx, getState());
  enterRule(_localctx, 32, MiniCParser::RuleEqExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(199);
    relExp();
    setState(205);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_EQ

    || _la == MiniCParser::T_NE) {
      setState(200);
      eqOp();
      setState(201);
      relExp();
      setState(207);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqOpContext ------------------------------------------------------------------

MiniCParser::EqOpContext::EqOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::EqOpContext::T_EQ() {
  return getToken(MiniCParser::T_EQ, 0);
}

tree::TerminalNode* MiniCParser::EqOpContext::T_NE() {
  return getToken(MiniCParser::T_NE, 0);
}


size_t MiniCParser::EqOpContext::getRuleIndex() const {
  return MiniCParser::RuleEqOp;
}


std::any MiniCParser::EqOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitEqOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::EqOpContext* MiniCParser::eqOp() {
  EqOpContext *_localctx = _tracker.createInstance<EqOpContext>(_ctx, getState());
  enterRule(_localctx, 34, MiniCParser::RuleEqOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(208);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_EQ

    || _la == MiniCParser::T_NE)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelExpContext ------------------------------------------------------------------

MiniCParser::RelExpContext::RelExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::AddExpContext *> MiniCParser::RelExpContext::addExp() {
  return getRuleContexts<MiniCParser::AddExpContext>();
}

MiniCParser::AddExpContext* MiniCParser::RelExpContext::addExp(size_t i) {
  return getRuleContext<MiniCParser::AddExpContext>(i);
}

std::vector<MiniCParser::RelOpContext *> MiniCParser::RelExpContext::relOp() {
  return getRuleContexts<MiniCParser::RelOpContext>();
}

MiniCParser::RelOpContext* MiniCParser::RelExpContext::relOp(size_t i) {
  return getRuleContext<MiniCParser::RelOpContext>(i);
}


size_t MiniCParser::RelExpContext::getRuleIndex() const {
  return MiniCParser::RuleRelExp;
}


std::any MiniCParser::RelExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitRelExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::RelExpContext* MiniCParser::relExp() {
  RelExpContext *_localctx = _tracker.createInstance<RelExpContext>(_ctx, getState());
  enterRule(_localctx, 36, MiniCParser::RuleRelExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(210);
    addExp();
    setState(216);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 30720) != 0)) {
      setState(211);
      relOp();
      setState(212);
      addExp();
      setState(218);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelOpContext ------------------------------------------------------------------

MiniCParser::RelOpContext::RelOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::RelOpContext::T_LE() {
  return getToken(MiniCParser::T_LE, 0);
}

tree::TerminalNode* MiniCParser::RelOpContext::T_LT() {
  return getToken(MiniCParser::T_LT, 0);
}

tree::TerminalNode* MiniCParser::RelOpContext::T_GE() {
  return getToken(MiniCParser::T_GE, 0);
}

tree::TerminalNode* MiniCParser::RelOpContext::T_GT() {
  return getToken(MiniCParser::T_GT, 0);
}


size_t MiniCParser::RelOpContext::getRuleIndex() const {
  return MiniCParser::RuleRelOp;
}


std::any MiniCParser::RelOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitRelOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::RelOpContext* MiniCParser::relOp() {
  RelOpContext *_localctx = _tracker.createInstance<RelOpContext>(_ctx, getState());
  enterRule(_localctx, 38, MiniCParser::RuleRelOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(219);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 30720) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddExpContext ------------------------------------------------------------------

MiniCParser::AddExpContext::AddExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::MulExpContext *> MiniCParser::AddExpContext::mulExp() {
  return getRuleContexts<MiniCParser::MulExpContext>();
}

MiniCParser::MulExpContext* MiniCParser::AddExpContext::mulExp(size_t i) {
  return getRuleContext<MiniCParser::MulExpContext>(i);
}

std::vector<MiniCParser::AddOpContext *> MiniCParser::AddExpContext::addOp() {
  return getRuleContexts<MiniCParser::AddOpContext>();
}

MiniCParser::AddOpContext* MiniCParser::AddExpContext::addOp(size_t i) {
  return getRuleContext<MiniCParser::AddOpContext>(i);
}


size_t MiniCParser::AddExpContext::getRuleIndex() const {
  return MiniCParser::RuleAddExp;
}


std::any MiniCParser::AddExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAddExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::AddExpContext* MiniCParser::addExp() {
  AddExpContext *_localctx = _tracker.createInstance<AddExpContext>(_ctx, getState());
  enterRule(_localctx, 40, MiniCParser::RuleAddExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(221);
    mulExp();
    setState(227);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_ADD

    || _la == MiniCParser::T_SUB) {
      setState(222);
      addOp();
      setState(223);
      mulExp();
      setState(229);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AddOpContext ------------------------------------------------------------------

MiniCParser::AddOpContext::AddOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::AddOpContext::T_ADD() {
  return getToken(MiniCParser::T_ADD, 0);
}

tree::TerminalNode* MiniCParser::AddOpContext::T_SUB() {
  return getToken(MiniCParser::T_SUB, 0);
}


size_t MiniCParser::AddOpContext::getRuleIndex() const {
  return MiniCParser::RuleAddOp;
}


std::any MiniCParser::AddOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitAddOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::AddOpContext* MiniCParser::addOp() {
  AddOpContext *_localctx = _tracker.createInstance<AddOpContext>(_ctx, getState());
  enterRule(_localctx, 42, MiniCParser::RuleAddOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(230);
    _la = _input->LA(1);
    if (!(_la == MiniCParser::T_ADD

    || _la == MiniCParser::T_SUB)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulExpContext ------------------------------------------------------------------

MiniCParser::MulExpContext::MulExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::UnaryExpContext *> MiniCParser::MulExpContext::unaryExp() {
  return getRuleContexts<MiniCParser::UnaryExpContext>();
}

MiniCParser::UnaryExpContext* MiniCParser::MulExpContext::unaryExp(size_t i) {
  return getRuleContext<MiniCParser::UnaryExpContext>(i);
}

std::vector<MiniCParser::MulOpContext *> MiniCParser::MulExpContext::mulOp() {
  return getRuleContexts<MiniCParser::MulOpContext>();
}

MiniCParser::MulOpContext* MiniCParser::MulExpContext::mulOp(size_t i) {
  return getRuleContext<MiniCParser::MulOpContext>(i);
}


size_t MiniCParser::MulExpContext::getRuleIndex() const {
  return MiniCParser::RuleMulExp;
}


std::any MiniCParser::MulExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMulExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::MulExpContext* MiniCParser::mulExp() {
  MulExpContext *_localctx = _tracker.createInstance<MulExpContext>(_ctx, getState());
  enterRule(_localctx, 44, MiniCParser::RuleMulExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(232);
    unaryExp();
    setState(238);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 14680064) != 0)) {
      setState(233);
      mulOp();
      setState(234);
      unaryExp();
      setState(240);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MulOpContext ------------------------------------------------------------------

MiniCParser::MulOpContext::MulOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::MulOpContext::T_MUL() {
  return getToken(MiniCParser::T_MUL, 0);
}

tree::TerminalNode* MiniCParser::MulOpContext::T_DIV() {
  return getToken(MiniCParser::T_DIV, 0);
}

tree::TerminalNode* MiniCParser::MulOpContext::T_MOD() {
  return getToken(MiniCParser::T_MOD, 0);
}


size_t MiniCParser::MulOpContext::getRuleIndex() const {
  return MiniCParser::RuleMulOp;
}


std::any MiniCParser::MulOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitMulOp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::MulOpContext* MiniCParser::mulOp() {
  MulOpContext *_localctx = _tracker.createInstance<MulOpContext>(_ctx, getState());
  enterRule(_localctx, 46, MiniCParser::RuleMulOp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(241);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 14680064) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpContext ------------------------------------------------------------------

MiniCParser::UnaryExpContext::UnaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

MiniCParser::UnaryExpContext* MiniCParser::UnaryExpContext::unaryExp() {
  return getRuleContext<MiniCParser::UnaryExpContext>(0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_SUB() {
  return getToken(MiniCParser::T_SUB, 0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_NOT() {
  return getToken(MiniCParser::T_NOT, 0);
}

MiniCParser::PrimaryExpContext* MiniCParser::UnaryExpContext::primaryExp() {
  return getRuleContext<MiniCParser::PrimaryExpContext>(0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

tree::TerminalNode* MiniCParser::UnaryExpContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

MiniCParser::RealParamListContext* MiniCParser::UnaryExpContext::realParamList() {
  return getRuleContext<MiniCParser::RealParamListContext>(0);
}


size_t MiniCParser::UnaryExpContext::getRuleIndex() const {
  return MiniCParser::RuleUnaryExp;
}


std::any MiniCParser::UnaryExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitUnaryExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::UnaryExpContext* MiniCParser::unaryExp() {
  UnaryExpContext *_localctx = _tracker.createInstance<UnaryExpContext>(_ctx, getState());
  enterRule(_localctx, 48, MiniCParser::RuleUnaryExp);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(252);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(243);
      _la = _input->LA(1);
      if (!(_la == MiniCParser::T_NOT

      || _la == MiniCParser::T_SUB)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(244);
      unaryExp();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(245);
      primaryExp();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(246);
      match(MiniCParser::T_ID);
      setState(247);
      match(MiniCParser::T_L_PAREN);
      setState(249);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 12886081538) != 0)) {
        setState(248);
        realParamList();
      }
      setState(251);
      match(MiniCParser::T_R_PAREN);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpContext ------------------------------------------------------------------

MiniCParser::PrimaryExpContext::PrimaryExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::PrimaryExpContext::T_L_PAREN() {
  return getToken(MiniCParser::T_L_PAREN, 0);
}

MiniCParser::ExprContext* MiniCParser::PrimaryExpContext::expr() {
  return getRuleContext<MiniCParser::ExprContext>(0);
}

tree::TerminalNode* MiniCParser::PrimaryExpContext::T_R_PAREN() {
  return getToken(MiniCParser::T_R_PAREN, 0);
}

tree::TerminalNode* MiniCParser::PrimaryExpContext::T_DIGIT() {
  return getToken(MiniCParser::T_DIGIT, 0);
}

MiniCParser::LValContext* MiniCParser::PrimaryExpContext::lVal() {
  return getRuleContext<MiniCParser::LValContext>(0);
}


size_t MiniCParser::PrimaryExpContext::getRuleIndex() const {
  return MiniCParser::RulePrimaryExp;
}


std::any MiniCParser::PrimaryExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitPrimaryExp(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::PrimaryExpContext* MiniCParser::primaryExp() {
  PrimaryExpContext *_localctx = _tracker.createInstance<PrimaryExpContext>(_ctx, getState());
  enterRule(_localctx, 50, MiniCParser::RulePrimaryExp);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(260);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case MiniCParser::T_L_PAREN: {
        enterOuterAlt(_localctx, 1);
        setState(254);
        match(MiniCParser::T_L_PAREN);
        setState(255);
        expr();
        setState(256);
        match(MiniCParser::T_R_PAREN);
        break;
      }

      case MiniCParser::T_DIGIT: {
        enterOuterAlt(_localctx, 2);
        setState(258);
        match(MiniCParser::T_DIGIT);
        break;
      }

      case MiniCParser::T_ID: {
        enterOuterAlt(_localctx, 3);
        setState(259);
        lVal();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RealParamListContext ------------------------------------------------------------------

MiniCParser::RealParamListContext::RealParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<MiniCParser::ExprContext *> MiniCParser::RealParamListContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::RealParamListContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::RealParamListContext::T_COMMA() {
  return getTokens(MiniCParser::T_COMMA);
}

tree::TerminalNode* MiniCParser::RealParamListContext::T_COMMA(size_t i) {
  return getToken(MiniCParser::T_COMMA, i);
}


size_t MiniCParser::RealParamListContext::getRuleIndex() const {
  return MiniCParser::RuleRealParamList;
}


std::any MiniCParser::RealParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitRealParamList(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::RealParamListContext* MiniCParser::realParamList() {
  RealParamListContext *_localctx = _tracker.createInstance<RealParamListContext>(_ctx, getState());
  enterRule(_localctx, 52, MiniCParser::RuleRealParamList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(262);
    expr();
    setState(267);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_COMMA) {
      setState(263);
      match(MiniCParser::T_COMMA);
      setState(264);
      expr();
      setState(269);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LValContext ------------------------------------------------------------------

MiniCParser::LValContext::LValContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* MiniCParser::LValContext::T_ID() {
  return getToken(MiniCParser::T_ID, 0);
}

std::vector<tree::TerminalNode *> MiniCParser::LValContext::T_L_BRACKET() {
  return getTokens(MiniCParser::T_L_BRACKET);
}

tree::TerminalNode* MiniCParser::LValContext::T_L_BRACKET(size_t i) {
  return getToken(MiniCParser::T_L_BRACKET, i);
}

std::vector<MiniCParser::ExprContext *> MiniCParser::LValContext::expr() {
  return getRuleContexts<MiniCParser::ExprContext>();
}

MiniCParser::ExprContext* MiniCParser::LValContext::expr(size_t i) {
  return getRuleContext<MiniCParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> MiniCParser::LValContext::T_R_BRACKET() {
  return getTokens(MiniCParser::T_R_BRACKET);
}

tree::TerminalNode* MiniCParser::LValContext::T_R_BRACKET(size_t i) {
  return getToken(MiniCParser::T_R_BRACKET, i);
}


size_t MiniCParser::LValContext::getRuleIndex() const {
  return MiniCParser::RuleLVal;
}


std::any MiniCParser::LValContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<MiniCVisitor*>(visitor))
    return parserVisitor->visitLVal(this);
  else
    return visitor->visitChildren(this);
}

MiniCParser::LValContext* MiniCParser::lVal() {
  LValContext *_localctx = _tracker.createInstance<LValContext>(_ctx, getState());
  enterRule(_localctx, 54, MiniCParser::RuleLVal);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(270);
    match(MiniCParser::T_ID);
    setState(277);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == MiniCParser::T_L_BRACKET) {
      setState(271);
      match(MiniCParser::T_L_BRACKET);
      setState(272);
      expr();
      setState(273);
      match(MiniCParser::T_R_BRACKET);
      setState(279);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void MiniCParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  minicParserInitialize();
#else
  ::antlr4::internal::call_once(minicParserOnceFlag, minicParserInitialize);
#endif
}
