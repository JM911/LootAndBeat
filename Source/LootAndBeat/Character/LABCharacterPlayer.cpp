// Fill out your copyright notice in the Description page of Project Settings.


#include "LABCharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#define MAX_PUNCH_COMBO_NUM 4
#define MAX_KICK_COMBO_NUM 3

ALABCharacterPlayer::ALABCharacterPlayer()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.0f; 	
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeRotation(FRotator(-50, 0, 0));

	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ALABCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALABCharacterPlayer::Move);
		EnhancedInputComponent->BindAction(AttackPunchAction, ETriggerEvent::Triggered, this, &ALABCharacterPlayer::AttackPunch);
		EnhancedInputComponent->BindAction(AttackKickAction, ETriggerEvent::Triggered, this, &ALABCharacterPlayer::AttackKick);
	}
	else
	{
		UE_LOG(LogLABCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ALABCharacterPlayer::Move(const FInputActionValue& InputActionValue)
{
	if(bAttackPunch || bAttackKick) return;
	
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	this->AddMovementInput(ForwardDirection, InputAxisVector.Y);
	this->AddMovementInput(RightDirection, InputAxisVector.X);
}

void ALABCharacterPlayer::AttackPunch(const FInputActionValue& InputActionValue)
{
	if(!AttackPunchMontage || bAttackKick) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AnimInstance) return;
	
	if(bAttackPunch)
	{
		if(!bComboEnable) return;
		
		ComboNumPunch++;
		if(ComboNumPunch > MAX_PUNCH_COMBO_NUM) return;

		const FString ComboSectionString = "Combo_" +  FString::FromInt(ComboNumPunch);
		const FName ComboSectionName = FName(*ComboSectionString);
		
		AnimInstance->Montage_Play(AttackPunchMontage);
		AnimInstance->Montage_JumpToSection(ComboSectionName);
		//UE_LOG(LogLABCharacter, Warning, TEXT("Start Attack - %s"), *ComboSectionString);
	}
	else
	{
		AnimInstance->Montage_Play(AttackPunchMontage);
		ComboNumPunch = 1;
	}

	bAttackPunch = true;
	AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ALABCharacterPlayer::OnAttackPunchEnd);
}

void ALABCharacterPlayer::AttackKick(const FInputActionValue& InputActionValue)
{
	if(!AttackKickMontage || bAttackPunch) return;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AnimInstance) return;
	
	if(bAttackKick)
	{
		if(!bComboEnable) return;
		
		ComboNumKick++;
		if(ComboNumKick > MAX_KICK_COMBO_NUM) return;

		const FString ComboSectionString = "Combo_" +  FString::FromInt(ComboNumKick);
		const FName ComboSectionName = FName(*ComboSectionString);
		
		AnimInstance->Montage_Play(AttackKickMontage);
		AnimInstance->Montage_JumpToSection(ComboSectionName);
		//UE_LOG(LogLABCharacter, Warning, TEXT("Start Attack - %s"), *ComboSectionString);
	}
	else
	{
		AnimInstance->Montage_Play(AttackKickMontage);
		ComboNumKick = 1;
	}

	bAttackKick = true;
	AnimInstance->OnMontageBlendingOut.AddDynamic(this, &ALABCharacterPlayer::OnAttackKickEnd);
}

void ALABCharacterPlayer::OnAttackPunchEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bAttackPunch = false;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AnimInstance) return;
	
	AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ALABCharacterPlayer::OnAttackPunchEnd);
}

void ALABCharacterPlayer::OnAttackKickEnd(UAnimMontage* Montage, bool bInterrupted)
{
	bAttackKick = false;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(!AnimInstance) return;
	
	AnimInstance->OnMontageBlendingOut.RemoveDynamic(this, &ALABCharacterPlayer::OnAttackKickEnd);
}


