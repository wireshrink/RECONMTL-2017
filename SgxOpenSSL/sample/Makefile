####### SGX SDK Settings ########

SGX_SDK ?= /opt/intel/sgxsdk
SGX_MODE ?= SIM
SGX_ARCH ?= x64

ifeq ($(shell getconf LONG_BIT), 32)
	SGX_ARCH := x86
else ifeq ($(findstring -m32, $(CXXFLAGS)), -m32)
	SGX_ARCH := x86
endif

ifeq ($(SGX_ARCH), x86)
	SGX_COMMON_CFLAGS := -m32
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib
	SGX_ENCLAVE_SIGNER := $(SGX_SDK)/bin/x86/sgx_sign
	SGX_EDGER8R := $(SGX_SDK)/bin/x86/sgx_edger8r
else
	SGX_COMMON_CFLAGS := -m64
	SGX_LIBRARY_PATH := $(SGX_SDK)/lib64
	SGX_ENCLAVE_SIGNER := $(SGX_SDK)/bin/x64/sgx_sign
	SGX_EDGER8R := $(SGX_SDK)/bin/x64/sgx_edger8r
endif

ifeq ($(SGX_DEBUG), 1)
ifeq ($(SGX_PRERELEASE), 1)
$(error Cannot set SGX_DEBUG and SGX_PRERELEASE at the same time!!)
endif
endif

ifeq ($(SGX_DEBUG), 1)
        SGX_COMMON_CFLAGS += -O0 -g
else
        SGX_COMMON_CFLAGS += -O2
endif

######## App Settings ########

ifneq ($(SGX_MODE), HW)
	Urts_Library_Name := sgx_urts_sim
else
	Urts_Library_Name := sgx_urts
endif

