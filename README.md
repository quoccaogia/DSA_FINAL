` 
 #### bash ở repo root
 

(1) gcc -c SQL/sqlite3.c -o sqlite3.o
(2) g++ -std=c++17 -I. backend/api.cpp sqlite3.o -pthread -o api
