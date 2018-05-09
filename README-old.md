# InfoBoard
## DF RTU info board

## How to start

### 1) Install openVG  
     sudo apt-get install libjpeg8-dev indent libfreetype6-dev ttf-dejavu-core  
     git clone git://github.com/ajstarks/openvg  
     cd openvg  
     make  
     sudo make install  
     cd client  
     make test  
### 2) Install libpng:  
     sudo apt-get install libpng-dev  
### 3) Install cURL:  
     sudo apt-get install libcurl4-openssl-dev  
### 4) Compile project:  
     cd InfoBoard  
     make -f makefile  
     ./infoboard  
