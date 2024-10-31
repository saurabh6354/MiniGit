#include <bits/stdc++.h>
#include <fstream>
#include <ctime>
#include <sstream>
#include "mygit.h"

using namespace std;

string generateTimestamp()
{
    time_t now = time(0);
    char *dt = ctime(&now);
    return string(dt);
}
vector<string> getBlobHashes()
{
    vector<string> hashes;
    ifstream indexFile(".mygit/index");
    string line;

    while (getline(indexFile, line))
    {
        stringstream ss(line);
        string filename, blobHash;
        ss >> filename >> blobHash;
        hashes.push_back(blobHash);
    }
    indexFile.close();
    return hashes;
}
string commit(const string &message)
{
    vector<pair<string, string>> indexEntries;
    ifstream indexFile(".mygit/index");
    string line;

    while (getline(indexFile, line))
    {
        stringstream ss(line);
        string filename, blobHash;
        ss >> filename >> blobHash;

        // Read the original file
        ifstream file(filename, ios::binary);
        if (file.is_open())
        {
            stringstream buffer;
            buffer << file.rdbuf();
            string content = buffer.str();
            file.close();

            // Compress the content
            string compressedContent = compressData(content);

            // Store the blob
            string objectPath = ".mygit/objects/" + blobHash;
            ofstream objectFile(objectPath, ios::binary);
            if (objectFile.is_open())
            {
                objectFile.write(compressedContent.c_str(), compressedContent.length());
                objectFile.close();
                cout << "Stored blob: " << blobHash << " for file: " << filename << endl;
            }
        }
        indexEntries.push_back({filename, blobHash});
    }
    indexFile.close();

    // Generate tree content
    stringstream treeContent;
    for (const auto &entry : indexEntries)
    {
        treeContent << "100644 blob " << entry.second << " " << entry.first << "\n";
    }

    // Create and store the tree object
    string treeSHA = computeSHA1(treeContent.str());
    string compressedTree = compressData(treeContent.str());
    ofstream treeFile(".mygit/objects/" + treeSHA, ios::binary);
    if (treeFile.is_open())
    {
        treeFile.write(compressedTree.c_str(), compressedTree.length());
        treeFile.close();
        cout << "Stored tree: " << treeSHA << endl;
    }

    // Generate commit content
    stringstream commitContent;
    commitContent << "tree " << treeSHA << "\n";

    // Add parent if exists
    ifstream headFile(".mygit/HEAD");
    string parentSHA;
    if (headFile.is_open() && getline(headFile, parentSHA) && !parentSHA.empty())
    {
        commitContent << "parent " << parentSHA << "\n";
    }
    headFile.close();

    commitContent << "author System User <user@system.com>\n";
    commitContent << "date " << generateTimestamp() << "\n\n";
    commitContent << message << "\n";

    // Create and store the commit object
    string commitSHA = computeSHA1(commitContent.str());
    string compressedCommit = compressData(commitContent.str());
    ofstream commitFile(".mygit/objects/" + commitSHA, ios::binary);
    if (commitFile.is_open())
    {
        commitFile.write(compressedCommit.c_str(), compressedCommit.length());
        commitFile.close();
        cout << "Stored commit: " << commitSHA << endl;
    }

    // Update HEAD
    ofstream headOut(".mygit/HEAD");
    headOut << commitSHA;
    headOut.close();

    return commitSHA;
}