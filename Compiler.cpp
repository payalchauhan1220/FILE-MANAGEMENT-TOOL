#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

class ArithmeticCompiler {
private:
    string expression;
    size_t pos;

    char peek() {
        return (pos < expression.length()) ? expression[pos] : '\0';
    }

    char get() {
        return (pos < expression.length()) ? expression[pos++] : '\0';
    }

    void skipWhitespace() {
        while (isspace(peek())) get();
    }

    // Parse numbers
    int parseNumber() {
        skipWhitespace();
        int num = 0;
        bool hasNumber = false;
        while (isdigit(peek())) {
            hasNumber = true;
            num = num * 10 + (get() - '0');
        }

        if (!hasNumber)
            throw runtime_error("Expected a number at position " + to_string(pos));

        return num;
    }

    // Parse parentheses or number
    int parseFactor() {
        skipWhitespace();
        if (peek() == '(') {
            get(); // consume '('
            int value = parseExpression();
            skipWhitespace();
            if (get() != ')')
                throw runtime_error("Missing closing parenthesis at position " + to_string(pos));
            return value;
        } else {
            return parseNumber();
        }
    }

    // Parse multiplication and division
    int parseTerm() {
        int result = parseFactor();
        while (true) {
            skipWhitespace();
            if (peek() == '*') {
                get();
                result *= parseFactor();
            } else if (peek() == '/') {
                get();
                int divisor = parseFactor();
                if (divisor == 0)
                    throw runtime_error("Division by zero");
                result /= divisor;
            } else {
                break;
            }
        }
        return result;
    }

    // Parse addition and subtraction
    int parseExpression() {
        int result = parseTerm();
        while (true) {
            skipWhitespace();
            if (peek() == '+') {
                get();
                result += parseTerm();
            } else if (peek() == '-') {
                get();
                result -= parseTerm();
            } else {
                break;
            }
        }
        return result;
    }

public:
    ArithmeticCompiler(const string& expr) : expression(expr), pos(0) {}

    int evaluate() {
        int result = parseExpression();
        skipWhitespace();
        if (pos != expression.length())
            throw runtime_error("Unexpected character at position " + to_string(pos));
        return result;
    }
};

int main() {
    cout << "🔢 Simple Arithmetic Expression Compiler\n";
    cout << "Enter an expression (use +, -, *, /, and parentheses):\n\n";

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);
        if (input.empty()) break;

        try {
            ArithmeticCompiler compiler(input);
            int result = compiler.evaluate();
            cout << "✅ Result: " << result << "\n\n";
        } catch (const exception& ex) {
            cerr << "❌ Error: " << ex.what() << "\n\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
