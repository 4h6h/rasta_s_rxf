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
./isc_client
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
./isc_client
````

## Running it as client or server

### Build as client: 
Configuration in RastaS_Lcfg_client.c

```
mkdir build
cd build
cmake "../" -D"CMAKE_BUILD_TYPE=Debug" -D"USE_TEST_CMOCKA=OFF"
make
```

### Execute as client
```
cd build
./isc_client
```

### Build as server
Configuration in RastaS_Lcfg_server.c

```
mkdir build
cd build
cmake "../" -D"CMAKE_BUILD_TYPE=Debug" -D"USE_TEST_CMOCKA=OFF" -D"USE_CFG_SERVER=ON"
make
```

### Execute as server
```
cd build
./isc_server
```


