modded class BaseBuildingBase
{	

	
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		string tool = "";
		Print("[Territories] [Debug] BaseBuildingBase EEHitBy Start = = = = = = = = = = = = = = = = = = = = =");
		if (source){
			tool = source.GetType();
			PlayerBase player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
			if (player && player.GetIdentity()){
				Print("[Territories] [Debug] Source: " +  source.GetType() + " held by player: " +  player.GetIdentity().GetName() + "(" +  player.GetIdentity().GetId() + ")");
			} else {
				Print("[Territories] [Debug] Source: " +  source.GetType());
			}
		} else {
			Print("[Territories] [Debug] Source: NULL"); 
		}
		if ( damageType == DT_EXPLOSION ) {
			Print("[Territories] [Debug] dmgZone: "+ dmgZone + " ammo: "+ ammo + " damageType: "+ damageType + "(DT_EXPLOSION)");
		} else if ( damageType == DT_FIRE_ARM ) {
			Print("[Territories] [Debug] dmgZone: "+ dmgZone +" ammo: "+ ammo + " damageType: "+ damageType + "(DT_FIRE_ARM)");
		} else if ( damageType == DT_CLOSE_COMBAT ) {
			Print("[Territories] [Debug] dmgZone: "+ dmgZone +" ammo: "+ ammo + " damageType: "+ damageType + "(DT_CLOSE_COMBAT)");
			
		} else if ( damageType == DT_CUSTOM ) {
			Print("[Territories] [Debug] dmgZone: "+ dmgZone +" ammo: "+ ammo + " damageType: "+ damageType + "(DT_CUSTOM)");
		} else {
			Print("[Territories] [Debug] dmgZone: "+ dmgZone +" ammo: "+ ammo + " damageType: "+ damageType + "(UNKNOWN)");
		}
				
		array<string> FullZonesList = new array<string>;
		GetDamageZones(FullZonesList);
		
		for (int i = 0; i < FullZonesList.Count(); i++){
			float zoneDmg = damageResult.GetDamage(FullZonesList.Get(i), "Health");
			string zoneName = FullZonesList.Get(i);
			float beforeHealth = GetHealth(zoneName, "Health");
			if (zoneDmg > 0){
				float HealBackAmount = GetBasicTerritoriesConfig().GetHealBack(zoneDmg, this.GetType(), damageType, tool, ammo, zoneName );
				if (HealBackAmount > 0){
					AddHealth(zoneName, "Health", HealBackAmount);
					Print("[Territories] [Debug] After Zone: " + zoneName + " - Damage: " + zoneDmg + "Health: "+ beforeHealth +" AddHealth: " + HealBackAmount + " New Health: " + this.GetHealth(zoneName, "Health"));
				} else if (HealBackAmount < 0){
					HealBackAmount = 0 - HealBackAmount;
					DecreaseHealth(zoneName, "Health", HealBackAmount);
					Print("[Territories] [Debug] After Zone: " + zoneName + " - Damage: " + zoneDmg + "Health: "+ beforeHealth +" DecreaseHealth: " + HealBackAmount + " New Health: " + this.GetHealth(zoneName, "Health"));
				}
			}
		}
		Print("[Territories] [Debug] BaseBuildingBase EEHitBy End = = = = = = = = = = = = = = = = = = = = = =");
	}
};
