class BasicTerritoriesConfig
{
	protected static string DirPATH = "$profile:BasicTerritories";
	protected static string ConfigPATH = DirPATH + "\\settings.json";
	protected static string RaidPATH = DirPATH + "\\RaidHandlers.json";
	string ConfigVersion = "1";
	ref TStringArray WhiteList = { 
		"Trap",
		"Paper"
	};
	ref TStringArray ServerAdmins = {
		"GUID"
	};
	
	float TentRadius = 6;
	float TentCESpawnLifeTime = 5600;
		
	float BuildBonusSledgeDamage = 300;
	
	int PublicPermissions = TerritoryPerm.PUBLIC;
	
	int MemberPermissions = TerritoryPerm.DEFAULTMEMBER;
	
	int Notifications = 0; //0 Chat | 1 Notifications Mod
	
	ref array< ref BasicTerritoriesNoBuildZones> NoBuildZones = new array< ref BasicTerritoriesNoBuildZones>;
		
	
	[NonSerialized()]
	protected bool m_BlockWarnPlayer = false;
	[NonSerialized()]
	protected string m_BlockLastMessage = "";
	[NonSerialized()]
	protected bool m_RaidHandlerLoaded = false;
	
	void Load(){
		Print("[BasicTerritories] Loading Config");
		if (GetGame().IsServer()){
			if (FileExist(ConfigPATH)){ //If config exist load File
			    JsonFileLoader<BasicTerritoriesConfig>.JsonLoadFile(ConfigPATH, this);
				if (ConfigVersion != "1"){
					ConfigVersion = "1";
					Save();
				}
			}else{ //File does not exist create file
				if (!FileExist(DirPATH)){
					MakeDirectory(DirPATH);
				}
				NoBuildZones.Insert(new ref BasicTerritoriesNoBuildZones(3693.56, 6010.05, 100));
				NoBuildZones.Insert(new ref BasicTerritoriesNoBuildZones(8345.61, 5985.93, 100));
				Save();
			}
			if (FileExist(RaidPATH)){
				LoadRaidHandler();
			} else {
				m_BasicTerritoriesRaidHandlers = new ref array<ref BasicRaidHandler>;
				m_BasicTerritoriesRaidHandlers.Insert(new ref BasicRaidHandler("fence"));
				m_BasicTerritoriesRaidHandlers.Insert(new ref BasicRaidHandler("watchtower"));
				m_BasicTerritoriesRaidHandlers.Insert(new ref BasicRaidHandler("*",false));
				
				m_RaidHandlerLoaded = true;
				SaveRaidHandler();
			}
			
		}
	}
	
	void Save(){
		JsonFileLoader<BasicTerritoriesConfig>.JsonSaveFile(ConfigPATH, this);
	}

	void LoadRaidHandler(){
		if (!FileExist(RaidPATH)) {
			Print("File At" + RaidPATH + " could not be found");
			return;
		}
		JsonSerializer m_Serializer = new JsonSerializer;
		m_BasicTerritoriesRaidHandlers = new ref array<ref BasicRaidHandler>;

		FileHandle fh = OpenFile(RaidPATH, FileMode.READ);
		string jsonData;
		string error;
		if (fh) {
			string line;
			while (FGets(fh, line) > 0) {
				jsonData = jsonData + "\n" + line;
			}

			CloseFile(fh);
			
			bool success = m_Serializer.ReadFromString(m_BasicTerritoriesRaidHandlers, jsonData, error);
			
			if (error != "" || !success) {
				Print("File At" + RaidPATH + " could not be parsed error: " + error );
			} else {
				Print("File Opened from" + RaidPATH);
				m_RaidHandlerLoaded = true;
			}
		} else {
			Print("File At" + RaidPATH + " could not be opened");
		}
	}
	
	void SaveRaidHandler(){
		JsonFileLoader<ref array<ref BasicRaidHandler>>.JsonSaveFile(RaidPATH, m_BasicTerritoriesRaidHandlers);
	}
	
	bool IsInWhiteList(string item){
		if (WhiteList && WhiteList.Count() > 0){
			for (int i = 0; i < WhiteList.Count(); i++){
				if (item.Contains(WhiteList.Get(i))){
					return true;
				}
			}
		}
		return false;
	}
	
