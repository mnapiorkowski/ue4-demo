// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "UnrealMono.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	IsModApplied = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.1f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM = FTransform(GetControlRotation(), HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void PrintDebugMessage(void* Ptr, FString FuncName)
{
	if (GEngine)
	{
		if (Ptr)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FuncName + TEXT(" succeeded"));
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FuncName + TEXT(" failed"));
	}
}

int RetrieveFromDll()
{
	FUnrealMonoModule* UnrealMono = FModuleManager::GetModulePtr<FUnrealMonoModule>("UnrealMono");

	FString DllName = TEXT("test.dll");
	MonoDomain* Domain = UnrealMono->MonoJitInit(DllName);
	PrintDebugMessage(Domain, "mono_jit_init");

	FString DllPath = FPaths::ProjectDir() + TEXT("Binaries/Win64/") + DllName;
	MonoAssembly* Assembly = UnrealMono->MonoDomainAssemblyOpen(Domain, DllPath);
	PrintDebugMessage(Assembly, "mono_domain_assembly_open");

	MonoImage* Image = UnrealMono->MonoAssemblyGetImage(Assembly);
	PrintDebugMessage(Image, "mono_assembly_get_image");

	FString MethodDescStr = TEXT("UnrealMonoTest::JumpHeightFactor()");
	MonoMethodDesc* MethodDesc = UnrealMono->MonoMethodDescNew(MethodDescStr, false);
	PrintDebugMessage(MethodDesc, "mono_method_desc_new");

	MonoMethod* Method = UnrealMono->MonoMethodDescSearchInImage(MethodDesc, Image);
	PrintDebugMessage(Method, "mono_method_desc_search_in_image");

	MonoObject* Result = UnrealMono->MonoRuntimeInvoke(Method, nullptr, nullptr, nullptr);
	PrintDebugMessage(Result, "mono_runtime_invoke");

	int ResultInt = *(int*)UnrealMono->MonoObjectUnbox(Result);

	UnrealMono->MonoJitCleanup(Domain);

	return ResultInt;
}

void ASCharacter::ApplyMod()
{
	if (!IsModApplied)
	{
		IsModApplied = true;
		int Factor = RetrieveFromDll();
		GetCharacterMovement()->JumpZVelocity *= Factor;
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);

	PlayerInputComponent->BindAction("ApplyMod", IE_Pressed, this, &ASCharacter::ApplyMod);
}

