# Issue #20: usb serial via xmc cell voltage receiving

**Repository:** adbms  
**Status:** Open  
**Created:** 2024-11-21  
**Updated:** 2024-12-03  
**Author:** @crmaarimuthu  
**Assignees:** @crmaarimuthu  

[View on GitHub](https://github.com/Simtestlab/adbms/issues/20)

## Description

DOD :ADBMS is a device used to monitor cell voltage. Each slave monitors 16 cells. Currently, we are using 2 slave devices, so it measures 32 cells. We are now converting from 2 devices to 12 devices, allowing us to measure 192 cells. The slave controller is XMC4800.

working with USART (USB)