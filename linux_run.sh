cmake .
make clean
make
./Project 80
rm 1.ppm
rm 1.jpg
./packager 80
rm *.partial
python convert.py