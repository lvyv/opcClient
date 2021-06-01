////////////////////////////////////////////////////////////////////////
// project: opcClient
// version: 2 
//
// subject: this is a simple OPC Client that read the value of items
// from an OPC server.
//
// file: SimpleOPCClient_v2.cpp
//
// coder: Awen - CASIC Cloud
//
// https://github.com/lvyv/opcClient

#include <atlbase.h>
#include <comutil.h>

#include <objbase.h>
#include <iostream>
#include <string>

#include "include/opcda.h"
#include "include/opcda_i.c"
#include "SimpleOPCClient_v2.h"

using namespace std;

//#define OPC_SERVER_NAME L"OPC.Evaluation:HV supply.1" // e.g.: L"OPC.Evaluation:HV supply.1"
#define OPC_SERVER_NAME L"OPCServer.WinCC.1"	
//#define ITEM_ID L"cr02.ch00.V0"
#define ITEM_ID L"DB2strB"
#define VT VT_BSTR
#define XVAL bstrVal




void SimpleRW() {
	// have to be done before using microsoft COM library:
	CoInitialize(NULL);
	
	IOPCServer* pIOPCServer = NULL;   //pointer to IOPServer interface
	IOPCItemMgt* pIOPCItemMgt = NULL; //pointer to IOPCItemMgt interface

	OPCHANDLE hServerGroup; // server handle to the group
	OPCHANDLE hServerItem;  // server handle to the item
	
	pIOPCServer = InstantiateServer(OPC_SERVER_NAME);

	AddTheGroup(pIOPCServer, pIOPCItemMgt, hServerGroup);
	// Add the OPC item
    AddTheItem(pIOPCItemMgt, hServerItem);
	
	//Read the value of the item from device:
	VARIANT varValue; //to stor the read value
	VariantInit(&varValue);
	ReadItem(pIOPCItemMgt, hServerItem, varValue);
	// print the read value:
	const std::string stdstr(_bstr_t(varValue.bstrVal, true));
	cout << "Read value: " << stdstr << endl;
	//if(varValue.XVAL != 0) 
	//	varValue.fltVal = 0;
	//else
	//	varValue.fltVal = -1;
	//WriteItem(pIOPCItemMgt, hServerItem, varValue);
	//CoTaskMemFree(&(varValue.bstrVal));//how to free?
	SysFreeString(varValue.bstrVal);
	VariantClear(&varValue);
	
	// Remove the OPC item:
	RemoveItem(pIOPCItemMgt, hServerItem);
	// Remove the OPC group: 
    RemoveGroup(pIOPCServer, hServerGroup);
	// release the interface references:
	pIOPCItemMgt->Release();


	pIOPCServer->Release();
	CoUninitialize();
}






void TestSingleRW() {
	IOPCServer* pIOPCServer = NULL;   //pointer to IOPServer interface
	IOPCItemMgt* pIOPCItemMgt = NULL; //pointer to IOPCItemMgt interface

	OPCHANDLE hServerGroup; // server handle to the group
	OPCHANDLE hServerItem;  // server handle to the item
	

	// have to be done before using microsoft COM library:
	CoInitialize(NULL);

	// Let's instantiante the IOPCServer interface and get a pointer of it:
	pIOPCServer = InstantiateServer(OPC_SERVER_NAME);

	// Add the OPC group the OPC server and get an handle to the IOPCItemMgt
	//interface:
	AddTheGroup(pIOPCServer, pIOPCItemMgt, hServerGroup);

	// Add the OPC item
    AddTheItem(pIOPCItemMgt, hServerItem);

	////Read the value of the item from device:
	VARIANT varValue; //to stor the read value
	VariantInit(&varValue);
	
	while(TRUE) {
		ReadItem(pIOPCItemMgt, hServerItem, varValue);
		// print the read value:
		cout << "Read value: " << varValue.XVAL << endl;
		if(varValue.XVAL != 0) 
			varValue.fltVal = 0;
		else
			varValue.fltVal = -1;
		WriteItem(pIOPCItemMgt, hServerItem, varValue);
	}
	// Remove the OPC item:
	RemoveItem(pIOPCItemMgt, hServerItem);

	// Remove the OPC group: 
    RemoveGroup(pIOPCServer, hServerGroup);

	// release the interface references:
	pIOPCItemMgt->Release();
	pIOPCServer->Release();

	//close the COM library:
	CoUninitialize();
}

