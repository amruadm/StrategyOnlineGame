// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDownShooter.h"

float UMathHelpers::Noise(int x, int y, FRandomStream & RandStream)
{
	/*RandStream.Reset();
	int n = x + y * RandStream.RandRange(1, 37098);
	noise::module::Perlin perlin;
	perlin.SetSeed(RandStream.RandRange(0, 1000));
	return (float)perlin.GetValue((double)x, (double)y, 0.5);*/
	return 0.0f;
}