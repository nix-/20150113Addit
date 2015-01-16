/*
 * tank.c
 *
 *  Created on: Dec 27, 2014
 *      Author: nix
 */
#include "tank.h"
#include "math.h"

#define ERROR 	-1

// CONSTANTS
#define PI 						(3.141592653589793)
#define GRAV_ACCELE		 		(9.80665)

#define height(a)	(((a *)(pointTank->pTank))->height)
#define lenght(a)	(((a *)(pointTank->pTank))->lenght)
#define width(a)	(((a *)(pointTank->pTank))->width)
#define radius(a)	(((a *)(pointTank->pTank))->radius)

// ToDo: proverka na pointeri !!! (SEKADE) by Srdjan
// note: site kalkulacvi ke bidat vo metric a vlez ili izlez ke bide vo zavisnost od baranja
// 		 (ke treba da se napravi kalkulacija za konverzija velicinite vo metric ili imperial)
// ToDo: struktura za kalibracija na senzorite

/*****************************************************************************
** Function name:		initTank
**
** Descriptions:		initialization of the tank
**
** parameters:			pointer to the structure "tank"
** 						pointer to the string (name of the tank)
** 						pinter to the type of the thank
** 						pointer to the function for reading tank level
** 						pointer to the function for reading temperature
**
** Returned value:		calculated value of the volume
**
*****************************************************************************/
int initTank(	tank *pTank,
				char *pName,
				void *pTankType,
				double (*read_ADC_tank_level)(void), 	// ADC read function
				double (*read_ADC_temperature)(void))	// ADC read function
{

	if((pTank == 0) && (pName == 0) && (pTankType == 0) && (read_ADC_tank_level == 0) && (read_ADC_temperature == 0)){
		//error
		return ERROR;
	}

	// parameters fine
	pTank->pNAME = pName;
	pTank->pTank = pTankType;
	pTank->read_ADC_tank_level = read_ADC_tank_level;
	pTank->read_ADC_temperature = read_ADC_temperature;

	// ToDo: update of the structure parameter (temperature, level, volume, ...)

	return 1;

}

/*****************************************************************************
** Function name:		calculate_volume
**
** Descriptions:		calculate the volume for the appropriate type of tank
**
** parameters:			pointer to the structure "tank"
**
** Returned value:		calculated value of the volume
**
*****************************************************************************/
double calculate_volume(tank *pointTank)
{
	double tmp;
	double volume;
	double R, h, w, d;

	// pointer check
	if(pointTank == 0){
		// invalid pointer
		return ERROR;
	}

	switch(((tank_block*)(pointTank->pTank))->type){

	case BLOCK:
		// calculation of volume for this type
		return 	( lenght(tank_block) * width(tank_block) * pointTank->fuel_level );
		break;

	case CYLINDER_BASE:
		// calculation of volume for this type
		return ( PI * radius(tank_cylinder) * radius(tank_cylinder) * pointTank->fuel_level );
		break;

	case CYLINDER_SIDE:
		// calculation of volume for this type
		tmp = 2*radius(tank_cylinder) - lenght(tank_cylinder);
		return 	(pointTank->fuel_level * (
				(PI * radius(tank_cylinder) * radius(tank_cylinder)) -
				(radius(tank_cylinder) * radius(tank_cylinder) * acos((radius(tank_cylinder) - tmp)/radius(tank_cylinder))) +
				((radius(tank_cylinder) + tmp)* sqrt((2*radius(tank_cylinder)*tmp) - (tmp*tmp)))
				));
		break;

	default:
		// undefined type
		return ERROR;
		break;

	}

}

/*****************************************************************************
** Function name:		tank_status_update
**
** Descriptions:		update tank status
**
** parameters:			pointer to the structure "tank"
**
** Returned value:		None
**
*****************************************************************************/
void tank_status_update(tank *pTank)
{
	double h, P, t, Dens;

	// pointer check
	if(pTank == 0){
		// invalid pointer
		return ERROR;
	}

	/** CALCULATION OF THE LINEARITY CONSTANTS OF THE DENSITY FUNC **/
	// fuel
	pTank->density.n = (1 - ((pTank->density.dp * pTank->density.Tn)/(100 * pTank->density.dT))) * pTank->density.Rn; // calculation of the n-constant
	pTank->density.k = (pTank->density.Rn * pTank->density.dp)/(pTank->density.dT * 100);
	/** END OF CALCULATIONS **/

	P = pTank->read_ADC_tank_level();	// read ADC liquid level
	t = pTank->read_ADC_temperature();
	// t = .. // ToDo: konverzija od ADC na soodvetna temperatura 	(vrzano e so kalibracijata)
	// P = .. // ToDo: konverzija od ADC na soodvetna pritisok	  	(vrzano e so kalibracijata)

	pTank->temperature 	= t;

	h = P/(GRAV_ACCELE* (pTank->density.k * pTank->temperature  + pTank->density.n)); 	// h(t)
	pTank->fuel_level = h;

	// Second step (before the calculations)
	pTank->volume = calculate_volume(pTank);	// first you should update Temperature and Liquid Level

}
// END OF FILE
