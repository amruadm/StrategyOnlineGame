// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"
#include "Public/World/IslandLevel.h"
#include "Public/World/WorldIsland.h"
#include "KismetProceduralMeshLibrary.h"

//#include "LibNoise/GeneratorModules.h"
//#include "../../ThirdParty/LibNoise/Includes/noise.h"
//#include "LibNoise.h"

#define RED 2
#define GREEN 1
#define BLUE 0
#define ALPHA 3

struct FVectorIndexPair
{
	FVector * vect;
	int index;
	FVectorIndexPair(FVector * Vector, int ind) :vect(Vector), index(ind) {};
};

// Sets default values for this actor's properties
AWorldIsland::AWorldIsland()
{
	bReplicates = true;
	Resolution = 64;
	SegmentWidth = 100.0f;
	mDynamicColors = nullptr;
	mDynamicColorsFloat = nullptr;
	mUpdateTextureRegion = nullptr;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
	DynamicTextureParam = FName("HeightMap");
}

// Called when the game starts or when spawned
void AWorldIsland::BeginPlay()
{
	Super::BeginPlay();
	RandStream.GenerateNewSeed();
}

void AWorldIsland::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	delete[] mDynamicColors; mDynamicColors = nullptr;
	delete[] mDynamicColorsFloat; mDynamicColorsFloat = nullptr;
	delete mUpdateTextureRegion; mUpdateTextureRegion = nullptr;
	Super::EndPlay(EndPlayReason);
}

void AWorldIsland::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (mDynamicColors) delete[] mDynamicColors;
	if (mDynamicColorsFloat) delete[] mDynamicColorsFloat;
	if (mUpdateTextureRegion) delete mUpdateTextureRegion;

	if(!DynamicMaterial) DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	MapTexture = UTexture2D::CreateTransient(Resolution, Resolution);
	MapTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	MapTexture->SRGB = 0;
	MapTexture->AddToRoot();
	MapTexture->UpdateResource();
	mUpdateTextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, Resolution, Resolution);

	if(DynamicMaterial)
		DynamicMaterial->SetTextureParameterValue(DynamicTextureParam, MapTexture);
	mDataSize = Resolution * Resolution * 4; // width*height (Res*Res) of 4 byte color (RGBA)
	mDataSqrtSize = Resolution * 4;
	mArraySize = Resolution * Resolution;
	mRowSize = Resolution;
	mDynamicColors = new uint8[mDataSize];
	mDynamicColorsFloat = new float[mArraySize];
	RegenerateIsland();
}

void AWorldIsland::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWorldIsland, RandStream)
}

void AWorldIsland::ResetColors()
{

}

void AWorldIsland::UpdateMap()
{
	UCommonHelpers::UpdateTextureRegions(MapTexture, 0, 1, mUpdateTextureRegion, mDataSqrtSize, (uint32)4, mDynamicColors, false);
	if (!DynamicMaterial) DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	if(DynamicMaterial)
		DynamicMaterial->SetTextureParameterValue(DynamicTextureParam, MapTexture);
}

void AWorldIsland::GenerateMap()
{
	/*
	RandStream.GenerateNewSeed();
	//noise::module::Perlin* perlinNoise1 = new noise::module::Perlin();
	noise::module::Perlin* perlinNoise = new noise::module::Perlin();
	noise::module::Perlin* perlinNoise2 = new noise::module::Perlin();
	noise::module::Multiply* mul = new noise::module::Multiply();
	perlinNoise->SetSeed(RandStream.GetCurrentSeed());
	perlinNoise2->SetSeed(RandStream.GetCurrentSeed() * RandStream.GetCurrentSeed() % 100);
	perlinNoise2->SetFrequency(perlinNoise->GetFrequency() / 2);
	mul->SetSourceModule(0, *perlinNoise);
	mul->SetSourceModule(1, *perlinNoise2);	
	const int w = Resolution * 4;
	const float halfR = Resolution / 2;
	const float toonPower = 5.0f;
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int colorIndex = y*Resolution * 4 + x;
			int xOff = x/4 - halfR;
			int yOff = y - halfR;
			
			//float ColorScale = 1.0f - FMath::Clamp<float>(FMath::Sqrt(xOff*xOff + yOff*yOff) / (halfR), 0.0f, 1.0f);
			float ColorScale = FMath::Sqrt(xOff*xOff + yOff*yOff) <= halfR ? 1.0f : 0.0f;
			float val = perlinNoise->GetValue(2 * (float)(x) / (float)w, 2 * (float)(y) / (float)Resolution, 0.5);
			float val2 = 2*FMath::Abs<float>((float)perlinNoise2->GetValue(2 * (float)(x) / (float)w, 2 * (float)(y) / (float)Resolution, 0.5));						
			float value = val2 * FMath::Ceil(toonPower*val) / toonPower;
			mDynamicColors[colorIndex] = FMath::Clamp<int>(value * ColorScale * 255, 0, 255);
		}
	}
	UpdateMap();
	delete perlinNoise;
	delete perlinNoise2;
	delete mul;*/
}

