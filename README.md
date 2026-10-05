#### HOW TO RUN?

##### Bước 1:
    Mở terminal (powershell gì gì đó)
    cd tới root của repo
##### Bước 2:
    Nếu có sẵn test data (lưu dưới dạng scholarship.db) thì nhét vô root repo.
    Không thì chạy chương trình rồi nhạp dữ liệu vào.
#### Bước 3:
    Bash:
    g++ -std=c++17 -O2 benchmark_auto.cpp ../SQL/sqlite3.o -I . -I ../SQL -o bench_auto
    bench_auto
        

   



 


