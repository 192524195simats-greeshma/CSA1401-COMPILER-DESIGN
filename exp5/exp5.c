#include <iostream>
#include <string>

using namespace std;

int main()
{
    string line;
    int spaces = 0, tabs = 0, newlines = 0;

    cout << "Enter the text (Type END on a new line to finish):\n";

    while (getline(cin, line))
    {
        if (line == "END")
            break;

        newlines++;

        for (char ch : line)
        {
            if (ch == ' ')
                spaces++;
            else if (ch == '\t')
                tabs++;
        }
    }

    cout << "\nNumber of Spaces      : " << spaces << endl;
    cout << "Number of Tabs        : " << tabs << endl;
    cout << "Number of New Lines   : " << newlines << endl;

    return 0;
}