Prerequisites
==============

the following programs should be installed to have full functionality of the interface example. If the user is only interested in interfacing Geant4 with Garfield++, the installation of Degrad is not mandatory as it is used as an external build at runtime. On the other hand, it is not possible to build the project without a working version of Garfield++.

Geant4
--------

Geant4 is used to simulate the primary interaction of the particle with the gas medium and for tracking the particles through the rest of the detector setup. The author used version 10.4p02 of Geant4_. Hence, it is recommended to use a version at least as recent, since some parts of the interface code are not supported by previous versions.

.. _Geant4: http://geant4.web.cern.ch/support/download

Garfield++
------------

Garfield_ simulates the secondary interactions in the gas such as ionization, electron drift, signal creation, etc... The author used revision 567. More recent versions should work as well.

.. _Garfield: http://garfieldpp.web.cern.ch/garfieldpp/

Degrad
--------

Degrad_ is the most detailed software package on the market regarding the interaction of particles with a gas medium. It includes a variety of processes subsequent to the primary ionization among which are, Auger, Coster-Kronig, shake-off and fluorescence emission. As mentioned above, the user can run the Interface project without an installation of Degrad, e.g. if the user is not interested in running a detailed simulation of ionisation and subsequent de-excitation processes.

.. _Degrad: http://magboltz.web.cern.ch/magboltz/

Other
--------

Installation of the following packages and libraries are required to build and/or run the Interface project. Which part of the project needs the particular prerequisite is mentioned in brackets.

   - A C++11 compiler
   - A fortran compiler, e.g. GFortran_ (for Degrad and Garfield++)
   - The ROOT_ framework (to build Garfield++)
   - python2.7 or python3x (for the Degrad part of the interface code)

.. _GFortran: https://gcc.gnu.org/wiki/GFortranBinaries
.. _ROOT: https://root.cern.ch/

