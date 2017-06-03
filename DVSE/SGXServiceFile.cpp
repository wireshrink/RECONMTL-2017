#include "SGXServiceFile.h"

// Constructors/Destructors
//  

SGXServiceFile::SGXServiceFile () {
initAttributes();
}

SGXServiceFile::~SGXServiceFile () { }

//  
// Methods
//  


// Accessor methods
//  


// Other methods
//  

void SGXServiceFile::initAttributes () {
  decrypted_content = nullptr;
  current_data_size = -1L;
}

