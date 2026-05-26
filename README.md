# 2D Intestinal Epithelium Chip-Inspired Model

This repository contains the core code for a 2D agent-based model of intestinal epithelial growth in a chip-inspired domain. The model was developed as part of my PhD thesis to investigate epithelial growth regulation, mechanical heterogeneity, and contact inhibition in a simplified organoid-on-a-chip context.

The implementation is based on [Chaste](https://www.cs.ox.ac.uk/chaste/), using a mesh-based epithelial modelling framework adapted from an inherited crypt fission model.

## Thesis context

This repository supports the 2D modelling work presented in my PhD thesis. The model was used as a controlled testbed before moving to the 3D node-based framework.

The main modelling aims are to study:

- chip-inspired epithelial confinement
- cell-type-dependent mechanical properties
- neighbour-dependent contact inhibition
- height-dependent proliferation restriction
- epithelial growth regulation in a 2D setting

## Repository contents

This repository is being prepared for public release. It will contain the core source files needed to run the 2D model and a compact demonstration test.

Planned demo test:

- `test2Ddemo`

## Requirements

- Chaste
- CMake/SCons setup compatible with Chaste projects
- C++ compiler supported by Chaste

## References

- Pitt-Francis et al. (2009). Chaste: a test-driven approach to software development for biological modelling. *Computer Physics Communications*, 180(12), 2452–2471. https://doi.org/10.1016/j.cpc.2009.07.019
- Mirams et al. (2013). Chaste: an open source C++ library for computational physiology and biology. *PLoS Computational Biology*, 9(3), e1002970. https://doi.org/10.1371/journal.pcbi.1002970
- Langlands et al. (2016). Paneth Cell-Rich Regions Separated by a Cluster of Lgr5+ Cells Initiate Crypt Fission in the Intestinal Stem Cell Niche. *PLoS Biology*, 14(6), e1002491. https://doi.org/10.1371/journal.pbio.1002491
- Almet et al. (2018). A multicellular model of intestinal crypt buckling and fission. *Bulletin of Mathematical Biology*, 80(2), 335–359. https://doi.org/10.1007/s11538-017-0377-z
- Montes Olivas (2022). Mathematical and computational modelling of intestinal organoids. PhD thesis, University of Bristol.

## Citation

If you use this code, please cite the associated PhD thesis and the Chaste software.

## License

This project is released under the BSD 3-Clause License.
