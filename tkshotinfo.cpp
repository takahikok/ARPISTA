#include "tkshotinfo.h"
#include <cmath>
#define _CRT_NONSTDC_NO_WARNINGS

inline void TKSHOT::TKDATA::readHDRLineString(std::string buf, unsigned int trace_number, char* key_name, std::string* odata)
{
	char data[4][256];
	switch (trace_number) {
	case 1:
		std::sscanf(buf.c_str(), "%s %s", key_name, &(data[0]));
		break;
	case 2:
		std::sscanf(buf.c_str(), "%s %s %s", key_name, &(data[0]), &(data[1]));
		break;
	case 3:
		std::sscanf(buf.c_str(), "%s %s %s %s", key_name, &(data[0]), &(data[1]), &(data[2]));
		break;
	case 4:
		std::sscanf(buf.c_str(), "%s %s %s %s %s", key_name, &(data[0]), &(data[1]), &(data[2]), &(data[3]));
		break;
	}
	for (unsigned int i = 0; i < trace_number; i++)
		odata[i] = (std::string)data[i];
}
template<typename T> void TKSHOT::TKDATA::readHDRLine(std::string buf, unsigned int trace_number, char* key_name, T* data)
{
	readHDRLineString(buf, trace_number, key_name, data);
}
void TKSHOT::TKDATA::readHDRLine(std::string buf, unsigned int trace_number, char* key_name, float* data)
{
	std::string idata[4];
	readHDRLineString(buf, trace_number, key_name, idata);
	for (unsigned int i = 0; i < trace_number; i++)
		data[i] = std::stof(idata[i], NULL);
}
void TKSHOT::TKDATA::readHDRLine(std::string buf, unsigned int trace_number, char* key_name, int* data)
{
	std::string idata[4];
	readHDRLineString(buf, trace_number, key_name, idata);
	for (unsigned int i = 0; i < trace_number; i++)
		data[i] = std::stoi(idata[i], NULL, 10);
}
void TKSHOT::TKDATA::readHDRLine(std::string buf, unsigned int trace_number, char* key_name, struct std::tm* data)
{
	std::string idata[4];
	readHDRLineString(buf, trace_number, key_name, idata);
	for (unsigned int i = 0; i < trace_number; i++, data) {
		std::stringstream ss;
		std::string ibuf;
		ss << idata[i];
		if ((std::string)key_name == "Date") {
			std::getline(ss, ibuf, '/');
			(data + i)->tm_year = std::stoi(ibuf) - 1900;
			std::getline(ss, ibuf, '/');
			(data + i)->tm_mon = std::stoi(ibuf) - 1;
			std::getline(ss, ibuf, '/');
			(data + i)->tm_mday = std::stoi(ibuf);
		} else if ((std::string)key_name == "Time") {
			std::getline(ss, ibuf, ':');
			(data + i)->tm_hour = std::stoi(ibuf);
			std::getline(ss, ibuf, ':');
			(data + i)->tm_min = std::stoi(ibuf);
			std::getline(ss, ibuf, ':');
			(data + i)->tm_sec = (int)std::stof(ibuf);
		}
	}
}

