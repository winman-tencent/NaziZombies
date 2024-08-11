#include "NaziZombiesGameMode.h"
#include "NaziZombiesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANaziZombiesGameMode::ANaziZombiesGameMode() : Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