//////////////////////////////////////////////////////////////////////
// Read the value of an item on an OPC server. 
//
void TestMultiRW() {

	IOPCServer* pIOPCServer = NULL;   //pointer to IOPServer interface
	IOPCItemMgt* pIOPCItemMgt = NULL; //pointer to IOPCItemMgt interface

	OPCHANDLE hServerGroup; // server handle to the group
	//OPCHANDLE hServerItem;  // server handle to the item
	

	// have to be done before using microsoft COM library:
	CoInitialize(NULL);

	// Let's instantiante the IOPCServer interface and get a pointer of it:
	pIOPCServer = InstantiateServer(OPC_SERVER_NAME);

	// Add the OPC group the OPC server and get an handle to the IOPCItemMgt
	//interface:
	AddTheGroup(pIOPCServer, pIOPCItemMgt, hServerGroup);

	// Add the OPC item
    // AddTheItem(pIOPCItemMgt, hServerItem);

	// Add the OPC items
	// Array of items to add:
	OPCITEMDEF Items[3] =
	{{
	/*szAccessPath*/ L"",
	/*szItemID*/ L"213_BTN_I07",
	/*bActive*/ FALSE,
	/*hClient*/ 1,
	/*dwBlobSize*/ 0,
	/*pBlob*/ NULL,
	/*vtRequestedDataType*/ VT_R4,
	/*wReserved*/0
	},{
		L"",
		L"213_BTN_I02",
		FALSE,
		2,
		0,
		NULL,
		VT_R4,
		0
	},{
		L"",
		L"213_LED_Q01",
		FALSE,
		2,
		0,
		NULL,
		VT_R4,
		0
	}};
	
	OPCHANDLE hServerItems[3];	// server handle array

	AddItems(pIOPCItemMgt, hServerItems,Items);

	VARIANT varValue[3];
	for(LONG ii = 0; ii < 3; ii++) {
		VariantInit(&varValue[ii]);
	}

	while(TRUE) {
		ReadItems(pIOPCItemMgt, hServerItems, varValue);
		cout << "(I07: " << varValue[0].XVAL 
			 << " I02: " << varValue[1].XVAL 
			 << " Q01: " << varValue[2].XVAL << endl;
		if(varValue[0].XVAL != 0) 
			varValue[0].fltVal = 0;
		else
			varValue[0].fltVal = -1;
		WriteItems(pIOPCItemMgt, hServerItems, varValue);
	}
	
	RemoveItems(pIOPCItemMgt, hServerItems);

	// Remove the OPC group: 
    RemoveGroup(pIOPCServer, hServerGroup);

	// release the interface references:
	pIOPCItemMgt->Release();
	pIOPCServer->Release();

	//close the COM library:
	CoUninitialize();
}


//////////////////////////////////////////////////////////////////////
// Read the value of an item on an OPC server. 
//
void InitGroup(OPCITEMDEF (&Items)[3]) {

	IOPCServer* pIOPCServer = NULL;   //pointer to IOPServer interface
	IOPCItemMgt* pIOPCItemMgt = NULL; //pointer to IOPCItemMgt interface

	OPCHANDLE hServerGroup; // server handle to the group
	//OPCHANDLE hServerItem;  // server handle to the item
	

	// have to be done before using microsoft COM library:
	CoInitialize(NULL);

	// Let's instantiante the IOPCServer interface and get a pointer of it:
	pIOPCServer = InstantiateServer(OPC_SERVER_NAME);

	// Add the OPC group the OPC server and get an handle to the IOPCItemMgt
	//interface:
	AddTheGroup(pIOPCServer, pIOPCItemMgt, hServerGroup);

	// Add the OPC item
    // AddTheItem(pIOPCItemMgt, hServerItem);

	//// Add the OPC items
	//// Array of items to add:
	//OPCITEMDEF Items[3] =
	//{{
	///*szAccessPath*/ L"",
	///*szItemID*/ L"213_BTN_I07",
	///*bActive*/ FALSE,
	///*hClient*/ 1,
	///*dwBlobSize*/ 0,
	///*pBlob*/ NULL,
	///*vtRequestedDataType*/ VT_R4,
	///*wReserved*/0
	//},{
	//	L"",
	//	L"213_BTN_I02",
	//	FALSE,
	//	2,
	//	0,
	//	NULL,
	//	VT_R4,
	//	0
	//},{
	//	L"",
	//	L"213_LED_Q01",
	//	FALSE,
	//	2,
	//	0,
	//	NULL,
	//	VT_R4,
	//	0
	//}};
	
	OPCHANDLE hServerItems[3];	// server handle array

	AddItems(pIOPCItemMgt, hServerItems, Items);

	VARIANT varValue[3];
	for(LONG ii = 0; ii < 3; ii++) {
		VariantInit(&varValue[ii]);
	}

	while(TRUE) {
		ReadItems(pIOPCItemMgt, hServerItems, varValue);
		cout << "(I07: " << varValue[0].XVAL 
			 << " I02: " << varValue[1].XVAL 
			 << " Q01: " << varValue[2].XVAL << endl;
		if(varValue[0].XVAL != 0) 
			varValue[0].fltVal = 0;
		else
			varValue[0].fltVal = -1;
		WriteItems(pIOPCItemMgt, hServerItems, varValue);
	}
	
	RemoveItems(pIOPCItemMgt, hServerItems);

	// Remove the OPC group: 
    RemoveGroup(pIOPCServer, hServerGroup);

	// release the interface references:
	pIOPCItemMgt->Release();
	pIOPCServer->Release();

	//close the COM library:
	CoUninitialize();
}


