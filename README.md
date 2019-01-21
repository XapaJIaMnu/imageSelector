# imageSelector
A small program to easily select which images to keep from any existing collection.

This program addresses a common issues that people have when they transfer phone or camera photos on the PC before uploading them to facebook/instagram or sending them out to friends and family. If you went to the beach and shot 100 photos, but you only want to upload a few of them, the selection process is normally very annoying: You have to open an image viewer program, browse to the image and every time you find an image that you like, you have to somehow mark it for upload/keeping/post-processing. Normally that involves remembering the filename, opening a file browser and manually copying that file to a different location... And you have to do that for every single file.

**imageSelector** uses pygame to implement a bare bones image viewer that addresses this particular problem. The program is given two paths: A *source directory* which contains all the images from our beach day and a *target directory* which is the place where we are going to save the images we want to show our friends. Using **leftarrow** and **rightarrow** keys we browse the image collection and using the **s** key we *select* a file, copying it to the *target directory*. In addition we can use the **d** key on an image if we changed our mind and no longer want it among the saved files in *target directory*.

## Requirements:
- python >= 3.5 
- pygame
## Usage
```bash
python imageSelector.py imagesDirectory selectedImagesDirectory
```
Where *imagesDirectory* is the original directory with many pictures and *selectedImagesDirectory* is the new directory where we are going to keep only the very best of images
### Inputs
- **leftarrow** - move to the next image
- **rightarrow** - move to the previous image
- **s** - save an image to *selectedImagesDirectory*
- **d** - remove an image from *selectedImagesDirectory*. Please note that pressing this key **DOES NOT** remove the image from the original location.

## Version history
- 0.1 - Initial release

## TODO
- GUI dialog in addition to command line arguments for selecting directories.
- Exe file for ease of use for windows users
- Maybe something else
