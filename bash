sudo apt-get update
sudo apt-get install wiringpi
//sau khi cài, biên dịch và chạy chương trình bằng câu lệnh
gcc -o main main.c -lwiringPi -lpthread
sudo ./main

