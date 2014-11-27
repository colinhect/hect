///////////////////////////////////////////////////////////////////////////////
// This source file is part of Zeroth.
//
// Copyright (c) 2014 Colin Hill
//
///////////////////////////////////////////////////////////////////////////////
#include <Hect/Core/Configuration.h>
#include <Hect/Runtime/Platform.h>
#include <Hect/Runtime/Engine.h>

#ifdef HECT_WINDOWS_BUILD
#ifdef HECT_DEBUG_BUILD
#include <vld.h>
#endif
#endif

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

hect::Engine* engine = nullptr;

int main(int argc, char* const argv[])
{
    try
    {
        // Use hardcoded command-line arguments for the engine to use
        char configPath[] = "HectSystemTests.config";
        char* const engineArgv[] =
        {
            argv[0],
            configPath
        };

        hect::Engine engine(2, engineArgv);
        ::engine = &engine;

        return Catch::Session().run(argc, argv);
    }
    catch (hect::Error& error)
    {
        HECT_ERROR(error.what());
    }

    return EXIT_SUCCESS;
}