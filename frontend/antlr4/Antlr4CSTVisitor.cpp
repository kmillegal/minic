///
/// @file Antlr4CSTVisitor.cpp
/// @brief Antlr4的具体语法树的遍历产生AST
/// @author zenglj (zenglj@live.com)
/// @version 1.1
/// @date 2024-11-23
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// <tr><td>2024-11-23 <td>1.1     <td>zenglj  <td>表达式版增强
/// </table>
///

#include <any>
#include <string>

#include "Antlr4CSTVisitor.h"
#include "AST.h"
#include "AttrType.h"
#include "MiniCParser.h"

#define Instanceof(res, type, var) auto res = dynamic_cast<type>(var)

/// @brief 构造函数
MiniCCSTVisitor::MiniCCSTVisitor()
{}

/// @brief 析构函数
MiniCCSTVisitor::~MiniCCSTVisitor()
{}

/// @brief 遍历CST产生AST
/// @param root CST语法树的根结点
/// @return AST的根节点
ast_node * MiniCCSTVisitor::run(MiniCParser::CompileUnitContext * root)
{
    return std::any_cast<ast_node *>(visitCompileUnit(root));
}

/// @brief 非终结运算符compileUnit的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitCompileUnit(MiniCParser::CompileUnitContext * ctx)
{
    // compileUnit: (funcDef | varDecl)* EOF

    // 请注意这里必须先遍历全局变量后遍历函数。肯定可以确保全局变量先声明后使用的规则，但有些情况却不能检查出。
    // 事实上可能函数A后全局变量B后函数C，这时在函数A中是不能使用变量B的，需要报语义错误，但目前的处理不会。
    // 因此在进行语义检查时，可能追加检查行号和列号，如果函数的行号/列号在全局变量的行号/列号的前面则需要报语义错误
    // TODO 请追加实现。

    ast_node * temp_node;
    ast_node * compileUnitNode = create_contain_node(ast_operator_type::AST_OP_COMPILE_UNIT);

    // 可能多个变量，因此必须循环遍历
    for (auto varCtx: ctx->varDecl()) {

        // 变量函数定义
        temp_node = std::any_cast<ast_node *>(visitVarDecl(varCtx));
        (void) compileUnitNode->insert_son_node(temp_node);
    }

    // 可能有多个函数，因此必须循环遍历
    for (auto funcCtx: ctx->funcDef()) {

        // 变量函数定义
        temp_node = std::any_cast<ast_node *>(visitFuncDef(funcCtx));
        (void) compileUnitNode->insert_son_node(temp_node);
    }

    return compileUnitNode;
}

/// @brief 非终结运算符formalParam的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitFormalParam(MiniCParser::FormalParamContext * ctx)
{
    // 识别的文法产生式：formalParam : basicType T_ID arrayParamDimensions?;
    ast_node * formal_param_node;
    // 创建代表单个形参的 AST 节点 (AST_OP_FUNC_FORMAL_PARAM)
    type_attr typeAttr = std::any_cast<type_attr>(visitBasicType(ctx->basicType()));
    // 获取所有 T_ID 终端节点 (用于名称和行号)
    auto idNode = ctx->T_ID()->getText();
    int64_t lineNo = (int64_t) ctx->T_ID()->getSymbol()->getLine();
    formal_param_node = create_func_formal_param(typeAttr, lineNo, idNode.c_str());
    if( ctx->arrayParamDimensions() ) {
        auto arrayParamDimensionsNode = std::any_cast<ast_node *>(visitArrayParamDimensions(ctx->arrayParamDimensions()));
        return ast_node::New(ast_operator_type::AST_OP_ARRAY_DECL,//TODO 不确定
                             ast_node::New(idNode, lineNo),
                             arrayParamDimensionsNode,
                             nullptr);
    }
    return formal_param_node; // 返回单个形参节点
}