void main(void)
{
	//TestSingleRW();
	//TestMultiRW();
	while(true) {
		SimpleRW();
	}
}

////////////////////////////////////////////////////////////////////
// Instantiate the IOPCServer interface of the OPCServer
// having the name ServerName. Return a pointer to this interface
//
IOPCServer* InstantiateServer(wchar_t ServerName[])
{
	CLSID CLSID_OPCServer;
	HRESULT hr;

	// get the CLSID from the OPC Server Name:
	hr = CLSIDFromString(ServerName, &CLSID_OPCServer);
	_ASSERT(!FAILED(hr));

	IOPCServer* pIOPCSvr = NULL;
	hr = CoCreateInstance(CLSID_OPCServer, NULL, CLSCTX_SERVER, IID_IOPCServer, reinterpret_cast<void**>(&pIOPCSvr));
	_ASSERT(!hr);


	return pIOPCSvr;
}


/////////////////////////////////////////////////////////////////////
// Add group "Group1" to the Server whose IOPCServer interface
// is pointed by pIOPCServer. 
// Returns a pointer to the IOPCItemMgt interface of the added group
// and a server opc handle to the added group.
//
void AddTheGroup(IOPCServer* pIOPCServer, IOPCItemMgt* &pIOPCItemMgt, 
				 OPCHANDLE& hServerGroup)
{
	DWORD dwUpdateRate = 0;
	OPCHANDLE hClientGroup = 0;
	
	

	// Add an OPC group and get a pointer to the IUnknown I/F:
    HRESULT hr = pIOPCServer->AddGroup(/*szName*/ L"CASIC_WINCC_RW_GRP",
		/*bActive*/ FALSE,
		/*dwRequestedUpdateRate*/ dwUpdateRate,
		/*hClientGroup*/ hClientGroup,
		/*pTimeBias*/ 0,
		/*pPercentDeadband*/ 0,
		/*dwLCID*/0,
		/*phServerGroup*/&hServerGroup,
		&dwUpdateRate,
		/*riid*/ IID_IOPCItemMgt,
		/*ppUnk*/ (IUnknown**) &pIOPCItemMgt);
	
	_ASSERT(!FAILED(hr));
}

//////////////////////////////////////////////////////////////////
// Add the Item ITEM_ID to the group whose IOPCItemMgt interface
// is pointed by pIOPCItemMgt pointer. Return a server opc handle
// to the item.
 
