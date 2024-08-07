#ifndef UNIT_TEST_HPP
#define UNIT_TEST_HPP

#include <iostream>
#include <cstring>
#include <gtest/gtest.h>
#include <Kepler/HttpServer/HttpServer.hpp>
#include <filesystem>
#include <unordered_map>
#include <fstream>

class UnitTest : public ::testing::Test
{
protected:
    void SetUp() override
        {
        }
};

#endif