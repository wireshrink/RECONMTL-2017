/************************************************************************************************************
*	This application is a TRAINING TARGET for exercises in HACKING Intel® SGX ENCLAVES.                     *
*	This application made vulnerable DELIBERATELY - its main purpose is to demonstrate, shame and blame     *
*   common mistakes usually made with SGX enclave programming.                                              *
*   ONCE AGAIN, IT CONTAINS MISTAKES.                                                                       *
*   Please DO NOT USE it's source for any healthy development/production activity.                          *
*	It is intended to use for training of hacking SGX enclaves ONLY.                                        *
*	It is written ugly(deliberately), designed badly(intentionally) and running very slow.		        	*
*	You can freely use this application for training purposes.												*
*	I'd be glad to hear about your progress.    															*
*																											*
*	This application requires QT5.8 (which uses LGPL v3 license), Intel SGX SDK and							*
*   the Intel® Software Guard Extensions SSL (Intel® SGX SSL) to be compiled.								*
*	This application is written by Michael Atlas (wireshrink@gmail.com) during 2017.						*
*	Happy hacking.																							*
*************************************************************************************************************/


This is a training target for hacking SGX enclaves, named "damn vulnerable SGX enclave" according to old good tradition of naming such a things in such a way.
It is presented first time @Recon 2017 conference in Montreal, Canada.
The visual studio solution for all related projects resides at reconmtl2017dvse folder.
It includes 4 projects:

1 - DVSE		- Damn vulnerable SGX enclave itself
2 - SecreServer - simple SSL python server for it. 
	It includes self-signed certificate for and private keys, some (AFAIK) public domain cartoons as media examples and configuration files. 
3 - videogui	- reference QT5 based UI for viewing the sealed movies
4 - EnclaveTest - Template utility for exploiting the enclave, which contains exploit of data disclosure.    



