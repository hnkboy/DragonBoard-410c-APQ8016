#CROSS_COMPILE = gcc
LD = $(CROSS_COMPILE)gcc
CC = $(CROSS_COMPILE)gcc
TARGET = test_app   #目标文件名

SRCS = $(wildcard ./*.c)
export SRCS
include ./framework/framework.mk
include ./infrastructure/infrastructure.mk
include ./scenarios/scenarios.mk
include ./application/zigbee/zigbee.mk
OBJS := $(SRCS:.c=.o)
INCLUDES =-I ./include
LIBS=-L ./ -lpthread -lcurl -lasound -pthread -lrt -lmosquitto
CCFLAGS=-g -Wall -O0
%.o: %.c
	$(CC) -o $@ $(INCLUDES) $(CCFLAGS) -c $<
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

clean:
	-rm $(OBJS) $(TARGET)
