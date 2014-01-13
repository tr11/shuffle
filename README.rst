shuffle
=======

.. image:: https://travis-ci.org/tr11/shuffle.png?branch=master
   :target: https://travis-ci.org/tr11/shuffle

Shuffle if a small C++ library with Python bindings designed to shuffle an array in a similar manner to what the Shuffle filter in the HDF5 file format does.
This is particularly useful when compressing arrays of numerical values of similar magnitudes.

For example, in Python::

  import shuffle
  import numpy
  import zlib
  import gzip

  vec = numpy.random.rand(1024*1024)      # 8 MB in a 64bit machine
  print('Vector size:                   %d' % len(bytes(vec.data)))
  print('Compressed size:               %d' % len(gzip.compress(vec.data)))
  print('Compressed size with shuffle:  %d' % len(gzip.compress(shuffle.shuffle_ndarray(vec).data)))
  print('Compressed difference:         %0.2f%% smaller' % (100*(1 - len(gzip.compress(shuffle.shuffle_ndarray(vec).data)) / len(gzip.compress(vec.data)))))

gives results along the lines of::

  Vector size:                  8388608
  Compressed size:              7910645
  Compressed size with shuffle: 7355671
  Compressed difference:        7.02% smaller

