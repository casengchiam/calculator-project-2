#include <iostream>
#include <stdexcept>
#include <string>
#include <numbers>
#include <unordered_map>

// numbers header includes mathematical constants 
// unordered_map for organizing variables 

std::unordered_map<std::string, double> variables;

double resolve_variable(const std::string& name) {
    auto iter = variables.find(name);
    if (iter != variables.end()) {
        return iter->second; // Return the value if variable exists
    } else {
        throw std::runtime_error("Unknown variable: " + name);
    }
}

// Token stuff
// Token “kind” values:
char const number = '8';    // a floating-point number
char const quit = 'q';      // an exit command
char const print = ';';     // a print command

// defining mathematical constants
const std::string pi_name = "pi";
const std::string e_name = "e";

class token
{
    char kind_;       // what kind of token
    std::string name_; // adding a name of token
    double value_;    // for numbers: a value

public:
    // constructors
    token(char ch)
      : kind_(ch)
      , value_(0)
    {
    }
    token(double val)
      : kind_(number)    // let ‘8’ represent “a number”
      , value_(val)
    {
    }
    token(char ch, std::string name)
        : kind_(ch)
        , name_(std::move(name))
        , value_(0)
    {
        if (kind_ == number && name_ == pi_name)
            value_ = std::numbers::pi;
        else if (kind_ == number && name_ == e_name)
            value_ = std::numbers::e;
    }

    char kind() const
    {
        return kind_;
    }
    std::string name() const
    {
        return name_;
    }
    double value() const
    {
        return value_;
    }
};

// User interaction strings:
std::string const prompt = "> ";
std::string const result = "= ";    // indicate that a result follows

class token_stream
{
    // representation: not directly accessible to users:
    bool full;       // is there a token in the buffer?
    token buffer;    // here is where we keep a Token put back using
                     // putback()
public:
    // user interface:
    token get();            // get a token
    void putback(token);    // put a token back into the token_stream
    void ignore(char c);    // discard tokens up to and including a c

    // constructor: make a token_stream, the buffer starts empty
    token_stream()
      : full(false)
      , buffer('\0')
    {
    }

    void set_variable(const std::string& name, double value)
    {
        variables[name] = value;
    }

    double get_variable(const std::string& name)
    {
        return variables[name];
    }
};

// single global instance of the token_stream
token_stream ts;

void token_stream::putback(token t)
{
    if (full)
        throw std::runtime_error("putback() into a full buffer");
    buffer = t;
    full = true;
}

token token_stream::get()    // read a token from the token_stream
{
    // check if we already have a Token ready
    if (full)
    {
        full = false;
        return buffer;
    }

    // note that >> skips whitespace (space, newline, tab, etc.)
    char ch;
    std::cin >> ch;

    switch (ch)
    {
    case '(':
    case ')':
    case ';':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':   // adding modulo 
    case '=':   // for variable assignment
        return token(ch);    // let each character represent itself
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        std::cin.putback(ch);    // put digit back into the input stream
        double val;
        std::cin >> val;    // read a floating-point number
        return token(val);
    }
    case 'q': // adding a new case for q to ensure it only quits if alone
    {
        std::string s;
        s += ch;
        while (std::cin.get(ch) && (isalpha(ch) || isdigit(ch)))  // read until non-alpha
            s += ch;
        std::cin.putback(ch);  // put back
        if (s.size() == 1 && s[0] == quit && !isalpha(std::cin.peek()) && !isdigit(std::cin.peek())) 
            return token(quit);  
        else
            return token('a', s);   // treat identifier as variable token
    }
    default:
        if (isalpha(ch)) 
        {  
            std::string s;
            s += ch;
            while (std::cin.get(ch) && (isalpha(ch) || isdigit(ch)))  // read until non-alpha
                s += ch;
            std::cin.putback(ch);  // put back
            if (s == pi_name || s == e_name)  // check if the string matches a predefined constant name
                return token(number, s);  
            else
                return token('a', s);   // treat identifier as variable token
        }
        else
            throw std::runtime_error("Bad token");
    }
}



// discard tokens up to and including a c
void token_stream::ignore(char c)
{
    // first look in buffer:
    if (full && c == buffer.kind())    // && means 'and'
    {
        full = false;
        return;
    }
    full = false;    // discard the contents of buffer

    // now search input:
    char ch = 0;
    while (std::cin >> ch)
    {
        if (ch == c)
            break;
    }
}

