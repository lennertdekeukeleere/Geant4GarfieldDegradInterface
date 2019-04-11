#!/usr/bin/env python
import os
import re


WORKING_DIRECTORY = os.getcwd()
print WORKING_DIRECTORY
for files in os.listdir(WORKING_DIRECTORY):
    if files.endswith(".OUT"):
        # replace all space characters by tab
        infile = open(files, "r")
        outfile = open("tempFile.dat", "w")
        replacements = {' ': '\t'}
        print files
        print "replacing lines..."
        for line in infile:
            for src, target in replacements.iteritems():
                line = line.replace(src, target)
            outfile.write(line)
        infile.close()
        outfile.close()
        # replace all double tab by tab
        infile = open("tempFile.dat", "r")
        outfile = open(files, "w")
        replacements = {'\t\t': '\t'}
        for line in infile:
            while '\t\t' in line:
                for src, target in replacements.iteritems():
                    line = line.replace(src, target)
            line = line[1:]  # remove o primeiro caracter de cada linha
            outfile.write(line)
        infile.close()
        outfile.close()
        os.system("rm tempFile.dat")
print "Replacing Finished"
