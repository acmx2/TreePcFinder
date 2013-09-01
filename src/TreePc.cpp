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
//- TreePc.cpp : Initialization functions
//-----------------------------------------------------------------------------
#include "StdAfx.h"


#ifdef _ACRXAPP
//-----------------------------------------------------------------------------
//- DLL Entry Point
extern "C"
BOOL WINAPI DllMain (HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	//- Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved) ;

	if ( dwReason == DLL_PROCESS_ATTACH ) {
        _hdllInstance =hInstance ;
	} else if ( dwReason == DLL_PROCESS_DETACH ) {
	}
	return (TRUE) ;
}

#else

#include "UiCommands.h"
#include <string>

using namespace Tpc::Ui;
using namespace std;

int _tmain(int argc, wchar_t* argv[])
{
	try
	{
		if (argc <2)
		{
			cout <<"\nExpecting point cloud .txt file path as argument. Exit.";
			return 0;
		}

		std::wstring wfilePath(argv[1]);
		std::string filePath(wfilePath.begin(), wfilePath.end());

		Commands::understandTree(filePath.c_str());
	}
	catch(...)
	{
		std::cout << "\nArgument parsing error. Exit.\n";
	}

	return 0;
}

#endif
