#!/bin/bash
FILE_NAME_INPUT="main"
OUTPUT_DIRECTORY="build"
echo "Building ..."
mkdir "./$OUTPUT_DIRECTORY"
pdflatex -output-directory=$OUTPUT_DIRECTORY --shell-escape $FILE_NAME_INPUT
makeindex "./$OUTPUT_DIRECTORY/$FILE_NAME_INPUT.idx" -s "./StyleInd.ist"
biber -aux-directory=$OUTPUT_DIRECTORY main
pdflatex -output-directory=$OUTPUT_DIRECTORY --shell-escape $FILE_NAME_INPUT

echo "Moving pdf ..."
mv "./$OUTPUT_DIRECTORY/$FILE_NAME_INPUT.pdf" "./$FILE_NAME_INPUT.pdf"
