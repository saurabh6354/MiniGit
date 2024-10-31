#include <filesystem>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <openssl/sha.h> // Requires OpenSSL library
#include <iomanip>       // For hexadecimal formatting
#include "mygit.h"       // Include the header file
#include <sys/stat.h>    // For file mode retrieval

namespace fs = std::filesystem;
using namespace std;

// Function to get the file mode in Git's octal string format
string getFileMode(const string &filePath)
{
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) != 0)
    {
        cerr << "Error: Could not get file status for " << filePath << endl;
        return "100644"; // Default to regular file mode if error
    }

    // Handle symbolic links
    if (S_ISLNK(fileStat.st_mode))
    {
        return "120000";
    }

    if (S_ISREG(fileStat.st_mode)) // Check if it's a regular file
    {
        // Handle executable files
        if (fileStat.st_mode & S_IXUSR)
        {
            return "100755"; // Executable file
        }
        else
        {
            return "100644"; // Non-executable regul
        }
    }

    return "100644";
}

string computeFileSHA1(const string &filePath)
{
    ifstream file(filePath, ios::binary);
    stringstream buffer;
    buffer << file.rdbuf();
    string fileContent = buffer.str();
    return computeSHA1(fileContent);
}

string writeTree(const string &path)
{
    stringstream treeContent;

    for (const auto &entry : fs::recursive_directory_iterator(path))
    {
        if (fs::is_regular_file(entry.status()))
        {
            string filePath = entry.path().string();
            string fileName = entry.path().filename().string();

            if (fileName[0] == '.' || filePath.find("/.mygit/") != string::npos)
            {
                continue;
            }

            string fileHash = computeFileSHA1(filePath);
            string fileMode = getFileMode(filePath);

            treeContent << fileMode << " blob " << fileHash << " " << filePath << "\n"; // Use fileMode
        }
    }

    // Compute tree hash
    string treeHash = computeSHA1(treeContent.str());

    // Compress and store tree object
    string compressedTree = compressData(treeContent.str()); // Assume compressData exists
    string treePath = ".mygit/objects/" + treeHash;
    ofstream treeFile(treePath, ios::binary);
    if (treeFile.is_open())
    {
        treeFile.write(compressedTree.c_str(), compressedTree.length());
        treeFile.close();
        cout << "Stored tree object: " << treeHash << endl;
    }
    else
    {
        cerr << "Error: Could not write tree object\n";
        return "";
    }

    return treeHash;
}
