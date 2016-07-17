TARGET=test
OBJS=proc.o
INCLUDE=proc.c
CC=gcc
CFLAG=-g

$(TARGET):$(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

$(OBJS):$(INCLUDE)
	$(CC) -o $(OBJS) -c $(INCLUDE) $(CFLAG)

clean:
	rm $(TARGET) $(OBJS) -rf

