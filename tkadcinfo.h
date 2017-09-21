#include "tkadc.h"
#ifndef __TKADCINFO__
#define __TKADCINFO__

#define TKADCINFO_ADC_ID_DL750 TKADCINFO::ADCID::ADCNameDL750
#define TKADCINFO_ADC_ID_DL850 TKADCINFO::ADCID::ADCNameDL850

namespace TKADCINFO
{
	enum class ADCID : int
	{
		ADCNameDL750,
		ADCNameDL850
	};
//	extern TKADCCONTROL* tkadc_ptr[(int)sizeof(ADCID)];
	TKADCCONTROL*& ADCIDToTKADCPtr(ADCID adcid);
//	TKADCCONTROL::ADCModel ADCIDToADCType(TKADCINFO::ADCID adcid);
//	std::string ADCIDToADCName(TKADCINFO::ADCID adcid);
	std::string ADCIDToSectionName(TKADCINFO::ADCID adcid);
};
#endif