App_Cpp_Files := $(wildcard test_vrfcert/*.cpp)
App_Include_Paths := -I$(SGX_SDK)/include

App_Compile_CFlags := -fPIC -Wno-attributes $(App_Include_Paths)
# Three configuration modes - Debug, prerelease, release
#   Debug - Macro DEBUG enabled.
#   Prerelease - Macro NDEBUG and EDEBUG enabled.
#   Release - Macro NDEBUG enabled.
ifeq ($(SGX_DEBUG), 1)
        App_Compile_CFlags += -DDEBUG -UNDEBUG -UEDEBUG
else ifeq ($(SGX_PRERELEASE), 1)
        App_Compile_CFlags += -DNDEBUG -DEDEBUG -UDEBUG
else
        App_Compile_CFlags += -DNDEBUG -UEDEBUG -UDEBUG
endif

App_Compile_CXXFlags := -std=c++11 $(App_Compile_CFlags)
App_Link_Flags := -L$(SGX_LIBRARY_PATH) -l$(Urts_Library_Name) -lpthread

ifneq ($(SGX_MODE), HW)
	App_Link_Flags += -lsgx_uae_service_sim
else
	App_Link_Flags += -lsgx_uae_service
endif

App_Cpp_Objects := $(App_Cpp_Files:.cpp=.o)

App_Name := vrfcert_app

####### topenssl Library settings #######
Topenssl_Lib_Name := libcrypto.a

######## Enclave Settings ########

ifneq ($(SGX_MODE), HW)
	Trts_Library_Name := sgx_trts_sim
	Service_Library_Name := sgx_tservice_sim
	Crypto_Library_Name := sgx_tcrypto
else
	Trts_Library_Name := sgx_trts
	Service_Library_Name := sgx_tservice
	Crypto_Library_Name := sgx_tcrypto_opt
endif

Enclave_Cpp_Files := $(wildcard vrfcert/*.cpp)
Enclave_Include_Paths := -I./topenssl/include -I$(SGX_SDK)/include -I$(SGX_SDK)/include/tlibc -I$(SGX_SDK)/include/stlport

Enclave_Compile_CFlags := -fPIC -nostdinc $(Enclave_Include_Paths)
Enclave_Compile_CXXFlags := -nostdinc++ -std=c++03 $(Enclave_Compile_CFlags)
Enclave_Link_Flags := -Wl,--no-undefined -nostdlib -nodefaultlibs -nostartfiles -L$(SGX_LIBRARY_PATH) \
	-Wl,--whole-archive -l$(Trts_Library_Name) -Wl,--no-whole-archive \
	-Wl,--start-group -lsgx_tstdc -lsgx_tstdcxx topenssl/libcrypto.a -lsgx_tcrypto -l$(Service_Library_Name) -Wl,--end-group \
        -Wl,-Bstatic -Wl,-Bsymbolic -Wl,--no-undefined \
        -Wl,-pie,-eenclave_entry -Wl,--export-dynamic  \
        -Wl,--defsym,__ImageBase=0 \
	-Wl,--version-script=vrfcert/vrfcert.lds

Enclave_Cpp_Objects := $(Enclave_Cpp_Files:.cpp=.o)

Enclave_Name := libvrfcert_enclave.so

ifeq ($(SGX_MODE), HW)
ifneq ($(SGX_DEBUG), 1)
ifneq ($(SGX_PRERELEASE), 1)
Build_Mode = HW_RELEASE
endif
endif
endif

ifeq ($(Build_Mode), HW_RELEASE)
all: $(App_Name) vrfcert.so
	@echo "Build $(App_Name) [$(Build_Mode)|$(SGX_ARCH)] succcess!"
	@echo "In this mode, please sign the enclave file vrfcert.so first with your signing key before you run the $(App_Name) to launch and access the enclave."
else
all: $(App_Name) $(Enclave_Name)
	@cp -rf test_vrfcert/data/ ./
	@echo "Build $(App_Name) [$(SGX_MODE)|$(SGX_ARCH)] Success!"
	@echo "Please RUN the project with command:"
	@echo "    $(App_Name) <Cert File>"
endif

######## App Objects ########

test_vrfcert/vrfcert_u.c: $(SGX_EDGER8R) vrfcert/vrfcert.edl
	@cd test_vrfcert && $(SGX_EDGER8R) --untrusted ../vrfcert/vrfcert.edl --search-path $(SGX_SDK)/include
	@echo "GEN  =>  $@"

test_vrfcert/vrfcert_u.o: test_vrfcert/vrfcert_u.c
	@$(CC) $(SGX_COMMON_CFLAGS) $(App_Compile_CFlags) -c $< -o $@
	@echo "CC   <=  $<"

test_vrfcert/%.o: test_vrfcert/%.cpp
	@$(CXX) $(SGX_COMMON_CFLAGS) $(App_Compile_CXXFlags) -c $< -o $@
	@echo "CXX  <=  $<"

$(App_Name): test_vrfcert/vrfcert_u.o $(App_Cpp_Objects)
	@$(CXX) $(SGX_COMMON_CFLAGS) $^ -o $@ $(App_Link_Flags)
	@echo "LINK =>  $@"


######## Enclave Objects ########

vrfcert/vrfcert_t.c: $(SGX_EDGER8R) vrfcert/vrfcert.edl
	@cd vrfcert && $(SGX_EDGER8R) --trusted vrfcert.edl --search-path $(SGX_SDK)/include
	@echo "GEN  =>  $@"
vrfcert/vrfcert_t.o: vrfcert/vrfcert_t.c
	@$(CC) $(SGX_COMMON_CFLAGS) $(Enclave_Compile_CFlags) -c $< -o $@
	@echo "CC   <=  $<"

vrfcert/%.o: vrfcert/%.cpp
	@$(CXX) $(SGX_COMMON_CFLAGS) $(Enclave_Compile_CXXFlags) -c $< -o $@
	@echo "CXX  <=  $<"

vrfcert.so: $(Topenssl_Lib_Name) vrfcert/vrfcert_t.o $(Enclave_Cpp_Objects)
	$(CXX) $(SGX_COMMON_CFLAGS) vrfcert/vrfcert_t.o $(Enclave_Cpp_Objects) -o $@ $(Enclave_Link_Flags)
	@echo "LINK =>  $@"

$(Enclave_Name): vrfcert.so
	@$(SGX_ENCLAVE_SIGNER) sign -key vrfcert/vrfcert_private.pem -enclave vrfcert.so -out $@ -config vrfcert/vrfcert.config.xml
	@echo "SIGN =>  $@"


########## Topenssl Library Objects #######
$(Topenssl_Lib_Name):topenssl/Makefile
	@$(MAKE) -C topenssl/

topenssl/Makefile:
	@cd topenssl && ./config --with-sgx=$(SGX_SDK)


######### clean up ########
.PHONY: clean

ifeq ($(wildcard topenssl/Makefile), )
Cleanup_Topenssl :=
else
Cleanup_Topenssl := $(MAKE) -C topenssl/ clean; rm -f topenssl/Makefile
endif


clean:
	@rm -f $(App_Name) $(App_Cpp_Objects) test_vrfcert/vrfcert_u.* vrfcert.* $(Enclave_Name) $(Enclave_Cpp_Objects) vrfcert/vrfcert_t.* vrfcert.so
	@rm -f certlists.txt
	$(Cleanup_Topenssl)