///  @brief 非终结运算符formalParamList的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitFormalParamList(MiniCParser::FormalParamListContext * ctx)
{
    // 识别的文法产生式：formalParamList : formalParam (T_COMMA formalParam)*;
    // 创建代表整个形参列表的 AST 节点 (AST_OP_FUNC_FORMAL_PARAMS)
    ast_node * formal_params_list_node = create_contain_node(ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS);

    // 遍历每个 formalParam
    for (auto formalParamCtx: ctx->formalParam()) {
		// 递归调用 visitFormalParam 获取每个形参的 AST 节点
		auto formal_param_node = std::any_cast<ast_node *>(visitFormalParam(formalParamCtx));
		// 将每个形参节点插入到 formal_params_list_node 中
		(void) formal_params_list_node->insert_son_node(formal_param_node);
	}

    return std::any(formal_params_list_node);
}
/// @brief 非终结运算符funcDef的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitFuncDef(MiniCParser::FuncDefContext * ctx)
{
    // 识别的文法产生式：funcDef : (T_INT|T_VOID) T_ID T_L_PAREN formalParamList? T_R_PAREN block;

    // 函数返回类型，终结符
    type_attr funcReturnType;
    // 函数返回类型只能是int和void
    if(ctx->T_VOID()) {
		// 函数返回类型为void
		funcReturnType={BasicType::TYPE_VOID, (int64_t) ctx->T_VOID()->getSymbol()->getLine()};
	} else if(ctx->T_INT()) {
		// 函数返回类型为int
		funcReturnType={BasicType::TYPE_INT, (int64_t) ctx->T_INT()->getSymbol()->getLine()};
	} else {
		std::cerr << "Error: Unknown function return type." << std::endl;
		return nullptr; // 或者抛出异常
	}

    // 创建函数名的标识符终结符节点，终结符
    char * id = strdup(ctx->T_ID()->getText().c_str());

    var_id_attr funcId{id, (int64_t) ctx->T_ID()->getSymbol()->getLine()};

    // 形参结点
    ast_node * formalParamsNode = nullptr; // 初始化为 nullptr
    MiniCParser::FormalParamListContext * formalParamListCtx = ctx->formalParamList();

    if (formalParamListCtx != nullptr) {
        // 只有当源代码中存在形参列表时，才调用 visitFormalParamList
        std::any result_any = visitFormalParamList(formalParamListCtx);
        if (result_any.has_value()) {
            try {
                formalParamsNode = std::any_cast<ast_node *>(result_any);
                // 如果 std::any_cast 成功，但结果是 nullptr (即 visitFormalParamList 返回了 std::any(nullptr))
                // formalParamsNode 已经是 nullptr 了，符合预期
            } catch (const std::bad_any_cast & e) {
                std::cerr << "Warning: visitFormalParamList returned non-ast_node* type for function " << id
                          << ". Error: " << e.what() << ". Assuming no formal parameters." << std::endl;
                formalParamsNode = nullptr; // 确保为 nullptr
            }
        } else {
            formalParamsNode = nullptr; // 确保为 nullptr
        }
    }

    // 遍历block结点创建函数体节点，非终结符
    auto blockNode = std::any_cast<ast_node *>(visitBlock(ctx->block()));

    // 创建函数定义的节点，孩子有类型，函数名，语句块和形参
    // create_func_def函数内会释放funcId中指向的标识符空间，切记，之后不要再释放，之前一定要是通过strdup函数或者malloc分配的空间
    return create_func_def(funcReturnType, funcId, blockNode, formalParamsNode);
}

/// @brief 非终结运算符block的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitBlock(MiniCParser::BlockContext * ctx)
{
    // 识别的文法产生式：block : T_L_BRACE blockItemList? T_R_BRACE';
    if (!ctx->blockItemList()) {
        // 语句块没有语句

        // 为了方便创建一个空的Block节点
        return create_contain_node(ast_operator_type::AST_OP_BLOCK);
    }

    // 语句块含有语句

    // 内部创建Block节点，并把语句加入，这里不需要创建Block节点
    return visitBlockItemList(ctx->blockItemList());
}

/// @brief 非终结运算符blockItemList的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitBlockItemList(MiniCParser::BlockItemListContext * ctx)
{
    // 识别的文法产生式：blockItemList : blockItem +;
    // 正闭包 循环 至少一个blockItem
    auto block_node = create_contain_node(ast_operator_type::AST_OP_BLOCK);

    for (auto blockItemCtx: ctx->blockItem()) {

        // 非终结符，需遍历
        auto blockItem = std::any_cast<ast_node *>(visitBlockItem(blockItemCtx));

        // 插入到块节点中
        (void) block_node->insert_son_node(blockItem);
    }

    return block_node;
}

