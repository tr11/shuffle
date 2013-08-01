# -*- coding: utf-8 -*-
"""
Shuffle module.
"""

__version__ = (0,0,1)
from .lib import *

try:
  import snappy
  def snappy_shuffle(data, dtypesize=8, blocksize=1024):
      return snappy.compress(shuffle(data, dtypesize, blocksize))

  def snappy_unshuffle(data, dtypesize=8, blocksize=1024):
      return unshuffle(snappy.decompress(data), dtypesize, blocksize)
except ImportError:
  pass