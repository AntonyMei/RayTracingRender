cmake .
make clean
make
./Project 60
rm 1.ppm
rm 1.jpg
./packager 60
rm *.partial
python convert.py