///
/// @brief 非终结运算符blockItem的遍历
/// @param ctx CST上下文
///
std::any MiniCCSTVisitor::visitBlockItem(MiniCParser::BlockItemContext * ctx)
{
    // 识别的文法产生式：blockItem : statement | varDecl
    if (ctx->statement()) {
        // 语句识别

        return visitStatement(ctx->statement());
    } else if (ctx->varDecl()) {
        return visitVarDecl(ctx->varDecl());
    }

    return nullptr;
}

/// @brief 非终结运算符statement中的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitStatement(MiniCParser::StatementContext * ctx)
{
    // 识别的文法产生式：statement: T_ID T_ASSIGN expr T_SEMICOLON  # assignStatement
    // | T_RETURN expr T_SEMICOLON # returnStatement
    // | block  # blockStatement
    // | expr ? T_SEMICOLON #expressionStatement;
    // | T_IF T_L_PAREN expr T_R_PAREN statement (T_ELSE statement)? # ifStatement
    // | T_WHILE T_L_PAREN expr T_R_PAREN statement # whileStatement;
    if (Instanceof(assignCtx, MiniCParser::AssignStatementContext *, ctx)) {
        return visitAssignStatement(assignCtx);
    } else if (Instanceof(returnCtx, MiniCParser::ReturnStatementContext *, ctx)) {
        return visitReturnStatement(returnCtx);
    } else if (Instanceof(blockCtx, MiniCParser::BlockStatementContext *, ctx)) {
        return visitBlockStatement(blockCtx);
    } else if (Instanceof(exprCtx, MiniCParser::ExpressionStatementContext *, ctx)) {
        return visitExpressionStatement(exprCtx);
    } else if (Instanceof(exprCtx, MiniCParser::IfStatementContext *, ctx)) {
		return visitIfStatement(exprCtx);
    } else if (Instanceof(exprCtx, MiniCParser::WhileStatementContext *, ctx)) {
		return visitWhileStatement(exprCtx);
    } else if (Instanceof(exprCtx, MiniCParser::BreakStatementContext *, ctx)) {
		return visitBreakStatement(exprCtx);
    } else if (Instanceof(exprCtx, MiniCParser::ContinueStatementContext *, ctx)) {
		return visitContinueStatement(exprCtx);
	}

    return nullptr;
}

///
/// @brief 非终结运算符statement中的returnStatement的遍历
/// @param ctx CST上下文
///
std::any MiniCCSTVisitor::visitReturnStatement(MiniCParser::ReturnStatementContext * ctx)
{
    // 识别的文法产生式：returnStatement -> T_RETURN expr T_SEMICOLON

    // 非终结符，表达式expr遍历
    auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 创建返回节点，其孩子为Expr
    return create_contain_node(ast_operator_type::AST_OP_RETURN, exprNode);
}


std::any MiniCCSTVisitor::visitAssignStatement(MiniCParser::AssignStatementContext * ctx)
{
    // 识别文法产生式：assignStatement: lVal T_ASSIGN expr T_SEMICOLON

    // 赋值左侧左值Lval遍历产生节点
    auto lvalNode = std::any_cast<ast_node *>(visitLVal(ctx->lVal()));

    // 赋值右侧expr遍历
    auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 创建一个AST_OP_ASSIGN类型的中间节点，孩子为Lval和Expr
    return ast_node::New(ast_operator_type::AST_OP_ASSIGN, lvalNode, exprNode, nullptr);
}

std::any MiniCCSTVisitor::visitBlockStatement(MiniCParser::BlockStatementContext * ctx)
{
    // 识别文法产生式 blockStatement: block

    return visitBlock(ctx->block());
}