void AddTheItem(IOPCItemMgt* pIOPCItemMgt, OPCHANDLE& hServerItem)
{
	HRESULT hr;

	// Array of items to add:
	OPCITEMDEF ItemArray[1] =
	{{
	/*szAccessPath*/ L"",
	/*szItemID*/ ITEM_ID,
	/*bActive*/ FALSE,
	/*hClient*/ 1,
	/*dwBlobSize*/ 0,
	/*pBlob*/ NULL,
	/*vtRequestedDataType*/ VT,
	/*wReserved*/0
	}};

	//Add Result:
	OPCITEMRESULT* pAddResult=NULL;
	HRESULT* pErrors = NULL;

	// Add an Item to the previous Group:
	hr = pIOPCItemMgt->AddItems(1, ItemArray, &pAddResult, &pErrors);
	_ASSERT(!hr);

	// Server handle for the added item:
	hServerItem = pAddResult[0].hServer;

	// release memory allocated by the server:
	CoTaskMemFree(pAddResult[0].pBlob);//->pBlob);

	CoTaskMemFree(pAddResult);
	pAddResult = NULL;

	CoTaskMemFree(pErrors);
	pErrors = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Read from device the value of the item having the "hServerItem" server 
// handle and belonging to the group whose one interface is pointed by
// pGroupIUnknown. The value is put in varValue. 
//
void ReadItem(IUnknown* pGroupIUnknown, OPCHANDLE hServerItem, VARIANT& varValue)
{
	// value of the item:
	OPCITEMSTATE* pValue = NULL;

	//get a pointer to the IOPCSyncIOInterface:
	IOPCSyncIO* pIOPCSyncIO;
	pGroupIUnknown->QueryInterface(__uuidof(pIOPCSyncIO), (void**) &pIOPCSyncIO);

	// read the item value from the device:
	HRESULT* pErrors = NULL; //to store error code(s)
	HRESULT hr = pIOPCSyncIO->Read(OPC_DS_CACHE, 1, &hServerItem, &pValue, &pErrors);
	_ASSERT(!hr);
	_ASSERT(pValue!=NULL);

	varValue = pValue[0].vDataValue;
	VariantClear(&pValue[0].vDataValue);
	CoTaskMemFree(&pValue[0].ftTimeStamp);


	//Release memeory allocated by the OPC server:
	CoTaskMemFree(pErrors);
	pErrors = NULL;

	CoTaskMemFree(pValue);
	pValue = NULL;

	// release the reference to the IOPCSyncIO interface:
	pIOPCSyncIO->Release();
	pIOPCSyncIO = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Write the value of the item having the "hServerItem" server 
// handle and belonging to the group whose one interface is pointed by
// pGroupIUnknown. The value is put in varValue. 
//
void WriteItem(IUnknown* pGroupIUnknown, OPCHANDLE hServerItem, VARIANT& varValue)
{

	//get a pointer to the IOPCSyncIOInterface:
	IOPCSyncIO* pIOPCSyncIO;
	pGroupIUnknown->QueryInterface(__uuidof(pIOPCSyncIO), (void**) &pIOPCSyncIO);

	// read the item value from the device:
	HRESULT* pErrors = NULL; //to store error code(s)
	HRESULT hr = pIOPCSyncIO->Write(1, &hServerItem, &varValue, &pErrors);

	_ASSERT(!hr);



	//Release memeory allocated by the OPC server:
	CoTaskMemFree(pErrors);
	pErrors = NULL;

	// release the reference to the IOPCSyncIO interface:
	pIOPCSyncIO->Release();
	pIOPCSyncIO = NULL;
}

///////////////////////////////////////////////////////////////////////////
// Remove the item whose server handle is hServerItem from the group
// whose IOPCItemMgt interface is pointed by pIOPCItemMgt
//
void RemoveItem(IOPCItemMgt* pIOPCItemMgt, OPCHANDLE hServerItem)
{
	// server handle of items to remove:
	OPCHANDLE hServerArray[1];
	hServerArray[0] = hServerItem;
	
	//Remove the item:
	HRESULT* pErrors; // to store error code(s)
	HRESULT hr = pIOPCItemMgt->RemoveItems(1, hServerArray, &pErrors);
	_ASSERT(!hr);

	//release memory allocated by the server:
	CoTaskMemFree(pErrors);
	pErrors = NULL;
}


////////////////////////////////////////////////////////////////////////
// Remove the Group whose server handle is hServerGroup from the server
// whose IOPCServer interface is pointed by pIOPCServer
//
void RemoveGroup (IOPCServer* pIOPCServer, OPCHANDLE hServerGroup)
{
	// Remove the group:
	HRESULT hr = pIOPCServer->RemoveGroup(hServerGroup, TRUE);//0x0004000FL,inuse.
	_ASSERT(!hr);
}