void AWorldIsland::GenerateMesh()
{
	/*
	TArray<FVector> verts;
	TArray<int> tris; // % 3 = 0;
	TArray<FVector> normals;
		
	TMap<int, FVectorIndexPair> vectorIndexMap;
	TArray<FVector2D> UVs;
	TArray<FColor> colors;
	TArray<FProcMeshTangent> tangents;
	int vertIndex = 0;
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			//Get colors indexes
			int colorIndex = y*Resolution*4 + x*4;
			int rightColor = y*Resolution * 4 + (x+1) * 4;
			int rightTopColor = (y+1)*Resolution * 4 + (x+1) * 4;
			int TopColor = (y+1)*Resolution * 4 + x * 4;

			if (mDynamicColors[colorIndex] > 0) {				
				
				if (y < Resolution - 1 && x < Resolution - 1 && mDynamicColors[rightTopColor] > 0)
				{
					//tris 1 condition
					if (mDynamicColors[TopColor] > 0)
					{
						if (!vectorIndexMap.Contains(colorIndex))
						{
							float ZOffst = mDynamicColors[colorIndex] * heightScale;
							FVector* v1 = new FVector(x*SegmentWidth, y*SegmentWidth, ZOffst);
							vectorIndexMap.Add(colorIndex, FVectorIndexPair(v1, verts.Add(*v1)));

							UVs.Add(FVector2D(x, y));

							normals.Add(FVector(0.0f, 0.0f, 1.0f));
							normals.Add(FVector(0.0f, 0.0f, 1.0f));
						}

						if (!vectorIndexMap.Contains(rightTopColor))
						{
							float ZOffst = mDynamicColors[rightTopColor] * heightScale;
							FVector* v2 = new FVector((x + 1)*SegmentWidth, (y + 1)*SegmentWidth, ZOffst);
							vectorIndexMap.Add(rightTopColor, FVectorIndexPair(v2, verts.Add(*v2)));

							UVs.Add(FVector2D(x + 1, y + 1));

							normals.Add(FVector(0.0f, 0.0f, 1.0f));
							normals.Add(FVector(0.0f, 0.0f, 1.0f));
						}

						if (!vectorIndexMap.Contains(TopColor))
						{
							float ZOffst = mDynamicColors[TopColor] * heightScale;
							FVector* v3 = new FVector(x*SegmentWidth, (y + 1)*SegmentWidth, ZOffst);
							vectorIndexMap.Add(TopColor, FVectorIndexPair(v3, verts.Add(*v3)));

							UVs.Add(FVector2D(x, y + 1));

							normals.Add(FVector(0.0f, 0.0f, 1.0f));
							normals.Add(FVector(0.0f, 0.0f, 1.0f));
						}						

						tris.Add(vectorIndexMap[TopColor].index);
						tris.Add(vectorIndexMap[rightTopColor].index);
						tris.Add(vectorIndexMap[colorIndex].index);
												
					}

					//tris 2 condition
					if (mDynamicColors[rightColor] > 0)
					{						
						if (!vectorIndexMap.Contains(colorIndex))
						{
							float ZOffst = mDynamicColors[rightColor] * heightScale;
							FVector* v = new FVector(x*SegmentWidth, y*SegmentWidth, ZOffst);
							vectorIndexMap.Add(colorIndex, FVectorIndexPair(v, verts.Add(*v)));

							UVs.Add(FVector2D(x, y));

							normals.Add(FVector(0.0f, 0.0f, 1.0f));
							normals.Add(FVector(0.0f, 0.0f, 1.0f));
						}
						if (!vectorIndexMap.Contains(rightTopColor))
						{
							float ZOffst = mDynamicColors[rightTopColor] * heightScale;
							FVector* v = new FVector((x + 1)*SegmentWidth, (y + 1)*SegmentWidth, ZOffst);
							vectorIndexMap.Add(rightTopColor, FVectorIndexPair(v, verts.Add(*v)));

							UVs.Add(FVector2D(x + 1, y + 1));

							normals.Add(FVector(0.0f, 0.0f, 1.0f));
							normals.Add(FVector(0.0f, 0.0f, 1.0f));
						}
						if (!vectorIndexMap.Contains(rightColor))
						{
							float ZOffst = mDynamicColors[rightColor] * heightScale;
							FVector* v = new FVector((x + 1)*SegmentWidth, y*SegmentWidth, ZOffst);
							vectorIndexMap.Add(rightColor, FVectorIndexPair(v, verts.Add(*v)));

							UVs.Add(FVector2D(x + 1, y));

							normals.Add(FVector(0.0f, 0.0f, 1.0f));
							normals.Add(FVector(0.0f, 0.0f, 1.0f));
						}
						tris.Add(vectorIndexMap[rightTopColor].index);
						tris.Add(vectorIndexMap[rightColor].index);
						tris.Add(vectorIndexMap[colorIndex].index);
					}					
				}
			}			
		}
	}

	Mesh->CreateMeshSection(0, verts, tris, normals, UVs, colors, tangents, true);*/
}

