#include "Actors/NoteActor.h"
#include "ActorComponents/InspectableNoteComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

ANoteActor::ANoteActor()
{
    PrimaryActorTick.bCanEverTick = false;

    USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    PaperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaperMesh"));
    PaperMesh-> SetupAttachment(RootComponent);
    PaperMesh-> SetMobility(EComponentMobility::Movable);
    PaperMesh-> SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    NoteComponent = CreateDefaultSubobject<UInspectableNoteComponent>(TEXT("NoteComponent"));

    //PaperMesh 설정하면 MeshToInspect 
    NoteComponent->MeshToInspect = PaperMesh;
}

void ANoteActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (NoteComponent)
    {
        if (PaperMesh)
        {
            NoteComponent->MeshToInspect = PaperMesh; 
        }

    }
}