#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main()
{
    string id;

    cout << "Enter an identifier: ";
    cin >> id;

    // Check first character
    if (!(isalpha(id[0]) || id[0] == '_'))
    {
        cout << "Invalid Identifier";
        return 0;
    }

    // Check remaining characters
    for (int i = 1; i < id.length(); i++)
    {
        if (!(isalnum(id[i]) || id[i] == '_'))
        {
            cout << "Invalid Identifier";
            return 0;
        }
    }

    cout << "Valid Identifier";

    return 0;
}