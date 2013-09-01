// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"


#include "UiCommands.h"


using namespace Tpc::Ui;

#ifdef _ACRXAPP

//-----------------------------------------------------------------------------
#define szRDS _RXST("Sm")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CTreePcApp : public AcRxArxApp {

public:
	CTreePcApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void Tpc_PC_ADD ()
	{
		try
		{
			ACHAR buff[MAX_PATH];
			int ret = acedGetStringB(1,_T("Get point cloud txt file path: "),buff,MAX_PATH-1);
			if ( ret==RTCAN || ret!= RTNORM )
			{
				acutPrintf(_T("\nNo path input. Exit.\n"));
				return;
			}

			std::wstring wfilePath(buff);
			std::string filePath(wfilePath.begin(), wfilePath.end());

			Commands::understandTree(filePath.c_str());
		}
		catch(...)
		{
			acutPrintf(_T("\nArgument parsing error. Exit.\n"));
		}
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CTreePcApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CTreePcApp, Tpc, _PC_ADD, PC_ADD, ACRX_CMD_MODAL, NULL)

#endif