/// @brief 非终结运算符statement中的IfStatement的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitIfStatement(MiniCParser::IfStatementContext * ctx)
{
    // 识别的文法产生式：ifStatement : T_IF T_L_PAREN expr T_R_PAREN statement (T_ELSE statement)?;

    // 非终结符，表达式expr遍历
    auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

    // 遍历if语句块
    auto ifBlockNode = std::any_cast<ast_node *>(visitStatement(ctx->statement(0)));

    ast_node * elseBlockNode = nullptr;

    if (ctx->statement().size() > 1) {
        // 遍历else语句块
        elseBlockNode = std::any_cast<ast_node *>(visitStatement(ctx->statement(1)));
    }

    // 创建if节点，其孩子为Expr和if语句块和else语句块
    return create_contain_node(ast_operator_type::AST_OP_IF, exprNode, ifBlockNode, elseBlockNode);
}

/// @brief 非终结运算符statement中的WhileStatement的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitWhileStatement(MiniCParser::WhileStatementContext * ctx)
{
	//识别的文法产生式：whileStatement : T_WHILE T_L_PAREN expr T_R_PAREN statement;

	//非终结符，表达式expr遍历
	auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

	// 遍历while语句块
	auto whileBlockNode = std::any_cast<ast_node *>(visitStatement(ctx->statement()));

    // 创建while节点，其孩子为Expr和while语句块
	return create_contain_node(ast_operator_type::AST_OP_WHILE, exprNode, whileBlockNode);
}

/// @brief 非终结运算符statement中的BreakStatement的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitBreakStatement(MiniCParser::BreakStatementContext * ctx)
{
	// 识别的文法产生式：breakStatement : T_BREAK T_SEMICOLON;

	// 创建一个AST_OP_BREAK类型的中间节点
	return create_contain_node(ast_operator_type::AST_OP_BREAK);
}

/// @brief 非终结运算符statement中的ContinueStatement的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitContinueStatement(MiniCParser::ContinueStatementContext * ctx)
{
	// 识别的文法产生式：continueStatement : T_CONTINUE T_SEMICOLON;

	// 创建一个AST_OP_CONTINUE类型的中间节点
	return create_contain_node(ast_operator_type::AST_OP_CONTINUE);
}

/// @brief 非终结运算符expr的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitExpr(MiniCParser::ExprContext * ctx)
{
    // 识别产生式：expr: lorExp;

    return visitLorExp(ctx->lorExp());
}

/// @brief 非终结运算符lorExp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitLorExp(MiniCParser::LorExpContext * ctx)
{
	// 识别产生式：lorExp: landExp (T_OR landExp)*;

	if (ctx->landExp().size() == 1) {
		return visitLandExp(ctx->landExp()[0]);
	}

	ast_node * left, * right;

	auto opsCtxVec = ctx->landExp();

	for (int k = 0; k < (int) opsCtxVec.size()-1; k++) {

		if (k == 0) {
			left = std::any_cast<ast_node *>(visitLandExp(opsCtxVec[k]));
		}

		right = std::any_cast<ast_node *>(visitLandExp(opsCtxVec[k + 1]));

		left = ast_node::New(ast_operator_type::AST_OP_OR, left, right, nullptr);
	}

	return left;
}

/// @brief 非终结运算符landExp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitLandExp(MiniCParser::LandExpContext * ctx)
{
	// 识别产生式：landExp: eqExp (T_AND eqExp)*;
    if (ctx->eqExp().size() == 1) {
        return visitEqExp(ctx->eqExp()[0]);
    }

    ast_node * left, * right;

	auto opsCtxVec = ctx->eqExp();// eqExp的个数，后续得到size-1

	for (int k = 0; k < (int) opsCtxVec.size()-1; k++) {

		if (k == 0) {
			left = std::any_cast<ast_node *>(visitEqExp(opsCtxVec[k]));
		}

		right = std::any_cast<ast_node *>(visitEqExp(opsCtxVec[k + 1]));

		left = ast_node::New(ast_operator_type::AST_OP_AND, left, right, nullptr);
	}

	return left;
}

