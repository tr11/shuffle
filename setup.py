# -*- coding: utf-8 -*-
"""
"""

import setuptools
from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import numpy
import sys
import warnings



DISTNAME = 'shuffle'
LICENSE = 'BSD'
AUTHOR = "Tiago Requeijo"
EMAIL = "tiago.requeijo.dev@gmail.com"
URL = "https://github.com/tr11/shuffle"
DOWNLOAD_URL = ''
#CLASSIFIERS = [
    #'Development Status :: 4 - Beta',
    #'Environment :: Console',
    #'Operating System :: OS Independent',
    #'Intended Audience :: Science/Research',
    #'Programming Language :: Python',
    #'Programming Language :: Python :: 2',
    #'Programming Language :: Python :: 3',
    #'Programming Language :: Cython',
    #'Topic :: Scientific/Engineering',
#]

MAJOR = 0
MINOR = 0
MICRO = 1
ISRELEASED = False
VERSION = '%d.%d.%d' % (MAJOR, MINOR, MICRO)
QUALIFIER = ''

DESCRIPTION = ""
LONG_DESCRIPTION = ""

FULLVERSION = VERSION
if not ISRELEASED:
    FULLVERSION += '.dev'
    try:
        import subprocess
        try:
            pipe = subprocess.Popen(["git", "rev-parse", "--short", "HEAD"],
                                    stdout=subprocess.PIPE).stdout
        except OSError:
            # msysgit compatibility
            pipe = subprocess.Popen(["git.cmd", "rev-parse", "--short", "HEAD"],
                                    stdout=subprocess.PIPE).stdout
        rev = pipe.read().strip()
        # makes distutils blow up on Python 2.7
        if sys.version_info[0] >= 3:
            rev = rev.decode('ascii')

        FULLVERSION += "-%s" % rev
    except:
        warnings.warn("WARNING: Couldn't get git revision")
else:
    FULLVERSION += QUALIFIER



setup(name=DISTNAME,
      version=FULLVERSION,
      maintainer=AUTHOR,
      cmdclass = {'build_ext': build_ext},
      packages = ['shuffle',
                 ],
      ext_modules = [Extension("shuffle.lib", 
                               ["shuffle.pyx"],
                               language='c++',
                               extra_compile_args= ['--std=c++0x', '-Wall', '-O3', '-DSHUFFLE_MAX_DATATYPE_SIZE=8']
                               )],
      include_dirs = [numpy.get_include()],
      maintainer_email=EMAIL,
      description=DESCRIPTION,
      license=LICENSE,
      url=URL,
      #download_url=DOWNLOAD_URL,
      #long_description=LONG_DESCRIPTION,
      #classifiers=CLASSIFIERS,
      platforms='any',
      test_suite = "nose.collector",
)