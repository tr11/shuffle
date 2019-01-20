# -*- coding: utf-8 -*-
"""
Shuffle module.
"""

__version__ = (0, 0, 2)
from .lib import shuffle, unshuffle, shuffle_ndarray, unshuffle_ndarray     # noqa: F401

__all__ = [
    'shuffle',
    'unshuffle',
    'shuffle_ndarray',
    'unshuffle_ndarray'
]


try:
    import snappy

    def snappy_shuffle(data: bytes, dtypesize: int = 8, blocksize: int = 1024) -> bytes:
        return snappy.compress(shuffle(data, dtypesize, blocksize))  # type: ignore

    def snappy_unshuffle(data: bytes, dtypesize: int = 8, blocksize: int = 1024) -> bytes:
        return unshuffle(snappy.decompress(data), dtypesize, blocksize)  # type: ignore
except ImportError:  # pragma: no cover
    pass
