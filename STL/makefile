# OBJS=test.o

INCLUDE=afxstd.h Alloc.h Config.h Construct.h IteratorTraitsBase.h IteratorTraits.h Trace.h TypeTraits.h Vector.h\
 Uninitialized.h Threads.h
TARGET=test

# $(TARGET):$(OBJS)
# 	@g++ $(OBJS) -o $(TARGET)

$(TARGET):test.cpp $(INCLUDE)
	@g++ test.cpp $(INCLUDE) -o $(TARGET) -g
	@echo "successful!"


clean:
	@rm -rf $(TARGET) $(OBJS)

run:
	@./$(TARGET)