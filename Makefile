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
smash.o: smash.c commands.h
	$(CXX) $(CXXFLAGS) -c smash.c
signals.o: signals.c signals.h
	$(CXX) $(CXXFLAGS) -c signals.c
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

