ELEN4002-Lab-Project
====================

Wits University Laboratory Project 2012

Instructions:
- There are three SSVEP classifier types: PSDAclassifier, MECclassifier and CCAclassifier
- The classifier type can be changed by specifying instantiating the type in main.cpp
- Parameters such as use of zero padding, windowing functions and SSVEP target frequency can be changed in main.cpp.
- Simulated/test data can be used instead of live data. Test data is initialised in Signal::loadTestData(). To use test data, call the EmoController loop function with the parameter '1' 