/// @brief 非终结运算符eqExp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitEqExp(MiniCParser::EqExpContext * ctx)
{
    // 识别的文法产生式：eqExp : relExp (eqOp relExp)*;

    if (ctx->eqOp().empty()) {
		// 没有eqOp运算符，则说明闭包识别为0，只识别了第一个非终结符relExp
		return visitRelExp(ctx->relExp()[0]);
	}

	ast_node * left, * right;

	auto opsCtxVec = ctx->eqOp();// eqOp的个数

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int) opsCtxVec.size(); k++) {
        // 获取运算符

        ast_operator_type op = std::any_cast<ast_operator_type>(visitEqOp(opsCtxVec[k]));
        if (k == 0) {
			// 左操作数
			left = std::any_cast<ast_node *>(visitRelExp(ctx->relExp()[k]));
		}

		right = std::any_cast<ast_node *>(visitRelExp(ctx->relExp()[k + 1]));

		// 新建结点作为下一个运算符的右操作符
		left = ast_node::New(op, left, right, nullptr);
	}

	return left;
}

/// @brief 非终结运算符eqOp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitEqOp(MiniCParser::EqOpContext * ctx)
{
	// 识别的文法产生式：eqOp : T_EQ | T_NE

	if (ctx->T_EQ()) {
		return ast_operator_type::AST_OP_EQ;
	} else {
		return ast_operator_type::AST_OP_NE;
	}
}

/// @brief 非终结运算符relExp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitRelExp(MiniCParser::RelExpContext * ctx)
{
    // 识别文法产生式：relExp : addExp (relOp addExp)*;

	if (ctx->relOp().empty()) {
		// 没有addOp运算符，则说明闭包识别为0，只识别了第一个非终结符mulExp
		return visitAddExp(ctx->addExp()[0]);
	}

	ast_node *left, *right;

	// 存在addOp运算符
	auto opsCtxVec = ctx->relOp();

	// 有操作符，肯定会进循环，使得right设置正确的值
	for (int k = 0; k < (int) opsCtxVec.size(); k++) {

		// 获取运算符
		ast_operator_type op = std::any_cast<ast_operator_type>(visitRelOp(opsCtxVec[k]));

		if (k == 0) {

			// 左操作数
			left = std::any_cast<ast_node *>(visitAddExp(ctx->addExp()[k]));
		}

		// 右操作数
		right = std::any_cast<ast_node *>(visitAddExp(ctx->addExp()[k + 1]));

		// 新建结点作为下一个运算符的右操作符
		left = ast_node::New(op, left, right, nullptr);
	}

	return left;
}


std::any MiniCCSTVisitor::visitRelOp(MiniCParser::RelOpContext * ctx)
{
    // 识别的文法产生式：relOp :  T_LE | T_GE | T_LT | T_GT
    if (ctx->T_LE()) {
		return ast_operator_type::AST_OP_LE;
	} else if (ctx->T_GE()) {
		return ast_operator_type::AST_OP_GE;
	} else if (ctx->T_LT()) {
		return ast_operator_type::AST_OP_LT;
	} else {
		return ast_operator_type::AST_OP_GT;
	}

}
std::any MiniCCSTVisitor::visitAddExp(MiniCParser::AddExpContext * ctx)
{
    // 识别的文法产生式：addExp : mulExp (addOp mulExp)*;

    if (ctx->addOp().empty()) {

        // 没有addOp运算符，则说明闭包识别为0，只识别了第一个非终结符mulExp
        return visitMulExp(ctx->mulExp()[0]);
    }

    ast_node *left, *right;

    // 存在addOp运算符，自
    auto opsCtxVec = ctx->addOp();

    // 有操作符，肯定会进循环，使得right设置正确的值
    for (int k = 0; k < (int) opsCtxVec.size(); k++) {

        // 获取运算符
        ast_operator_type op = std::any_cast<ast_operator_type>(visitAddOp(opsCtxVec[k]));

        if (k == 0) {

            // 左操作数
            left = std::any_cast<ast_node *>(visitMulExp(ctx->mulExp()[k]));
        }

        // 右操作数
        right = std::any_cast<ast_node *>(visitMulExp(ctx->mulExp()[k + 1]));

        // 新建结点作为下一个运算符的右操作符
        left = ast_node::New(op, left, right, nullptr);
    }

    return left;
}

/// @brief 非终结运算符addOp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitAddOp(MiniCParser::AddOpContext * ctx)
{
    // 识别的文法产生式：addOp : T_ADD | T_SUB

    if (ctx->T_ADD()) {
        return ast_operator_type::AST_OP_ADD;
    } else {
        return ast_operator_type::AST_OP_SUB;
    }
}

