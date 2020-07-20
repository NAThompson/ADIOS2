*********
ADIOS
*********

The ``adios2::ADIOS`` component is the initial contact point between an application and the ADIOS2 library.
This component is created by passing a runtime configuration file and an MPI communicator.

.. code-block:: c++

    adios2::ADIOS adios("config.xml", MPI_COMM_WORLD);

``adios2::ADIOS`` objects can be created in MPI or serial mode.

**Constructors for MPI applications**

.. code-block:: c++

    // version that accepts an optional runtime adios2 config file
    adios2::ADIOS(const std::string configFile,
                  MPI_COMM mpiComm = MPI_COMM_SELF);

    adios2::ADIOS(MPI_COMM mpiComm = MPI_COMM_SELF);

    /** Examples */
    adios2::ADIOS adios(MPI_COMM_WORLD);
    adios2::ADIOS adios("config.xml", MPI_COMM_WORLD);

**Constructors for serial applications**

.. code-block:: c++

    adios2::ADIOS(const std::string configFile);

    adios2::ADIOS();

    /** Examples */
    adios2::ADIOS adios("config.xml");
    adios2::ADIOS adios; // Do not use () for empty constructor.


The ``adios2::ADIOS`` object is the factory of ``adios2::IO`` objects.
Multiple ``adios::IO`` objects can be created from within the scope of an ``adios::ADIOS`` object by calling ``DeclareIO``:

.. code-block:: c++

    /** Signature */
    adios2::IO& ADIOS::DeclareIO(const std::string ioName);

    /** Examples */
    adios2::IO& bpWriter = adios.DeclareIO("BPWriter");
    adios2::IO& bpReader = adios.DeclareIO("BPReader");

The ``ioName`` string must be unique; declaring two ``adios2::IO`` objects with the same name will throw an exception.
These names are used to identify IO components in the runtime configuration file; see :ref:`Runtime Configuration Files` for details.

As shown in the diagram below, each ``adios2::IO`` object is self-managed and independent, thus providing an adaptable way to perform different kinds of I/O operations.
Users must be careful not to create conflicts between system level unique I/O identifiers: file names, IP address and port, MPI Send/Receive message rank and tag, etc.

.. blockdiag::

    blockdiag {
        default_fontsize = 18;
        default_shape = roundedbox;
        default_linecolor = blue;
        span_width = 150;

        ADIOS -> IO_1, B, IO_N[label = "DeclareIO", fontsize = 13];
        B[shape = "dots"];
        ADIOS -> B[style = "none"];
    }

.. tip::

    The ``adios2::ADIOS`` object is the only ``adios2`` object directly owned by the application.
    All other components of the ADIOS2 API are managed through the ``adios2::ADIOS`` object (*e.g.* ``IO``, ``Operator``) or indirectly through the ``IO`` components (*e.g.* ``Variable``, ``Engine``).
