#include <fstream>
#include <bits/stdc++.h>
#include <sstream>
#include "mygit.h"

using namespace std;

void logHistory()
{
    ifstream headFile(".mygit/HEAD");
    string commitSHA;
    if (!getline(headFile, commitSHA))
    {
        cerr << "Error: Unable to read HEAD file.\n";
        return;
    }
    headFile.close();

    unordered_set<string> seenCommits; // To track processed commits

    while (!commitSHA.empty())
    {
        if (seenCommits.find(commitSHA) != seenCommits.end())
        {
            cerr << "Error: Circular reference detected for commit " << commitSHA << "\n";
            break;
        }
        seenCommits.insert(commitSHA);

        string commitFilePath = ".mygit/objects/" + commitSHA;
        ifstream commitFile(commitFilePath, ios::binary);

        if (!commitFile.is_open())
        {
            cerr << "Error: Unable to open commit " << commitSHA << "\n";
            break;
        }

        // Read and decompress commit data
        stringstream compressedContent;
        compressedContent << commitFile.rdbuf();
        commitFile.close();
        string commitContent = decompressData(compressedContent.str());

        cout << "Commit SHA: " << commitSHA << "\n";

        // Parsing commit data
        string line, parentSHA, commitMessage, author, date;
        bool isMessageSection = false;
        stringstream contentStream(commitContent);

        while (getline(contentStream, line))
        {
            if (line.find("parent") == 0)
            {
                parentSHA = line.substr(7);
                cout << "Parent: " << parentSHA << "\n";
            }
            else if (line.find("author") == 0)
            {
                author = line.substr(7);
                cout << "Author: " << author << "\n";
            }
            else if (line.find("date") == 0)
            {
                date = line.substr(5);
                cout << "Date: " << date << "\n";
            }
            else if (line.empty())
            {
                isMessageSection = true;
            }
            else if (isMessageSection)
            {
                commitMessage += line + "\n";
            }
        }

        if (!commitMessage.empty())
        {
            cout << "Message: " << commitMessage << "\n";
        }

        cout << "-------------------------\n";

        commitSHA = parentSHA;
    }
}
