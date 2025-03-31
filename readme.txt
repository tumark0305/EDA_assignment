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
5. 檔案壓縮方式: tar zcvf b11007157-p1.tgz b11007157-p1
6. 各檔案說明：
g++：
	b11007157-p1/main.cpp			：主程式 source code
	b11007157-p1/include.h			：header file
	b11007157-p1/macro.cpp			：cpp file
nvcc：
	b11007157-p1/cuda_main.cu		：GPU source code
	b11007157-p1/cuda_macro.cu		：cu file
	b11007157-p1/cuda_inlucde.cuh		：cu header file
一般：
  	b11007157-p1/Makefile			：Makefile
	b11007157-p1/readme.txt			：本檔案

7. 編譯方式說明：        	
   主程式：
	   在 b11007157-p1/ 這個資料夾下指令 : make
	   即可在 b11007157-p1 產生 genPlot 的執行檔
	
8. 執行、使用方式說明：
主程式：
   	   compile 完成後，在 b11007157-p1/ 目錄下會產生一個 genPlot 的執行檔
   	   執行檔的命令格式為 :
   	   ./genPlot [MSB CS width] [MSB CS height] [input file name] [output file name]

	   ex: ./genPlot 7100 6600 CS12x12.def output.gp
9.  輸入：
	將輸入檔案放入b11007157-p1資料夾即可使用
10. 輸出：
	當前路徑
	10個隨機小數
	Run ON: \\你使用的GPU型號
	排序後的小數
	---end writing.
	
	會生成一個output.gp 預設生成2K大小圖片
	結束

