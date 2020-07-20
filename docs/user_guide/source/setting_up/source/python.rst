****************************
Enabling the Python bindings
****************************

Building the Python bindings in ADIOS2 requires Python 2.7 or above, ``numpy``, and ``mpi4py``.

When ``cmake`` is invoked with ``-DADIOS2_USE_Python=ON``, an ``adios2.so`` library containing the Python module is generated in the build directory under ``lib/pythonX.X/site-packages/``
To use this library, make sure your ``PYTHONPATH`` contains the path to ``adios2.so``.
The Python interpreter must have the same version as the interpreter used during for compilation.
The Python tests may be run with ``ctest -R Python``, and a minimal working example may be tested via

    .. code-block:: bash

            $ mpirun -n 4 python helloBPWriter.py
            $ python helloBPWriter.py
