# RECONMTL-2017

This is a training target for hacking SGX enclaves, named "damn vulnerable SGX enclave" according to old good tradition of naming such a things in such a way.
It is presented first time @Recon 2017 conference in Montreal, Canada.
The visual studio solution for all related projects resides at reconmtl2017dvse folder.
It includes 4 projects:

1 - DVSE		- Damn vulnerable SGX enclave itself
2 - SecreServer - simple SSL python server for it. 
	It includes self-signed certificate for and private keys, some (AFAIK) public domain cartoons as media examples and configuration files. 
3 - videogui	- reference QT5 based UI for viewing the sealed movies
4 - EnclaveTest - Template utility for exploiting the enclave, which contains exploit of data disclosure.    



