# Makefile for the smash program
# need to add -Werror
CXX = g++
CXXFLAGS = -std=c++11 -Wall    -pedantic-errors -DNDEBUG
CCLINK = $(CXX)
OBJS = smash.o commands.o signals.o
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CXX) $(CXXFLAGS) -o smash $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.h
	$(CXX) $(CXXFLAGS) -c commands.cpp
smash.o: smash.cpp commands.h
	$(CXX) $(CXXFLAGS) -c smash.cpp
signals.o:  signals.cpp signals.h  
	$(CXX) $(CXXFLAGS)  -c signals.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

