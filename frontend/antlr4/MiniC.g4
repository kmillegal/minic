grammar MiniC;

// 词法规则名总是以大写字母开头

// 语法规则名总是以小写字母开头

// 每个非终结符尽量多包含闭包、正闭包或可选符等的EBNF范式描述

// 若非终结符由多个产生式组成，则建议在每个产生式的尾部追加# 名称来区分，详细可查看非终结符statement的描述

// 语法规则描述：EBNF范式

// 源文件编译单元定义
compileUnit: (funcDef | varDecl)* EOF;


// 形参列表
formalParamList: basicType T_ID (T_COMMA basicType T_ID)*;

// 函数定义，支持形参，支持返回void类型
funcDef: (T_INT|T_VOID) T_ID T_L_PAREN formalParamList? T_R_PAREN block;

// 语句块看用作函数体，这里允许多个语句，并且不含任何语句
block: T_L_BRACE blockItemList? T_R_BRACE;

// 每个ItemList可包含至少一个Item
blockItemList: blockItem+;

// 每个Item可以是一个语句，或者变量声明语句
blockItem: statement | varDecl;

// 变量声明
varDecl: basicType varDef (T_COMMA varDef)* T_SEMICOLON;

// 基本类型
basicType: T_INT;

// 变量定义,可以是一个赋值语句，或者不赋值
varDef: T_ID(T_ASSIGN expr)?;


// 目前语句支持return和赋值语句
statement:
	T_RETURN expr T_SEMICOLON			# returnStatement
	| lVal T_ASSIGN expr T_SEMICOLON	# assignStatement
	| block								# blockStatement
	| expr? T_SEMICOLON					# expressionStatement
	| T_IF T_L_PAREN expr T_R_PAREN statement (T_ELSE statement)? # ifStatement // 新增：if语句
	| T_WHILE T_L_PAREN expr T_R_PAREN statement # whileStatement // 新增：while语句
	| T_BREAK T_SEMICOLON				# breakStatement
	| T_CONTINUE T_SEMICOLON			# continueStatement;

	// 表达式文法 expr : lorExp
expr: lorExp;

// 逻辑或表达式 (二目运算符 ||) 操作数是逻辑与表达式 (landExp)，优先级低于逻辑与
lorExp: landExp (T_OR landExp)*;

// 逻辑与表达式 (二目运算符 &&) 操作数是关系表达式 (relExp)，优先级低于关系
landExp: eqExp (T_AND eqExp)*; // 操作数通常是相等性表达式

// 相等性表达式 (二目运算符 ==, !=) 操作数是关系表达式 (relExp)
eqExp: relExp (eqOp relExp)*;

// 相等性运算符
eqOp: T_EQ | T_NE;

// 关系表达式 (二目运算符 <, <=, >, >=) 操作数是加减表达式 (addExp)
relExp: addExp (relOp addExp)*;

// 关系运算符 (不含 ==, !=)
relOp: T_LE | T_LT | T_GE | T_GT;

// 加减表达式 (二目运算符 + 和 -) 操作数现在是乘法表达式 (mulExp)，优先级低于乘法、除法、求余
addExp: mulExp (addOp mulExp)*;

// 加减运算符
addOp: T_ADD | T_SUB;

// 乘法、除法、求余表达式 (二目运算符 *, /, %) 操作数是 unaryExp，优先级低于单目运算符
mulExp: unaryExp (mulOp unaryExp)*;

// 乘法、除法、求余运算符 (二目)
mulOp: T_MUL | T_DIV | T_MOD;

// 一元表达式:单目求负运算、基本表达式
unaryExp:
	(T_SUB|T_NOT) unaryExp
	| primaryExp
	| T_ID T_L_PAREN realParamList? T_R_PAREN;

// 基本表达式：括号表达式、整数、左值表达式
primaryExp: T_L_PAREN expr T_R_PAREN | T_DIGIT | lVal;

// 实参列表
realParamList: expr (T_COMMA expr)*;


// 左值表达式
lVal: T_ID;

// 用正规式来进行词法规则的描述

T_L_PAREN: '(';
T_R_PAREN: ')';
T_SEMICOLON: ';';
T_L_BRACE: '{';
T_R_BRACE: '}';

T_ASSIGN: '=';
T_EQ: '==';
T_NE: '!=';
T_LE: '<=';
T_LT: '<';
T_GE: '>=';
T_GT: '>';
T_AND: '&&';
T_OR: '||';
T_NOT: '!';
T_COMMA: ',';

T_ADD: '+';
T_SUB: '-';
T_MUL: '*';
T_DIV: '/';
T_MOD: '%';

// 要注意关键字同样也属于T_ID，因此必须放在T_ID的前面，否则会识别成T_ID
T_RETURN: 'return';
T_INT: 'int';
T_VOID: 'void';
T_IF: 'if';
T_ELSE: 'else';
T_WHILE: 'while';
T_BREAK: 'break';
T_CONTINUE: 'continue';

T_ID: [a-zA-Z_][a-zA-Z0-9_]*;
// 无符号整数定义，支持十进制、八进制和十六进制 十六进制以 0x 或 0X 开头，后跟十六进制数字 [0-9a-fA-F]+ 八进制以 0 开头，后跟零个或多个八进制数字 [0-7]*

T_DIGIT:
	'0' [xX] [0-9a-fA-F]+ // 十六进制
	| '0' [0-7]* // 八进制，0被八进制匹配
	| [1-9] [0-9]*; // 十进制

/* 注释规则：这些规则会匹配注释并跳过它们，使其不进入语法分析阶段 */
SL_COMMENT: '//' ~[\r\n]* -> skip; // 单行注释：匹配 '//' 及其后直到行尾的非换行符
ML_COMMENT:
	'/*' .*? '*/' -> skip; // 多行注释：匹配 '/*' 开始，到最近的 '*/' 结束的所有内容（非贪婪）

/* 空白符丢弃 */
WS: [ \r\n\t]+ -> skip;