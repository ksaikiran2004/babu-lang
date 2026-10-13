#include <charconv>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

namespace {

struct LanguageError : std::runtime_error {
    std::size_t line;
    std::size_t column;

    LanguageError(std::size_t errorLine, std::size_t errorColumn,
                  const std::string& message)
        : std::runtime_error(message), line(errorLine), column(errorColumn) {}
};

enum class TokenType {
    End, Identifier, Number, String, LeftParen, RightParen, LeftBrace,
    RightBrace, Semicolon, Comma, Colon, Arrow, Plus, Minus, Star, Slash,
    Percent, Equal
};

struct Token {
    TokenType type;
    std::string text;
    std::size_t line;
    std::size_t column;
    std::size_t startOffset = 0;
    std::size_t endOffset = 0;
};

class Lexer {
public:
    explicit Lexer(const std::string& source, std::size_t initialLine = 1,
                   std::size_t initialColumn = 1)
        : source_(source), line_(initialLine), column_(initialColumn) {}

    Token next() {
        skipWhitespace();
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;
        const std::size_t tokenStart = offset_;
        if (atEnd()) return {TokenType::End, "", tokenLine, tokenColumn};

        const char current = advance();
        if (isIdentifierStart(current)) {
            std::string text(1, current);
            while (!atEnd() && isIdentifierPart(peek())) text += advance();
                return {TokenType::Identifier, text, tokenLine, tokenColumn,
                    tokenStart, offset_};
        }
        if (std::isdigit(static_cast<unsigned char>(current))) {
            std::string text(1, current);
            while (!atEnd() && std::isdigit(static_cast<unsigned char>(peek()))) {
                text += advance();
            }
            if (!atEnd() && peek() == '.' && std::isdigit(
                    static_cast<unsigned char>(peekNext()))) {
                text += advance();
                while (!atEnd() && std::isdigit(
                           static_cast<unsigned char>(peek()))) {
                    text += advance();
                }
            }
                return {TokenType::Number, text, tokenLine, tokenColumn,
                    tokenStart, offset_};
        }
            if (current == '"') return readString(tokenLine, tokenColumn, tokenStart);

        switch (current) {
            case '(':
                return {TokenType::LeftParen, "(", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case ')':
                return {TokenType::RightParen, ")", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case '{':
                return {TokenType::LeftBrace, "{", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case '}':
                return {TokenType::RightBrace, "}", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case ';':
                return {TokenType::Semicolon, ";", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case ',':
                return {TokenType::Comma, ",", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case ':':
                return {TokenType::Colon, ":", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case '+':
                return {TokenType::Plus, "+", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case '-':
                if (!atEnd() && peek() == '>') {
                advance();
                return {TokenType::Arrow, "->", tokenLine, tokenColumn,
                    tokenStart, offset_};
                }
                return {TokenType::Minus, "-", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case '*':
                return {TokenType::Star, "*", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case '/':
                return {TokenType::Slash, "/", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case '%':
                return {TokenType::Percent, "%", tokenLine, tokenColumn,
                    tokenStart, offset_};
            case '=':
                return {TokenType::Equal, "=", tokenLine, tokenColumn,
                    tokenStart, offset_};
        default:
            throw LanguageError(tokenLine, tokenColumn,
                                "unexpected character");
        }
    }

private:
    const std::string& source_;
    std::size_t offset_ = 0;
    std::size_t line_ = 1;
    std::size_t column_ = 1;

    bool atEnd() const { return offset_ >= source_.size(); }
    char peek() const { return atEnd() ? '\0' : source_[offset_]; }
    char peekNext() const {
        return offset_ + 1 >= source_.size() ? '\0' : source_[offset_ + 1];
    }

    char advance() {
        const char value = source_[offset_++];
        if (value == '\n') {
            ++line_;
            column_ = 1;
        } else {
            ++column_;
        }
        return value;
    }

    void skipWhitespace() {
        while (!atEnd() && std::isspace(static_cast<unsigned char>(peek()))) {
            advance();
        }
    }

    static bool isIdentifierStart(char value) {
        const auto byte = static_cast<unsigned char>(value);
        return std::isalpha(byte) || value == '_';
    }

    static bool isIdentifierPart(char value) {
        const auto byte = static_cast<unsigned char>(value);
        return std::isalnum(byte) || value == '_';
    }

    Token readString(std::size_t tokenLine, std::size_t tokenColumn,
                     std::size_t tokenStart) {
        std::string value;
        while (!atEnd() && peek() != '"') {
            const char character = advance();
            if (character != '\\') {
                value += character;
                continue;
            }
            if (atEnd()) {
                throw LanguageError(tokenLine, tokenColumn,
                                    "unterminated string escape");
            }
            const char escaped = advance();
            switch (escaped) {
            case 'n': value += '\n'; break;
            case 'r': value += '\r'; break;
            case 't': value += '\t'; break;
            case '\\': value += '\\'; break;
            case '"': value += '"'; break;
            default:
                throw LanguageError(line_, column_ - 1,
                                    "unsupported string escape");
            }
        }
        if (atEnd()) {
            throw LanguageError(tokenLine, tokenColumn, "unterminated string");
        }
        advance();
        return {TokenType::String, value, tokenLine, tokenColumn,
            tokenStart, offset_};
    }
};

using Value = std::variant<std::int64_t, double, std::string>;
using Environment = std::unordered_map<std::string, Value>;

struct Parameter {
    std::string name;
    std::string type;
    std::size_t line;
    std::size_t column;
};

struct Function {
    std::string name;
    std::vector<Parameter> parameters;
    std::optional<std::string> returnType;
    std::string body;
    Environment globals;
    std::unordered_set<std::string> constants;
    std::size_t line;
    std::size_t column;
    std::size_t bodyLine;
    std::size_t bodyColumn;
    bool checking = false;
};

using FunctionMap = std::unordered_map<std::string, Function>;

std::string typeName(const Value& value) {
    if (std::holds_alternative<std::int64_t>(value)) return "integer";
    if (std::holds_alternative<double>(value)) return "float";
    return "string";
}

Value sampleValue(const std::string& type) {
    if (type == "integer") return std::int64_t{1};
    if (type == "float") return 1.0;
    return std::string("sample");
}

std::string display(const Value& value) {
    if (const auto* integer = std::get_if<std::int64_t>(&value)) {
        return std::to_string(*integer);
    }
    if (const auto* floating = std::get_if<double>(&value)) {
        std::ostringstream output;
        output << std::setprecision(15) << *floating;
        return output.str();
    }
    return std::get<std::string>(value);
}

class Parser {
public:
    Parser(const std::string& source, std::ostream& output,
           FunctionMap& functions, Environment variables = {},
           bool functionBody = false, bool validationMode = false,
              std::optional<std::string> expectedReturnType = std::nullopt,
              std::size_t sourceLine = 1, std::size_t sourceColumn = 1,
              std::unordered_set<std::string> constants = {})
          : source_(source), lexer_(source, sourceLine, sourceColumn), output_(output), functions_(functions),
          variables_(std::move(variables)), functionBody_(functionBody),
          validationMode_(validationMode),
          expectedReturnType_(std::move(expectedReturnType)),
          current_(lexer_.next()), constants_(std::move(constants)) {}

    std::optional<Value> run() {
        while (current_.type != TokenType::End && !returnValue_) statement();
        if (functionBody_ && !returnValue_) {
            throw LanguageError(current_.line, current_.column,
                                "function must return a value");
        }
        return returnValue_;
    }

private:
    const std::string& source_;
    Lexer lexer_;
    std::ostream& output_;
    FunctionMap& functions_;
    Environment variables_;
    bool functionBody_;
    bool validationMode_;
    std::optional<std::string> expectedReturnType_;
    Token current_;
    std::optional<Value> returnValue_;
    std::unordered_set<std::string> constants_;

    void advance() { current_ = lexer_.next(); }

    Token consume(TokenType type, const std::string& message) {
        if (current_.type != type) {
            throw LanguageError(current_.line, current_.column, message);
        }
        Token result = current_;
        advance();
        return result;
    }

    void statement() {
        if (current_.type == TokenType::LeftBrace) {
            advance();
            while (current_.type != TokenType::RightBrace) {
                if (returnValue_) return;
                if (current_.type == TokenType::End) {
                    throw LanguageError(current_.line, current_.column,
                                        "expected '}' to close block");
                }
                statement();
            }
            advance();
            return;
        }
        if (current_.type == TokenType::RightBrace) {
            throw LanguageError(current_.line, current_.column,
                                "unexpected '}'");
        }

        const Token name = consume(TokenType::Identifier,
                                   "expected an assignment, function, or chupi output");
        if (name.text == "kriya" || name.text == "fun") {
            if (functionBody_) {
                throw LanguageError(name.line, name.column,
                                    "nested function declarations are not supported");
            }
            functionDeclaration(name);
            return;
        }
        if (name.text == "pampu" || name.text == "return") {
            if (!functionBody_) {
                throw LanguageError(name.line, name.column,
                                    "return is only valid inside a function");
            }
            const Value value = expression();
            consume(TokenType::Semicolon, "expected ';' after statement");
            if (expectedReturnType_ &&
                typeName(value) != *expectedReturnType_) {
                throw LanguageError(name.line, name.column,
                                    "function must return " + *expectedReturnType_ +
                                        ", got " + typeName(value));
            }
            returnValue_ = value;
            return;
        }
        if (name.text == "chupi") {
            consume(TokenType::LeftParen, "expected '(' after chupi");
            const Value value = expression();
            if (!validationMode_) output_ << display(value);
            while (current_.type == TokenType::Comma) {
                advance();
                const Value nextValue = expression();
                if (!validationMode_) output_ << display(nextValue);
            }
            consume(TokenType::RightParen, "expected ')' after output value");
            consume(TokenType::Semicolon, "expected ';' after statement");
            if (!validationMode_) output_ << '\n';
            return;
        }

        if (name.text == "astiram" || name.text == "stiram") {
            variableDeclaration(name, name.text == "stiram");
            return;
        }

        consume(TokenType::Equal, "expected '=' after variable name");
        const Value value = expression();
        consume(TokenType::Semicolon, "expected ';' after statement");
        if (constants_.find(name.text) != constants_.end()) {
            throw LanguageError(name.line, name.column,
                                "cannot reassign constant '" + name.text + "'");
        }
        const auto existing = variables_.find(name.text);
        if (existing != variables_.end() &&
            typeName(existing->second) != typeName(value)) {
            throw LanguageError(name.line, name.column,
                                "cannot assign " + typeName(value) +
                                    " to " + typeName(existing->second) +
                                    " variable '" + name.text + "'");
        }
        variables_[name.text] = value;
    }

    void variableDeclaration(const Token& keyword, bool constant) {
        const Token name = consume(TokenType::Identifier,
                                   "expected variable name after '" +
                                       keyword.text + "'");
        if (variables_.find(name.text) != variables_.end()) {
            throw LanguageError(name.line, name.column,
                                "variable '" + name.text + "' is already defined");
        }
        consume(TokenType::Equal, "expected '=' after variable name");
        const Value value = expression();
        consume(TokenType::Semicolon, "expected ';' after statement");
        variables_[name.text] = value;
        if (constant) constants_.insert(name.text);
    }

    std::string parseType() {
        const Token type = consume(TokenType::Identifier,
                                   "expected a parameter or return type");
        if (type.text != "integer" && type.text != "float" &&
            type.text != "string") {
            throw LanguageError(type.line, type.column,
                                "unknown type '" + type.text + "'");
        }
        return type.text;
    }

    void functionDeclaration(const Token& keyword) {
        const Token name = consume(TokenType::Identifier,
                       "expected function name after 'kriya'");
        if (functions_.find(name.text) != functions_.end()) {
            throw LanguageError(name.line, name.column,
                                "function '" + name.text + "' is already defined");
        }
        consume(TokenType::LeftParen, "expected '(' after function name");
        std::vector<Parameter> parameters;
        if (current_.type != TokenType::RightParen) {
            do {
                const Token parameter = consume(TokenType::Identifier,
                                                "expected parameter name");
                consume(TokenType::Colon, "expected ':' after parameter name");
                const std::string type = parseType();
                for (const auto& existing : parameters) {
                    if (existing.name == parameter.text) {
                        throw LanguageError(parameter.line, parameter.column,
                                            "duplicate parameter '" +
                                                parameter.text + "'");
                    }
                }
                parameters.push_back({parameter.text, type, parameter.line,
                                      parameter.column});
                if (current_.type != TokenType::Comma) break;
                advance();
            } while (true);
        }
        consume(TokenType::RightParen, "expected ')' after parameters");

        std::optional<std::string> returnType;
        if (current_.type == TokenType::Arrow) {
            advance();
            returnType = parseType();
        }
        const Token openingBrace = consume(TokenType::LeftBrace,
                                           "expected '{' before function body");
        std::size_t depth = 1;
        std::size_t bodyEnd = openingBrace.endOffset;
        while (depth > 0) {
            if (current_.type == TokenType::End) {
                throw LanguageError(current_.line, current_.column,
                                    "expected '}' to close function body");
            }
            if (current_.type == TokenType::LeftBrace) ++depth;
            if (current_.type == TokenType::RightBrace) {
                --depth;
                if (depth == 0) bodyEnd = current_.startOffset;
            }
            advance();
        }

        Function function{name.text, std::move(parameters), returnType,
                          source_.substr(openingBrace.endOffset,
                                         bodyEnd - openingBrace.endOffset),
                  variables_, constants_, keyword.line, keyword.column,
                          openingBrace.line, openingBrace.column + 1, true};
        auto [entry, inserted] = functions_.emplace(name.text, std::move(function));
        (void)inserted;

        Environment sampleEnvironment = entry->second.globals;
        for (const auto& parameter : entry->second.parameters) {
            sampleEnvironment[parameter.name] = sampleValue(parameter.type);
        }
        Parser validator(entry->second.body, output_, functions_,
                         std::move(sampleEnvironment), true, true,
                         entry->second.returnType, entry->second.bodyLine,
                         entry->second.bodyColumn, entry->second.constants);
        const auto result = validator.run();
        if (!result) {
            throw LanguageError(keyword.line, keyword.column,
                                "function must return a value");
        }
        const std::string inferredType = typeName(*result);
        if (entry->second.returnType &&
            *entry->second.returnType != inferredType) {
            throw LanguageError(keyword.line, keyword.column,
                                "function returns " + inferredType +
                                    " but declares " +
                                    *entry->second.returnType);
        }
        entry->second.returnType = inferredType;
        entry->second.checking = false;
    }

    Value expression() { return addition(); }

    Value addition() {
        Value value = multiplication();
        while (current_.type == TokenType::Plus ||
               current_.type == TokenType::Minus) {
            const Token operation = current_;
            advance();
            value = apply(operation, value, multiplication());
        }
        return value;
    }

    Value multiplication() {
        Value value = unary();
        while (current_.type == TokenType::Star ||
               current_.type == TokenType::Slash ||
               current_.type == TokenType::Percent) {
            const Token operation = current_;
            advance();
            value = apply(operation, value, unary());
        }
        return value;
    }

    Value unary() {
        if (current_.type == TokenType::Minus) {
            const Token operation = current_;
            advance();
            Value value = unary();
            if (auto* integer = std::get_if<std::int64_t>(&value)) {
                if (*integer == std::numeric_limits<std::int64_t>::min()) {
                    if (validationMode_) return std::int64_t{0};
                    throw LanguageError(operation.line, operation.column,
                                        "integer overflow");
                }
                return -*integer;
            }
            if (auto* floating = std::get_if<double>(&value)) return -*floating;
            throw LanguageError(operation.line, operation.column,
                                "unary '-' requires a number");
        }
        return primary();
    }

    Value callFunction(const Token& name) {
        consume(TokenType::LeftParen, "expected '(' after function name");
        std::vector<Value> arguments;
        if (current_.type != TokenType::RightParen) {
            do {
                arguments.push_back(expression());
                if (current_.type != TokenType::Comma) break;
                advance();
            } while (true);
        }
        consume(TokenType::RightParen, "expected ')' after arguments");

        const auto found = functions_.find(name.text);
        if (found == functions_.end()) {
            throw LanguageError(name.line, name.column,
                                "undefined function '" + name.text + "'");
        }
        Function& function = found->second;
        if (arguments.size() != function.parameters.size()) {
            throw LanguageError(name.line, name.column,
                                "function '" + name.text + "' expects " +
                                    std::to_string(function.parameters.size()) +
                                    " arguments, got " +
                                    std::to_string(arguments.size()));
        }

        Environment localVariables = function.globals;
        for (std::size_t index = 0; index < arguments.size(); ++index) {
            const Parameter& parameter = function.parameters[index];
            const std::string argumentType = typeName(arguments[index]);
            if (argumentType != parameter.type) {
                throw LanguageError(name.line, name.column,
                                    "argument " + std::to_string(index + 1) +
                                        " of '" + name.text + "' must be " +
                                        parameter.type + ", got " + argumentType);
            }
            localVariables[parameter.name] = arguments[index];
        }

        if (function.checking) {
            if (!function.returnType) {
                throw LanguageError(name.line, name.column,
                                    "recursive function '" + name.text +
                                        "' needs an explicit return type");
            }
            return sampleValue(*function.returnType);
        }

        Parser invocation(function.body, output_, functions_,
                          std::move(localVariables), true, validationMode_,
                          function.returnType, function.bodyLine,
                          function.bodyColumn, function.constants);
        const auto result = invocation.run();
        if (!result) {
            throw LanguageError(name.line, name.column,
                                "function '" + name.text +
                                    "' did not return a value");
        }
        if (function.returnType && typeName(*result) != *function.returnType) {
            throw LanguageError(name.line, name.column,
                                "function '" + name.text + "' returned " +
                                    typeName(*result) + ", expected " +
                                    *function.returnType);
        }
        return *result;
    }

    Value primary() {
        if (current_.type == TokenType::Number) {
            const Token number = current_;
            advance();
            if (number.text.find('.') != std::string::npos) {
                char* end = nullptr;
                const double value = std::strtod(number.text.c_str(), &end);
                if (end == number.text.c_str() || *end != '\0') {
                    throw LanguageError(number.line, number.column,
                                        "invalid floating-point literal");
                }
                return value;
            }
            std::int64_t value = 0;
            const auto parsed = std::from_chars(
                number.text.data(), number.text.data() + number.text.size(), value);
            if (parsed.ec != std::errc()) {
                throw LanguageError(number.line, number.column,
                                    "integer literal is out of range");
            }
            return value;
        }
        if (current_.type == TokenType::String) {
            const std::string value = current_.text;
            advance();
            return value;
        }
        if (current_.type == TokenType::Identifier) {
            const Token name = current_;
            advance();
            if (current_.type == TokenType::LeftParen) return callFunction(name);
            const auto value = variables_.find(name.text);
            if (value == variables_.end()) {
                throw LanguageError(name.line, name.column,
                                    "undefined variable '" + name.text + "'");
            }
            return value->second;
        }
        if (current_.type == TokenType::LeftParen) {
            advance();
            Value value = expression();
            consume(TokenType::RightParen, "expected ')' after expression");
            return value;
        }
        throw LanguageError(current_.line, current_.column,
                            "expected a literal, variable, or parenthesized expression");
    }

    Value apply(const Token& operation, const Value& left, const Value& right) {
        const bool leftInteger = std::holds_alternative<std::int64_t>(left);
        const bool rightInteger = std::holds_alternative<std::int64_t>(right);
        const bool leftNumber = leftInteger || std::holds_alternative<double>(left);
        const bool rightNumber = rightInteger || std::holds_alternative<double>(right);

        if (operation.type == TokenType::Plus &&
            std::holds_alternative<std::string>(left) &&
            std::holds_alternative<std::string>(right)) {
            return std::get<std::string>(left) + std::get<std::string>(right);
        }
        if (!leftNumber || !rightNumber) {
            throw LanguageError(operation.line, operation.column,
                                "operator '" + operation.text +
                                    "' requires compatible values");
        }
        if (operation.type == TokenType::Percent &&
            (!leftInteger || !rightInteger)) {
            throw LanguageError(operation.line, operation.column,
                                "operator '%' requires integer operands");
        }

        if (leftInteger && rightInteger &&
            operation.type != TokenType::Slash) {
            const auto first = std::get<std::int64_t>(left);
            const auto second = std::get<std::int64_t>(right);
            std::int64_t result = 0;
            bool overflow = false;
            switch (operation.type) {
            case TokenType::Plus:
                overflow = __builtin_add_overflow(first, second, &result);
                break;
            case TokenType::Minus:
                overflow = __builtin_sub_overflow(first, second, &result);
                break;
            case TokenType::Star:
                overflow = __builtin_mul_overflow(first, second, &result);
                break;
            case TokenType::Percent:
                if (second == 0) {
                    if (validationMode_) return std::int64_t{0};
                    throw LanguageError(operation.line, operation.column,
                                        "division by zero");
                }
                if (first == std::numeric_limits<std::int64_t>::min() &&
                    second == -1) return std::int64_t{0};
                result = first % second;
                break;
            default: break;
            }
            if (overflow) {
                if (validationMode_) return std::int64_t{0};
                throw LanguageError(operation.line, operation.column,
                                    "integer overflow");
            }
            return result;
        }

        const double first = leftInteger
                                 ? static_cast<double>(std::get<std::int64_t>(left))
                                 : std::get<double>(left);
        const double second = rightInteger
                                  ? static_cast<double>(std::get<std::int64_t>(right))
                                  : std::get<double>(right);
        if ((operation.type == TokenType::Slash) && second == 0.0) {
            if (validationMode_) return 0.0;
            throw LanguageError(operation.line, operation.column,
                                "division by zero");
        }
        switch (operation.type) {
        case TokenType::Plus: return first + second;
        case TokenType::Minus: return first - second;
        case TokenType::Star: return first * second;
        case TokenType::Slash: return first / second;
        default: break;
        }
        throw LanguageError(operation.line, operation.column,
                            "unsupported numeric operator");
    }
};

} // namespace

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: babu <source.babu>\n";
        return 2;
    }

    const std::string filename = argv[1];
    std::ifstream sourceFile(filename, std::ios::binary);
    if (!sourceFile) {
        std::cerr << filename << ": error: cannot open source file\n";
        return 2;
    }
    const std::string source((std::istreambuf_iterator<char>(sourceFile)),
                             std::istreambuf_iterator<char>());
    try {
        FunctionMap functions;
        Parser parser(source, std::cout, functions);
        parser.run();
    } catch (const LanguageError& error) {
        std::cerr << filename << ':' << error.line << ':' << error.column
                  << ": error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}