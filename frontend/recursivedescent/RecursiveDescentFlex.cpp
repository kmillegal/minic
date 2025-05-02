///
/// @file RecursiveDescentFlex.cpp
/// @brief 词法分析的手动实现源文件
/// @author zenglj (zenglj@live.com)
/// @version 1.1
/// @date 2024-11-23
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-11-21 <td>1.0     <td>zenglj  <td>新做
/// <tr><td>2024-11-23 <td>1.1     <td>zenglj  <td>表达式版增强
/// </table>
///
#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>

#include "RecursiveDescentParser.h"
#include "Common.h"

/// @brief 词法分析的行号信息
int64_t rd_line_no = 1;

/// @brief 词法分析的token对应的字符识别
std::string tokenValue;

/// @brief 输入源文件指针
FILE * rd_filein;

/// @brief 关键字与Token类别的数据结构
struct KeywordToken {
    std::string name;
    enum RDTokenType type;
};

/// @brief  关键字与Token对应表
static KeywordToken allKeywords[] = {
    {"int", RDTokenType::T_INT},
    {"return", RDTokenType::T_RETURN},
};

/// @brief 在标识符中检查是否时关键字，若是关键字则返回对应关键字的Token，否则返回T_ID
/// @param id 标识符
/// @return Token
static RDTokenType getKeywordToken(std::string id)
{
    //如果在allkeywords中找到，则说明为关键字
    for (auto & keyword: allKeywords) {
        if (keyword.name == id) {
            return keyword.type;
        }
    }
    // 如果不再allkeywords中，说明是标识符
    return RDTokenType::T_ID;
}

static int hexDigitValue(int c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1; // Not a hex digit
}

