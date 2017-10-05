#include "tkshotinfo.h"
#define _CRT_NONSTDC_NO_WARNINGS

inline void TKDATA::readHDRLineString(std::string buf, unsigned int trace_number, char* key_name, std::string* odata)
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
template<typename T> void TKDATA::readHDRLine(std::string buf, unsigned int trace_number, char* key_name, T* data)
{
	readHDRLineString(buf, trace_number, key_name, data);
}
void TKDATA::readHDRLine(std::string buf, unsigned int trace_number, char* key_name, float* data)
{
	std::string idata[4];
	readHDRLineString(buf, trace_number, key_name, idata);
	for (unsigned int i = 0; i < trace_number; i++)
		data[i] = std::stof(idata[i], NULL);
}
void TKDATA::readHDRLine(std::string buf, unsigned int trace_number, char* key_name, int* data)
{
	std::string idata[4];
	readHDRLineString(buf, trace_number, key_name, idata);
	for (unsigned int i = 0; i < trace_number; i++)
		data[i] = std::stoi(idata[i], NULL, 10);
}
void TKDATA::readHDRLine(std::string buf, unsigned int trace_number, char* key_name, struct std::tm* data)
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

void TKDATA::ParseHDR()
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
				CHData[trace_number_offset + i].trace_name = sdata[i];
				//TraceNameからADCチャンネルを推測(HDRパースには不要)
				CHData[trace_number_offset + i].ch_number = traceNameToCHNumber(sdata[i]);
				if (CHData[trace_number_offset + i].ch_number != TKADCCONST::CHANNEL::NA)
					channel_number_to_trace_number[static_cast<unsigned int>(CHData[trace_number_offset + i].ch_number)]
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
				CHData[trace_number_offset + i].block_size = idata[i];
			break;

			//VResolution
		case 17:
		case 37:
		case 57:
		case 77:
			readHDRLine(buf, trace_number, key_name, fdata);
			for (unsigned int i = 0; i < trace_number; i++)
				CHData[trace_number_offset + i].v_resolution = fdata[i];
			break;

			//VOffset
		case 18:
		case 38:
		case 58:
		case 78:
			readHDRLine(buf, trace_number, key_name, fdata);
			for (unsigned int i = 0; i < trace_number; i++)
				CHData[trace_number_offset + i].v_offset = fdata[i];
			break;

			//VMaxData
		case 24:
		case 44:
		case 64:
		case 84:
			readHDRLine(buf, trace_number, key_name, idata);
			for (unsigned int i = 0; i < trace_number; i++)
				CHData[trace_number_offset + i].v_max_data = idata[i];
			break;

			//VMinData
		case 25:
		case 45:
		case 65:
		case 85:
			readHDRLine(buf, trace_number, key_name, idata);
			for (unsigned int i = 0; i < trace_number; i++)
				CHData[trace_number_offset + i].v_min_data = idata[i];
			break;

			//HResolution
		case 26:
		case 46:
		case 66:
		case 86:
			readHDRLine(buf, trace_number, key_name, fdata);
			for (unsigned int i = 0; i < trace_number; i++)
				CHData[trace_number_offset + i].h_resolution = fdata[i];
			break;

			//HOffset
		case 27:
		case 47:
		case 67:
		case 87:
			readHDRLine(buf, trace_number, key_name, fdata);
			for (unsigned int i = 0; i < trace_number; i++)
				CHData[trace_number_offset + i].h_offset = fdata[i];
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
				CHData[trace_number_offset + i].time = tmdata[i];
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

void TKDATA::SetADCID(unsigned int const iadc_id)
{
	adc_id = iadc_id;
}

unsigned int TKDATA::GetADCID()
{
	return adc_id;
}

void TKDATA::SetDataFileName(std::string idata_file_name)
{
	data_file_name = idata_file_name;
}

std::string TKDATA::GetDataFileName()
{
	return data_file_name;
}

float TKDATA::GetHResolution()
{
	return CHData[0].h_resolution;
}

float TKDATA::GetVOffset(const int trace_index)
{
	return CHData[trace_index].v_offset;
}

float TKDATA::GetVResolution(const int trace_index)
{
	return CHData[trace_index].v_resolution;
}

