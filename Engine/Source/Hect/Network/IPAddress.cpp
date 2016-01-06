///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include "IPAddress.h"

#include <enet/enet.h>

using namespace hect;

IPAddress::IPAddress()
{
}

IPAddress::IPAddress(const char* hostName)
{
    ENetAddress address = { 0, 0 };
    if (enet_address_set_host(&address, hostName) == 0)
    {
        _address = address.host;
    }
}

IPAddress::IPAddress(const std::string& hostName) :
    IPAddress(hostName.c_str())
{
}

IPAddress::IPAddress(uint32_t address) :
    _address(address)
{
}

bool IPAddress::isValid() const
{
    return _address != 0;
}

std::string IPAddress::asString() const
{
    ENetAddress address = { 0, 0 };
    address.host = _address;

    char hostName[128];
    if (enet_address_get_host_ip(&address, hostName, sizeof(hostName)) == 0)
    {
        return hostName;
    }
    else
    {
        return "<unknown>";
    }
}

IPAddress::operator uint32_t() const
{
    return _address;
}