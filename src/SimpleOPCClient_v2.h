////////////////////////////////////////////////////////////////////////
// project: opcClient
// version: 2 
//
// subject: this is a simple OPC Client that read the value of items
// from an OPC server.
//
// file: SimpleOPCClient_v2.h
//
// coder: Awen - CASIC Cloud
//
// https://github.com/lvyv/opcClient



#ifndef SIMPLE_OPC_CLIENT_H
#define SIMPLE_OPC_CLIENT_H

IOPCServer* InstantiateServer(wchar_t ServerName[]);
void AddTheGroup(IOPCServer* pIOPCServer, IOPCItemMgt* &pIOPCItemMgt, 
				 OPCHANDLE& hServerGroup);
void AddTheItem(IOPCItemMgt* pIOPCItemMgt, OPCHANDLE& hServerItem);
void ReadItem(IUnknown* pGroupIUnknown, OPCHANDLE hServerItem, VARIANT& varValue);
void WriteItem(IUnknown* pGroupIUnknown, OPCHANDLE hServerItem, VARIANT& varValue);
void RemoveItem(IOPCItemMgt* pIOPCItemMgt, OPCHANDLE hServerItem);
void RemoveGroup (IOPCServer* pIOPCServer, OPCHANDLE hServerGroup);

//////////////////////////////////////////////////////////////////
// Add the Item ITEM_ID to the group whose IOPCItemMgt interface
// is pointed by pIOPCItemMgt pointer. Return a server opc handle
// to the item.
template<typename TH, size_t CN>
void AddItems(IOPCItemMgt* pIOPCItemMgt, TH(&hSvrItems)[CN], OPCITEMDEF (&ItemArray)[CN])
{
	HRESULT hr;

	//Add Result:
	OPCITEMRESULT* pAddResult=NULL;
	HRESULT* pErrors = NULL;

	// Add an Item to the previous Group:
	hr = pIOPCItemMgt->AddItems(3, ItemArray, &pAddResult, &pErrors);
	_ASSERT(!hr);

	// Server handle for the added item:
	for(LONG ii = 0; ii < CN; ii++) {
		hSvrItems[ii] = pAddResult[ii].hServer;
	}
	// release memory allocated by the server:
	CoTaskMemFree(pAddResult->pBlob);

	CoTaskMemFree(pAddResult);
	pAddResult = NULL;

	CoTaskMemFree(pErrors);
	pErrors = NULL;
};
///////////////////////////////////////////////////////////////////////////
// Remove items to be fixed.
//
//
template<typename TH, size_t CN>
void RemoveItems(IOPCItemMgt* pIOPCItemMgt, TH(&hSvrItems)[CN])
{
	
	//Remove the item:
	HRESULT* pErrors; // to store error code(s)
	HRESULT hr = pIOPCItemMgt->RemoveItems(CN, hSvrItems, &pErrors);
	_ASSERT(!hr);

	//release memory allocated by the server:
	CoTaskMemFree(pErrors);
	pErrors = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Read from device the value of the item having the "hServerItem" server 
// handle and belonging to the group whose one interface is pointed by
// pGroupIUnknown. The value is put in varValue. 
//
template<typename TH, size_t CN>
void ReadItems(IUnknown* pGroupIUnknown, TH(&hSvrItems)[CN], VARIANT(& varValue)[CN])
{
	// value of the item:
	OPCITEMSTATE* pValues = NULL;

	//get a pointer to the IOPCSyncIOInterface:
	IOPCSyncIO* pIOPCSyncIO;
	pGroupIUnknown->QueryInterface(__uuidof(pIOPCSyncIO), (void**) &pIOPCSyncIO);

	// read the item value from the device:
	HRESULT* pErrors = NULL; //to store error code(s)
	HRESULT hr = pIOPCSyncIO->Read(OPC_DS_CACHE, CN, hSvrItems, &pValues, &pErrors);
	_ASSERT(!hr);
	_ASSERT(pValues!=NULL);

	for(LONG ii = 0; ii < CN; ii++) {
		varValue[ii] = pValues[ii].vDataValue;
		VariantClear(&pValues[ii].vDataValue);//caller should be in charge of clear memory 
	}

	//Release memeory allocated by the OPC server:
	CoTaskMemFree(pErrors);
	pErrors = NULL;

	CoTaskMemFree(pValues);//是否调用了内部指针释放.
	pValues = NULL;

	// release the reference to the IOPCSyncIO interface:
	pIOPCSyncIO->Release();
}

#endif // SIMPLE_OPC_CLIENT_H not defined