# -*- coding: utf-8 -*-

import numpy
cimport numpy
from libc.stdint cimport uint16_t, uint8_t


cdef extern from "shuffle.hpp" namespace "Shuffle":
    void shuffle_p "Shuffle::shuffle"     (unsigned char* data, 
                                           unsigned char* target, 
                                           size_t data_sz,
                                           uint16_t dtypesize,
                                           uint16_t blocksize)
    void unshuffle_p "Shuffle::unshuffle" (unsigned char* data, 
                                           unsigned char* target, 
                                           size_t data_sz,
                                           uint16_t dtypesize,
                                           uint16_t blocksize)


def shuffle_ndarray(numpy.ndarray data, blocksize=1024):
    # dtype size
    sz = data.dtype.itemsize
    # allocate a new buffer
    cdef numpy.ndarray[numpy.int8_t] data_buff = data.view(numpy.int8)
    cdef numpy.ndarray[numpy.int8_t] target_buff = numpy.ndarray(data_buff.size, dtype=numpy.int8)
    # interpret the buffers as char arrays
    cdef unsigned char* dbuff = <unsigned char*> data_buff.data     
    cdef unsigned char* tbuff = <unsigned char*> target_buff.data     
    # shuffle
    shuffle_p(dbuff, tbuff, len(data_buff), data.dtype.itemsize, blocksize)
    return target_buff.view(dtype=data.dtype)


def shuffle(bytes data, uint16_t dtypesize=32, uint16_t blocksize=1024)  -> bytes:
    cdef long sz = len(data) 
    target = bytes(bytearray(sz))
    cdef unsigned char* dbuff = <unsigned char*> data
    cdef unsigned char* tbuff = <unsigned char*> target
    # shuffle
    shuffle_p(dbuff, tbuff, sz, dtypesize, blocksize)
    return target


def unshuffle_ndarray(numpy.ndarray data, blocksize=1024):
    # dtype size
    sz = data.dtype.itemsize
    # allocate a new buffer
    cdef numpy.ndarray[numpy.int8_t] data_buff = data.view(numpy.int8)
    cdef numpy.ndarray[numpy.int8_t] target_buff = numpy.ndarray(data_buff.size, dtype=numpy.int8)
    # interpret the buffers as char arrays
    cdef unsigned char* dbuff = <unsigned char*> data_buff.data     
    cdef unsigned char* tbuff = <unsigned char*> target_buff.data     
    # unshuffle
    unshuffle_p(dbuff, tbuff, len(data_buff), data.dtype.itemsize, blocksize)
    return target_buff.view(dtype=data.dtype)


def unshuffle(bytes data, uint16_t dtypesize=32, uint16_t blocksize=1024) -> bytes:
    cdef long sz = len(data) 
    target = bytes(bytearray(sz))
    cdef unsigned char* dbuff = <unsigned char*> data
    cdef unsigned char* tbuff = <unsigned char*> target
    # shuffle
    unshuffle_p(dbuff, tbuff, sz, dtypesize, blocksize)
    return target
