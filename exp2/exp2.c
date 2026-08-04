#include <iostream>
#include <string>

using namespace std;

int main()
{
    string str;

    cout << "Enter a line: ";
    getline(cin, str);

    // Check for single-line comment
    if (str.length() >= 2 && str[0] == '/' && str[1] == '/')
    {
        cout << "It is a Single-Line Comment.";
    }

    // Check for multi-line comment
    else if (str.length() >= 4 &&
             str[0] == '/' && str[1] == '*' &&
             str[str.length() - 2] == '*' &&
             str[str.length() - 1] == '/')
    {
        cout << "It is a Multi-Line Comment.";
    }

    // Not a comment
    else
    {
        cout << "It is NOT a Comment.";
    }

    return 0;
}