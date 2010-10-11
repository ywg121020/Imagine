/*-------------------------------------------------------------------------
** Copyright (C) 2005-2010 Timothy E. Holy and Zhongsheng Guo
**    All rights reserved.
** Author: All code authored by Zhongsheng Guo.
** License: This file may be used under the terms of the GNU General Public
**    License version 2.0 as published by the Free Software Foundation
**    and appearing at http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
**
**    If this license is not appropriate for your use, please
**    contact holy@wustl.edu or zsguo@pcg.wustl.edu for other license(s).
**
** This file is provided WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**-------------------------------------------------------------------------*/


#include "camera_g.hpp"


Camera::Camera()
{
      hbin=vbin=1;
      hstart=vstart=1;
      hend=vend= -1; //

      pImageArray=NULL;
      imageArraySize=0;

      nFrames=100;  //todo: make this as a param

      model="Unknown";

}

Camera::~Camera()
{
      // free all allocated memory
      if(pImageArray){
         delete []pImageArray;
         pImageArray = NULL;
      }

}

bool Camera::allocImageArray(int nFrames, bool shouldReallocAnyway)
{
      int nPixels=getImageWidth()*getImageHeight()*nFrames;

      if(shouldReallocAnyway || nPixels>imageArraySize){
         if(pImageArray){
            delete []pImageArray;
            pImageArray = NULL;
            imageArraySize=0;
         }
      }//if, should delete old allocation

      //here if pImageArray!=NULL, 
      //     then !shouldReallocAnyway && nPixels<=imageArraySize.

      if(!pImageArray){
         pImageArray=new PixelValue[nPixels];
         if(!pImageArray){
            errorCode=getExtraErrorCode(eOutOfMem);
            errorMsg="no enough memory";
            return false;
         }//if, fail to alloc enough mem
         imageArraySize=nPixels;
      }//if, no allocated space before

      return true;

}
