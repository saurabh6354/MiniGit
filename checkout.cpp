#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <zlib.h>
#include "mygit.h"

namespace fs = std::filesystem;
using namespace std;

void checkout(const string &commitSHA)
{
    string commitPath = ".mygit/objects/" + commitSHA;
    ifstream commitFile(commitPath, ios::binary);
    if (!commitFile.is_open())
    {
        cerr << "Error: Commit " << commitSHA << " does not exist.\n";
        return;
    }

    stringstream buffer;
    buffer << commitFile.rdbuf();
    string compressedContent = buffer.str();
    commitFile.close();

    string commitContent = decompressData(compressedContent);
    if (commitContent.empty())
    {
        cerr << "Error: Decompression failed for commit.\n";
        return;
    }

    stringstream ss(commitContent);
    string line, treeSHA;
    while (getline(ss, line))
    {
        if (line.find("tree ") == 0)
        {
            treeSHA = line.substr(5);
            break;
        }
    }

    if (treeSHA.empty())
    {
        cerr << "Error: Invalid commit format\n";
        return;
    }

    restoreTree(treeSHA, ".");
    ofstream headFile(".mygit/HEAD");
    headFile << commitSHA;
    headFile.close();
}

void restoreTree(const string &treeSHA, const string &path)
{
    string treeFilePath = ".mygit/objects/" + treeSHA;
    ifstream treeFile(treeFilePath, ios::binary);

    if (!treeFile.is_open())
    {
        cerr << "Error: Tree " << treeSHA << " does not exist.\n";
        return;
    }

    stringstream buffer;
    buffer << treeFile.rdbuf();
    string compressedContent = buffer.str();
    treeFile.close();

    string treeContent = decompressData(compressedContent);
    if (treeContent.empty())
    {
        cerr << "Error: Failed to decompress tree object.\n";
        return;
    }

    stringstream treeStream(treeContent);
    string line;
    while (getline(treeStream, line))
    {
        stringstream lineStream(line);
        string mode, type, sha1, filename;
        lineStream >> mode >> type >> sha1;
        getline(lineStream, filename);
        filename = filename.substr(1);

        if (type == "blob")
        {
            restoreBlob(sha1, path + "/" + filename);
        }
    }
}

void restoreBlob(const string &sha1, const string &filename)
{
    string objectPath = ".mygit/objects/" + sha1;
    ifstream objectFile(objectPath, ios::binary);
    if (!objectFile.is_open())
    {
        cerr << "Error: Object " << sha1 << " does not exist.\n";
        return;
    }

    stringstream buffer;
    buffer << objectFile.rdbuf();
    string compressedContent = buffer.str();
    objectFile.close();

    uLongf decompressedLength = compressedContent.length() * 5;
    string decompressedContent(decompressedLength, '\0');

    while (true)
    {
        int result = uncompress(
            reinterpret_cast<Bytef *>(&decompressedContent[0]),
            &decompressedLength,
            reinterpret_cast<const Bytef *>(compressedContent.c_str()),
            compressedContent.length());

        if (result == Z_OK)
        {
            decompressedContent.resize(decompressedLength);
            break;
        }
        else if (result == Z_BUF_ERROR)
        {
            decompressedLength *= 2;
            decompressedContent.resize(decompressedLength);
        }
        else
        {
            cerr << "Error: Failed to decompress object " << sha1 << "\n";
            return;
        }
    }

    fs::path filePath(filename);
    if (filePath.has_parent_path())
    {
        fs::create_directories(filePath.parent_path());
    }

    ofstream outFile(filename, ios::binary);
    if (!outFile.is_open())
    {
        // cerr << "Error: Could not create file " << filename << "\n";
        return;
    }

    outFile.write(decompressedContent.c_str(), decompressedContent.length());
    outFile.close();
}
