#### HOW TO RUN?

##### Bước 1:
    Mở terminal (powershell gì gì đó)
    cd tới root của repo
##### Bước 2:
    Nếu có sẵn test data (lưu dưới dạng scholarship.db) thì nhét vô root repo.
    Bash:
        gcc -c SQL/sqlite3.c -o sqlite3.o
        g++ -std=c++17 -I. backend/api.cpp sqlite3.o -pthread -o api

    Sau khi compile sẽ được 2 file "api" và "sqlite3.o"

##### Bước 3:
    Chạy mọi thứ:
        bash: ./api
    Vào visual chuột phải lên file main.html, chọn "open with intergated brownser"

 


