# CS Roofline Toolkit #

Welcome to the CS Roofline Toolkit Bitbucket site.  The Empirical Roofline Tool is currently available here.  In the future additional tools will be hosted here.  Please see the [repository Wiki](https://bitbucket.org/berkeleylab/cs-roofline-toolkit/wiki/Home) for additional information.

For an overview of the Roofline Performance Model and this software's place in that context see [https://crd.lbl.gov/departments/computer-science/performance-and-algorithms-research/research/roofline/](https://crd.lbl.gov/departments/computer-science/performance-and-algorithms-research/research/roofline/).

## Empirical Roofline Tool ##

The Empirical Roofline Tool, ERT, automatically generates a roofline datafor a given computer.  This includes the maximum bandwidth for the various levels of the memory hierarchy and the maximum gflop rate.  This data is obtained using a variety of "micro-kernels".

The ERT comes with a set of configuration files for a number of computers/architectures.  These configuration file can be adapted to your local environment and needs to better measure the roofline parameters of your computer(s).

This is version 0.8.0 of the ERT which represents an early beta-release.

For details about the ERT, please refer to the User's Manual in the repository under the "Empirical_Roofline_Tool" directory.

## Contact Information ##

Please contact [Terry Ligocki](mailto:tjligocki@lbl.gov) with any questions, problems, corrections, suggestions, etc.