/*
 * tank.h
 *
 *  Created on: Dec 27, 2014
 *      Author: nix
 */

#ifndef TANK_H_
#define TANK_H_

#define BLOCK			1
#define CYLINDER_BASE	2	// lay on base
#define CYLINDER_SIDE	3	// lay on side

//FUNCTION DENSITY
#pragma pack(push, 1)
typedef volatile struct fuel_density_t{
	double dT;		// promena na temperatura za promena na procent od (dp)
	double dp;		// procentualna promena na gustinata za (dT) (!!! default 1)
	double Tn;		// nominal temperature
	double Rn;		// density at nominal temperature
	double n;		// : y = k*x + n
	double k;		//
}fuel_density;
#pragma pack(pop)

//BLOCK TYPE
#pragma pack(push, 1)
typedef volatile struct tank_type_block{
	const int type; // used for further identifications, must be added in to declaration of the structure
	double height;
	double lenght;
	double width;
}tank_block;
#pragma pack(pop)

//CYLINDER TYPE
#pragma pack(push, 1)
typedef volatile struct tank_type_cylinder{
	const int type; // used for further identifications, must be added in to declaration of the structure
	double radius;
	double lenght;
}tank_cylinder;
#pragma pack(pop)

// TANK
#pragma pack(push, 1)
typedef volatile struct tank_t{

	char *pNAME; // name of the tank
	void *pTank; // points to any of these structures: tank_block, tank_cylinder_base, tank_cylinder_side;
	/** status of the tank **/
	double temperature;
	double fuel_level;
	double volume; // volume of fullness of the tank
	double add_pro; // last additive proportion [%]
	double (*read_ADC_tank_level)(void);	// pointer to the function for reading ADC value
	double (*read_ADC_temperature)(void);	// pointer to the function for reading ADC value
	fuel_density density;		// Parameters of the function of changing density by the temperature
	// ToDo: imperial units / metric units flag

}tank;
#pragma pack(pop)

// DEFINITION OF THE FUNCTIONS
void tank_status_update(tank *pTank);

#endif /* TANK_H_ */
