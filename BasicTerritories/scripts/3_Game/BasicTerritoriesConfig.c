class BasicTerritoriesConfig
{
	protected static string ConfigPATH = "$profile:BasicTerritories.json";
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
	
	[NonSerialized()]
	protected bool m_BlockWarnPlayer = false;
	
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
				Save();
			}
		}
	}
	
	void Save(){
		JsonFileLoader<BasicTerritoriesConfig>.JsonSaveFile(ConfigPATH, this);
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
	
	bool CanWarnPlayer(){
		if ( !m_BlockWarnPlayer ){
			//Print("m_BlockWarnPlayer True");
			m_BlockWarnPlayer = true;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ResetWarning, 15 * 1000);
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
	
}
ref BasicTerritoriesConfig m_BasicTerritoriesConfig;

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