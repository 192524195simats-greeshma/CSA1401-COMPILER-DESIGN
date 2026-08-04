#include <iostream>
#include <string>

using namespace std;

int main()
{
    string input;

    cout << "Enter an expression: ";
    getline(cin, input);

    cout << "\nArithmetic Operators Found:\n";

    bool found = false;

    for (int i = 0; i < input.length(); i++)
    {
        switch (input[i])
        {
            case '+':
                cout << "+ : Addition Operator" << endl;
                found = true;
                break;

            case '-':
                cout << "- : Subtraction Operator" << endl;
                found = true;
                break;

            case '*':
                cout << "* : Multiplication Operator" << endl;
                found = true;
                break;

            case '/':
                cout << "/ : Division Operator" << endl;
                found = true;
                break;
        }
    }

    if (!found)
        cout << "No Arithmetic Operators Found.";

    return 0;
}