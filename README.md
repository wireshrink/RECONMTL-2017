# RECONMTL-2017, SGX programming: common mistakes presentation and supporting materials

## What's this ?

This is a training target for hacking SGX enclaves, named "damn vulnerable SGX enclave" according to the old good tradition of naming such a things in such a way.
It is presented first time @Recon 2017 conference in Montreal, Canada.

## What do we have inside ?

1. The presentation presentation itself in docs folder
2. Visual Studio 2015 community edition solution ( @reconmtl2017dvse ) with supporting code which include includes 4 projects:
	1. DVSE		- Damn vulnerable SGX enclave itself (which is obviously damn vulnerable, I made a lot of efforts to achieve this)
	2. SecureServer - simple SSL python server for it. It includes self-signed certificate for and private keys, some (AFAIK) public domain cartoons as media examples and configuration files. 
	3. videogui	- reference QT5 based UI for viewing the sealed movies
	4. EnclaveTest - Template utility for exploiting the enclave, which contains exploit of data disclosure.    
	5. SGXOpenSSL - Intels library thath implements usage of OpenSSL inside of enclave. It resides here only for reference to the specific version I worked with because I wasn't able to find it on github.

## What kind of bugs are there ?

Actually almost everything I could think about which is specific to SGX enclaves usage.
See the presentation for more details.

## How to use it ?

1. Check that you're using the right hardware (see the presentation for more details, make sure that you are using the right processor and SGX is enabled by the BIOS)
2. Install Intel SGX SDK, Intel SGX PSW(platform software) and QT5 (I installed QT5 @c:\Qt, probably some pathes in the solution will lead there).
3. Compile the solution in Debug x64 or Prerelease x64 configuration
4. Run the server. ( I run it as ``` {your folder}\RECONMTL-2017\secureServer\secureServer>python .\secureServer.py .\media 9876 .\certs ```)  
5. Run the videogui, the setup dialog will appear.
6. Enter server properties (IP of the machine and the port, the port mentioned in previous command is 9876 and ip is 127.0.0.1 if you are running it on the local machine)
7. Create a folder for the local encrypted media library, enter it to the corresponding and press a button  "Choose a folder and init user"
8. Press open button. Doubleclick on the cartoon you want to watch (there is one for free)
9. If you are able to deduce the value of so called coupon, you can use it, it will increase amount of coins you have (See balance on the setup dialog). If you got this coupon, 
   open the setup dialog (Menu, Library-->connect and setup), paste it into the corresponfing edit box and press apply. Each coupon will add 25 coins, viewing  payed cartoon requires 50.
   There are 5 coupons, (starting with DVSE_CPN_) and these coupons shouldn't be used twice according to the initial intention.

Use, hack, enjoy. 

## Where to get all these components ?

1. [SGX SDK & PSW] (https://software.intel.com/en-us/sgx-sdk/download)
2. [QT5] (https://www.qt.io/download/)

## Something doesn't work or I found a bug in your code !

0. Open an issue of something of the below isn't working. That's quite possible.
1. Some of the mistakes were inserted intentionally. In this case I'll send you a message and remove the issue.

## What are the rules ?

1. The main goal is to write an application which will unseal(decrypt) the movies sealed with the SGX enclave or extract its other secrets. 
   There are at least 5 different ways to do that. 
2. You can use EnclaveTest project as a template for that.

##  Please let me know about your progress.
You can find my e-mail in the presentation in the docs folder. 
##  Good luck. 
   
