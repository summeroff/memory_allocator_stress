## USE

Launch from console. 
It will ask for count of allocations and for size of allocation. 
After that it will launch some thread and each thread will allocate entered amount memory. 

## BUILD 
```mkdir build 
cd build 
cmake -G "Visual Studio 15 2017 Win64"  ../   
cmake --build . --target ALL_BUILD --config Release -- /p:CharacterSet=Unicode```

