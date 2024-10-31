#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "mygit.h"

namespace fs = std::filesystem;

using namespace std;
void updateIndex(const string &filename, const string &sha1)
{
    vector<pair<string, string>> entries;
    bool updated = false;

    ifstream indexFile(".mygit/index");
    string line;
    while (getline(indexFile, line))
    {
        stringstream ss(line);
        string fname, hash;
        ss >> fname >> hash;
        if (fname == filename)
        {
            entries.push_back({filename, sha1});
            updated = true;
        }
        else
        {
            entries.push_back({fname, hash});
        }
    }
    indexFile.close();

    // Add new entry if not updated
    if (!updated)
    {
        entries.push_back({filename, sha1});
    }

    // Write back the index
    ofstream indexOut(".mygit/index");
    for (const auto &entry : entries)
    {
        indexOut << entry.first << " " << entry.second << "\n";
    }
    indexOut.close();
}

void addFileToIndex(const string &filename)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    // Read file content
    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    // Calculate hash before compression
    string sha1 = computeSHA1(content);

    // Compress content
    string compressedContent = compressData(content);
    if (compressedContent.empty())
    {
        cerr << "Error: Failed to compress file content\n";
        return;
    }

    // Store the compressed object
    string objectPath = ".mygit/objects/" + sha1;
    ofstream objectFile(objectPath, ios::binary);
    if (!objectFile.is_open())
    {
        cerr << "Error: Could not create object file\n";
        return;
    }
    objectFile.write(compressedContent.c_str(), compressedContent.length());
    objectFile.close();

    // Update index
    updateIndex(filename, sha1);

    cout << "Successfully stored object " << sha1 << " for " << filename << "\n";
}

void add(const vector<string> &files)
{
    for (const auto &file : files)
    {
        if (fs::is_directory(file))
        {
            for (const auto &entry : fs::directory_iterator(file))
            {
                addFileToIndex(entry.path().string());
            }
        }
        else if (fs::exists(file))
        {
            addFileToIndex(file);
        }
        else
        {
            cerr << "Error: " << file << " does not exist.\n";
        }
    }
}
