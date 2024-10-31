#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <zlib.h>

using namespace std;

string computeSHA1(const string &data)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(data.c_str()), data.size(), hash);

    ostringstream oss;
    oss << hex << setfill('0');
    for (auto byte : hash)
        oss << setw(2) << static_cast<int>(byte);
    return oss.str();
}

void createDirectoryIfNotExists(const string &dir)
{
    struct stat info;
    if (stat(dir.c_str(), &info) != 0)
        mkdir(dir.c_str(), 0777);
}

string compressData(const string &data)
{
    uLongf compressedSize = compressBound(data.size());
    string compressedData(compressedSize, '\0');

    if (compress(reinterpret_cast<Bytef *>(&compressedData[0]), &compressedSize,
                 reinterpret_cast<const Bytef *>(data.c_str()), data.size()) != Z_OK)
    {
        cerr << "Error: Compression failed.\n";
        return "";
    }
    compressedData.resize(compressedSize);
    return compressedData;
}

void hashObject(const string &filename, bool write)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    string sha1 = computeSHA1(content);

    if (write)
    {
        createDirectoryIfNotExists(".mygit/objects");
        string compressedContent = compressData(content);
        if (compressedContent.empty())
            return;

        ofstream objectFile(".mygit/objects/" + sha1, ios::binary);
        if (objectFile.is_open())
        {
            objectFile.write(compressedContent.c_str(), compressedContent.length());
            objectFile.close();
            cout << "Stored object: " << sha1 << endl;
        }
        else
        {
            cerr << "Error: Could not write to repository.\n";
        }
    }

    cout << "SHA-1: " << sha1 << "\n";
}
