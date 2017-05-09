// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#ifndef __TOPDOWNSHOOTER_H__
#define __TOPDOWNSHOOTER_H__

//#include "EngineMinimal.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "Public/Common/CommonHelpers.h"
#include "Public/Common/MathHelpers.h"
#include "Public/GameTypes.h"
#include "Public/GameHelpers.h"
#include "Public/Interfaces/TeamObjectInterface.h"
#include "Public/Interfaces/StorageInterface.h"
#include "Public/Interfaces/WorkerInterface.h"
#include "Public/Interfaces/UnitPlaceInterface.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogTopDownShooter, Log, All);

//Extractor building
#define EXTRACTOR_TIMING_RATE 0.1f
#define EXTRACTOR_MIN_TIMING 0.5f
#define EXTRACTOR_EXP_RATE_LIMIT 2.0f

//World
#define WORLD_CELL_SZ 500.0f
#define BUILDING_CELL_SZ 100.0f

#endif

