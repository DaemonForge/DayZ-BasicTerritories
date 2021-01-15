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
	
	int Notifications = 0; //0 Chat | 1 Notifications Mod
	
	ref array< ref BasicTerritoriesNoBuildZones> NoBuildZones = new array< ref BasicTerritoriesNoBuildZones>;
	
	[NonSerialized()]
	protected bool m_BlockWarnPlayer = false;
	[NonSerialized()]
	protected string m_BlockLastMessage = "";
	
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
				NoBuildZones.Insert(new BasicTerritoriesNoBuildZones(3693.56, 6010.05, 100));
				NoBuildZones.Insert(new BasicTerritoriesNoBuildZones(8345.61, 5985.93, 100));
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