/// @brief 词法文法，获取下一个Token
/// @return  Token，值保存在rd_lval中
int rd_flex()
{
    int c;              // 扫描的字符
    int tokenKind = -1; // Token的值

    // 忽略空白符号，主要有空格，TAB键和换行符
    while ((c = fgetc(rd_filein)) == ' ' || c == '\t' || c == '\n' || c == '\r') {

        // 支持Linux/Windows/Mac系统的行号分析
        // Windows：\r\n
        // Mac: \n
        // Unix(Linux): \r
        if (c == '\r') {
            c = fgetc(rd_filein);
            rd_line_no++;
            if (c != '\n') {
                // 不是\n，则回退
                ungetc(c, rd_filein);
            }
        } else if (c == '\n') {
            rd_line_no++;
        }
    }

    // 文件结束符
    if (c == EOF) {
        // 返回文件结束符
        return RDTokenType::T_EOF;
    }

    // TODO 请自行实现删除源文件中的注释，含单行注释和多行注释等

    // 处理数字 (Decimal, Octal, Hexadecimal)
    if (isdigit(c)) {
        std::string num_str;
        long long val = 0;
        int base = 10;
        bool is_hex_prefix = false;

        num_str.push_back(c); // Add the first digit

        if (c == '0') {
            int next_c = fgetc(rd_filein);
            if (next_c == 'x' || next_c == 'X') {
                // Potential Hexadecimal (0x or 0X)
                base = 16;
                is_hex_prefix = true;
                num_str.push_back(next_c);
                int hex_c = fgetc(rd_filein); // Read first char after 0x/0X

                while (hexDigitValue(hex_c) != -1) { // While it's a valid hex digit
                    int digit_value = hexDigitValue(hex_c);
                    val = val * base + digit_value; // Manual calculation
                    num_str.push_back(hex_c);
                    hex_c = fgetc(rd_filein); // Read next char
                }
                ungetc(hex_c, rd_filein); // Put back the first non-hex char

            } else {
                // Potential Octal (0 followed by 0-7) or just '0'
                base = 8;
                ungetc(next_c, rd_filein); // Put the second char back; the loop will read it if it's an octal digit
                val = 0;                   // Initial value is 0 from the first '0'

                // Read and collect octal digits (0-7)
                c = fgetc(rd_filein); // Read the char after the leading '0' (this is next_c)

                while (c >= '0' && c <= '7') { // Loop as long as it's a valid octal digit
                    int digit_value = c - '0';
                    val = val * base + digit_value; // Manual calculation
                    num_str.push_back(c);           // Add octal digit
                    c = fgetc(rd_filein);           // Read next char
                }
                ungetc(c, rd_filein); // Put back the first non-octal digit
            }
        } else {
            // Decimal (starts with '1'-'9')
            base = 10;
            val = c - '0'; // Set initial value from the first digit

            // Read and collect decimal digits (0-9)
            c = fgetc(rd_filein); // Read the next char
            while (isdigit(c)) {
                int digit_value = c - '0';
                val = val * base + digit_value; // Manual calculation
                num_str.push_back(c);           // Add digit
                c = fgetc(rd_filein);           // Read next char
            }
            ungetc(c, rd_filein); // Put back the first non-digit char
        }

        // After parsing the number string
        tokenValue = num_str; // Store the original number string
        rd_lval.integer_num.lineno = rd_line_no;

        // Check for "0x" or "0X" not followed by hex digits
        if (is_hex_prefix && num_str.length() == 2) {
            fprintf(stderr,
                    "Line(%lld): Error: Hexadecimal literal '%.*s' requires at least one digit after 0x/0X.\n",
                    (long long) rd_line_no,
                    (int) num_str.length(),
                    num_str.c_str());
            tokenKind = RDTokenType::T_ERR;
        } else {
            rd_lval.integer_num.val = val; // Store the converted value
            tokenKind = RDTokenType::T_DIGIT;
        }
    } else if (c == '(') {
        // 识别字符(
        tokenKind = RDTokenType::T_L_PAREN;
        // 存储字符(
        tokenValue = "(";
    } else if (c == ')') {
        // 识别字符)
        tokenKind = RDTokenType::T_R_PAREN;
        // 存储字符)
        tokenValue = ")";
    } else if (c == '{') {
        // 识别字符{
        tokenKind = RDTokenType::T_L_BRACE;
        // 存储字符{
        tokenValue = "{";
    } else if (c == '}') {
        // 识别字符}
        tokenKind = RDTokenType::T_R_BRACE;
        // 存储字符}
        tokenValue = "}";
    } else if (c == ';') {
        // 识别字符;
        tokenKind = RDTokenType::T_SEMICOLON;
        // 存储字符;
        tokenValue = ";";
    } else if (c == '+') {
        // 识别字符+
        tokenKind = RDTokenType::T_ADD;
		// 存储字符+
        tokenValue = "+";
    } else if (c == '-') {
        // 识别字符-
        tokenKind = RDTokenType::T_SUB;
		// 存储字符-
        tokenValue = "-";
    } else if (c == '*') {
        // 识别字符*
        tokenKind = RDTokenType::T_MUL;
        // 存储字符*
        tokenValue = "*";
    } else if (c == '/') {
        // 识别字符/
		tokenKind = RDTokenType::T_DIV;
		// 存储字符/
		tokenValue = "/";
	} else if(c == '%') {
		// 识别字符%
        tokenKind = RDTokenType::T_MOD;
        // 存储字符%
        tokenValue = "%";
    } else if (c == '=') {
        // 识别字符=
        tokenKind = RDTokenType::T_ASSIGN;
    } else if (c == ',') {
        // 识别字符;
        tokenKind = RDTokenType::T_COMMA;
		// 存储字符,
        tokenValue = ",";
    } else if (isLetterUnderLine(c)) {
        // 识别标识符，包含关键字/保留字或自定义标识符

        // 最长匹配标识符
        std::string name;

        do {
            // 记录字符
            name.push_back(c);
            c = fgetc(rd_filein);
        } while (isLetterDigitalUnderLine(c));

        // 存储标识符
        tokenValue = name;

        // 多读的字符恢复，下次可继续读到该字符
        ungetc(c, rd_filein);

        // 检查是否是关键字，若是则返回对应的Token，否则返回T_ID
        tokenKind = getKeywordToken(name);
        if (tokenKind == RDTokenType::T_ID) {
            // 自定义标识符

            // 设置ID的值
            rd_lval.var_id.id = strdup(name.c_str());

            // 设置行号
            rd_lval.var_id.lineno = rd_line_no;
        } else if (tokenKind == RDTokenType::T_INT) {
            // int关键字

            // 设置类型与行号
            rd_lval.type.type = BasicType::TYPE_INT;
            rd_lval.type.lineno = rd_line_no;
        }
    } else {
        printf("Line(%lld): Invalid char %s\n", (long long) rd_line_no, tokenValue.c_str());
        tokenKind = RDTokenType::T_ERR;
    }

    // Token的类别
    return tokenKind;
}
