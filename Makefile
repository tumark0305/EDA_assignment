NVCC = nvcc
CC = g++
CFLAGS = -O3 -arch=sm_50
LDFLAGS = -std=c++11 -O3 -g -lm
SRC_CUDA = cuda_main.cu cuda_macro.cu
SRC_CPP = *.cpp modules/*.cpp

OBJ_CUDA = cuda_main.o cuda_macro.o
OBJ_CPP = *.cpp modules/*.cpp
OUTPUT = picRouting

all: $(OUTPUT)
$(OBJ_CUDA): %.o: %.cu
	$(NVCC) $(CFLAGS) -c $< -o $@
$(OBJ_CPP): %.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# 連接 CUDA 和 C++ 物件檔案
$(OUTPUT): $(OBJ_CUDA) $(OBJ_CPP)
	$(CC) $(LDFLAGS) $(OBJ_CUDA) $(OBJ_CPP) -lcudart  -o $(OUTPUT)

clean:
	rm -f $(OBJ_CUDA) $(OBJ_CPP) $(OUTPUT)
run: $(OUTPUT)
	./$(OUTPUT)
