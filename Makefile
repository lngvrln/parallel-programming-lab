CXX = g++
CXXFLAGS = -O2 -std=c++17 -pthread

all: serial parallel4 parallel8

serial: main.cpp
	$(CXX) $(CXXFLAGS) -DSERIAL main.cpp -o serial

parallel4: main.cpp
	$(CXX) $(CXXFLAGS) -DPARALLEL -DTHREADS=4 main.cpp -o parallel4

parallel8: main.cpp
	$(CXX) $(CXXFLAGS) -DPARALLEL -DTHREADS=8 main.cpp -o parallel8

# Цель для проверки: собирает все варианты, запускает и печатает результат
full: serial parallel4 parallel8
	@echo "===== SERIAL ====="
	@./serial
	@echo "===== PARALLEL, 4 потока ====="
	@./parallel4
	@echo "===== PARALLEL, 8 потоков ====="
	@./parallel8

clean:
	rm -f serial parallel4 parallel8

.PHONY: all full clean
