#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main()
{
    string program = "", line;

    cout << "Enter the program (Type END on a new line to finish):\n";

    while (getline(cin, line))
    {
        if (line == "END")
            break;
        program += line + '\n';
    }

    cout << "\nProgram after removing spaces and comments:\n\n";

    int i = 0;

    while (i < program.length())
    {
        // Ignore spaces, tabs and new lines
        if (isspace(program[i]))
        {
            i++;
            continue;
        }

        // Ignore single-line comments
        if (program[i] == '/' && i + 1 < program.length() && program[i + 1] == '/')
        {
            i += 2;
            while (i < program.length() && program[i] != '\n')
                i++;
            continue;
        }

        // Ignore multi-line comments
        if (program[i] == '/' && i + 1 < program.length() && program[i + 1] == '*')
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

        // Print remaining characters
        cout << program[i];
        i++;
    }

    cout << endl;

    return 0;
}