/// @brief 非终结运算符mulExp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitMulExp(MiniCParser::MulExpContext * ctx)
{
	// 识别的文法产生式：mulExp : unaryExp (mulOp unaryExp)*;

	if (ctx->mulOp().empty()) {
		// 没有mulOp运算符，则说明闭包识别为0，只识别了第一个非终结符unaryExp
		return visitUnaryExp(ctx->unaryExp()[0]);
	}

	ast_node *left, *right;

	// 存在mulOp运算符，自
	auto opsCtxVec = ctx->mulOp();

	// 有操作符，肯定会进循环，使得right设置正确的值
	for (int k = 0; k < (int) opsCtxVec.size(); k++) {

		// 获取运算符
		ast_operator_type op = std::any_cast<ast_operator_type>(visitMulOp(opsCtxVec[k]));

		if (k == 0) {

			// 左操作数
			left = std::any_cast<ast_node *>(visitUnaryExp(ctx->unaryExp()[k]));
		}

		// 右操作数
		right = std::any_cast<ast_node *>(visitUnaryExp(ctx->unaryExp()[k + 1]));

		// 新建结点作为下一个运算符的右操作符
		left = ast_node::New(op, left, right, nullptr);
	}

	return left;
}

/// @brief 非终结运算符mulOp的遍历
/// @param ctx CST上下文
std::any MiniCCSTVisitor::visitMulOp(MiniCParser::MulOpContext * ctx)
{
	// 识别的文法产生式：mulOp : T_MUL | T_DIV | T_MOD

	if (ctx->T_MUL()) {
		return ast_operator_type::AST_OP_MUL;
	} else if (ctx->T_DIV()) {
		return ast_operator_type::AST_OP_DIV;
	} else {
		return ast_operator_type::AST_OP_MOD;
	}
}

std::any MiniCCSTVisitor::visitUnaryExp(MiniCParser::UnaryExpContext * ctx)
{
    // 识别文法产生式：unaryExp:(T_SUB | T_NOT) unaryExp | primaryExp | T_ID T_L_PAREN realParamList ? T_R_PAREN;

    if (ctx->T_NOT()) {
		// 单目非运算
		std::any operandResult = visitUnaryExp(ctx->unaryExp()); // 递归调用 visitUnaryExp

		ast_node * operandNode = std::any_cast<ast_node *>(operandResult);

		// 创建一个表示单目非操作的 AST 节点
		int64_t lineNo = (int64_t) ctx->T_NOT()->getSymbol()->getLine();
		ast_node * unaryNotNode = create_unary_not_node(operandNode, lineNo);

		return unaryNotNode;
	} else if (ctx->T_SUB()) {
        // 单目求负运算
        std::any operandResult = visitUnaryExp(ctx->unaryExp()); // 递归调用 visitUnaryExp

        ast_node * operandNode = std::any_cast<ast_node *>(operandResult);

        // 创建一个表示单目求负操作的 AST 节点
        int64_t lineNo = (int64_t) ctx->T_SUB()->getSymbol()->getLine();
        ast_node * unaryMinusNode = create_unary_minus_node(operandNode, lineNo);

        return unaryMinusNode;
    } else if (ctx->primaryExp()) {
        // 普通表达式
        return visitPrimaryExp(ctx->primaryExp());
    } else if (ctx->T_ID()) {

        // 创建函数调用名终结符节点
        ast_node * funcname_node = ast_node::New(ctx->T_ID()->getText(), (int64_t) ctx->T_ID()->getSymbol()->getLine());

        // 实参列表
        ast_node * paramListNode = nullptr;

        // 函数调用
        if (ctx->realParamList()) {
            // 有参数
            paramListNode = std::any_cast<ast_node *>(visitRealParamList(ctx->realParamList()));
        }

        // 创建函数调用节点，其孩子为被调用函数名和实参，
        return create_func_call(funcname_node, paramListNode);
    } else {
        return nullptr;
    }
}

