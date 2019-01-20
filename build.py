import numpy
from typing import Any

from Cython.Build import cythonize
from distutils.extension import Extension
from Cython.Distutils import build_ext


modules = [
    Extension("shuffle.lib",
              ["shuffle.pyx"],
              language='c++',
              extra_compile_args=['--std=c++0x', '-Wall', '-O3', '-DSHUFFLE_MAX_DATATYPE_SIZE=8', '-DCYTHON_TRACE=1']
              ),
]
extensions = cythonize(modules, language_level='3str',
                       compiler_directives={
                           'binding': True,
                           'linetrace': True
                       })


def build(setup_kwargs: Any) -> None:
    """Needed for the poetry building interface."""

    setup_kwargs.update({
        'ext_modules': cythonize(modules),
        'cmdclass': {'build_ext': build_ext},
        'include_dirs': [
            numpy.get_include()
        ],
    })
