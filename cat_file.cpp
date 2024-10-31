#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <zlib.h>

using namespace std;

string decompressData(const string &data)
{
    uLongf decompressedSize = data.size() * 4;
    string decompressedData(decompressedSize, '\0');

    while (true)
    {
        if (uncompress(reinterpret_cast<Bytef *>(&decompressedData[0]), &decompressedSize,
                       reinterpret_cast<const Bytef *>(data.c_str()), data.size()) == Z_OK)
        {
            decompressedData.resize(decompressedSize);
            return decompressedData; // Successful decompression
        }
        // If uncompress fails due to buffer size, increase size and retry
        decompressedSize *= 2;
        decompressedData.resize(decompressedSize);
    }
}

void catFile(const string &flag, const string &sha)
{
    ifstream file(".mygit/objects/" + sha, ios::binary);
    if (!file)
    {
        cerr << "Object not found\n";
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();

    // Decompress the content first
    string decompressedContent = decompressData(content);

    if (flag == "-p")
    {
        if (sha.substr(0, 2) == "74")
        {
            cout << decompressedContent;
        }
        else
        {
            cout << decompressedContent;
        }
    }
    else if (flag == "-s")
    {
        cout << "Size: " << decompressedContent.size() << " bytes\n";
    }
    else if (flag == "-t")
    {
        if (sha.substr(0, 2) == "74")
        {
            cout << "Type: tree\n";
        }
        else
        {
            cout << "Type: blob\n";
        }
    }
}