void TKSHOT::TKDATA::parseHDR()
{
	std::ifstream ifs(data_file_name + ".HDR");
	std::string buf;
	for (unsigned int lines = 1, trace_number_offset = 0;
	     std::getline(ifs, buf); lines++) {
		unsigned int group_number;
		unsigned int trace_number;
		char key_name[64];
		int idata[4];
		float fdata[4];
		std::string sdata[4];
		struct std::tm tmdata[4];

		switch (lines) {

			//Model
		case 5:
			readHDRLine(buf, 1, key_name, sdata);
			model_name = [](std::string m) -> std::string
			{
				if (m == "Xviewer")
					return "DL850";
				else
					return m;
			}(sdata[0]);
			break;

			//Endian
		case 6:
			readHDRLine(buf, 1, key_name, sdata);
			switch (sdata[0].c_str()[0]) {

				// big endian 680xx
			case 'b':
			case 'B':
				byte_order = TKADCCONST::BYTEORDER::BIG_ENDIAN;
				break;

				// little endian 80x86
			case 'l':
			case 'L':
				byte_order = TKADCCONST::BYTEORDER::LITTLE_ENDIAN;
				break;

			default:
				byte_order = TKADCCONST::BYTEORDER::BIG_ENDIAN;
				break;
			}
			break;

			//DataFormat
		case 7:
			readHDRLine(buf, 1, key_name, sdata);
			switch (sdata[0].c_str()[0]) {

				// trace
			case 't':
			case 'T':
				data_format = TKADCCONST::DATAFORMAT::TRACE;
				break;

				// bloack
			case 'b':
			case 'B':
				data_format = TKADCCONST::DATAFORMAT::BLOCK;
				break;

			default:
				data_format = TKADCCONST::DATAFORMAT::BLOCK;
				break;
			}
			break;

			//GroupNumber
		case 8:
			readHDRLine(buf, 1, key_name, idata);
			group_number = idata[0];
			break;

			//TraceTotalNumber
		case 9:
			readHDRLine(buf, 1, key_name, idata);
			trace_total_number = idata[0];
			CHData.resize(trace_total_number);
			break;

			//DataOffset
		case 10:
			readHDRLine(buf, 1, key_name, idata);
			data_offset = idata[0];
			break;

			//TraceNumber
		case 33:
		case 53:
		case 73:
			trace_number_offset += trace_number;
		case 13:
			readHDRLine(buf, 1, key_name, idata);
			trace_number = idata[0];
			break;

			//TraceName
		case 15:
		case 35:
		case 55:
		case 75:
			readHDRLine(buf, trace_number, key_name, sdata);
			for (unsigned int i = 0; i < trace_number; i++) {
				this->Ch(trace_number_offset + i).SetTraceName(sdata[i]);
				//TraceNameからADCチャンネルを推測(HDRパースには不要)
				this->Ch(trace_number_offset + i).SetChNumber(traceNameToChNumber(sdata[i]));
				if (this->Ch(trace_number_offset + i).GetChNumber() != TKADCCONST::CHANNEL::NA)
					channel_number_to_trace_number[static_cast<unsigned int>(this->Ch(trace_number_offset + i).GetChNumber())]
					= trace_number_offset + i;
			}
			break;

			//BlockSize
		case 16:
		case 36:
		case 56:
		case 76:
			readHDRLine(buf, trace_number, key_name, idata);
			for (unsigned int i = 0; i < trace_number; i++)
				this->Ch(trace_number_offset + i).SetBlockSize(idata[i]);
			break;

			//VResolution
		case 17:
		case 37:
		case 57:
		case 77:
			readHDRLine(buf, trace_number, key_name, fdata);
			for (unsigned int i = 0; i < trace_number; i++)
				this->Ch(trace_number_offset + i).SetVResolution(fdata[i]);
			break;

			//VOffset
		case 18:
		case 38:
		case 58:
		case 78:
			readHDRLine(buf, trace_number, key_name, fdata);
			for (unsigned int i = 0; i < trace_number; i++)
				this->Ch(trace_number_offset + i).SetVOffset(fdata[i]);
			break;

			//VMaxData
		case 24:
		case 44:
		case 64:
		case 84:
			readHDRLine(buf, trace_number, key_name, idata);
			for (unsigned int i = 0; i < trace_number; i++)
				this->Ch(trace_number_offset + i).SetVMaxData(idata[i]);
			break;

			//VMinData
		case 25:
		case 45:
		case 65:
		case 85:
			readHDRLine(buf, trace_number, key_name, idata);
			for (unsigned int i = 0; i < trace_number; i++)
				this->Ch(trace_number_offset + i).SetVMinData(idata[i]);
			break;

			//HResolution
		case 26:
		case 46:
		case 66:
		case 86:
			readHDRLine(buf, trace_number, key_name, fdata);
			for (unsigned int i = 0; i < trace_number; i++)
				this->Ch(trace_number_offset + i).SetHResolution(fdata[i]);
			break;

			//HOffset
		case 27:
		case 47:
		case 67:
		case 87:
			readHDRLine(buf, trace_number, key_name, fdata);
			for (unsigned int i = 0; i < trace_number; i++)
				this->Ch(trace_number_offset + i).SetHOffset(fdata[i]);
			break;

			//Date
		case 29:
		case 49:
		case 69:
		case 89:
			//Time
		case 30:
		case 50:
		case 70:
		case 90:
			readHDRLine(buf, trace_number, key_name, tmdata);
			for (unsigned int i = 0; i < trace_number; i++)
				this->Ch(trace_number_offset + i).SetTime(tmdata[i]);
			break;

			//EOF
		case 31:
		case 51:
		case 71:
		case 91:
			if (trace_number_offset + trace_number >= trace_total_number)
				return;
			break;
		}
	}
}

void TKSHOT::TKDATA::SetADCID(unsigned int const iadc_id)
{
	adc_id = iadc_id;
}

