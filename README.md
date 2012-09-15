ELEN4002-Lab-Project
====================

Wits University Laboratory Project 2012

Instructions:
- There are presently two SSVEP classifier types: PSDAclassifier and MECclassifier
- The classifier type can be changed by manually instantiating the type in main.cpp
- The SSVEP target frequency can be changed in main.cpp.
- Simulated/test data can be used instead of live data. Test data is initialised in Signal::loadTestData(). To use test data, call the EmoController loop function with the parameter '1' 