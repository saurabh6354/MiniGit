#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void init()
{
    if (fs::exists(".mygit"))
    {
        cout << "Repository already initialized.\n";
        return;
    }

    // Create necessary directories inside .mygit
    fs::create_directory(".mygit");
    fs::create_directory(".mygit/objects");
    fs::create_directory(".mygit/refs");
    cout << "Initialized empty repository in .mygit/\n";
}
