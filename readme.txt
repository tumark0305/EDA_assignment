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
5. 檔案壓縮方式: tar zcvf b11007157-p2.tgz b11007157-p2
6. 各檔案說明：
g++ modules：
    b11007157-p2/modules/def_file.cpp                  ：檔案讀寫/資料前處理cpp file
	b11007157-p2/modules/def_file.h                ：檔案讀寫/資料前處理header
	b11007157-p2/modules/global_variable.h         ：管理所有跨檔案變數header
	b11007157-p2/modules/Legalization.cpp          ：合法化過程cpp file
	b11007157-p2/modules/Legalization.h            ：合法化過程header
	b11007157-p2/modules/os_info.cpp               ：管理路徑cpp file
	b11007157-p2/modules/os_info.h                 ：管理路徑header
	b11007157-p2/modules/tqdm.cpp                  ：進度條顯示cpp file
	b11007157-p2/modules/tqdm.h                    ：進度條顯示header
g++ 主程式：
	b11007157-p2/main.cpp			       ：主程式 source code
	b11007157-p2/include.h			       ：未分類程式header
	b11007157-p2/macro.cpp			       ：未分類程式cpp file
nvcc：
	b11007157-p1/cuda_main.cu		：GPU source code
	b11007157-p1/cuda_macro.cu		：cu file
	b11007157-p1/cuda_inlucde.cuh		：cu header file
	
一般：
  	b11007157-p2/Makefile			       ：Makefile
	b11007157-p2/readme.txt			       ：本檔案

7. 編譯方式說明：        	
   主程式：
	   在 b11007157-p2/ 這個資料夾下指令 : make
	   即可在 b11007157-p2 產生 legalizer 的執行檔
	
8. 執行、使用方式說明：
主程式：
   	   compile 完成後，在 b11007157-p2/ 目錄下會產生一個 legalizer 的執行檔
   	   執行檔的命令格式為 :
   	   ./legalizer [cell width] [alpha] [input file name] [output file name]

	   ex: ./legalizer 7 1 case1.def output.def
9.  輸入：
	將輸入檔案放入b11007157-p2資料夾即可使用
10. 輸出：
row,col=60,7 行、列
site_w,site_c=400,2400 site長寬
training with method :abacus 開始訓練abacus/spring
epoch:0    legal: false    , quality: 34211.6 第0次會輸出原圖並保存權重、legal是否合法、quality solution quality
Processing [==============================] 100% current:18/18 - 766.35 it/s 計算跨步進度條
epoch:1    legal: true     , quality: 34212.4 第1次之後輸出上次計算結果
Processing [==============================] 100% current:18/18 - 764.46 it/s
epoch:2    legal: true     , quality: 34212.4 若方塊擁擠多跑幾次會有更佳解
Processing [==============================] 100% current:18/18 - 1004.19 it/s
end training time cost:0.351   計算時間(秒)
best quality=5189.51   選擇最佳解
writing 寫入檔案
~output.gp:end writing. 完成寫入

