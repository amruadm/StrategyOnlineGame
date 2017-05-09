// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "IslandLevel.h"

/*void FIslandLevelMap::UpdateTextureRegions(int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
if (Texture && Texture->Resource)
{
struct FUpdateTextureRegionsData
{
FTexture2DResource* Texture2DResource;
int32 MipIndex;
uint32 NumRegions;
FUpdateTextureRegion2D* Regions;
uint32 SrcPitch;
uint32 SrcBpp;
uint8* SrcData;
};

FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

RegionData->Texture2DResource = (FTexture2DResource*)Texture->Resource;
RegionData->MipIndex = MipIndex;
RegionData->NumRegions = NumRegions;
RegionData->Regions = Regions;
RegionData->SrcPitch = SrcPitch;
RegionData->SrcBpp = SrcBpp;
RegionData->SrcData = SrcData;

ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
UpdateTextureRegionsData,
FUpdateTextureRegionsData*, RegionData, RegionData,
bool, bFreeData, bFreeData,
{
for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
{
int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
if (RegionData->MipIndex >= CurrentFirstMip)
{
RHIUpdateTexture2D(
RegionData->Texture2DResource->GetTexture2DRHI(),
RegionData->MipIndex - CurrentFirstMip,
RegionData->Regions[RegionIndex],
RegionData->SrcPitch,
RegionData->SrcData
+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
);
}
}
if (bFreeData)
{
FMemory::Free(RegionData->Regions);
FMemory::Free(RegionData->SrcData);
}
delete RegionData;
});
}
}*/

UIslandLevel::UIslandLevel()
{

}

void UIslandLevel::GenerateLevel()
{
	/*levelMap.textureMap = UTexture2D::CreateTransient(AWorldIsland::resolution, AWorldIsland::resolution);
	//Convert the static material in our mesh into a dynamic one, and store it (please note that if you have more than one material that you wish to mark dynamic, do so here).
	//mDynamicMaterials.Add(StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0));
	//Create a dynamic texture with the default compression (B8G8R8A8)
	levelMap.textureMap = UTexture2D::CreateTransient(w, h);
	//Make sure it won't be compressed
	levelMap.textureMap->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	//Turn off Gamma-correction
	levelMap.textureMap->SRGB = 0;
	//Guarantee no garbage collection by adding it as a root reference
	levelMap.textureMap->AddToRoot();
	//Update the texture with new variable values.
	levelMap.textureMap->UpdateResource();
	//Grab the colorvalues from our existing texture (the one we created at '''Texture Setup''') and copy it into a uint8* mTextureColors variable.
	int32 w, h;
	w = textureToReadFrom->GetSizeX();
	h = textureToReadFrom->GetSizeY();
	FTexture2DMipMap& readMip = textureToReadFrom->PlatformData->Mips[0];
	levelMap.mDataSize = w * h * 4; // * 4 because we're working with uint8's - which are 4 bytes large
	levelMap.mDataSqrtSize = w * 4; // * 4 because we're working with uint8's - which are 4 bytes large
	readMip.BulkData.GetCopy((void**)&levelMap.mTextureColors);
	// Initalize our dynamic pixel array with data size
	levelMap.mDynamicColors = new uint8[levelMap.mDataSize];
	// Copy our current texture's colors into our dynamic colors
	FMemory::Memcpy(levelMap.mDynamicColors, levelMap.mTextureColors, levelMap.mDataSize);
	// Create a new texture region with the width and height of our dynamic texture
	levelMap.mUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, w, h);
	// Set the Paramater in our material to our texture
	//mDynamicMaterials[0]->SetTextureParameterValue("DynamicTextureParam", mDynamicTexture);

	for (int32 y = 0; y < AWorldIsland::resolution; y++)
	{
	for (int32 x = 0; x < AWorldIsland::resolution; x++)
	{
	//levelMap.textureMap
	}
	}*/
}