const unsigned int& TKSHOT::TKDATA::GetADCID() const
{
	return adc_id;
}

void TKSHOT::TKDATA::setDataFileName(const std::string& idata_file_name)
{
	data_file_name = idata_file_name;
}

const std::string& TKSHOT::TKDATA::GetDataFileName() const
{
	return data_file_name;
}

const float& TKSHOT::TKDATA::GetHResolution() const
{
	return this->Ch(0).GetHResolution();
}

//float TKSHOT::TKDATA::GetVOffset(int const trace_index)
//{
//	return CHData[trace_index].v_offset;
//}

//float TKSHOT::TKDATA::GetVResolution(int const trace_index)
//{
//	return CHData[trace_index].v_resolution;
//}

//int TKSHOT::TKDATA::GetVMaxData(int const trace_index)
//{
//	return CHData[trace_index].v_max_data;
//}

//int TKSHOT::TKDATA::GetVMinData(int const trace_index)
//{
//	return CHData[trace_index].v_min_data;
//}

const unsigned int& TKSHOT::TKDATA::GetBlockSize() const
{
	return this->Ch(0).GetBlockSize();
}

const std::tm& TKSHOT::TKDATA::GetTime() const
{
	return this->Ch(0).GetTime();
}

const float& TKSHOT::TKDATA::GetHOffset() const
{
	return this->Ch(0).GetHOffset();
}

const std::string& TKSHOT::TKDATA::GetModelName() const
{
	return model_name;
}

const TKADCCONST::BYTEORDER& TKSHOT::TKDATA::GetByteOrder() const
{
	return byte_order;
}

const TKADCCONST::DATAFORMAT& TKSHOT::TKDATA::GetDataFormat() const
{
	return data_format;
}

const unsigned int& TKSHOT::TKDATA::GetDataOffset() const
{
	return data_offset;
}

const unsigned int& TKSHOT::TKDATA::GetTraceNumber() const
{
	return trace_total_number;
}

std::vector<std::vector<double> >*TKSHOT::TKDATA::GetDataPointsPtr()
{
	return &points;
}

void TKSHOT::TKDATA::SetEvery(const unsigned int& every)
{
	this->every = every;
}

const unsigned int& TKSHOT::TKDATA::GetEvery() const
{
	return every;
}

unsigned int TKSHOT::TKDATA::LoadDataPoints(unsigned int const every, unsigned int const start_position, unsigned int point_number)
{
	//		points = std::vector<std::vector<double> >(this->GetTraceTotalNumber() + 1, std::vector<double>(point_number, 0.0f));
	if (point_number > this->GetBlockSize() / (every + 1))
		point_number = this->GetBlockSize() / (every + 1);

	points.resize(this->GetTraceNumber() + 1, std::vector<double>(point_number, 0.0f));

	std::ifstream ifsRawBin;
	ifsRawBin.open(this->GetDataFileName() + ".WVF", std::ios::in | std::ios::binary );

	// time axis
	for (unsigned int j = 0; j < point_number; j++)
		points[0][j] = this->GetHOffset() + this->GetHResolution() * (start_position + (every + 1) * j);

	// for IS2
	for (unsigned int i = 0; i < this->GetTraceNumber(); i++) {
		unsigned int sp;
		unsigned char bytes[2];
		unsigned short decoded_integer;
		sp = this->GetDataOffset() + (this->GetBlockSize() * i) * 2;

		for (unsigned int j = 0;
		     j < point_number;
		     j++) {
			ifsRawBin.seekg(sp + (start_position + (every + 1) * j) * 2, std::ios::beg);
			ifsRawBin.read(reinterpret_cast<char*>(bytes), 2);
			switch (this->GetByteOrder()) {
			case TKADCCONST::BYTEORDER::BIG_ENDIAN:
				decoded_integer = bytes[0] << 8 | bytes[1];
				break;
			case TKADCCONST::BYTEORDER::LITTLE_ENDIAN:
				decoded_integer = bytes[0] | bytes[1] << 8;
				break;
			}
			points[i + 1][j] = this->Ch(i).GetVOffset() + static_cast<signed short>(decoded_integer) * this->Ch(i).GetVResolution();
		}
	}

	return point_number;
}

const std::vector<std::vector<double> >&TKSHOT::TKDATA::GetDataPoints() const
{
	return points;
}

//TKADCCONST::VDIV TKSHOT::TKDATA::GetV_div(const int trace_index)
//{
//	return CHData[trace_index].v_div;
//}

