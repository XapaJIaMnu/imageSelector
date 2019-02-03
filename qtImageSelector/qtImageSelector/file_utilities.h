#ifndef FILEITERATOR_H
#define FILEITERATOR_H
#include <filesystem>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <iostream>

//Try to resolve different FS implementations
#if defined(Q_OS_WIN)
namespace fs = std::experimental::filesystem;
#elif defined(Q_OS_LINUX)
namespace fs = std::filesystem;
#elif defined(Q_OS_MAC)
namespace fs = std::filesystem;
#else
namespace fs = std::filesystem;
#endif
namespace fileUtilities {
template<class iterclass>
void populateList(iterclass& iter, std::vector<std::string>& paths,
                  std::vector<std::string>& filenames);

void fileWalker (const char * root_dir, std::vector<std::string>& paths,
                 std::vector<std::string>& filenames, bool recursive, std::string& error);

bool checkIfExistOrDir(const char * path);

int checkIfWritable(const char * path);

bool checkIfImagesExist (const char * path, bool recursive, std::string& error);
} //namespace

class fileCopier {
public:
    fs::path savedir;
    fileCopier(const char * path) : savedir(path) {
        // create the target folder if it doesn't exist
        if (!fs::is_directory(path) || !fs::exists(path)) {
            fs::create_directory(path);
        }
    }

    void copy(std::string& path) {
        fs::path filepath(path);
        fs::path hypofile = fs::path(savedir.string() + std::string("/") + filepath.filename().string());
        if (!fs::exists(hypofile)) {
            fs::copy(filepath, savedir);
        }
    }

    void uncopy(std::string& path) {
        fs::path origfile(path);
        fs::path hypofile = fs::path(savedir.string() + std::string("/") + origfile.filename().string());
        if (fs::exists(hypofile)) {
            fs::remove(hypofile);
        }
    }
};

#endif // FILEITERATOR_H
