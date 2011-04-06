//-----------------------------------------------------------------//
// Name        | PCO_Structures.h            | Type: ( ) source    //
//-------------------------------------------|       (*) header    //
// Project     | PCO                         |       ( ) others    //
//-----------------------------------------------------------------//
// Platform    | PC                                                //
//-----------------------------------------------------------------//
// Environment | Visual 'C++'                                      //
//-----------------------------------------------------------------//
// Purpose     | PCO - Common PCO functions                        //
//-----------------------------------------------------------------//
// Author      | FRE, PCO AG                                       //
//-----------------------------------------------------------------//
// Revision    |  rev. 0.10 rel. 0.10                              //
//-----------------------------------------------------------------//
// Notes       |                                                   //
//-----------------------------------------------------------------//
// (c) 2003 PCO AG * Donaupark 11 *                                //
// D-93309      Kelheim / Germany * Phone: +49 (0)9441 / 2005-0 *  //
// Fax: +49 (0)9441 / 2005-20 * Email: info@pco.de                 //
//-----------------------------------------------------------------//


//-----------------------------------------------------------------//
// Revision History:                                               //
//-----------------------------------------------------------------//
// Rev.:     | Date:      | Changed:                               //
// --------- | ---------- | ---------------------------------------//
//  0.10     | 23.02.2010 |  new file, FRE                         //
//-----------------------------------------------------------------//
//  0.0      |   .  .2009 |                                        //
//           |            |                                        //
//-----------------------------------------------------------------//
// PCO_Structures.h: Defines helper functions and classes for pco
//

#ifndef PCO_STRUCTURES_H
#define PCO_STRUCTURES_H

struct sRGB_color_correction_coefficients
{
	double da11, da12, da13;
	double da21, da22, da23;
	double da31, da32, da33;
};

typedef struct sRGB_color_correction_coefficients SRGBCOLCORRCOEFF;
#endif
