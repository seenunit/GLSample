// Parasolid.cpp : implementation file
//

#include "stdafx.h"

void RegisterParasolidFrustrum()
{
	PK_SESSION_frustrum_t fr;
	PK_SESSION_frustrum_o_m(fr);
	
	fr.fstart = FSTART;
	fr.fabort = FABORT;
	fr.fstop = FSTOP;
	fr.fmallo = FMALLO;
	fr.fmfree = FMFREE;
	//fr.gosgmt = GOSGMT;
	//fr.goopsg = GOOPSG;
	//fr.goclsg = GOCLSG;
	//fr.gopixl = GOPIXL;
	//fr.gooppx = GOOPPX;
	//fr.goclpx = GOCLPX;
	fr.ffoprd = FFOPRD;
	fr.ffopwr = FFOPWR;
	fr.ffclos = FFCLOS;
	fr.ffread = FFREAD;
	fr.ffwrit = FFWRIT;
	fr.ffoprb = FFOPRB;
	fr.ffseek = FFSEEK;
	fr.fftell = FFTELL;
	//fr.fgcrcu = FGCRCU;
	//fr.fgcrsu = FGCRSU;
	//fr.fgevcu = FGEVCU;
	//fr.fgevsu = FGEVSU;
	//fr.fgprcu = FGPRCU;
	//fr.fgprsu = FGPRSU;
	//fr.ucoprd = UCOPRD;
	//fr.ucopwr = UCOPWR;
	PK_SESSION_register_frustrum(&fr);
}

bool ParasolidSessionStart()
{
	//Register frustrum
	RegisterParasolidFrustrum();

	//session start
	PK_SESSION_start_o_t pk_otStart;
	PK_SESSION_start_o_m(pk_otStart);
	PK_ERROR_t pk_Err = PK_SESSION_start(&pk_otStart);

	if(pk_Err)
	{
		PK_SESSION_stop();
		return false;
	}
	return true;
}

bool ParasolidSessionStop()
{
	//session stop
	PK_ERROR_t pk_Err = PK_SESSION_stop();

	if(pk_Err)
	{
		PK_SESSION_stop();
		return false;
	}
	return true;
}