// declaration so that primary() can call expression()
double expression();

double resolve_constant(const std::string& name){
    if (name == pi_name)
        return std::numbers::pi;
    else if (name == e_name)
        return std::numbers::e;
    else 
        throw std::runtime_error("Unknown constant: " + name);
}


double primary() {
    token t = ts.get();
    switch (t.kind()) {
        case '(': {
            double d = expression();
            t = ts.get();
            if (t.kind() != ')')
                throw std::runtime_error("')' expected");
            return d;
        }
        case number:
            return t.value();
        case 'a': {
            std::string name = t.name();
            token next_token = ts.get();
            if (next_token.kind() == '=') {
                double value = expression();
                ts.set_variable(name, value);
                return value;
            } else if (next_token.kind() == ';' || next_token.kind() == '+' || next_token.kind() == '-' || next_token.kind() == '*' || next_token.kind() == '/') {
                ts.putback(next_token);
                if (variables.find(name) != variables.end())
                    return resolve_variable(name);
                else
                    throw std::runtime_error("Unknown variable: " + name);
            } else {
                ts.putback(next_token);
                if (variables.find(name) != variables.end())
                    return resolve_variable(name);
                else
                    throw std::runtime_error("Unknown variable: " + name);
            }
        }
        case '-':
            return -primary();
        default:
            if (!t.name().empty())
                return resolve_constant(t.name());
            else
                throw std::runtime_error("Primary expected");
    }
}

// exactly like expression(), but for * and /
double term()
{
    double left = primary();    // get the Primary
    while (true)
    {
        token t = ts.get();    // get the next Token ...
        switch (t.kind())
        {
        case '*':
            left *= primary();
            break;
        case '/':
        {
            double d = primary();
            if (d == 0)
                throw std::runtime_error("divide by zero");
            left /= d;
            break;
        }
        case '%':               // creating case for modulo
        {
            double d = primary();
            if (d==0)
                throw std::runtime_error("divide by zero");
            left = std::fmod(left, d);  // calling the std c++ function for modulo
            break;
        }
        default:
            ts.putback(t);    // <<< put the unused token back
            return left;      // return the value
        }
    }
}

// read and evaluate: 1   1+2.5   1+2+3.14  etc.
// 	 return the sum (or difference)
double expression()
{
    double left = term();    // get the Term
    while (true)
    {
        token t = ts.get();    // get the next token…
        switch (t.kind())      // ... and do the right thing with it
        {
        case '+':
            left += term();
            break;
        case '-':
            left -= term();
            break;
        default:
            ts.putback(t);    // <<< put the unused token back
            return left;      // return the value of the expression
        }
    }
}

void clean_up_mess()
{
    ts.ignore(print);
}

void calculate()
{
    while (std::cin)
    {
        try
        {
            std::cout << prompt;    // print prompt
            token t = ts.get();

            // first discard all “prints”
            while (t.kind() == print)
                t = ts.get();

            if (t.kind() == quit)
                return;    // ‘q’ for “quit”

            if (t.kind() == '=')
            {
                token var_token = ts.get();
                if (var_token.kind() != 'a')
                    throw std::runtime_error("Variable name missing");
                
                token assign_token = ts.get();
                if (assign_token.kind() != '=')
                    throw std::runtime_error("Expected '=' after variable name");
                
                double value = expression();

                ts.set_variable(var_token.name(), value);

                continue;
            }

            if (t.kind() == 'a')
            {
                token next_token = ts.get();
                if (next_token.kind() == '=')
                {
                    double value = expression();
                    ts.set_variable(t.name(), value);
                    continue;
                }
                ts.putback(next_token); // not followed by '='
            }

            ts.putback(t);

            std::cout << result << expression() << std::endl;
        }
        catch (std::runtime_error const& e)
        {
            std::cerr << e.what() << std::endl;    // write error message
            clean_up_mess();                       // <<< The tricky part!
        }
    }
}

int main()
{
    std::cout << "Welcome to the calculator program. Please enter an expression followed by ; to print. \n";
    std::cout << "For variable assignments, enter as follows: x = 7; \n";
    std::cout << "For variable manuipulation, please do not have variables be the first character. \n";
    std::cout << "To quit the program, type 'q'. \n";

    try
    {
        calculate();
        return 0;
    }
    catch (...)
    {
        // other errors (don't try to recover)
        std::cerr << "exception\n";
        return 2;
    }
}