unsigned int TKDATA::GetVMaxData(const int trace_index)
{
	return CHData[trace_index].v_max_data;
}

unsigned int TKDATA::GetVMinData(const int trace_index)
{
	return CHData[trace_index].v_min_data;
}

unsigned int TKDATA::GetBlockSize()
{
	return CHData[0].block_size;
}

tm TKDATA::GetTime()
{
	return CHData[0].time;
}

float TKDATA::GetHOffset()
{
	return CHData[0].h_offset;
}

std::string TKDATA::GetModelName()
{
	return model_name;
}

TKADCCONST::BYTEORDER TKDATA::GetByteOrder()
{
	return byte_order;
}

TKADCCONST::DATAFORMAT TKDATA::GetDataFormat()
{
	return data_format;
}

int TKDATA::GetDataOffset()
{
	return data_offset;
}

unsigned int TKDATA::GetTraceNumber()
{
	return trace_total_number;
}

std::vector<std::vector<double> >*TKDATA::GetDataPointsPtr()
{
	return &points;
}

unsigned int TKDATA::SetEvery(const unsigned int every_)
{
	return every = every_;
}

unsigned int TKDATA::GetEvery()
{
	return every;
}

unsigned int TKDATA::LoadDataPoints(unsigned int every, unsigned int start_position, unsigned int point_number)
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
			points[i + 1][j] = this->GetVOffset(i) + static_cast<signed short>(decoded_integer) * this->GetVResolution(i);
		}
	}

	return point_number;
}

std::vector<std::vector<double> >&TKDATA::GetDataPoints()
{
	return points;
}
TKADCCONST::CHANNEL TKDATA::traceNameToCHNumber(std::string trace_name)
{
	for (unsigned int i = 1; i < 100; i++)
		if (trace_name == ("CH" + std::to_string(i)))
			return static_cast<TKADCCONST::CHANNEL>(i);
	return TKADCCONST::CHANNEL::NA;
}

TKADCCONST::VDIV TKDATA::v_resolutionToV_div(float v_resolution)
{
	return TKADCCONST::VDIV::_1V_div;
}

TKDATA::TKDATA()
{
	for (int i = 0; i < TKADC_ADC_CHANNEL_MAX; i++)
		channel_number_to_trace_number[i] = -1;
}

int TKSHOT::getADCDataIndexByADCID(int adc_id)
{
	for (int i = 0; i < adc_num; i++)
		if (TKData[i].GetADCID() == adc_id)
			return i;
	return -1;
}

TKSHOT::TKSHOT()
{
	this->Clear();
}

unsigned int TKSHOT::GetADCNumber()
{
	return adc_num;
}

int TKSHOT::NameShotNumber(int ishot_number)
{
	shot_number = ishot_number;
	return 0;
}

int TKSHOT::Clear()
{
	adc_num = 0;
	TKData.clear();
	total_trace_number = 0;
	return 0;
}

int TKSHOT::AppendDataFile(std::string data_file_name)
{

	TKDATA *this_data;
	adc_num++;
	TKData.push_back(TKDATA());
	this_data = &(TKData[adc_num - 1]);
	this_data->SetADCID(adc_num - 1);
	this_data->SetDataFileName(data_file_name);
	this_data->ParseHDR();

	total_trace_number += this->Data(adc_num - 1).GetTraceNumber();

	return adc_num-1;
}

int TKSHOT::ADCIDToADCDataIndex(int adc_id)
{
	return getADCDataIndexByADCID(adc_id);
}

unsigned int TKSHOT::GetADCID(int adc_index)
{
	return TKData[adc_index].GetADCID();
}

std::vector<TKDATA>& TKSHOT::Data()
{
	return TKData;
}

unsigned int TKSHOT::GetTotalTraceNumber()
{
	return total_trace_number;
}

TKDATA& TKSHOT::Data(unsigned int n)
{
	return TKData.at(getADCDataIndexByADCID(n));
}

//std::vector<TKDATA>::iterator TKSHOT::operator [](unsigned int n) {
//	return TKData.at(getADCDataIndexByADCID(n));
//}
//std::vector<TKDATA>::iterator TKSHOT::operator [](const unsigned int n) {
//	return TKData.at(getADCDataIndexByADCID(n));
//}
