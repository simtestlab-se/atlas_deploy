# Issue #22: adbms slave 20 device config ecat amd spi

**Repository:** adbms  
**Status:** Closed  
**Created:** 2024-11-26  
**Updated:** 2024-11-29  
**Closed:** 2024-11-28  
**Author:** @crmaarimuthu  
**Assignees:** @crmaarimuthu  
**Labels:** `enhancement`  

[View on GitHub](https://github.com/Simtestlab/adbms/issues/22)

## Description

DOD:ADBMS is a device used to monitor cell voltage. Each slave monitors 16 cells. Currently, we are using 2 slave devices, so it measures 32 cells. We are now converting from 2 devices to 20 devices, allowing us to measure 320 cells. The slave controller is XMC4800.