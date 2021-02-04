class BasicRaidHandler{
	string TypeName = "";
	float FireArms = 0.3;
	float Melee = 0.1;
	float Explosive = 1;
	float Other = 0;
	bool ToolsFirst = 1;
	ref array<ref BasicPartRaidHandler> PartModifier = new ref array<ref BasicPartRaidHandler>;
	ref array<ref BasicToolsRaidHandler> ToolModifiers = new ref array<ref BasicToolsRaidHandler>;
	[NonSerialized()]
	ref map<string, ref BasicToolsRaidHandler> ToolModifier; //To make getting stuff more effecient
	
	
	void BasicRaidHandler(string type, bool init = true){
		TypeName = type;
		if (init){
			PartModifier.Insert(new ref BasicPartRaidHandler("wall_wood_", 1.2));
			PartModifier.Insert(new ref BasicPartRaidHandler("wall_metal_", 0.7, {"m67Grenade", "rgd5grenade", "landminetrap"}));
			ToolModifiers.Insert(new ref BasicToolsRaidHandler("sledgehammer", 100));
		}
		PartModifier.Insert(new ref BasicPartRaidHandler("*", 1 ));
	}
	
	float GetTrueDamage(float dmg, int dmgType, string tool, string ammo, string part, TStringArray PartsWithHealth = NULL, float distance = 0){
		int i = 0;
		float toolmdf = 1;
		float toolMin = 0;
		float toolMax = -1;
		float partMdfr = 1;
		float typeMdfr = 1;
		part.ToLower();
		ammo.ToLower();
		tool.ToLower();
		if (dmgType == DT_CLOSE_COMBAT){
			typeMdfr = Melee;
		} else if (dmgType == DT_FIRE_ARM){
			typeMdfr = FireArms;
		} else if (dmgType == DT_EXPLOSION){
			typeMdfr = Explosive;
		} else {
			typeMdfr = Other;
		}
		if (!ToolModifier){
			ToolModifier = new ref map<string, ref BasicToolsRaidHandler>;
			for (i = 0; i < ToolModifiers.Count(); i++){
				ToolModifier.Set(ToolModifiers.Get(i).Tool, ToolModifiers.Get(i));
			}
		}
		autoptr BasicToolsRaidHandler toolRaidHander;
		if (ToolModifier){
			
			if ( !ToolModifier.Find(tool, toolRaidHander)){
				if (!ToolModifier.Find(ammo, toolRaidHander)){
					toolmdf = 1;
				}
			} 
		}
		bool CantDamageYet = false;
		if (toolRaidHander){
			toolMin = toolRaidHander.MinDmg;
			toolMax = toolRaidHander.MaxDmg;
			//Print("[Territories] [Debug] Tool: " + toolRaidHander.Tool + " Max Distance: " + toolRaidHander.MaxDistance + " distance: " + distance );
			if (toolRaidHander.MaxDistance > 0 && distance > toolRaidHander.MaxDistance){
				CantDamageYet = true;
				return 0;
			}
		}
		
		autoptr BasicPartRaidHandler partRaidHander;
		
		if (PartModifier){
			for ( i = 0; i < PartModifier.Count(); i++){
				string theKey = PartModifier.Get(i).Part;
				theKey.ToLower();
				if ( part.Contains( theKey ) ||  theKey == "*" ){
					partRaidHander = BasicPartRaidHandler.Cast(PartModifier.Get(i));
					if (theKey != "*"){
						break;
					}
				}
			}
		}
		if (partRaidHander) {
			if (partRaidHander.WhiteList && partRaidHander.WhiteList.Count() > 0 && partRaidHander.WhiteList.Find(ammo) == -1&& partRaidHander.WhiteList.Find(tool) == -1 ){
				partMdfr = 0;
			} else {
				partMdfr = partRaidHander.Modifier;
			}
			if (partRaidHander.RequiredDestroyed && PartsWithHealth && PartsWithHealth.Count() > 0 && partRaidHander.RequiredDestroyed.Count() > 0){
				//Print("Checking Required Destroyed");
				//partRaidHander.RequiredDestroyed.Debug();
				//PartsWithHealth.Debug();
				for (i = 0; i <  PartsWithHealth.Count(); i++) {
					string partname = PartsWithHealth.Get(i);
					partname.ToLower();
					if (partRaidHander.RequiredDestroyed.Find(partname) != -1){
						CantDamageYet = true;
						return 0.0;
					}
				}
			}
		} else {
			partMdfr = 1;
		}
		float rtrnValue = 0;
		if (ToolsFirst){
			rtrnValue = ProccessToolDmg((dmg * typeMdfr), toolMin, toolMax, toolmdf) * partMdfr;
		}else {
			rtrnValue = ProccessToolDmg((dmg * partMdfr * typeMdfr), toolMin, toolMax, toolmdf);
		}
		return rtrnValue;
	}
	
	static float ProccessToolDmg(float dmgin, float min, float max, float mdfr){
		float dmg = dmgin * mdfr;
		if (dmg < min && min != -1){
			return min;
		} else if (dmg > max && max != -1){
			return max;
		}
		return dmg;
	}
}

class BasicPartRaidHandler{
	string Part = "";
	float Modifier = 1;
	ref TStringArray WhiteList = new TStringArray;
	ref TStringArray RequiredDestroyed =  new TStringArray;
	
	void BasicPartRaidHandler( string part,  float modifier = 1, TStringArray whiteList = NULL )
	{
		Part = part;
		Modifier = modifier;
		if (whiteList){
			WhiteList = whiteList;
		}
	}
}

class BasicToolsRaidHandler{
	string Tool = "";
	float Modifier = 1;
	float MinDmg = 0;
	float MaxDmg = 21000;
	float MaxDistance = -1;
	
	void BasicToolsRaidHandler(string tool, float mdfr){
		Tool = tool;
		Modifier = mdfr;
	}
	
}