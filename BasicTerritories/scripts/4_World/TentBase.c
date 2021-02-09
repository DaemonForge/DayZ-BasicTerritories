modded class TentBase extends ItemBase
{
	override void EEOnCECreate() {
		super.EEOnCECreate();
		Print("[BASICTERRITORIES] Tent spawned in via CE");
		
		this.SetLifetime(GetBasicTerritoriesConfig().TentCESpawnLifeTime);
	}
	
	void ResetTentLifeTime(){
		Print("[BASICTERRITORIES] Reseting Tent Life Time");
		float OldLifeTime = GetLifetime();
		float MaxLifetime = GetLifetimeMax();
		this.SetLifetime(MaxLifetime);
		float NewLifeTime = GetLifetime();
		GetCEApi().RadiusLifetimeReset(GetPosition(), GetBasicTerritoriesConfig().TentRadius);
	}
	
	override void ToggleAnimation( string selection )
	{
		super.ToggleAnimation( selection );
		if (GetGame().IsServer() && GetBasicTerritoriesConfig().TentRadius >= 0){
			ResetTentLifeTime();
		}
	}
};
