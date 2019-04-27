.. _macro-label:

Macro Commands
====================

A few custom macro commands were implemented for a more convenient usage. Some of these commands are common for the two examples, these are handled below, whereas other commands are specific for one of the two setups, these are treated afterwards.

Common commands
---------------------------

There is one command linked to the *DetectorConstruction*-class (implemented in the *DetectorMessenger*-class) and setting the gas pressure:

.. code-block:: cpp

   /InterfaceExample/geometry/SetGasPressure <pressure + unit>
   
The *PhysicsList* class has a custom messenger class, *PhysicsListMessenger*. It contains a few commands to set cut values on the particle range and the particle energy:
   
.. code-block:: cpp
   
   /<setup>/phys/setGCut
   /<setup>/phys/setPCut
   /<setup>/phys/setECut
   /<setup>/phys/setCuts
   /<setup>/phys/setLowLimitE

Where <setup> should be replaced with either *ALICE* or *Xenon*, depending on which of the two examples is run. The first three commands set the range cut values for gamma's, protons and electrons, respectively, whereas the fourth one sets a common range cut for all particle types. The final command sets a lower threshold on the production energy for electrons as explained in :ref:`physicslist-label` and is especially important when the *HeedDeltaElectronModel* is used. Another macro command, which is important for the *G4VFastSimulationModel*, is the following

.. code-block:: cpp
   
   /<setup>/phys/AddParametrisation

This will call the *AddParametrisation*-method in the *PhysicsList* as already explained in :ref:`physicslist-label`. Keep in mind that this command should be run before the */run/initialize*-command. The final macro command for this class is

.. code-block:: cpp

   /<setup>/phys/InitializePhysics <name of physics list to be used>

This will set which physics list is to be used to model electromagnetic processes. By default this is the *G4EmLivermorePhysics*-class. The following physics lists are available at the moment:

.. table:: Available physics lists
   :align: center

   +-------------------------------+-------------------------+
   |      *G4PhysicsList*          |     command string      | 
   +===============================+=========================+
   | *G4EmLivermorePhysics*        |    emlivermore          | 
   +-------------------------------+-------------------------+
   | *PhysListEmStandard*          |    local                | 
   +-------------------------------+-------------------------+
   | *G4EmStandardPhysics*         |    emstandard_opt0      | 
   +-------------------------------+-------------------------+
   | *G4EmStandardPhysics_option1* |    emstandard_opt1      | 
   +-------------------------------+-------------------------+
   | *G4EmStandardPhysics_option2* |    emstandard_opt2      | 
   +-------------------------------+-------------------------+
   | *G4EmStandardPhysics_option3* |    emstandard_opt3      | 
   +-------------------------------+-------------------------+
   | *G4EmPenelopePhysics*         |    empenelope           | 
   +-------------------------------+-------------------------+
   | *G4EmStandardPhysics*  +      |    ionGasModels         | 
   | *AddIonGasModels()*           |                         | 
   +-------------------------------+-------------------------+

In addition to these custom common commands, the Geant4 code also provides a useful messenger class for parametrisation applications, i.e. *G4FastSimulationMessenger.cc*. Two useful commands activate/deactivate the *G4VFastSimulationModels* that are implemented in the user's application:

.. code-block:: cpp

   /param/InActivateModel <model name>
   /param/ActivateModel <model name>
   
Where <model name> is the name given to the model constructor.

ALICE commands
----------------------------

Each of the two user-cases has a third messenger class *GasModelParametersMessenger*, which is connected to the class *GasModelParameters* and enables the user to set some parameters with respect to the applied *G4VFastSimulationModel*'s. For the ALICE setup there are several commands regarding applicable particle types and energy ranges, as well as configuration parameters for the gas detector, such as the voltages on the electrodes. The former is accomodated by the following command:

.. code-block:: cpp

   /gasModelParameters/heed/<heedmodel>/addparticle <particle type> < E min> <E max>
   
<heedmodel> is the name of one of the two models, *HeedDeltaElectronModel* and *HeedNewTrackModel*, respectively denoted by 'heeddeltaelectron' and 'heednewtrack'. <particle type> is the Geant4 default name for the particle type for which the model should be applicable, e.g. 'e-' for an electron, and <E min>  and <E max> are the lower and higher energy threshold between which the model should be triggered.

The following listed commands are not that crucial to the interface, but can be useful for testing and visualization

.. list-table:: Macro commands
   :header-rows: 1
   :widths: 7 7 60
   
   *  -  Command
      -  Argument type
      -  Description and usage
   *  -  /gasModelParameters/heed/gasfile
      -  string
      -  full or relative path of the gas file name generated by Magboltz 
   *  -  /gasModelParameters/heed/ionmobilityfile
      -  string
      -  Name of the ion mobility file
   *  -  /gasModelParameters/heed/drift
      -  boolean
      -  Only simulate drift processes when true
   *  -  /gasModelParameters/heed/driftRKF
      -  boolean
      -  Use Runge-Kutta integration to simulate the drift when true
   *  -   /gasModelParameters/heed/trackmicroscopic
      -  boolean
      -  Use a microscopic tracking method to simulate the drift when true, is overwritten by *driftRKF*
   *  -   /gasModelParameters/heed/createAval
      -  boolean
      -  In case both *driftRKF* and *trackmicroscopic* are false, a monte-carlo method is used. If set to true, also avalanches are calculated by this method.
   *  -  /gasModelParameters/heed/visualizechamber
      -  boolean
      -  If true, visualize the chamber with Garfield++
   *  -  /gasModelParameters/heed/visualizesignals
      -  boolean
      -  If true, visualize the signal created on the readout electrodes, calculated by Garfield++
   *  -  /gasModelParameters/heed/visualizefield
      -  boolean
      -  If true, visualize the electric field calculated by Garfield++(in 2D)
   *  -  /gasModelParameters/heed/voltageplanehv
      -  double
      -  Voltage on the high voltage electrode on one side of the TPC
   *  -  /gasModelParameters/heed/voltageplanelow
      -  double
      -  Voltage on the low voltage electrode on the other side of the TPC
   *  -  /gasModelParameters/heed/voltageanodewire
      -  double
      -  Voltage on the anode wires
   *  -  /gasModelParameters/heed/voltagecathodewire
      -  double
      -  Voltage on the cathode wires
   *  -  /gasModelParameters/heed/voltagegate
      -  double
      -  Average voltage on the gating wires
   *  -  /gasModelParameters/heed/voltagedeltagate
      -  double
      -  Amplitude of the alternating component of the voltage on the gating wires

Xenon commands
--------------------------

Only one command is connected to the example with the Xenon-gas optical TPC:

.. code-block:: cpp
   
   /gasModelParameters/degrad/thermalenergy <energy + unit>

Where the argument is the energy at which Geant4 should treat the electron as thermal. Below this value the *GarfieldVUVPhotonModel* is triggered, which drifts the electron towards the photocathode.
