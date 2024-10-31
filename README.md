# MiniGit

MiniGit is a simplified, Git-like version control system implemented in
C++. It offers basic version control features such as repository
initialization, object hashing, file staging, commit creation, and
logging, designed for educational purposes or lightweight file tracking.

## Features

- **Initialize a MiniGit Repository**: Set up a repository in any
  directory.
- **Object Hashing**: Hash file contents, optionally storing them as
  unique blob objects.
- **File Management**: Add and track files, view file contents or
  metadata.
- **Tree Management**: Write directory structures as tree objects.
- **Commit History**: Create and log commits with snapshot details.
- **Version Recovery**: Checkout a commit to restore the project state.

## Prerequisites

- **g++ Compiler**: Ensure you have a C++ compiler like g++ installed.
- **OpenSSL and zlib Libraries**: Used for hashing and compression.

## Compilation and Setup

1.  **Clone the Repository**

    *git clone https://github.com/saurabh6354/MiniGit.git*

    *cd MiniGit*

2.  **Compile with Make**

    - Give executable permissions to Makefile (if needed):

      *chmod +x Makefile*

    - Run the *make* command to compile all source files:

      *make*

    - This will generate an executable named *mygit*.

3.  **Place the Executable**

    - Move the *mygit* executable to your project directory or any
      directory where you want to initialize a MiniGit repository.

4.  **Initialize the Repository**

    *./mygit init*

    This command creates a *.minigit* folder within the directory to
    store repository metadata.

## Commands and Usage

### 1. Initialize Repository {#initialize-repository}

Initializes a new repository in the current directory.

*./mygit init*

### 2. Hash-Object {#hash-object}

Hashes the contents of a file. Optionally saves it as a blob object if
*-w* flag is provided.

*echo -n \"hello world\" \> test.txt*

*./mygit hash-object \[-w\] test.txt*

### 3. Cat-File {#cat-file}

Prints content or metadata of objects stored in the repository.

- **Flags:**

  - *-p*: Prints the actual content.
  - *-s*: Shows the file size in bytes.
  - *-t*: Displays the object type (e.g., blob, tree).

*./mygit cat-file \<flag\> \<object_sha\>*

### 4. Write Tree {#write-tree}

Writes the current directory structure as a tree object.

*./mygit write-tree*

### 5. List Tree (ls-tree) {#list-tree-ls-tree}

Lists the contents of a tree object by SHA.

- **Flags:**

  - *\--name-only*: Displays only the names of files and directories in
    the tree.

*./mygit ls-tree \[\--name-only\] \<tree_sha\>*

### 6. Add Files {#add-files}

Adds files to the staging area for the next commit.

*./mygit add .*

*./mygit add main.cpp utils.cpp*

### 7. Commit Changes {#commit-changes}

Creates a commit with the current contents of the staging area.

*./mygit commit -m \"Commit message\"*

*./mygit commit*

### 8. View Commit Log {#view-commit-log}

Shows the commit history of the repository.

*./mygit log*

### 9. Checkout {#checkout}

Restores files and directory structures to match a specified commit.

*./mygit checkout \<commit_sha\>*

### Cleaning Up

To remove the *mygit* executable, use:

*make clean*

## Example Workflow

- **Initialize repository**

  *./mygit init*

- **Add files to staging**

  *./mygit add file1.txt file2.txt*

- **Commit the changes**

  *./mygit commit -m \"Initial commit\"*

- **View commit history**

  *./mygit log*

- **Restore to an earlier commit**

  *./mygit checkout \<commit_sha\>*

## License

This project is licensed under the MIT License.
