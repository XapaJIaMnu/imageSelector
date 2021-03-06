# imageSelector
A small program to easily select which images to keep from any existing collection.

This program addresses a common issue that people have when they transfer phone or camera photos on the PC before uploading them to facebook/instagram or sending them out to friends and family. If you went to the beach and shot 100 photos, but you only want to upload a few of them, the selection process is normally very annoying: You have to open an image viewer program, browse to the image and every time you find an image that you like, you have to somehow mark it for upload/keeping/post-processing. Normally that involves remembering the filename, opening a file browser and manually copying that file to a different location... And you have to do that for every single file.

**imageSelector** uses Qt to implement a bare bones image viewer that addresses this particular problem. The program is given two paths: A *source directory* which contains all the images from our beach day and a *target directory* which is the place where we are going to save the images we want to show our friends. Using **leftarrow** and **rightarrow** keys we browse the image collection and using the **s** key we *select* a file, copying it to the *target directory*. In addition we can use the **d** key on an image if we changed our mind and no longer want it among the saved files in *target directory*. In addition to that images can be rotated left and right using the **e** and **w** butons

## Usage
imageSelector has a feature complete GUI but also accepts command line arguments.
```bash
imageSelector --help
Usage: ./imageSelector [options]
A small program to identify images to keep from a collection and discard others.

Options:
  -h, --help                             Displays this help.
  -v, --version                          Displays version information.
  -r, --recursive                        Load images from multiple
                                         subdirectories recursively
  -l, --loop                             After reaching the final image,
                                         advance back to the first one
  -t, --target-directory <tgtDirectory>  The <tgtDirectory> where we will keep
                                         the images.
  -i, --images-directory <directory>     The location of the input image
                                         <directory>.
```
Where *imagesDirectory* is the original directory with many pictures and *selectedImagesDirectory* is the new directory where we are going to keep only the very best of images
### Inputs
- **leftarrow** - move to the next image
- **rightarrow** - move to the previous image
- **s** - save an image to *selectedImagesDirectory*
- **d** - remove an image from *selectedImagesDirectory*. Please note that pressing this key **DOES NOT** remove the image from the original location.
- **w** - rotate an image left
- **e** - rotate an image right

## Requirements:
- Qt >= 5.10 
- C++17 filesystem
- vcrun2015 ([Windows only](https://www.microsoft.com/en-us/download/details.aspx?id=52685))

### Building
You can import the project into QtCreator or build it from command line:
```bash
mkdir build
cd build
qmake-qt5 CONFIG+=release ../imageSelector.pro
make -j5
```
**For compilation on both Mac and Windows, I recommend importing the project in QtCreator, instead of running `qmake-qt5` directly**
### Compiling on Windows
If you are using `mingw`, you don't need to change anything. However there's is currently a bug when using `mingw` and c++17 Filesystem, which will be fixed once gcc 9.0 is released. Until then use the `msvc2017` compiler.

When using `msvc2017`, you need to remove the line
```bash
LIBS += -lstdc++fs
```
from the `imageSelector.pro` and specify a static build in the same file:
```bash
CONFIG += static
```
#### Deployment for Windows
On Windows, this is not enough to produce a standalone exe. To create a directory with all the necessary files, put the `imageSelector.exe` in a new directory and use the `windeployqt.exe` which comes with your Qt installation:
```bash
path/to/QtInstallation/msvc2017_64/bin/windeployqt.exe path/to/imageSelector.exe`
```
Replace `msvc2017_64` with whatever compiler you have. This command will add all Qt dependencies to the folder, which can then be distributed to other users.

### Compiling on Mac
You need to install [Qt framework](https://www.qt.io/download), which depends on the full Xcode installation. However, as of MacOs Mojave, there is still no support for C++17 filesystem in the bundled clang version. The solution is to either compile and install llvm and clang yourself (more difficult) or just use brew to install gcc:
`brew install gcc`
Then load up the project in qtCreator and set up the compilation kit executable to your locally installed gcc-8 executable. Furthermore you need to change the default linker. Then run `qmake` on the project. I could not find how to do that automatically from QtCreator, so instead I changed the `LINK` line in the Makefile generated by qmake to `/usr/local/bin/g++-8`. At this point you can hit compile and run the application from QtCreator.
#### Deployment on Mac
Just Like on Windows, Qt provides a tool to produce a standalone .app Folder:
```bash
/path/to/Qt/5.12.1/clang_64/bin/macdeployqt /path/to/imageSelector.app
```
Sometimes this is not quite enough to get the link path set up correctly on Mac, so you need an [additional step](https://forum.qt.io/topic/77001/macdeployqt-missing-dependancies-when-installing/10):
```bash
install_name_tool -add_rpath "@executable_path/../Frameworks" imageSelector.app/Contents/MacOS/imageSelector
```
You can also manually edit the Makefile and add `-rpath,@executable_path/../Frameworks` to the `LFLAGS`
## Version history
- 1.0 - Initial release

## TODO
- Installer for Windows.
- Better formatted error messages.
- Improved code quality. This is my first Qt application and I'm pretty sure I have done lots of things wrong.
- Rotated images are not saved in a rotated format. There should be an option to do that.
  - Modern cameras include some sort of orientation metadata. If we rotate the image without changing the orientation metadata, other viewer programs will display it wrongly, when rotating those images we need to also change the metadata.
- Application window looks huge on any mac with resolution bellow fullHD and on any retina mac unless you use the native resolution, instead of a scaled resolution.
  - This one is a bit annoying to fix, due to the way HiDPI scaling works on different platforms. By default Windows and Linux only scale font sizes on HiDPI displays, whereas Macs do a blanket scaling of everything. While it is possible to mimic mac's behaviour on Linux and Windows, it is not a default option for the majority of users. A proper solution would involve a specific window size settings for mac. You can read more about it on the Qt developer [forums](https://forum.qt.io/topic/100133/cross-platform-app-ui-in-macos-is-double-in-size-compared-to-the-linux-and-windows-version).
- Improve help overlay and add options to hide all help text by default after the first run.
- UTF8 filenames won't work on Windows
 - MSVC++ compiler doesn't assume UTF-8 strings by default (unlike GCC/Clang), which mangles UTF-8 characters in filenames and folders. This will be fixed by either cross-compiling, once GCC 9 is out or by explicitly adding a UTF-8 conversion in all string operations.
