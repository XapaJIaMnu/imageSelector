#ifndef FILEITERATOR_H
#define FILEITERATOR_H
#include <filesystem>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <iostream>

//Try to resolve different FS implementations
#if defined(Q_OS_WIN)
namespace fs = std::filesystem;
#elif defined(Q_OS_LINUX)
namespace fs = std::filesystem;
#elif defined(Q_OS_MAC)
namespace fs = std::filesystem;
#else
namespace fs = std::filesystem;
#endif

class fileIterator {
public:
std::vector<std::string> paths;
std::vector<std::string> filenames;
const std::unordered_set<std::string> extensions = {".bmp", ".gif", ".jpg", ".jpeg",
                                              ".png", ".pbm", ".pgm", ".ppm",
                                              ".xbm", ".xpm",
                                              ".BMP", ".GIF", ".JPG", ".JPEG",
                                              ".PNG", ".PBM", ".PGM", ".PPM",
                                              ".XBM", ".XPM"};

private:
    template<class iterclass>
    void populateList(iterclass& iter) {
        for (auto& p: iter) {
            fs::path file(p.path());
            std::string filename = file.filename();
            std::string extension = file.extension();
            std::string filepath = p.path();
            if (!fs::is_directory(filepath) && extensions.find(extension) != extensions.end()){
                paths.push_back(filepath);
                filenames.push_back(filename);
            }
        }
    }

public:
    fileIterator(const char * root_dir, bool recursive = false) {
        if (recursive) {
            fs::recursive_directory_iterator iter(root_dir);
            populateList(iter);
        } else {
            fs::directory_iterator iter(root_dir);
            populateList(iter);
        }
        std::sort(paths.begin(), paths.end());
        std::sort(filenames.begin(), filenames.end());
    }

};

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
