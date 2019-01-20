import sys
import shuffle
import pytest
import numpy
from pytest import raises


def test_basic():  # type: ignore
    msg = b"abcdefghijklm" * 1000
    assert shuffle.unshuffle(shuffle.shuffle(msg)) == msg

    
def test_type_error_on_py3k():  # type: ignore
    msg = "abcdefghi" * 1000
    if sys.version_info.major > 2:
        with raises(TypeError):
            shuffle.shuffle(msg)
    else:
        assert shuffle.unshuffle(shuffle.shuffle(msg)) == msg


# @pytest.mark.skipif('not hasattr(shuffle, "snappy_shuffle")')
def test_snappy_basic():  # type: ignore
    msg = b"abcdefghijklm" * 1000
    assert shuffle.snappy_unshuffle(shuffle.snappy_shuffle(msg)) == msg


# @pytest.mark.skipif('not hasattr(shuffle, "snappy_shuffle")')
def test_snappy_type_error_on_py3k():  # type: ignore
    msg = "abcdefghi" * 1000
    if sys.version_info.major > 2:
        with raises(TypeError):
            shuffle.snappy_shuffle(msg)
    else:
        assert shuffle.snappy_unshuffle(shuffle.snappy_shuffle(msg)) == msg


def test_ndarray_basic():  # type: ignore
    vals = numpy.arange(100)
    assert all(shuffle.unshuffle_ndarray(shuffle.shuffle_ndarray(vals)) == vals)
