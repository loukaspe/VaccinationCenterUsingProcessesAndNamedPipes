CC := g++
CFLAGS := -Wall -g
TARGET_TRAVELMONITOR := travelMonitor
TARGET_MONITOR := monitor

SRCS_TRAVELMONITOR := $(filter-out monitor.cpp ,$(wildcard *.cpp))
SRCS_MONITOR := $(filter-out main.cpp ,$(wildcard *.cpp))
OBJS_TRAVELMONITOR := $(patsubst %.cpp,%.o, $(SRCS_TRAVELMONITOR))
OBJS_MONITOR := $(patsubst %.cpp,%.o, $(SRCS_MONITOR))

all: $(TARGET_TRAVELMONITOR) $(TARGET_MONITOR)
$(TARGET_TRAVELMONITOR): $(OBJS_TRAVELMONITOR)
	$(CC) -o $@ $^
$(TARGET_MONITOR): $(OBJS_MONITOR)
	$(CC) -o $@ $^
%.o: %.cpp
	$(CC) $(CFLAGS) -c $<
clean:
	rm -rf $(TARGET_TRAVELMONITOR) $(TARGET_MONITOR) *.o
	rm -f pipeTravelMonitor*
	
.PHONY: all clean
