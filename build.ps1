g++ sandboxer.cpp -O3 -o main.exe -static-libgcc -static-libstdc++ -m64
pip install Cython
pip install pybind11
pip install setuptools wheel
python -m cython writecode.py -o writecode.c
gcc writecode.c -O3 -o writecode.exe -municode -emainCRTStartup -static-libgcc -static-libstdc++ -IC:/Users/Bogdan/AppData/Local/Programs/Python/Python313/include -LC:/Users/Bogdan/AppData/Local/Programs/Python/Python313/libs -lpython313 -m64