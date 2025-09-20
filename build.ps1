g++ sandboxer.cpp -O3 -o main.exe -static-libgcc -static-libstdc++ -m32
pip install Cython
python -m cython writecode.py -o writecode.c
gcc writecode.c -O3 -o writecode.exe -static-libgcc -static-libstdc++ -m32