
#include <bits/stdc++.h>
#include "mygit.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: ./mygit <command> [options]\n";
        return 1;
    }

    string command = argv[1];

    if (command == "init")
    {
        init();
    }
    else if (command == "hash-object")
    {
        bool write = false;
        string filename;

        if (argc == 4 && string(argv[2]) == "-w")
        {
            write = true;
            filename = argv[3];
        }
        else if (argc == 3)
        {
            filename = argv[2];
        }
        else
        {
            cerr << "Error: Invalid arguments for hash-object.\n";
            return 1;
        }

        hashObject(filename, write);
    }
    else if (command == "cat-file" && argc == 4)
    {
        catFile(argv[2], argv[3]);
    }
    else if (command == "write-tree")
    {
        writeTree(".");
    }
    else if (command == "ls-tree" && argc >= 3)
    {
        bool nameOnly = false;
        string sha;

        // Check if the second argument is the flag
        if (string(argv[2]) == "--name-only")
        {
            nameOnly = true;
            if (argc == 4)
            {
                sha = argv[3];
            }
            else
            {
                cerr << "Error: SHA-1 hash required after --name-only flag.\n";
                return 1;
            }
        }
        else
        {
            sha = argv[2];
            if (argc == 4 && string(argv[3]) == "--name-only")
            {
                nameOnly = true;
            }
        }

        lsTree(sha, nameOnly);
    }
    else if (command == "add")
    {
        if (argc == 3 && string(argv[2]) == ".")
        {
            add({"."});
        }
        else if (argc > 2)
        {
            add({argv + 2, argv + argc});
        }
        else
        {
            cerr << "Error: Invalid arguments for add command.\n";
            return 1;
        }
    }

    else if (command == "commit")
        commit(argc == 4 && string(argv[2]) == "-m" ? argv[3] : "Default commit message");
    else if (command == "log")
        logHistory();
    else if (command == "checkout" && argc == 3)
        checkout(argv[2]);
    else
        cerr << "Unknown command or missing arguments.\n";

    return 0;
}
