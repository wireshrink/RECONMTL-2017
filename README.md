# RECONMTL-2017, SGX programming: common mistakes presentation and supporting materials

## What's this ?

This is a training target for hacking SGX enclaves, named "damn vulnerable SGX enclave" according to old good tradition of naming such a things in such a way.
It is presented first time @Recon 2017 conference in Montreal, Canada.

## What do we have inside ?

1. The presentation presentation itself in docs folder
2. Visual Studio 2015 community edition solution ( @reconmtl2017dvse ) with supporting code which include includes 4 projects:
	1. DVSE		- Damn vulnerable SGX enclave itself (which is obviously damn vulnerable, I made a lot of efforts to achieve this)
	2. SecureServer - simple SSL python server for it. It includes self-signed certificate for and private keys, some (AFAIK) public domain cartoons as media examples and configuration files. 
	3. videogui	- reference QT5 based UI for viewing the sealed movies
	4. EnclaveTest - Template utility for exploiting the enclave, which contains exploit of data disclosure.    
	5. SGXOpenSSL - Intels library thath implements usage of OpenSSL inside of enclave. It resides here only for reference to the specific version I worked with because I wasn't able to find it on github.

## What kind of bugs are there there ?

Actually almost everything I could think about which is specific to SGX enclaves usage.
See the presentation for more details.

## How to use it ?

