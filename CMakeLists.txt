cmake_minimum_required(VERSION 3.16)
project(jnp1_moneybag)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(jnp1_moneybag
        moneybag.cc
        moneybag.h
        moneybag_example.cc)
