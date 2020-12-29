cmake_minimum_required(VERSION 2.8.9)

include(ExternalProject)

# Optionen an den Compiler
set(CXX_FLAGS
	-std=c++14               # Verwende den C++14 Standard
	-pedantic                # Achte auf genauere Einhaltung des Standards
	-Wall                    # Aktiviere alle Warnungen
)

# Diese Datei ist so vorkonfiguriert, dass sie auf den Bioinformatik
# CIPs problemlos funktioniert. Sollten Sie Ihren Code auf einem anderen
# Rechner testen wollen müssen sie einen aktuellen Compiler installiert
# haben (clang >= 3.5 || gcc >= 4.9) und cmake folgendermaßen aufrufen
# cmake .. -DUSE_CIPS=OFF -DGTEST_SRC_DIR=/path/to/googletest
option(USE_CIPS "Are we compiling on the Bioinformatics CIPs?" ON)

if(USE_CIPS)
	set(INSTALLER_PATH /installer/import/debian/c++)
	set(CMAKE_C_COMPILER ${INSTALLER_PATH}/clang/bin/clang)
	set(CMAKE_CXX_COMPILER ${INSTALLER_PATH}/clang/bin/clang++)
	set(CIP_STD_LIBS c++ c++abi m c gcc_s gcc)
	list(APPEND CXX_FLAGS -stdlib=libc++)
	link_directories(${INSTALLER_PATH}/libcxx/lib)
endif()

project(06_ILP)

include_directories(${PROJECT_SOURCE_DIR}/lpSolve)

# Erstelle das Programm "shortestPath".
add_executable(shortestPath
	main.cpp
)

# Damit convert Ihren Code verwenden kann muss
# die sequence Bibliothek zum Programm "gelinkt" werden.
target_link_libraries(shortestPath ${PROJECT_SOURCE_DIR}/lpSolve/liblpsolve55.so ${CIP_STD_LIBS})