std::any MiniCCSTVisitor::visitPrimaryExp(MiniCParser::PrimaryExpContext * ctx)
{
    // 识别文法产生式 primaryExp: T_L_PAREN expr T_R_PAREN | T_DIGIT | lVal;

    ast_node * node = nullptr;

    if (ctx->T_DIGIT()) {
        // 无符号整型字面量
        // 识别 primaryExp: T_DIGIT

        std::string text = ctx->T_DIGIT()->getText(); // 获取数字的文本表示
        uint64_t val_ull = 0; // 使用 uint64_t 来存储 stoull 的结果，以防超出 uint32_t 范围
        val_ull = std::stoull(text, nullptr, 0); // stoull 自动检测进制
        uint32_t val = static_cast<uint32_t>(val_ull);
        // uint32_t val = (uint32_t) stoull(ctx->T_DIGIT()->getText());
        int64_t lineNo = (int64_t) ctx->T_DIGIT()->getSymbol()->getLine();
        node = ast_node::New(digit_int_attr{val, lineNo});
    } else if (ctx->lVal()) {
        // 具有左值的表达式
        // 识别 primaryExp: lVal
        node = std::any_cast<ast_node *>(visitLVal(ctx->lVal()));
    } else if (ctx->expr()) {
        // 带有括号的表达式
        // primaryExp: T_L_PAREN expr T_R_PAREN
        node = std::any_cast<ast_node *>(visitExpr(ctx->expr()));
    }

    return node;
}

std::any MiniCCSTVisitor::visitLVal(MiniCParser::LValContext * ctx)
{
    // 识别文法产生式：lVal: T_ID(T_L_BRACKET expr T_R_BRACKET)*;
    // 获取ID的名字
    auto varId = ctx->T_ID()->getText();

    // 获取行号
    int64_t lineNo = (int64_t) ctx->T_ID()->getSymbol()->getLine();

    // 获取所有下标表达式的 CST 节点列表
    auto indexExprContexts = ctx->expr();

    if (!indexExprContexts.empty()) {
        // 有数组下标，这是一个数组元素访问

        // 创建代表数组变量名本身的 AST 节点
        ast_node * baseVarNode = ast_node::New(varId, lineNo);

        // 收集所有下标表达式的 AST 节点
        ast_node * indicesContainerNode =
            create_contain_node(ast_operator_type::AST_OP_ARRAY_INDEX);
        for (MiniCParser::ExprContext * exprCtx: indexExprContexts) {
            ast_node * singleIndexAstNode = std::any_cast<ast_node *>(visitExpr(exprCtx));
            if (singleIndexAstNode) {
                (void) indicesContainerNode->insert_son_node(singleIndexAstNode);
            }
        }

        // 创建数组访问节点
        return ast_node::New(ast_operator_type::AST_OP_ARRAY_ACCESS,
                             baseVarNode,
                             indicesContainerNode,
                             nullptr);
    } else {
        // 没有数组下标
        return ast_node::New(varId, lineNo);
    }
}

std::any MiniCCSTVisitor::visitVarDecl(MiniCParser::VarDeclContext * ctx)
{
    // varDecl: basicType varDef (T_COMMA varDef)* T_SEMICOLON;

    // 声明语句节点
    ast_node * stmt_node = create_contain_node(ast_operator_type::AST_OP_DECL_STMT);

    // 类型节点
    type_attr typeAttr = std::any_cast<type_attr>(visitBasicType(ctx->basicType()));

    for (auto & varCtx: ctx->varDef()) {
        // 变量名节点
        ast_node * id_node = std::any_cast<ast_node *>(visitVarDef(varCtx));

        // 创建类型节点
        ast_node * type_node = create_type_node(typeAttr);

        // 创建变量定义节点
        ast_node * decl_node = ast_node::New(ast_operator_type::AST_OP_VAR_DECL, type_node, id_node, nullptr);

        // 插入到变量声明语句
        (void) stmt_node->insert_son_node(decl_node);
    }

    return stmt_node;
}

