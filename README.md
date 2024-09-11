# Running the code

## Running tests using cmocka

### Prerequisite:
It needs test framework cmocka installed on your computer.

### Build:
```
mkdir build  
cd build  
cmake "../" -D"CMAKE_BUILD_TYPE=Debug"  
make  
```

### Execute:
```
cd build  
./isc  
```

## Running it standalone

### Build:
````
mkdir build
cd build
cmake "../" -D"CMAKE_BUILD_TYPE=Debug" -D"USE_TEST_CMOCKA=OFF"
make
````

### Execute:
````
cd build
./isc
````


