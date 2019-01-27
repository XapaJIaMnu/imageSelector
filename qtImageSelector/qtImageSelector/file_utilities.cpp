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
                 std::vector<std::string>& filenames, bool recursive) {

    if (recursive) {
        fs::recursive_directory_iterator iter(root_dir);
        populateList(iter, paths, filenames);
    } else {
        fs::directory_iterator iter(root_dir);
        populateList(iter, paths, filenames);
    }
    std::sort(paths.begin(), paths.end());
    std::sort(filenames.begin(), filenames.end());

}
} //namespace
