#include <fstream>
#include <bits/stdc++.h>
#include <sstream>
#include "mygit.h"

using namespace std;

void lsTree(const string &sha, bool nameOnly)
{
    string path = ".mygit/objects/" + sha;
    ifstream file(path, ios::binary);

    if (!file)
    {
        cerr << "Tree not found: " << sha << endl;
        return;
    }

    // Read the compressed content
    stringstream compressedContent;
    compressedContent << file.rdbuf();
    file.close();

    // Decompress the content
    string decompressedContent = decompressData(compressedContent.str());
    stringstream contentStream(decompressedContent);

    string line;
    bool hasEntries = false;

    while (getline(contentStream, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);
        string mode, type, sha1, name;

        ss >> mode >> type >> sha1;
        getline(ss, name);
        name.erase(0, name.find_first_not_of(' '));

        // Ignore entries inside .mygit
        if (name.find(".mygit") == 0)
        {
            continue;
        }

        hasEntries = true;

        if (nameOnly)
        {
            cout << name << "\n"; // Print only the nam
        }
        else
        {
            cout << mode << " " << type << " " << sha1 << " " << name << "\n"; // Print details
        }
    }

    if (!hasEntries)
    {
        cout << "The tree is empty.\n";
    }
}
