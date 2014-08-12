

#include "Fero.h"
#include "Landscape/Landscape.h"
#include "ProceduralTerrain.h"


UProceduralTerrain::UProceduralTerrain(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}


void UProceduralTerrain::GenerateTerrain(ALandscape *landscape)
{
	UTexture2D *heightMap = landscape->LandscapeComponents[0]->HeightmapTexture;
	EPixelFormat formatPixel = heightMap->GetPixelFormat();

	//texture resolution
	int sizeX = heightMap->GetSizeX();
	int sizeY = heightMap->GetSizeY();

	int pixelSize = sizeof(formatPixel)* 8;
	int pitch = pixelSize * sizeX;

	FUpdateTextureRegion2D region;
	region.DestX = 0;
	region.DestY = 0;
	region.SrcX = 0;
	region.SrcY = 0;
	region.Width = sizeX;
	region.Height = sizeY;



	//foreach pixel, I will change the height. Today, it's done by the CPU but I will move it later to the GPU
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
		{

		}
	}

	heightMap->UpdateTextureRegions(0, 1, &region, pitch, pixelSize, NULL, false);
}