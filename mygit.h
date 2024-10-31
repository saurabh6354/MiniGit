#ifndef MYGIT_H
#define MYGIT_H

#include <string>
#include <vector>
using namespace std;

void init();
string computeSHA1(const string &data);
string compressData(const std::string &data);
vector<string> getBlobHashes();
void createDirectoryIfNotExists(const std::string &dir);
// void restoreTree(const string &treeSHA);
void hashObject(const std::string &filename, bool write);
void catFile(const std::string &objectSHA, const std::string &type);
std::string writeTree(const std::string &path);
void lsTree(const std::string &treeSHA, bool nameOnly);
void add(const std::vector<std::string> &files);
std::string commit(const std::string &message = "Default commit message");
void logHistory();
// void checkout(const std::string &commitSHA);
//  void restoreTree(const string &treeSHA);
void checkout(const std::string &commitSHA);
void restoreTree(const std::string &treeSHA, const std::string &path);
string decompressData(const string &data);
void restoreBlob(const std::string &blobSHA, const std::string &filePath);
#endif
