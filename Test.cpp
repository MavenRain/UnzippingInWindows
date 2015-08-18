#include <fstream>
#include <iostream>
#include <Windows.h>
#include <ShlDisp.h>
#include <thread>
#include <chrono>

#pragma comment(lib,"Shell32.lib")

using namespace std;
using namespace this_thread;
using namespace chrono;

int main()
{
	CoInitialize(nullptr);

	BSTR source = L"C:\\Users\\v-oniobi\\Documents\\TestThatShouldReadTextFileFromZipFile\\Debug\\foo.zip\0\0";
	BSTR destination = L"bar.txt\0\0";
	Folder* fromDirectory = nullptr;
	Folder* toDirectory = nullptr;
	VARIANT fileSource;
	VARIANT options;
	VARIANT newItems;
	VARIANT fileDestination;
	FolderItem* folderItems = nullptr;
	IShellDispatch* dispatch;
	if (FAILED(CoCreateInstance(CLSID_Shell, nullptr, CLSCTX_INPROC_SERVER, IID_IShellDispatch, reinterpret_cast<void**>(&dispatch))))
	{
		cout << "There was an issue generating the shell dispatcher.\r\n";
		return -1111;
	}
	VariantInit(&fileSource);
	fileSource.vt = VT_BSTR;
	fileSource.bstrVal = source;
	if (FAILED(dispatch->NameSpace(fileSource, &fromDirectory)))
	{
		cout << "There was an issue dispatching from the zip file.\r\n";
		return -33333;
	}
	fromDirectory->ParseName(destination, &folderItems);
	VariantInit(&options);
	options.vt = VT_I4;
	options.lVal = FOF_NO_UI;
	VariantInit(&newItems);
	newItems.vt = VT_DISPATCH;
	newItems.pdispVal = folderItems;
	VariantInit(&fileDestination);
	fileDestination.vt = VT_BSTR;
	fileDestination.bstrVal = L"C:\\Users\\v-oniobi\\Documents\\TestThatShouldReadTextFileFromZipFile\\Debug";
	dispatch->NameSpace(fileDestination, &toDirectory);
	if (FAILED(toDirectory->CopyHere(newItems, options)))
	{
		cout << "There was a problem executing the file copy to a new location.\r\n";
		return -2222;
	}
	sleep_for(seconds(1));
	fromDirectory->Release();
	toDirectory->Release();
	ifstream inputText;
	inputText.open("C:\\Users\\v-oniobi\\Documents\\TestThatShouldReadTextFileFromZipFile\\Debug\\bar.txt");
	char buffer[50];
	while (!inputText.eof())
	{
		inputText >> buffer;
		cout << buffer << "\r\n";
	}
	inputText.close();
	CoUninitialize();

	cout << "Press enter to continue.\r\n";
	cin.ignore();

	return 0;
}