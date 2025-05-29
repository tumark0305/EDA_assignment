***GPU版本***
此程式只能在：實體機/WSL/VM Pro 上執行
Virtual Box不支援顯卡直通
另有CPU版本需要另外找我

***GPU CUDA安裝***
N卡之系統 CUDA toolkit安裝
sudo apt install nvidia-cuda-toolkit
查看nvcc能否運行
nvcc --version

----------------------------
1. 學號：b11007157
2. 姓名：涂竣程
3. 使用之程式語言：C++
4. 使用之編譯平台：Linux GNU g++ / NVIDIA nvcc
5. 檔案壓縮方式: tar zcvf b11007157-p3.tgz b11007157-p3
6. 各檔案說明：
g++ modules：
    b11007157-p3/modules/def_file.cpp                  ：檔案讀寫/資料前處理cpp file
	b11007157-p3/modules/def_file.h                ：檔案讀寫/資料前處理header
	b11007157-p3/modules/global_variable.h         ：管理所有跨檔案變數header
	b11007157-p3/modules/Legalization.cpp          ：合法化過程cpp file
	b11007157-p3/modules/Legalization.h            ：合法化過程header
	b11007157-p3/modules/os_info.cpp               ：管理路徑cpp file
	b11007157-p3/modules/os_info.h                 ：管理路徑header
	b11007157-p3/modules/tqdm.cpp                  ：進度條顯示cpp file
	b11007157-p3/modules/tqdm.h                    ：進度條顯示header
	b11007157-p3/modules/PIC.h                    ：光繞線header
	b11007157-p3/modules/PIC_file.h                    ：光繞線檔案管理header
	b11007157-p3/modules/PIC.cpp                    ：光繞線cpp file
	b11007157-p3/modules/PIC_file.cpp                  ：光繞線檔案管理cpp file
g++ 主程式：
	b11007157-p3/main.cpp			       ：主程式 source code
	b11007157-p3/include.h			       ：未分類程式header
	b11007157-p3/macro.cpp			       ：未分類程式cpp file
nvcc：
	b11007157-p3/cuda_main.cu		：GPU source code
	b11007157-p3/cuda_macro.cu		：cu file
	b11007157-p3/cuda_inlucde.cuh		：cu header file
	
一般：
  	b11007157-p3/Makefile			       ：Makefile
	b11007157-p3/readme.txt			       ：本檔案

7. 編譯方式說明：        	
   主程式：
	   在 b11007157-p3/ 這個資料夾下指令 : make
	   即可在 b11007157-p3 產生 picRouting 的執行檔
	
8. 執行、使用方式說明：
主程式：
   	   compile 完成後，在 b11007157-p3/ 目錄下會產生一個 picRouting 的執行檔
   	   執行檔的命令格式為 :
   	   ./picRouting [input file name] [output file name]

	   ex: ./picRouting pic60x60.in pic60x60.out
9.  輸入：
	將輸入檔案放入b11007157-p3資料夾即可使用
10. 輸出：
loss: 4423 //連接後光損失

