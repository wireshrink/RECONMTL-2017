
#ifndef SGXVIDEOPLAYERSTATE_H
#define SGXVIDEOPLAYERSTATE_H

#include "SGXBlob.h"
#include "SGXEpg.h"
#include "SGXCoupons.h"

#include <string.h>

/**
  * class SGXVideoPlayerState
  * 
  */

class SGXVideoPlayerState
{
public:

  // Constructors/Destructors
  //  


  /**
   * Empty Constructor
   */
  SGXVideoPlayerState ();

  /**
   * Empty Destructor
   */
  virtual ~SGXVideoPlayerState ();

  // Static Public attributes
  //  

  // Public attributes
  //  


  // Public attribute accessor methods
  //  


  // Public attribute accessor methods
  //  



  /**
   * @return SGXBlob*
   */
  SGXBlob* getBlob ()
  {
	  return nullptr;
  }


  /**
   * @return SGXEpg*
   */
  SGXEpg* getEPG ()
  {
  }


  /**
   * @return SGXCoupons*
   */
  SGXCoupons* getCoupons ()
  {
  }


  /**
   * @return bool
   */
  bool RefreshAndLoadServiceFiles ()
  {
  }


  /**
   * @return int
   */
  int getBalance ()
  {
	  if (getBlob())
		  return getBlob()->getBalance();
	  return 0;
  }


  /**
   * @return bool
   * @param  base_folder_name_1024_
   */
  bool initPlayerState (char *base_folder_name)
  {
	  return false;
  }


  /**
   * @return bool
   * @param  coupon_32_
   */
  bool applyCoupon (char coupon[32])
  {
	  return false;
  }

protected:

  // Static Protected attributes
  //  

  // Protected attributes
  //  

public:


  // Protected attribute accessor methods
  //  

protected:

public:


  // Protected attribute accessor methods
  //  

protected:


private:

  // Static Private attributes
  //  

  // Private attributes
  //  

  char base_folder[1024];
public:


  // Private attribute accessor methods
  //  

private:

public:


  // Private attribute accessor methods
  //  


  /**
   * Set the value of base_folder_1024_
   * @param new_var the new value of base_folder_1024_
   */
  void setBase_folder (char new_var[1024])   {
	  strncpy(base_folder, new_var, 1024);
  }

  /**
   * Get the value of base_folder_1024_
   * @return the value of base_folder_1024_
   */
  char* getBase_folder ()   {
    return base_folder;
  }
private:


  void initAttributes () ;

};

#endif // SGXVIDEOPLAYERSTATE_H
