'''This program selects images from one collection and saves them to a different folder'''
import os
import sys
from shutil import copy2
from math import isclose
import glob
import pygame

def getImg(filename, size):
    '''Prepares an image and scales it'''
    width, height = size
    image_surface = pygame.image.load(filename)

    #Figure out aspect ratio scaling
    s_width = image_surface.get_width()
    s_height = image_surface.get_height()

    if s_width <= width and s_height <= height:
        # If the image is smaller than our resolution, leave it unchanged
        return image_surface
    elif isclose((width/height), (s_width/s_height)):
        # If we have the same aspect ratio, scale the image up
        return pygame.transform.scale(image_surface, size)
    elif s_width/s_height > width/height:
        # if we landscape image
        new_width = min(s_width, width)
        new_height = min(s_height, s_height/(s_width/new_width))
        size = (int(new_width), int(new_height))
    else:
        # if we have a portrait image
        new_height = min(s_height, height)
        new_width = min(s_width, s_width/(s_height/new_height))
        size = (int(new_width), int(new_height))
    return pygame.transform.scale(image_surface, size)

def getFiles(directory):
    '''Get all the files in a certian directory... Maybe a bit better than listdir'''
    filelist = []
    for filename in glob.iglob(directory + '**/*', recursive=True):
        name = filename.lower()
        extension = name.split('.')[-1]
        if extension in ('bmp', 'jpg', 'png', 'jpeg', 'gif', 'pcx', 'tga', 'lbm', 'pbm', 'pbm', 'pgm', 'ppm', 'xpm'):
            filelist.append(filename)
    return filelist


def copyToDir(path, file):
    """Copies a file to a new directory. If the directory doesn exist,
    create it first"""
    if not os.path.exists(path):
        os.makedirs(path)
    copy2(file, path)

def deleteFile(savepath, origFile):
    """If you changed your mind and want to remove an image from the saved files list"""
    newpath = savepath + '/' + origFile.split('/')[-1]
    if os.path.exists(newpath):
        os.remove(newpath)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage:", sys.argv[0], " imageFolder selectedImagesFolder")
        exit(1)

    #Setup initial pygame values
    pygame.init()
    pygame.event.set_blocked(pygame.MOUSEMOTION) #We don't do mouse motion event so disable them
    screen = pygame.display.set_mode((1920, 1080), pygame.RESIZABLE)
    screenSize = (1920, 1080)

    #Setup initial filelist and locations
    fileidx = 0
    filez = getFiles(sys.argv[1])
    savedFiles = sys.argv[2]
    filez.sort() # Sort filelist for convenience sake
    while True:
        #Display a file
        file = filez[fileidx]
        pygame.display.set_caption("ImageDescriptor - " + file.split('/')[-1])
        img = getImg(file, screenSize)
        screen.fill([1, 1, 1])
        #align the image to the center of the screen and blit it
        alignment = ((screenSize[0] - img.get_width()) // 2, (screenSize[1] - img.get_height()) // 2)
        screen.blit(img, alignment)
        pygame.display.flip()

        #Wait for event
        event = pygame.event.wait()    # Look for any event
        if event.type == pygame.QUIT:  # Window close button clicked?
            break                   #   ... leave game loop
        if event.type == pygame.VIDEORESIZE:
            # There's some code to add back window content here.
            surface = pygame.display.set_mode((event.w, event.h), pygame.RESIZABLE)
            screenSize = (event.w, event.h)
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                if fileidx > 0:
                    fileidx -= 1
            if event.key == pygame.K_RIGHT:
                if fileidx < len(filez) - 1:
                    fileidx += 1
            if event.key == pygame.K_s:
                # Save the file
                copyToDir(savedFiles, file)
            if event.key == pygame.K_d:
                # Unsave the file
                deleteFile(savedFiles, file)
    pygame.quit()
