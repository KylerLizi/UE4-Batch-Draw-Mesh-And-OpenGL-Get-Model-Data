// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PMConvertSM.generated.h"
/**
 * 
 */
class UStaticMesh;
class UProceduralMeshComponent;
UCLASS()
class MINEBLAST_API UPMConvertSM : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Procedural Mesh Component")
		static UStaticMesh* ProceduralMeshConvertToStaticMesh(UProceduralMeshComponent* proMeshComp,FString outMeshName);//自定义绘制程序网格体转化为静态网格体
	UFUNCTION(BlueprintCallable, Category = "Load Asset")
		static class UStaticMesh* LoadStaticMeshFromPath(const FString &path);
};
