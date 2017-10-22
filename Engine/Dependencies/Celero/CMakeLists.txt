project(Celero CXX)

add_library(Celero STATIC
    "include/celero/Archive.h"
    "include/celero/Benchmark.h"
    "include/celero/Callbacks.h"
    "include/celero/Celero.h"
    "include/celero/CommandLine.h"
    "include/celero/Console.h"
    "include/celero/Distribution.h"
    "include/celero/Executor.h"
    "include/celero/Export.h"
    "include/celero/Factory.h"
    "include/celero/FileReader.h"
    "include/celero/GenericFactory.h"
    "include/celero/JUnit.h"
    "include/celero/Pimpl.h"
    "include/celero/PimplImpl.h"
    "include/celero/Print.h"
    "include/celero/Experiment.h"
    "include/celero/Result.h"
    "include/celero/ResultTable.h"
    "include/celero/Statistics.h"
    "include/celero/TestFixture.h"
    "include/celero/ThreadLocal.h"
    "include/celero/ThreadTestFixture.h"
    "include/celero/TestVector.h"
    "include/celero/Timer.h"
    "include/celero/Utilities.h"
    "src/Archive.cpp"
    "src/Benchmark.cpp"
    "src/Callbacks.cpp"
    "src/Celero.cpp"
    "src/Console.cpp"
    "src/Distribution.cpp"
    "src/Executor.cpp"
    "src/JUnit.cpp"
    "src/Print.cpp"
    "src/Experiment.cpp"
    "src/Result.cpp"
    "src/ResultTable.cpp"
    "src/Statistics.cpp"
    "src/TestVector.cpp"
    "src/TestFixture.cpp"
    "src/ThreadTestFixture.cpp"
    "src/Timer.cpp"
    "src/Utilities.cpp"
    )

target_include_directories(Celero PUBLIC "${PROJECT_SOURCE_DIR}/include")

if(MSVC)
    target_compile_options(Celero PRIVATE /W0 /D_CRT_SECURE_NO_WARNINGS /wd4251 /nologo)
else()
    target_compile_options(Celero PRIVATE -w -fPIC -std=c++11 -std=gnu++11)
endif()

target_compile_definitions(Celero PRIVATE CELERO_STATIC)

set_target_properties(Celero PROPERTIES
    PROJECT_LABEL Celero
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE CXX
    )
