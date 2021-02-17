modded class BaseBuildingBase
{	

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{		
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		if (!GetGame().IsClient() && GetBasicTerritoriesConfig().disableBaseDamage() < 1){
			PlayerBase player;
			string tool = "";
			float distance = 0;		
			if (source){
				distance = vector.Distance( this.GetPosition(), source.GetPosition());
				tool = source.GetType();
				player = PlayerBase.Cast(source.GetHierarchyRootPlayer());
			}
			TStringArray ZonesWithHealth = new TStringArray;
			array<string> FullZonesList = new array<string>;
			GetDamageZones(FullZonesList);
			int i;
			for (i = 0; i < FullZonesList.Count(); i++){
				string zoneNameFZ = FullZonesList.Get(i);
				zoneNameFZ.ToLower();
				float beforeHealthFZ = GetHealth(FullZonesList.Get(i), "Health");
				if (beforeHealthFZ > 0 && GetConstruction().IsPartConstructed(zoneNameFZ) ){
					ZonesWithHealth.Insert(zoneNameFZ);
				}
			}
			
			for (i = 0; i < FullZonesList.Count(); i++){
				float zoneDmg = damageResult.GetDamage(FullZonesList.Get(i), "Health");
				string zoneName = FullZonesList.Get(i);
				float beforeHealth = GetHealth(zoneName, "Health");
				if (zoneDmg > 0){
					float HealBackAmount = GetBasicTerritoriesConfig().GetHealBack(zoneDmg, this.GetType(), damageType, tool, ammo, zoneName, ZonesWithHealth, distance);
					AddHealth(zoneName, "Health", HealBackAmount);
					float resultingDamage = zoneDmg + HealBackAmount;
					if (player && player.GetIdentity()){
						GetGame().AdminLog("[Territories] Source: " +  source.GetType() + " from: " + distance + "m held by player: " +  player.GetIdentity().GetName() + "(" +  player.GetIdentity().GetId() + ") Zone: " + zoneName + "  Damage: " + resultingDamage + "  Current Health: " + this.GetHealth(zoneName, "Health"));
					} else {
						GetGame().AdminLog("[Territories] Source: " +  source.GetType() + " from: " + distance + "m Zone: " + zoneName + "  Damage: " + resultingDamage + "  Current Health: " + this.GetHealth(zoneName, "Health"));
					}
				}
			}
		}
	}
	
};