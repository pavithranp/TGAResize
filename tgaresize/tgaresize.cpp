// tgaresize.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
using namespace std;
// used TGA structure reference from this site http://www.paulbourke.net/dataformats/tga/
struct Header
{
	//No.of Characters in Identification field
	uint8_t idlength;
	uint8_t colourmaptype;	
	uint8_t datatypecode;
	//Color Map specification
	uint16_t colourmaporigin;
	uint16_t colourmaplength;
	uint8_t  colourmapdepth;
	
	//ImageSpecification
	uint16_t x_origin;
	uint16_t y_origin;
	uint16_t Width;
	uint16_t Height;
	uint8_t  Bits;
	uint8_t  ImageDescriptor;
};
int main(int argc, char *argv[])
{
	Header TGAHeader;
	int c = 0;

	if ((argc > 4) || (argc < 3)) {
		cout << "Usage :  tgaresize.exe <inputfile> <outfile name> <optional sampling method 0/1> " << endl;
		cout << "ERROR : format entered is wrong, exiting..." << endl;
		return 1;
	}
	int sampling = (argc == 3) ? 0 : atoi(argv[3]); // change sampling to down sampling ,i.e 
	
	cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";
	cout << "Sampling used is " << sampling << endl;
	//std::ifstream  src("XING_B32.TGA", std::ios::binary);
	std::ifstream  src(argv[1], std::ios::binary);
	if (src.is_open())
	{
		cout << "file found and opened successfully" << endl;
			}
	else
	{
		cout << "ERROR : file not found" << endl;
		return 1;
	}
	//std::ifstream  src("FARO_LOGO.tga", std::ios::binary);
	//std::ifstream  src("CTC24.TGA", std::ios::binary);
	//std::ofstream  dst("to.TGA", std::ios::binary| std::ios::out);
	std::ofstream  dst(argv[2], std::ios::binary | std::ios::out);
	//Finding File size before working on the header
	src.seekg(0, std::ios_base::end);  // moving cursor to find the file size
	size_t FileSize = src.tellg();
	src.seekg(0, std::ios_base::beg);
	
	
	uint8_t* Data;
	int orgHeight, orgWidth;
	//read file according to the structure
	src.read((char*)&TGAHeader.idlength, sizeof(TGAHeader.idlength));
	src.read((char*)&TGAHeader.colourmaptype, sizeof(TGAHeader.colourmaptype));
	src.read((char*)&TGAHeader.datatypecode, sizeof(TGAHeader.datatypecode));
	src.read((char*)&TGAHeader.colourmaporigin, sizeof(TGAHeader.colourmaporigin));
	src.read((char*)&TGAHeader.colourmaplength, sizeof(TGAHeader.colourmaplength));
	src.read((char*)&TGAHeader.colourmapdepth, sizeof(TGAHeader.colourmapdepth));
	src.read((char*)&TGAHeader.x_origin, sizeof(TGAHeader.x_origin));
	src.read((char*)&TGAHeader.y_origin, sizeof(TGAHeader.y_origin));
	src.read((char*)&TGAHeader.Width, sizeof(TGAHeader.Width));
	src.read((char*)&TGAHeader.Height, sizeof(TGAHeader.Height));
	src.read((char*)&TGAHeader.Bits, sizeof(TGAHeader.Bits));
	src.read((char*)&TGAHeader.ImageDescriptor, sizeof(TGAHeader.ImageDescriptor));
	if (int(TGAHeader.datatypecode) != 2)
	{
		std::cout << "ERROR :sorry cannot read non-RGB or RLE" << endl;
		return 1;
	}
	
	orgWidth = TGAHeader.Width ;
	orgHeight = TGAHeader.Height ;
	TGAHeader.Width = TGAHeader.Width/2;
	TGAHeader.Height = TGAHeader.Height/2;
	//Copy the sample header with different width and height
	dst.write((char*)&TGAHeader.idlength, sizeof(TGAHeader.idlength));
	dst.write((char*)&TGAHeader.colourmaptype, sizeof(TGAHeader.colourmaptype));
	dst.write((char*)&TGAHeader.datatypecode, sizeof(TGAHeader.datatypecode));
	dst.write((char*)&TGAHeader.colourmaporigin, sizeof(TGAHeader.colourmaporigin));
	dst.write((char*)&TGAHeader.colourmaplength, sizeof(TGAHeader.colourmaplength));
	dst.write((char*)&TGAHeader.colourmapdepth, sizeof(TGAHeader.colourmapdepth));
	dst.write((char*)&TGAHeader.x_origin, sizeof(TGAHeader.x_origin));
	dst.write((char*)&TGAHeader.y_origin, sizeof(TGAHeader.y_origin));
	dst.write((char*)&TGAHeader.Width, sizeof(TGAHeader.Width));
	dst.write((char*)&TGAHeader.Height, sizeof(TGAHeader.Height));
	dst.write((char*)&TGAHeader.Bits, sizeof(TGAHeader.Bits));
	dst.write((char*)&TGAHeader.ImageDescriptor, sizeof(TGAHeader.ImageDescriptor));


	std::cout << int(TGAHeader.datatypecode) << endl;
	
	uint8_t* Desc = new uint8_t[TGAHeader.ImageDescriptor];
	src.read((char*)Desc, TGAHeader.ImageDescriptor);
	dst.write((char*)Desc, TGAHeader.ImageDescriptor);
	size_t ColorMapElementSize = TGAHeader.colourmapdepth / 8;

	size_t CMapSize = TGAHeader.colourmaplength * ColorMapElementSize;

	uint8_t* ColorMap = new uint8_t[CMapSize];
	uint8_t* ID = new uint8_t[int(TGAHeader.idlength)];
	
	if (int(TGAHeader.idlength) != 0) {

		src.read((char*)ColorMap, int(TGAHeader.idlength));
		dst.write((char*)&ColorMap, int(TGAHeader.idlength));
	}

	if (TGAHeader.colourmaptype == 1)
	{
		src.read((char*)ColorMap, CMapSize);
		dst.write((char*)&ColorMap, CMapSize);
	}

	 
	size_t PixelSize = TGAHeader.colourmaplength == 0 ? (TGAHeader.Bits / 8) : ColorMapElementSize;
	size_t DataSize = FileSize - sizeof(Header) - (TGAHeader.colourmaptype == 1 ? CMapSize : 0) - sizeof(int(TGAHeader.idlength));
	size_t ImageSize = orgWidth * orgHeight *PixelSize;
	size_t newSize = TGAHeader.Width * TGAHeader.Height *PixelSize;

	uint8_t* Buffer = new uint8_t[DataSize];

	src.read((char*)Buffer, DataSize);
	
	uint8_t* newData = new uint8_t[newSize];
	Data = new uint8_t[ImageSize];
	memset(Data, 0, ImageSize);

	if (TGAHeader.Bits = 24 || TGAHeader.Bits == 32)
	{
		std::copy(&Buffer[0], &Buffer[ImageSize], &Data[0]); //Copy the image data section to buffer
	}
	//adjust dimesnions for dividing by two
	int adjustedHeight = orgHeight % 2 == 0 ? orgHeight : orgHeight - 1;
	int adjustedWidth = orgWidth % 2 == 0 ? orgWidth : orgWidth-1;
	
	//cout << orgHeight << endl << orgWidth << endl;

	for (int j = 0;j < adjustedHeight;j+=2) //loop to traverse through alternate pixels in the image 
	{

		for (int k = 0;k < adjustedWidth;k+=2)
		{
			
			if (sampling == 1) { //box sampling, average using 2x2 kernel, for better images

				// append RGBA components to new buffer 
				newData[c] = (Data[(j*orgWidth + k)*PixelSize] + Data[((j + 1)*orgWidth + k)*PixelSize] + Data[((j + 1)*orgWidth + (k + 1))*PixelSize] + Data[(j*orgWidth + (k + 1))*PixelSize]) / 4;
				newData[c + 1] = (Data[(j*orgWidth + k)*PixelSize + 1] + Data[((j + 1)*orgWidth + k)*PixelSize + 1] + Data[((j + 1)*orgWidth + (k + 1))*PixelSize + 1] + Data[(j*orgWidth + (k + 1))*PixelSize + 1]) / 4;
				newData[c + 2] = (Data[(j*orgWidth + k)*PixelSize + 2] + Data[((j + 1)*orgWidth + k)*PixelSize + 2] + Data[((j + 1)*orgWidth + (k + 1))*PixelSize + 2] + Data[(j*orgWidth + (k + 1))*PixelSize + 2]) / 4;
				if (PixelSize == 4) //FOR RGBA images
				{
					newData[c + 3] = (Data[(j*orgWidth + k)*PixelSize + 3] + Data[((j + 1)*orgWidth + k)*PixelSize + 3] + Data[((j + 1)*orgWidth + (k + 1))*PixelSize + 3] + Data[(j*orgWidth + (k + 1))*PixelSize + 3]) / 4;
					c += 4;
				} //FOR RGB Images
				else
					c += 3;
				
			}
			else { //choose 1 out of every 4 neighbouring pixel, for quick performance


				newData[c] = Data[(j*orgWidth + k)*PixelSize]; 
					newData[c + 1] = Data[(j*orgWidth + k)*PixelSize + 1];
				newData[c + 2] = Data[(j*orgWidth + k)*PixelSize + 2];
				if (PixelSize == 4) //FOR RGBA images
				{
					newData[c + 3] = Data[(j*orgWidth + k)*PixelSize + 3];
					c += 4;
				} //FOR RGB Images
				else
					c += 3;
			}
					//Debug
					/*cout << "R:" << j << " " << k << " " << (j*orgWidth + k)*PixelSize << " " << int(Data[(j*orgWidth + k)*PixelSize]);
					cout << "G:" << j << " " << k << " " << (j*orgWidth + k)*PixelSize + 1 << " " << int(Data[(j*orgWidth + k)*PixelSize + 1]);
					cout << "B:" << j << " " << k << " " << (j*orgWidth + k)*PixelSize + 2 << " " << int(Data[(j*orgWidth + k)*PixelSize + 2]) << endl;*/
				
			
			
		}
	}
	std::copy(&newData[0], &newData[newSize], & Buffer[0]);
	dst.write((char*)Buffer, newSize );
	cout << endl << "File created successfully" << endl;
	return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
