#include <charconv>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

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
    RightBrace, Semicolon, Plus, Minus, Star, Slash, Percent, Equal
};

struct Token {
    TokenType type;
    std::string text;
    std::size_t line;
    std::size_t column;
};

class Lexer {
public:
    explicit Lexer(const std::string& source) : source_(source) {}

    Token next() {
        skipWhitespace();
        const std::size_t tokenLine = line_;
        const std::size_t tokenColumn = column_;
        if (atEnd()) return {TokenType::End, "", tokenLine, tokenColumn};

        const char current = advance();
        if (isIdentifierStart(current)) {
            std::string text(1, current);
            while (!atEnd() && isIdentifierPart(peek())) text += advance();
            return {TokenType::Identifier, text, tokenLine, tokenColumn};
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
            return {TokenType::Number, text, tokenLine, tokenColumn};
        }
        if (current == '"') return readString(tokenLine, tokenColumn);

        switch (current) {
        case '(': return {TokenType::LeftParen, "(", tokenLine, tokenColumn};
        case ')': return {TokenType::RightParen, ")", tokenLine, tokenColumn};
        case '{': return {TokenType::LeftBrace, "{", tokenLine, tokenColumn};
        case '}': return {TokenType::RightBrace, "}", tokenLine, tokenColumn};
        case ';': return {TokenType::Semicolon, ";", tokenLine, tokenColumn};
        case '+': return {TokenType::Plus, "+", tokenLine, tokenColumn};
        case '-': return {TokenType::Minus, "-", tokenLine, tokenColumn};
        case '*': return {TokenType::Star, "*", tokenLine, tokenColumn};
        case '/': return {TokenType::Slash, "/", tokenLine, tokenColumn};
        case '%': return {TokenType::Percent, "%", tokenLine, tokenColumn};
        case '=': return {TokenType::Equal, "=", tokenLine, tokenColumn};
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

    Token readString(std::size_t tokenLine, std::size_t tokenColumn) {
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
        return {TokenType::String, value, tokenLine, tokenColumn};
    }
};

using Value = std::variant<std::int64_t, double, std::string>;

std::string typeName(const Value& value) {
    if (std::holds_alternative<std::int64_t>(value)) return "integer";
    if (std::holds_alternative<double>(value)) return "float";
    return "string";
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
    Parser(const std::string& source, std::ostream& output)
        : lexer_(source), output_(output), current_(lexer_.next()) {}

    void run() {
        while (current_.type != TokenType::End) statement();
    }

private:
    Lexer lexer_;
    std::ostream& output_;
    Token current_;
    std::unordered_map<std::string, Value> variables_;

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
                                   "expected an assignment or chupi output");
        if (name.text == "chupi") {
            consume(TokenType::LeftParen, "expected '(' after chupi");
            const Value value = expression();
            consume(TokenType::RightParen, "expected ')' after output value");
            consume(TokenType::Semicolon, "expected ';' after statement");
            output_ << display(value) << '\n';
            return;
        }

        consume(TokenType::Equal, "expected '=' after variable name");
        const Value value = expression();
        consume(TokenType::Semicolon, "expected ';' after statement");
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
        Parser parser(source, std::cout);
        parser.run();
    } catch (const LanguageError& error) {
        std::cerr << filename << ':' << error.line << ':' << error.column
                  << ": error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}