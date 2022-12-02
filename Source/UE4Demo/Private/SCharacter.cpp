// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include <iostream>
#include <mono/jit/jit.h>
#include "mono/metadata/assembly.h"
#include "mono/metadata/mono-config.h"
#include "mono/metadata/debug-helpers.h"

void mono_run();

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

	GetCharacterMovement()->JumpZVelocity = 1600.f;

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

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCharacter::InvokeMono()
{
	mono_run();
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

	PlayerInputComponent->BindAction("InvokeMono", IE_Pressed, this, &ASCharacter::InvokeMono);
}

void mono_run() {
	/*
	 * Load the default Mono configuration file
	 */
	mono_config_parse(nullptr);

	/**
	 * Create a domain
	 */
	MonoDomain* domain = mono_jit_init("TestDomain");
	if (!domain) {
		std::cout << "mono_jit_init failed" << std::endl;
		return;
	}

	/*
	 * Open the executable
	 */
	const char* filename = "managed.exe";
	MonoAssembly* assembly = mono_domain_assembly_open(domain, filename);
	if (!assembly) {
		std::cout << "mono_domain_assembly_open failed" << std::endl;
		return;
	}

	// >>> MAIN METHOD <<<
	/*
	 * Run the Main method
	 */
	std::cout << "Running: Main()" << std::endl;
	int argc = 1;
	char* argv[1] = {0};
	mono_jit_exec(domain, assembly, argc, argv);

	// >>> STATIC METHOD <<<
	/*
	 * Get the image from the assembly
	 */
	MonoImage* image = mono_assembly_get_image(assembly);
	if (!image) {
		std::cout << "mono_assembly_get_image failed" << std::endl;
		return;
	}

	/*
	 * Build a method description object
	 */
	const char* static_method_desc_str = "Test:Hello()";
	MonoMethodDesc* static_method_desc = mono_method_desc_new(static_method_desc_str, false);
	if (!static_method_desc) {
		std::cout << "mono_method_desc_new failed" << std::endl;
		return;
	}

	/*
	 * Search for the method in the image
	 */
	MonoMethod* static_method = mono_method_desc_search_in_image(static_method_desc, image);
	if (!static_method) {
		std::cout << "mono_method_desc_search_in_image failed" << std::endl;
		return;
	}

	/*
	 * Run the static method
	 */
	std::cout << "Running the static method: " << static_method_desc_str << std::endl;
	mono_runtime_invoke(static_method, nullptr, nullptr, nullptr);

	// >>> NON-STATIC METHOD <<<
	/*
	 * Get the class
	 */
	MonoClass* test_class = mono_class_from_name(image, "", "Test");
	if (!test_class) {
		std::cout << "mono_class_from_name failed" << std::endl;
		return;
	}

	/*
	 * Create an instance of the class
	 */
	MonoObject* object = mono_object_new(domain, test_class);
	if (!object) {
		std::cout << "mono_object_new failed" << std::endl;
		return;
	}

	/*
	 * Call its default constructor
	 */
	mono_runtime_object_init(object);

	/*
	 * Build a method description object
	 */
	const char* method_desc_str = "Test:Add(int,int)";
	MonoMethodDesc* method_desc = mono_method_desc_new(method_desc_str, false);
	if (!method_desc) {
		std::cout << "mono_method_desc_new failed" << std::endl;
		return;
	}

	/*
	 * Search for the method in the image
	 */
	MonoMethod* method = mono_method_desc_search_in_image(method_desc, image);
	if (!method) {
		std::cout << "mono_method_desc_search_in_image failed" << std::endl;
		return;
	}

	/*
	 * Set the arguments for the method
	 */
	int a = 2137;
	int b = 420;
	void* args[] = { &a, &b };

	/*
	 * Run the method
	 */
	std::cout << "Running the method: " << method_desc_str << std::endl;
	MonoObject* result = mono_runtime_invoke(method, object, args, nullptr);

	/*
	 * Unbox and dereference the result
	 */
	int int_result = *(int*)mono_object_unbox(result);
	std::cout << a << " + " << b << " = " << int_result << std::endl;

	/*
	 * Clean up the domain
	 */
	mono_jit_cleanup(domain);
	return;
}