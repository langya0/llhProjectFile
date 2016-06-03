OBJS=main.o
TARGET=test
$(TARGET):$(OBJS)
	@g++ -o $(TARGET) $(OBJS) 

$(OBJS):main.cpp
	@g++ -o $(OBJS) -c main.cpp -g
	@echo "successful!"
clean:
	@rm -f $(TARGET) $(OBJS)

run:
	@./$(TARGET)