	bool CanWarnPlayer(string message = ""){
		message.ToLower();
		if ( !m_BlockWarnPlayer && m_BlockLastMessage != message){
			m_BlockLastMessage = message;
			m_BlockWarnPlayer = true;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ResetWarning, 12 * 1000);
			return true;
		}
		//Print("m_BlockWarnPlayer false");
		return false;
	}
	
	void ResetWarning(){
		//Print("ResetWarning True");
		m_BlockWarnPlayer = false;
	}
	
	int PublicPermission(){
		return PublicPermissions;
	}
	
	int MemberPermission(){
		return MemberPermissions;
	}
	
	void SendNotification(string text, string icon = "BasicTerritories/images/NoBuild.paa"){
		if (GetGame().IsClient()){
			if (Notifications == 0){
				GetGame().Chat(text,"");
			} 
			#ifdef NOTIFICATIONS 
			else if (Notifications == 1){
				NotificationSystem.SimpleNoticiation(text, "Territory", icon, ARGB(230, 209, 60, 60), 8, NULL);
			}
			#endif
		}
	}
	
	
	bool CanBuildHere(vector pos){
		if (NoBuildZones){
			for (int i = 0; i < NoBuildZones.Count(); i++){
				if (NoBuildZones.Get(i) && NoBuildZones.Get(i).Check(pos) ){
					return false;
				}
			}
		}
		return true;
	}
	
	float GetHealBack(float dmg, string type, int dmgType, string tool, string ammo, string part ){
		if (!m_RaidHandlerLoaded || !GetRaidHandlers()){
			return 0;
		}
		float dmgmdf = -1;
		type.ToLower();
		for (int i = 0; i < GetRaidHandlers().Count(); i++){
			string theKey = GetRaidHandlers().Get(i).TypeName;
			theKey.ToLower();
			if ( type.Contains( theKey ) || theKey == "*"){
				dmgmdf = GetRaidHandlers().Get(i).GetDamageMdf(dmgType, tool, ammo, part);
				if (theKey != "*"){
					break;
				}
			}
		}
		
		if (dmgmdf >= 0){
			float totalDmg = dmg * dmgmdf;
			float rtnValue = dmg - totalDmg;
			return rtnValue;
		}
		return 0;
	}
}

class BasicTerritoriesNoBuildZones{
	float X;
	float Z;
	float R;
	bool DrawOnMap = false;
	
	void BasicTerritoriesNoBuildZones(float x, float z, float r){
		X = x;
		Z = z;
		R = r;
	}
	
	//Returns True If is in zone
	bool Check(vector checkPos){
		if (checkPos){
			vector ZeroedHeightPos = Vector(checkPos[0], 0,checkPos[2]);
			if (vector.Distance(ZeroedHeightPos, Vector(X, 0, Z)) <= R){
				return true;
			}
		}
		return false;
	}
	
	vector GetPos(){
		return Vector(X, GetGame().SurfaceY(X,Z),Z);
	}
		
}

class BasicRaidHandler{
	string TypeName = "";
	float FireArms = 0.3;
	float Melee = 0.1;
	float Explosive = 1;
	float Other = 0;
	ref array<ref BasicPartRaidHandler> PartModifier = new ref array<ref BasicPartRaidHandler>;
	ref array<ref BasicToolsRaidHandler> ToolModifiers = new ref array<ref BasicToolsRaidHandler>;
	[NonSerialized()]
	ref map<string, float> ToolModifier; //To make getting stuff more effecient
	
	
	void BasicRaidHandler(string type, bool init = true){
		TypeName = type;
		if (init){
			PartModifier.Insert(new ref BasicPartRaidHandler("wall_wood_", 1.2));
			PartModifier.Insert(new ref BasicPartRaidHandler("wall_metal_", 0.7, {"m67Grenade", "rgd5grenade", "landminetrap"}));
			ToolModifiers.Insert(new ref BasicToolsRaidHandler("sledgehammer", 100));
		}
		PartModifier.Insert(new ref BasicPartRaidHandler("*", 1 ));
	}
	
	float GetDamageMdf(int dmgType, string tool, string ammo, string part){
		int i = 0;
		float toolmdf = 1;
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
			ToolModifier = new ref map<string, float>;
			for (i = 0; i < ToolModifiers.Count(); i++){
				ToolModifier.Set(ToolModifiers.Get(i).Tool, ToolModifiers.Get(i).Modifier);
			}
		}
		if (ToolModifier){
			
			if ( !ToolModifier.Find(tool, toolmdf)){
				if (!ToolModifier.Find(ammo, toolmdf)){
					toolmdf = 1;
				}
			} 
		}
		
		BasicPartRaidHandler partRaidHander;
		
		if (PartModifier){
			for ( i = 0; i < PartModifier.Count(); i++){
				string theKey = PartModifier.Get(i).Part;
				theKey.ToLower();
				if ( part.Contains( theKey ) ||  theKey == "*"){
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
		} else {
			partMdfr = 1;
		}
		float rtrnValue = toolmdf * partMdfr * typeMdfr;
		Print("[Territories] [Debug] GetDamageMdf Returning: " + rtrnValue + "( " + toolmdf + " * " + partMdfr + " * " + typeMdfr + " )");
		return rtrnValue;
	}
}

class BasicPartRaidHandler{
	string Part = "";
	float Modifier = 1;
	ref TStringArray WhiteList = new TStringArray;
	
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
	
	void BasicToolsRaidHandler(string tool, float mdfr){
		Tool = tool;
		Modifier = mdfr;
	}
	
}

ref BasicTerritoriesConfig m_BasicTerritoriesConfig;

ref array<ref BasicRaidHandler> m_BasicTerritoriesRaidHandlers = new ref array<ref BasicRaidHandler>;

//Helper function to return Config
static ref BasicTerritoriesConfig GetBasicTerritoriesConfig()
{
	if (!m_BasicTerritoriesConfig)
	{
		m_BasicTerritoriesConfig = new ref BasicTerritoriesConfig;
			
		if ( GetGame().IsServer() ){
			m_BasicTerritoriesConfig.Load();
		}
	}
	return m_BasicTerritoriesConfig;
};

//Helper function to return Config
static ref array<ref BasicRaidHandler> GetRaidHandlers()
{
	return m_BasicTerritoriesRaidHandlers;
};