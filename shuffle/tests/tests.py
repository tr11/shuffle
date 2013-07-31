# -*- coding: utf-8 -*-
"""
Created on Fri Jun 15 21:01:49 2012

@author: tiago
"""

import sys
import shuffle
import unittest
import numpy
from numpy.testing import assert_array_equal

class TestShuffle(unittest.TestCase):

    def test_basic(self):
        msg = b"abcdefghijklm" * 1000
        self.assertEqual(shuffle.unshuffle(shuffle.shuffle(msg)), msg)
        
    def test_type_error_on_py3k(self):
        msg = "abcdefghi" * 1000
        if sys.version_info.major > 2:
          self.assertRaises(TypeError, shuffle.shuffle, msg)
        else:
          self.assertEqual(shuffle.unshuffle(shuffle.shuffle(msg)), msg)

    def test_snappy_basic(self):
        msg = b"abcdefghijklm" * 1000
        self.assertEqual(shuffle.snappy_unshuffle(shuffle.snappy_shuffle(msg)), msg)
        
    def test_snappy_type_error_on_py3k(self):
        msg = "abcdefghi" * 1000
        if sys.version_info.major > 2:
          self.assertRaises(TypeError, shuffle.snappy_shuffle, msg)
        else:
          self.assertEqual(shuffle.snappy_unshuffle(shuffle.snappy_shuffle(msg)), msg)

    def test_ndarray_basic(self):
        vals = numpy.arange(100)
        assert_array_equal(shuffle.unshuffle_ndarray(shuffle.shuffle_ndarray(vals)), vals)



if __name__ == '__main__':
    unittest.main()