TKSHOT::TKDATA::CHDATA& TKSHOT::TKDATA::Ch(const unsigned int index)
{
	return CHData[index];

}

const TKSHOT::TKDATA::CHDATA&TKSHOT::TKDATA::Ch(const unsigned int index) const
{
	return CHData[index];
}

const TKADCCONST::CHANNEL TKSHOT::TKDATA::traceNameToChNumber(std::string const& trace_name) const
{
	for (unsigned int i = 1; i < 100; i++)
		if (trace_name == ("CH" + std::to_string(i)))
			return static_cast<TKADCCONST::CHANNEL>(i);

	return TKADCCONST::CHANNEL::NA;
}

const TKADCCONST::VDIV TKSHOT::TKDATA::v_resolutionToV_div(float const& v_resolution) const
{
	const float V_RESOLUTION_1V_DIV = 4.1666667e-4f;
	switch (static_cast<int>(std::round(std::log10(v_resolution / V_RESOLUTION_1V_DIV) * 10))) {
	case -30:
		return TKADCCONST::VDIV::_1mV_div;
	case -27:
		return TKADCCONST::VDIV::_2mV_div;
	case -23:
		return TKADCCONST::VDIV::_5mV_div;
	case -20:
		return TKADCCONST::VDIV::_10mV_div;
	case -17:
		return TKADCCONST::VDIV::_20mV_div;
	case -13:
		return TKADCCONST::VDIV::_50mV_div;
	case -10:
		return TKADCCONST::VDIV::_100mV_div;
	case -7:
		return TKADCCONST::VDIV::_200mV_div;
	case -3:
		return TKADCCONST::VDIV::_500mV_div;
	case 0:
		return TKADCCONST::VDIV::_1V_div;
	case 3:
		return TKADCCONST::VDIV::_2V_div;
	case 7:
		return TKADCCONST::VDIV::_5V_div;
	case 10:
		return TKADCCONST::VDIV::_10V_div;
	case 13:
		return TKADCCONST::VDIV::_20V_div;
	case 17:
		return TKADCCONST::VDIV::_50V_div;
	case 20:
		return TKADCCONST::VDIV::_100V_div;
	default:
		return TKADCCONST::VDIV::UNKNOWN;
	}
}

void TKSHOT::TKDATA::estimateParameters()
{
	for (auto& this_ch : this->CHData) {
		this_ch.SetV_div(v_resolutionToV_div(this_ch.GetVResolution()));
	}
}

void TKSHOT::TKDATA::LoadHDR(std::string const& data_file_name)
{
	this->setDataFileName(data_file_name);
	this->parseHDR();
	this->estimateParameters();
}

TKSHOT::TKDATA::TKDATA()
{
	for (int i = 0; i < TKADC_ADC_CHANNEL_MAX; i++)
		channel_number_to_trace_number[i] = -1;
}

const int TKSHOT::getADCDataIndexByADCID(int const& adc_id) const
{
	for (unsigned int i = 0; i < TKData.size(); i++)
		if (TKData[i].GetADCID() == adc_id)
			return i;
	return -1;
}

TKSHOT::TKSHOT()
{
	this->Clear();
}

const unsigned int TKSHOT::GetADCNumber() const
{
	return this->adc_num;
}

void TKSHOT::NameShotNumber(int const ishot_number)
{
	shot_number = ishot_number;
}

void TKSHOT::Clear()
{
	this->adc_num = 0;
	this->TKData.clear();
	this->total_trace_number = 0;
}

const unsigned int TKSHOT::AppendDataFile(const std::string& data_file_name)
{

	TKDATA *this_data;
	TKData.push_back(TKDATA());
	this_data = &(TKData[adc_num]);
	this_data->SetADCID(adc_num);
	this_data->LoadHDR(data_file_name);

	total_trace_number += this->Data(adc_num).GetTraceNumber();

	return ++adc_num;
}

const int TKSHOT::ADCIDToADCDataIndex(int const& adc_id) const
{
	return getADCDataIndexByADCID(adc_id);
}

const unsigned int TKSHOT::GetADCID(int const& adc_index) const
{
	return TKData[adc_index].GetADCID();
}

const unsigned int TKSHOT::GetTotalTraceNumber() const
{
	return this->total_trace_number;
}

TKSHOT::TKDATA& TKSHOT::Data(unsigned int const& index)
{
	return TKData.at(getADCDataIndexByADCID(index));
}

const TKSHOT::TKDATA&TKSHOT::Data(const unsigned int& index) const
{
	return TKData.at(getADCDataIndexByADCID(index));
}