void AWorldIsland::GenerateBottomMesh()
{
	/*TArray<FVector> verts;
	TArray<int> tris; // % 3 = 0;
	TArray<FVector> normals;

	TMap<int, FVectorIndexPair> vectorIndexMap;
	TArray<FVector2D> UVs;
	TArray<FColor> colors;
	TArray<FProcMeshTangent> tangents;
	int vertIndex = 0;
	const float hScale = -1.0f;
	noise::module::Perlin* perlin = new noise::module::Perlin();
	perlin->SetSeed(RandStream.GetCurrentSeed() + RandStream.GetCurrentSeed() % 323);
	for (int y = 0; y < Resolution; y++)
	{
		for (int x = 0; x < Resolution; x++)
		{
			//Get colors indexes
			int colorIndex = y*Resolution * 4 + x * 4;
			int rightColor = y*Resolution * 4 + (x + 1) * 4;
			int rightTopColor = (y + 1)*Resolution * 4 + (x + 1) * 4;
			int TopColor = (y + 1)*Resolution * 4 + x * 4;

			float val = FMath::Abs<float>(perlin->GetValue((double)x /(double) Resolution, (double)y / (double)Resolution,0.5f));
			float valFilter = mDynamicColors[colorIndex] > 0 ? 1.0f : 0.0f;
			val = val*valFilter;
			float ZOffset = val*hScale*50;

			if (mDynamicColors[colorIndex] > 0) {
				if (y < Resolution - 1 && x < Resolution - 1 && mDynamicColors[rightTopColor] > 0)
				{
					//tris 1 condition
					if (mDynamicColors[TopColor] > 0)
					{
						if (!vectorIndexMap.Contains(colorIndex))
						{
							FVector* v1 = new FVector(x*SegmentWidth, y*SegmentWidth, mDynamicColors[colorIndex] * (CheckNeightbours(x, y) ? ZOffset : heightScale));
							vectorIndexMap.Add(colorIndex, FVectorIndexPair(v1, verts.Add(*v1)));

							UVs.Add(FVector2D(x, y));

							normals.Add(FVector(0.0f, 0.0f, -1.0f));
							normals.Add(FVector(0.0f, 0.0f, -1.0f));
						}

						if (!vectorIndexMap.Contains(rightTopColor))
						{
							FVector* v2 = new FVector((x + 1)*SegmentWidth, (y + 1)*SegmentWidth, mDynamicColors[rightTopColor] * (CheckNeightbours(x + 1, y + 1) ? ZOffset : heightScale));
							vectorIndexMap.Add(rightTopColor, FVectorIndexPair(v2, verts.Add(*v2)));

							UVs.Add(FVector2D(x + 1, y + 1));

							normals.Add(FVector(0.0f, 0.0f, -1.0f));
							normals.Add(FVector(0.0f, 0.0f, -1.0f));
						}

						if (!vectorIndexMap.Contains(TopColor))
						{
							FVector* v3 = new FVector(x*SegmentWidth, (y + 1)*SegmentWidth, mDynamicColors[TopColor] * (CheckNeightbours(x, y + 1) ? ZOffset : heightScale));
							vectorIndexMap.Add(TopColor, FVectorIndexPair(v3, verts.Add(*v3)));

							UVs.Add(FVector2D(x, y + 1));

							normals.Add(FVector(0.0f, 0.0f, -1.0f));
							normals.Add(FVector(0.0f, 0.0f, -1.0f));
						}
						
						tris.Add(vectorIndexMap[TopColor].index);						
						tris.Add(vectorIndexMap[colorIndex].index);
						tris.Add(vectorIndexMap[rightTopColor].index);
					}

					//tris 2 condition
					if (mDynamicColors[rightColor] > 0)
					{
						if (!vectorIndexMap.Contains(colorIndex))
						{
							FVector* v = new FVector(x*SegmentWidth, y*SegmentWidth, mDynamicColors[colorIndex] * (CheckNeightbours(x, y) ? ZOffset : heightScale));
							vectorIndexMap.Add(colorIndex, FVectorIndexPair(v, verts.Add(*v)));

							UVs.Add(FVector2D(x, y));

							normals.Add(FVector(0.0f, 0.0f, -1.0f));
							normals.Add(FVector(0.0f, 0.0f, -1.0f));
						}
						if (!vectorIndexMap.Contains(rightTopColor))
						{
							FVector* v = new FVector((x + 1)*SegmentWidth, (y + 1)*SegmentWidth, mDynamicColors[rightTopColor] * (CheckNeightbours(x+1, y+1) ? ZOffset : heightScale));
							vectorIndexMap.Add(rightTopColor, FVectorIndexPair(v, verts.Add(*v)));

							UVs.Add(FVector2D(x + 1, y + 1));

							normals.Add(FVector(0.0f, 0.0f, -1.0f));
							normals.Add(FVector(0.0f, 0.0f, -1.0f));
						}
						if (!vectorIndexMap.Contains(rightColor))
						{
							FVector* v = new FVector((x + 1)*SegmentWidth, y*SegmentWidth, mDynamicColors[rightColor] * (CheckNeightbours(x+1, y) ? ZOffset : heightScale));
							vectorIndexMap.Add(rightColor, FVectorIndexPair(v, verts.Add(*v)));

							UVs.Add(FVector2D(x + 1, y));

							normals.Add(FVector(0.0f, 0.0f, -1.0f));
							normals.Add(FVector(0.0f, 0.0f, -1.0f));
						}
						tris.Add(vectorIndexMap[colorIndex].index);
						tris.Add(vectorIndexMap[rightColor].index);
						tris.Add(vectorIndexMap[rightTopColor].index);						
					}
				}
			}
		}
	}
	
	Mesh->CreateMeshSection(1, verts, tris, normals, UVs, colors, tangents, true);
	
	delete perlin;*/
}

bool AWorldIsland::CheckNeightbours(int x, int y)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			int YInd = (y + i);
			int XInd = (x + j);
			if (YInd > 0 && YInd < Resolution && XInd > 0 && XInd < Resolution*4)
			{
				int ind = YInd*Resolution * 4 + XInd*4;
				if (mDynamicColors[ind] <= 0)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void AWorldIsland::RegenerateIsland()
{
	Mesh->ClearMeshSection(0);
	Mesh->ClearMeshSection(1);
	ResetColors();
	GenerateMap();
	GenerateMesh();
	GenerateBottomMesh();
}

void AWorldIsland::OnRep_RandomSeed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString("OnRep_RandomSeed"));
}