#include "tmctlqt.h"


TmctlQt::TmctlQt()
{
//	tmctlLib = new QLibrary("tmctl.dll");
}

TmctlQt::~TmctlQt()
{
//	tmctlLib.unload();
}

int TmctlQt::TmcInitialize(int wire, const char* adr, int* id) {
	QLibrary tmctlLib("tmctl");
	pTmcInitialize TmcInitialize;
	TmcInitialize = (pTmcInitialize)tmctlLib.resolve("TmcInitialize") ;
	tmctlLib.unload();
	return TmcInitialize(wire, adr, id);
}

int TmctlQt::TmcSend(int id, const char* msg) {
	QLibrary tmctlLib("tmctl");
	pTmcSend TmcSend;
	TmcSend = (pTmcSend)tmctlLib.resolve("TmcSend");
	tmctlLib.unload();
	return TmcSend(id, msg);
}

int TmctlQt::TmcGetLastError(int id)
{
	QLibrary tmctlLib("tmctl");
	pTmcGetLastError TmcGetLastError;
	TmcGetLastError = (pTmcGetLastError)tmctlLib.resolve("TmcGetLastError");
	tmctlLib.unload();
	return TmcGetLastError(id);
}

int TmctlQt::TmcReceive(int id, const char* buff, int blen, int* rlen)
{
	QLibrary tmctlLib("tmctl");
	pTmcReceive TmcReceive;
	TmcReceive = (pTmcReceive)tmctlLib.resolve("TmcReceive");
	tmctlLib.unload();
	return TmcReceive(id, buff, blen, rlen);
}

int TmctlQt::TmcFinish(int id)
{
	QLibrary tmctlLib("tmctl");
	pTmcFinish TmcFinish;
	TmcFinish = (pTmcFinish)tmctlLib.resolve("TmcFinish");
	tmctlLib.unload();
	return TmcFinish(id);
}


