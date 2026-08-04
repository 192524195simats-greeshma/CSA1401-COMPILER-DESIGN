#include <iostream>
#include <string>
#include <cctype>

using namespace std;

bool isOperator(char ch)
{
    string op = "+-*/%=<>!&|^";
    return op.find(ch) != string::npos;
}

int main()
{
    cout << "Enter the C program (Type END on a new line to finish):\n\n";

    string program = "", line;

    while (getline(cin, line))
    {
        if (line == "END")
            break;
        program += line + '\n';
    }

    cout << "\n----- Lexical Analysis -----\n\n";

    int i = 0;

    while (i < program.length())
    {
        char ch = program[i];

        // Ignore spaces, tabs and new lines
        if (isspace(ch))
        {
            i++;
            continue;
        }

        // Ignore single-line comments
        if (ch == '/' && i + 1 < program.length() && program[i + 1] == '/')
        {
            i += 2;
            while (i < program.length() && program[i] != '\n')
                i++;
            continue;
        }

        // Ignore multi-line comments
        if (ch == '/' && i + 1 < program.length() && program[i + 1] == '*')
        {
            i += 2;
            while (i + 1 < program.length())
            {
                if (program[i] == '*' && program[i + 1] == '/')
                {
                    i += 2;
                    break;
                }
                i++;
            }
            continue;
        }

        // Identifier
        if (isalpha(ch) || ch == '_')
        {
            string token;

            while (i < program.length() &&
                  (isalnum(program[i]) || program[i] == '_'))
            {
                token += program[i];
                i++;
            }

            cout << "Identifier : " << token << endl;
            continue;
        }

        // Constant
        if (isdigit(ch))
        {
            string token;

            while (i < program.length() && isdigit(program[i]))
            {
                token += program[i];
                i++;
            }

            cout << "Constant   : " << token << endl;
            continue;
        }

        // Operator
        if (isOperator(ch))
        {
            cout << "Operator   : " << ch << endl;
            i++;
            continue;
        }

        i++;
    }

    return 0;
}