std::any MiniCCSTVisitor::visitVarDef(MiniCParser::VarDefContext * ctx)
{
    // varDef: T_ID arrayDimensions?(T_ASSIGN expr)?;

    auto varId = ctx->T_ID()->getText();

    // 获取行号
    int64_t lineNo = (int64_t) ctx->T_ID()->getSymbol()->getLine();

    if (ctx->arrayDimensions()) {
		// 数组维度
		// 识别 varDef: T_ID arrayDimensions
		auto arrayDimensionsNode = std::any_cast<ast_node *>(visitArrayDimensions(ctx->arrayDimensions()));
		// 创建数组变量定义节点，孩子为ID和ArrayDimensions
		return ast_node::New(ast_operator_type::AST_OP_ARRAY_DECL, ast_node::New(varId, lineNo), arrayDimensionsNode, nullptr);
	}
    else if (ctx->T_ASSIGN()) {
		// 变量定义有初始值
		// 识别 varDef: T_ID T_ASSIGN expr
		auto exprNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()));

		// 创建赋值节点，孩子为ID和Expr
		return ast_node::New(ast_operator_type::AST_OP_ASSIGN, ast_node::New(varId, lineNo), exprNode, nullptr);
    } else
	{
        return ast_node::New(varId, lineNo);
	}
}


std::any MiniCCSTVisitor::visitArrayDimensions(MiniCParser::ArrayDimensionsContext * ctx)
{
    // 识别的文法产生式：arrayDimensions : (T_L_BRACKET expr T_R_BRACKET)+;

	// 创建一个数组维度节点
    ast_node * arrayDimensionsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_DIMENSIONS);

    // 遍历每个维度
	for (auto & dimCtx: ctx->expr()) {
		// 获取每个维度的表达式节点
		auto dimNode = std::any_cast<ast_node *>(visitExpr(dimCtx));
		// 插入到数组维度节点中
		(void) arrayDimensionsNode->insert_son_node(dimNode);
	}

	return arrayDimensionsNode;
}

std::any MiniCCSTVisitor::visitArrayParamDimensions(MiniCParser::ArrayParamDimensionsContext * ctx)
{
    // 识别的文法产生式(T_L_BRACKET expr? T_R_BRACKET) (T_L_BRACKET expr T_R_BRACKET)*;
    // 创建一个数组维度节点
    ast_node * arrayParamDimensionsNode = create_contain_node(ast_operator_type::AST_OP_ARRAY_INDEX);
    // 遍历每个维度
    if (ctx->expr().empty()) {
		// 没有维度表达式，直接返回空的数组维度节点
		return arrayParamDimensionsNode;
    }
    else if (ctx->expr().size() == 1) {
		// 只有一个维度表达式
		auto dimNode = std::any_cast<ast_node *>(visitExpr(ctx->expr()[0]));
		(void) arrayParamDimensionsNode->insert_son_node(dimNode);
	} else {
		// 有多个维度表达式
		for (auto & dimCtx: ctx->expr()) {
			auto dimNode = std::any_cast<ast_node *>(visitExpr(dimCtx));
			(void) arrayParamDimensionsNode->insert_son_node(dimNode);
		}
	}

	return arrayParamDimensionsNode;
}
std::any MiniCCSTVisitor::visitBasicType(MiniCParser::BasicTypeContext * ctx)
{
    // basicType: T_INT;
    type_attr attr{BasicType::TYPE_VOID, -1};
    if (ctx->T_INT()) {
        attr.type = BasicType::TYPE_INT;
        attr.lineno = (int64_t) ctx->T_INT()->getSymbol()->getLine();
    }

    return attr;
}

std::any MiniCCSTVisitor::visitRealParamList(MiniCParser::RealParamListContext * ctx)
{
    // 识别的文法产生式：realParamList : expr (T_COMMA expr)*;

    auto paramListNode = create_contain_node(ast_operator_type::AST_OP_FUNC_REAL_PARAMS);

    for (auto paramCtx: ctx->expr()) {

        auto paramNode = std::any_cast<ast_node *>(visitExpr(paramCtx));

        paramListNode->insert_son_node(paramNode);
    }

    return paramListNode;
}

std::any MiniCCSTVisitor::visitExpressionStatement(MiniCParser::ExpressionStatementContext * ctx)
{
    // 识别文法产生式  expr ? T_SEMICOLON #expressionStatement;
    if (ctx->expr()) {
        // 表达式语句

        // 遍历expr非终结符，创建表达式节点后返回
        return visitExpr(ctx->expr());
    } else {
        // 空语句

        // 直接返回空指针，需要再把语句加入到语句块时要注意判断，空语句不要加入
        return nullptr;
    }
}