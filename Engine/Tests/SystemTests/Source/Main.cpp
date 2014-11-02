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
        hect::Engine engine(argc, argv);
        ::engine = &engine;
        return Catch::Session().run(1, argv);
    }
    catch (hect::Error& error)
    {
        HECT_ERROR(error.what());
    }

    return EXIT_SUCCESS;
}