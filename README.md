# 2D Intestinal Epithelium Chip-Inspired Model

This repository contains the core code for a 2D agent-based model of intestinal epithelial growth in a chip-inspired domain. The model was developed as part of my PhD thesis to investigate epithelial growth regulation, mechanical heterogeneity, and contact inhibition in a simplified organoid-on-a-chip context.

The implementation is based on [Chaste](https://www.cs.ox.ac.uk/chaste/), using a mesh-based epithelial modelling framework adapted from an inherited crypt fission model.

The initial Chaste project framework was based on work by Axel A. Almet, with later modifications by Sandra Montes-Olivas. Yue Wang substantially adapted and extended this framework for a 2D chip-inspired intestinal epithelial model.

## Thesis context

This repository supports the 2D modelling work presented in my PhD thesis. The model was used as a controlled testbed before moving to the 3D node-based framework.

The main modelling aims are to study:

- chip-inspired epithelial confinement
- cell-type-dependent mechanical properties
- neighbour-dependent contact inhibition
- height-dependent proliferation restriction

## Getting the code and installing dependencies

Before running this model, install Chaste using the official installation guides:

- https://chaste.cs.ox.ac.uk/trac/wiki/InstallGuides/InstallGuide
- https://chaste.cs.ox.ac.uk/trac/wiki/InstallGuides/UbuntuPackage

Ubuntu or an Ubuntu virtual machine is recommended. Chaste is primarily supported on Linux and Unix systems, so Windows or macOS users may need to use a virtual machine, WSL, or Docker.

As an optional environment, Chaste Docker images are available at:

- https://github.com/Chaste/chaste-docker

After installing Chaste, clone this repository into the projects folder under your Chaste source tree:

```sh
cd <Chaste_source_path>/projects
git clone https://github.com/Penguin365/intestinal-epithelium-2d-chip-model.git ChipFrame2D
```

## Repository contents

This repository is prepared as a compact demo and open-source version of the 2D chip-inspired model. It contains only the core files required to run the demonstration tests.

- src contains the custom classes needed to run the 2D simulations, including model-specific cell-cycle rules, contact-inhibition rules, cell killers, modifiers, writers, boundary conditions, and helper classes.
- test contains the demonstration tests.

The main demo files are:

- test/TestChipFrame_2D_AP_demo.hpp
- test/TestChipFrame_2D_NH_demo.hpp

TestChipFrame_2D_AP_demo.hpp is a 2D demo with modified proliferation regulation and cell-killer-based regulation.

TestChipFrame_2D_NH_demo.hpp is a 2D demo with modified proliferation regulation, neighbour-number-dependent contact inhibition, and height-dependent proliferation restriction.

## Running the demo

From your Chaste build directory, compile and run the demos:

```sh
cd <Chaste_build_path>
make -j4 TestChipFrame_2D_AP_demo
make -j4 TestChipFrame_2D_NH_demo
ctest -j4 -V -R TestChipFrame_2D
```

You can also run either demo test individually:

```sh
ctest -j4 -V -R TestChipFrame_2D_AP_demo
ctest -j4 -V -R TestChipFrame_2D_NH_demo
```

Depending on the Chaste build configuration, the exact target or test name may need to be adjusted.

## Requirements

- Chaste
- CMake/SCons setup compatible with Chaste projects
- C++ compiler supported by Chaste

## References

- Pitt-Francis et al. (2009). Chaste: a test-driven approach to software development for biological modelling. *Computer Physics Communications*, 180(12), 2452-2471. https://doi.org/10.1016/j.cpc.2009.07.019
- Mirams et al. (2013). Chaste: an open source C++ library for computational physiology and biology. *PLoS Computational Biology*, 9(3), e1002970. https://doi.org/10.1371/journal.pcbi.1002970
- Langlands et al. (2016). Paneth Cell-Rich Regions Separated by a Cluster of Lgr5+ Cells Initiate Crypt Fission in the Intestinal Stem Cell Niche. *PLoS Biology*, 14(6), e1002491. https://doi.org/10.1371/journal.pbio.1002491
- Almet et al. (2018). A multicellular model of intestinal crypt buckling and fission. *Bulletin of Mathematical Biology*, 80(2), 335-359. https://doi.org/10.1007/s11538-017-0377-z
- Montes Olivas (2022). Mathematical and computational modelling of intestinal organoids. PhD thesis, University of Bristol.

## Citation

If you use this code, please cite the associated PhD thesis and the Chaste software.

## License

This project is released under the BSD 3-Clause License.
