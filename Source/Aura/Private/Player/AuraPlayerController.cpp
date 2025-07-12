#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	// 1. 커서가 hover된 곳에 있는 actor 정보 가져오기
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	// 2. 지난 프레임에서 hover된 Actor와 이번 프레임에서 hover된 Actor 지정
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/* Line Trace from Cursor 시 나올 수 있는 상황들
	 *  A. LastActor is null && ThisActor is null
	 *		- hover하지 않음 : Do Nothing
	 *	B. LastActor is null && ThisActor is valid 
	 *		- hover가 시작됨 : Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		- hover가 끝남 : Unhighlight ThisActor
	 *	D. Both actors are valid, BUT LastActor != ThisActor
	 *		- hover 대상이 변경됨 : Unhighlight LastActor && Highlight ThisActor
	 *	E. Both actors are valid, AND LastActor == ThisActor
	 *		- 이미 hover중 : Do Nothing
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();	// B
		}
		else
		{
			// A - both are null - Do Nothing
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();	// C
		}
		else // both are valid
		{
			if (LastActor != ThisActor)
			{
				// D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// E - already Highlighted - Do Nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	/* IMC */
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	/* Cursor settings */
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const struct FInputActionValue& InputActionValue)
{
	// 여기 질문
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}


