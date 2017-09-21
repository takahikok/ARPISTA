#include "tkadc.h"
#include <string>  


TKADCCONTROL::TKADCCONTROL(ADCMODEL adcmodel)
{
	adc_model = adcmodel;

}

TKADCCONTROL::~TKADCCONTROL()
{
}

TKADCCONTROL::ADCMODEL TKADCCONTROL::Model()
{
	return adc_model;
}

int TKADCCONTROL::Open(int wire_type_, const char* adress_)
{
	wire_type = wire_type_;
	strcpy(adress, adress_);
	if (TmctlQt::TmcInitialize(wire_type, adress, &device_id))
		return TmctlQt::TmcGetLastError(device_id);
	return 0;
}

int TKADCCONTROL::Close()
{
	if (TmctlQt::TmcFinish(device_id))
		return TmctlQt::TmcGetLastError(device_id);
	return 0;
}

int TKADCCONTROL::SendMessage(const char* message)
{
	if (TmctlQt::TmcSend(device_id, (char*)message))
		return TmctlQt::TmcGetLastError(device_id);
	else
		return 0;
}

int TKADCCONTROL::Start()
{
	return this->SendMessage(":start");
}
int TKADCCONTROL::Stop()
{
	return this->SendMessage(":stop");
}



int TKADCCONTROL::WaitADC(TKADCCONTROL::CONDITIONFLAG flag)
{
	char rcv_msg[1024];
	int rcv_length;

	for (;;) {
		int condition;

		if (TmctlQt::TmcSend(device_id, ":status:condition?"))
			goto error;
		if (TmctlQt::TmcReceive(device_id, rcv_msg, sizeof(rcv_msg), &rcv_length))
			goto error;
		condition = static_cast<int>(std::stol((std::string)rcv_msg)) & static_cast<int>(flag);
		if (!condition)
			break;
#ifdef _MANAGED
		Sleep(1);
		System::Windows::Forms::Application::DoEvents();
#endif
	}
	return 0;
error:
	return TmctlQt::TmcGetLastError(device_id);
}
int TKADCCONTROL::GetDeviceID(void)
{
	return this->device_id;
}

int TKADCCONTROL::SaveShot(std:: string file_name)
{
	std::string send_msg;

	send_msg = (std::string)":file:save:name \"" + file_name + "\"";
	if (TmctlQt::TmcSend(device_id, (char*)send_msg.c_str()))
		goto error;
	if (TmctlQt::TmcSend(device_id, ":file:save:binary"))
		goto error;
	return 0;
error:
	return TmctlQt::TmcGetLastError(device_id);
}

int TKADCCONTROL::GetStatusCondition(TKADCCONTROL::CONDITIONFLAG flag)
{
	char rcv_msg[1024];
	int rcv_length;

	if (TmctlQt::TmcSend(device_id, ":status:condition?"))
		goto error;
	if (TmctlQt::TmcReceive(device_id, rcv_msg, sizeof(rcv_msg), &rcv_length))
		goto error;
	return static_cast<int>(std::stol((std::string)rcv_msg)) & static_cast<int>(flag);
error:
	return TmctlQt::TmcGetLastError(device_id);
}

int TKADCCONTROL::Delete(std::string file_name)
{
	return 0;
}

int TKADCCONTROL::GetLastLocalShotNumber()
{
	return next_local_shot_number - 1;
}

int TKADCCONTROL::GetNextLocalShotNumber()
{
	return next_local_shot_number;
}

int TKADCCONTROL::SetLastLocalShotNumber(int new_local_shot_number)
{
	return next_local_shot_number = new_local_shot_number + 1;
}

int TKADCCONTROL::IncrementLocalShotNumber()
{
	next_local_shot_number++;
	if (next_local_shot_number > local_shot_number_max)
		next_local_shot_number = 1;
	return next_local_shot_number;
}

int TKADCCONTROL::SetLocalShotNumberMax(int new_local_shot_number_max)
{
	return local_shot_number_max = new_local_shot_number_max;
}

int TKADCCONTROL::GetLocalShotNumberMax()
{
	return local_shot_number_max;
}




TKADCCONTROL_DL750::TKADCCONTROL_DL750() : TKADCCONTROL(TKADC_ADC_MODEL_DL750)
{
}
TKADCCONTROL_DL850::TKADCCONTROL_DL850() : TKADCCONTROL(TKADC_ADC_MODEL_DL850)
{
}
int TKADCCONTROL_DL750::GetStatusCondition(TKADCCONTROL_DL750::CONDITIONFLAG flag)
{
	char rcv_msg[1024];
	int rcv_length;

	int device_id = GetDeviceID();

	if (TmctlQt::TmcSend(device_id, ":status:condition?"))
		goto error;
	if (TmctlQt::TmcReceive(device_id, rcv_msg, sizeof(rcv_msg), &rcv_length))
		goto error;
	return static_cast<int>(std::stol((std::string)rcv_msg)) & static_cast<int>(flag);
error:
	return TmctlQt::TmcGetLastError(device_id);
}
int TKADCCONTROL_DL750::Delete(std::string file_name)
{
	int device_id = GetDeviceID();
	std::string send_msg = (std::string)":file:delete:binary \"" + file_name + "\"";

	if (TmctlQt::TmcSend(device_id, (char*)send_msg.c_str()))
		goto error;
	if (TmctlQt::TmcSend(device_id, ":file:save:binary"))
		goto error;
	return 0;
error:
	return TmctlQt::TmcGetLastError(device_id);
}

int TKADCCONTROL_DL850::GetStatusCondition(TKADCCONTROL_DL850::CONDITIONFLAG flag)
{
	char rcv_msg[1024];
	int rcv_length;

	int device_id = GetDeviceID();

	if (TmctlQt::TmcSend(device_id, ":status:condition?"))
		goto error;
	if (TmctlQt::TmcReceive(device_id, rcv_msg, sizeof(rcv_msg), &rcv_length))
		goto error;
	return static_cast<int>(std::stol((std::string)rcv_msg)) & static_cast<int>(flag);
error:
	return TmctlQt::TmcGetLastError(device_id);
}
int TKADCCONTROL_DL850::Delete(std::string file_name)
{
	int device_id = GetDeviceID();
	std::string send_msg = (std::string)":file:delete \"" + file_name + ".WDF\"";

	if (TmctlQt::TmcSend(device_id, (char*)send_msg.c_str()))
		goto error;
	if (TmctlQt::TmcSend(device_id, ":file:save:binary"))
		goto error;
	return 0;
error:
	return TmctlQt::TmcGetLastError(device_id);
}
