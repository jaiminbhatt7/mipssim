# mipssim
MIPS simulator

Files:

1. HelloWorld.cpp - source file
2. makemips - make file
3. readme file

The program runs successfully on an Ubuntu Machine with g++ compiler is installed.
lin309-02:165% g++ HelloWorld.cpp -o mipssim
lin309-02:167% ./mipssim inputfile_name outputfile_name dis

To run makefile
lin309-02:169% make -f makemips

The input file shuold be in the same location (unless you want to specify the location) as the object file created.
Output file will be generated in the same location unless specified else where.


