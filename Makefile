CXX := g++
CXXFLAGS := -std=c++17 -O2 -I. -Iinclude -I/usr/include/eigen3
LDFLAGS :=
GTEST_LDFLAGS := -lgtest -lgtest_main -pthread

TARGET := gauss_csv
OBJS := solver.o main.o

TEST_TARGET := gauss_test
TEST_OBJS := solver.o tests.o

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(GTEST_LDFLAGS)

%.o: %.cpp solver.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f *.o $(TARGET) $(TEST_TARGET)