const unsigned int TKSHOT::TotalTraceIndexToDataIndex(const unsigned int& total_index)
{
	unsigned int trace_index = total_index;
	unsigned int data_index = 0;

	for (; data_index < this->GetADCNumber(); data_index++)
		if (trace_index < this->Data(data_index).GetTraceNumber())
			break;
		else
			trace_index -= this->Data(data_index).GetTraceNumber();

	return data_index;
}

const unsigned int TKSHOT::TotalTraceIndexToChIndex(const unsigned int& total_index)
{
	unsigned int trace_index = total_index;
	unsigned int data_index = 0;

	for (; data_index < this->GetADCNumber(); data_index++)
		if (trace_index < this->Data(data_index).GetTraceNumber())
			break;
		else
			trace_index -= this->Data(data_index).GetTraceNumber();

	return trace_index;
}

TKSHOT::TKDATA::CHDATA&TKSHOT::TotalTraceIndexToCh(const unsigned int& total_index)
{
	unsigned int trace_index = total_index;
	unsigned int data_index = 0;

	for (; data_index < this->GetADCNumber(); data_index++)
		if (trace_index < this->Data(data_index).GetTraceNumber())
			break;
		else
			trace_index -= this->Data(data_index).GetTraceNumber();

	return this->Data(data_index).Ch(trace_index);
}

//std::vector<TKDATA>::iterator TKSHOT::operator [](unsigned int n) {
//	return TKData.at(getADCDataIndexByADCID(n));
//}
//std::vector<TKDATA>::iterator TKSHOT::operator [](const unsigned int n) {
//	return TKData.at(getADCDataIndexByADCID(n));
//}

void TKSHOT::TKDATA::CHDATA::SetChNumber(const TKADCCONST::CHANNEL& ch_number)
{
	this->ch_number = ch_number;
}

void TKSHOT::TKDATA::CHDATA::SetTraceName(const std::string& trace_name)
{
	this->trace_name = trace_name;
}

void TKSHOT::TKDATA::CHDATA::SetVOffset(const float& v_offset)
{
	this->v_offset = v_offset;
}

void TKSHOT::TKDATA::CHDATA::SetVResolution(const float& v_resolution)
{
	this->v_resolution = v_resolution;
}

void TKSHOT::TKDATA::CHDATA::SetVMaxData(const int& v_max_data)
{
	this->v_max_data = v_max_data;
}

void TKSHOT::TKDATA::CHDATA::SetVMinData(const int& v_min_data)
{
	this->v_min_data = v_min_data;
}

void TKSHOT::TKDATA::CHDATA::SetHResolution(const float& h_resolution)
{
	this->h_resolution = h_resolution;
}

void TKSHOT::TKDATA::CHDATA::SetHOffset(const float& h_offset)
{
	this->h_offset = h_offset;
}

void TKSHOT::TKDATA::CHDATA::SetV_div(const TKADCCONST::VDIV& v_div)
{
	this->v_div = v_div;
}

void TKSHOT::TKDATA::CHDATA::SetTime(const std::tm& time)
{
	this->time = time;
}

const TKADCCONST::CHANNEL& TKSHOT::TKDATA::CHDATA::GetChNumber() const
{
	return this->ch_number;
}

const std::string& TKSHOT::TKDATA::CHDATA::GetTraceName() const
{
	return this->trace_name;
}

const float& TKSHOT::TKDATA::CHDATA::GetHResolution() const
{
	return this->h_resolution;
}

const float& TKSHOT::TKDATA::CHDATA::GetHOffset() const
{
	return this->h_offset;
}

const unsigned int& TKSHOT::TKDATA::CHDATA::GetBlockSize() const
{
	return this->block_size;
}

void TKSHOT::TKDATA::CHDATA::SetBlockSize(const unsigned int& block_size)
{
	this->block_size = block_size;
}

const float& TKSHOT::TKDATA::CHDATA::GetVOffset() const
{
	return this->v_offset;
}

const float& TKSHOT::TKDATA::CHDATA::GetVResolution() const
{
	return this->v_resolution;
}

const int& TKSHOT::TKDATA::CHDATA::GetVMaxData() const
{
	return this->v_max_data;
}

const int& TKSHOT::TKDATA::CHDATA::GetVMinData() const
{
	return this->v_min_data;
}

const TKADCCONST::VDIV& TKSHOT::TKDATA::CHDATA::GetV_div() const
{
	return this->v_div;
}

const tm& TKSHOT::TKDATA::CHDATA::GetTime() const
{
	return this->time;
}
