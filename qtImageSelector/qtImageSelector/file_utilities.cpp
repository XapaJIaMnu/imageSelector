#include "file_utilities.h"
namespace fileUtilities {
template<class iterclass>
void populateList(iterclass& iter, std::vector<std::string>& paths,
                  std::vector<std::string>& filenames) {
    //Extensions list
    const std::unordered_set<std::string> extensions = {".bmp", ".gif", ".jpg", ".jpeg",
                                                  ".png", ".pbm", ".pgm", ".ppm",
                                                  ".xbm", ".xpm",
                                                  ".BMP", ".GIF", ".JPG", ".JPEG",
                                                  ".PNG", ".PBM", ".PGM", ".PPM",
                                                  ".XBM", ".XPM"};
    for (auto& p: iter) {
        const fs::path& file(p.path());
        std::string filename = file.filename();
        std::string extension = file.extension();
        std::string filepath = p.path();
        if (!fs::is_directory(filepath) && extensions.find(extension) != extensions.end()){
            paths.push_back(filepath);
            filenames.push_back(filename);
        }
    }
}

void fileWalker (const char * root_dir, std::vector<std::string>& paths,
                 std::vector<std::string>& filenames, bool recursive, std::string& error) {

    try {
        if (recursive) {
            fs::recursive_directory_iterator iter(root_dir);
            populateList(iter, paths, filenames);
        } else {
            fs::directory_iterator iter(root_dir);
            populateList(iter, paths, filenames);
        }
    } catch (fs::filesystem_error & e) {
        error = e.what();
    }
    std::sort(paths.begin(), paths.end());
    std::sort(filenames.begin(), filenames.end());
}

bool checkIfExistOrDir(const char * path) {
    try {
        fs::path pathObj(path);
        // Check if the path exists and it's a directory
        if (fs::exists(pathObj) && fs::is_directory(pathObj))
            return true;
    } catch (fs::filesystem_error & e) {
        std::cerr << e.what() << std::endl;
    }
    return false;
}

template<class iterclass>
bool _checkIfImages(iterclass& iter) {
    const std::unordered_set<std::string> extensions = {".bmp", ".gif", ".jpg", ".jpeg",
                                                  ".png", ".pbm", ".pgm", ".ppm",
                                                  ".xbm", ".xpm",
                                                  ".BMP", ".GIF", ".JPG", ".JPEG",
                                                  ".PNG", ".PBM", ".PGM", ".PPM",
                                                  ".XBM", ".XPM"};
    for (auto& p: iter) {
        const fs::path& file(p.path());

        std::string extension = file.extension();
        std::string filepath = p.path();
        if (!fs::is_directory(filepath) && extensions.find(extension) != extensions.end()){
            return true;
        }
    }
    return false;

}

bool checkIfImagesExist (const char * path, bool recursive, std::string& error) {
    try {
        if (recursive) {
            fs::recursive_directory_iterator iter(path);
            return _checkIfImages(iter);
        } else {
            fs::directory_iterator iter(path);
            return _checkIfImages(iter);
        }
    } catch (fs::filesystem_error & e) {
        error = e.what();
    }
    return false;
}

int checkIfWritable(const char * path) {
    try {
        fs::path pathObj(path);
        // Check if the path exists and it's a directory
        if (fs::exists(pathObj)) {
            if (fs::is_directory(pathObj)) {
                //Try if writable
                pathObj /= "test";
                fs::create_directory(pathObj);
                fs::remove(pathObj);
                return 0;
            } else {
                //Not a directory
                return 1;
            }
        } else {
            fs::create_directory(path); //Test if we can create and remove the directory
            fs::remove(path);
            return 0;
        }
    }
    catch (fs::filesystem_error & e) {
        std::cerr << e.what() << std::endl;
    }
    return 2; //No write permissions

}

} //namespace
