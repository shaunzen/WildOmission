// Copyright Telephone Studios. All Rights Reserved.


#include "Projectiles/RocketProjectile.h"

ARocketProjectile::ARocketProjectile()
{

}

void ARocketProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);


    UE_LOG(LogTemp, Warning, TEXT("OnHit Called on server = %i"), HasAuthority());
    // Is this on the server?
    
    // TODO do explosion

    // TODO play boom sound
}