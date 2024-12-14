// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Staff.h"
#include "Bow.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class GRADUATIONPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// 충돌 이벤트 처리
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> GetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ThrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AimingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<UAnimMontage> AttackMontage;

	void AnimCommand();
	void Attack(const FInputActionValue& Value);
	int32 CurrentCombo;
	bool bCanCombo;
	//UFUNCTION()
	//void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	//
	FName GetNextComboSection();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Get(const FInputActionValue& Value);
	void Throw(const FInputActionValue& Value);

	void StartRunning();
	void StopRunning();
	void StartAiming();
	void StopAiming();

	void EquipWeapon(AWeapon* Weapon);
	void DropWeapon();
private:
	// 플레이어가 현재 달리고 있는지 여부를 확인
	bool bIsRunning;

	// 기본 속도와 달리기 속도를 설정하는 변수
	float DefaultWalkSpeed;
	float RunSpeed;
	bool bIsMove;
	bool bIsAiming;
	float AimingSpeed;
	UPROPERTY(EditAnywhere, Category = "Throw")
	float ThrowSpeed;

	UPROPERTY(EditAnywhere, Category = "Rock")
	class ARock* Rock; // ARock 포인터로 선언


protected:

	// 현재 획득한 무기
	AWeapon* EquippedWeapon;
	AWeapon* CurrentWeapon;


	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	// 충돌 감지를 위한 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere)
	USphereComponent* PickupRange;
	bool bIsWeaponEquipped;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Magic")
	UStaticMeshComponent* MagicArea; // 마법 범위
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMesh* MagicAreaMesh;
	void SetMagicArea();

public:
	void EnableNextCombo();

	UFUNCTION()
	void OnPickupMontageEnded(UAnimMontage* Montage, bool bInterrupted);
};
