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
	float* JumpPtr = &(GetCharacterMovement()->JumpZVelocity);
	UE_LOG(LogTemp, Log, TEXT("JumpZVelocity pointer: %p"), (void*)JumpPtr);
	UnrealMono::Module* UMono = FModuleManager::GetModulePtr<UnrealMono::Module>("UnrealMono");

	FString DllName = "test.dll";
	FString DllPath = FPaths::ProjectDir() + TEXT("Managed/") + DllName;
	MonoImage* Image = UMono->LoadAssembly(DllPath);
	MonoMethod* SetPtr = UMono->FindMethod(Image, TEXT("UnrealMonoTest:SetPtr(void*)"), false);
	void* Args[1] = { JumpPtr };
	UMono->InvokeMethod(SetPtr, nullptr, Args, nullptr);

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

void ASCharacter::ApplyMod()
{
	if (!IsModApplied)
	{
		IsModApplied = true;

		UnrealMono::Module* UMono = FModuleManager::GetModulePtr<UnrealMono::Module>("UnrealMono");

		FString DllName = "test.dll";
		FString DllPath = FPaths::ProjectDir() + TEXT("Managed/") + DllName;
		MonoImage* Image = UMono->LoadAssembly(DllPath);
		MonoMethod* ChangeVal = UMono->FindMethod(Image, TEXT("UnrealMonoTest:ChangeVal()"), false);
		UpdateMethod = ChangeVal;
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	if (UpdateMethod)
	{
		UnrealMono::Module* UMono = FModuleManager::GetModulePtr<UnrealMono::Module>("UnrealMono");
		UMono->InvokeMethod(UpdateMethod, nullptr, nullptr, nullptr);
		UE_LOG(LogTemp, Log, TEXT("New jump height: %f"), GetCharacterMovement()->JumpZVelocity);
	}

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

