CC=g++
CFLAGS=-Wall -g -pthread -std=c++11
SRCDIRECTORY=./src/
INCLUDEDIRECTORY=./include/
RESOURCEDIRECTORY=./resources/
# define any directories containing header files other than /usr/include
INCLUDES=-I$(INCLUDEDIRECTORY)
LIBS=`pkg-config --cflags --libs opencv`
MAIN=final
SRCS=main.cpp $(SRCDIRECTORY)perspectiveCorrection.cpp $(SRCDIRECTORY)transform.cpp $(SRCDIRECTORY)queueDensity.cpp $(SRCDIRECTORY)opticalFlow.cpp
OBJS=$(SRCS:.cpp=.o)

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LIBS)

main.o: main.cpp $(INCLUDEDIRECTORY)angleCorrection.hpp $(INCLUDEDIRECTORY)densityEstimation.hpp
	$(CC) $(CFLAGS) $(INCLUDES) -c main.cpp $(LIBS)

$(SRCDIRECTORY)perspectiveCorrection.o : $(SRCDIRECTORY)perspectiveCorrection.cpp $(INCLUDEDIRECTORY)angleCorrection.hpp $(INCLUDEDIRECTORY)densityEstimation.hpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCDIRECTORY)perspectiveCorrection.cpp $(LIBS) -o $(SRCDIRECTORY)perspectiveCorrection.o

$(SRCDIRECTORY)transform.o : $(SRCDIRECTORY)transform.cpp $(INCLUDEDIRECTORY)angleCorrection.hpp $(INCLUDEDIRECTORY)densityEstimation.hpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCDIRECTORY)transform.cpp $(LIBS) -o $(SRCDIRECTORY)transform.o

$(SRCDIRECTORY)queueDensity.o : $(SRCDIRECTORY)queueDensity.cpp $(INCLUDEDIRECTORY)angleCorrection.hpp $(INCLUDEDIRECTORY)densityEstimation.hpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCDIRECTORY)queueDensity.cpp $(LIBS) -o $(SRCDIRECTORY)queueDensity.o

$(SRCDIRECTORY)opticalFlow.o : $(SRCDIRECTORY)opticalFlow.cpp $(INCLUDEDIRECTORY)angleCorrection.hpp $(INCLUDEDIRECTORY)densityEstimation.hpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCDIRECTORY)opticalFlow.cpp $(LIBS) -o $(SRCDIRECTORY)opticalFlow.o

clean:
	rm -rf *.o $(SRCDIRECTORY)*.o $(MAIN)
#$(RESOURCEDIRECTORY)*_projected* $(RESOURCEDIRECTORY)*_cropped* 
