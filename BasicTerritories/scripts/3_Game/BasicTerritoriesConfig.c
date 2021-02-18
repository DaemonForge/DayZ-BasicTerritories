class BasicTerritoriesConfig
{
	protected static string DirPATH = "$profile:BasicTerritories";
	protected static string ConfigPATH = DirPATH + "\\settings.json";
	protected static string RaidPATH = DirPATH + "\\RaidHandlers.json";
	string ConfigVersion = "1";
	ref TStringArray WhiteList = { 
		"Trap",
		"Paper",
		"Fireplace"
	};
	ref TStringArray ServerAdmins = {
		"GUID"
	};
	
	float TentRadius = 6;
	float TentCESpawnLifeTime = 5600;
		
	float BuildBonusSledgeDamage = 300;
	
	bool RequireTerritory = false;
	
	int PublicPermissions = TerritoryPerm.PUBLIC;
	
	int MemberPermissions = TerritoryPerm.DEFAULTMEMBER;
	
	int Notifications = 0; //0 Chat | 1 Notifications Mod
	
	ref array< ref BasicTerritoriesNoBuildZones> NoBuildZones = new array< ref BasicTerritoriesNoBuildZones>;
	
	string NoBuildZoneMessage = "You can't build here, are trying to build in a designated no build zones";
	string TerritoryConflictMessage = "Sorry you can't build a territory this close to another territory";
	string WithinTerritoryWarning = "Sorry you can't build this close to an enemy territory";
	string DeSpawnWarningMessage = "You are building outside a territory, $ITEMNAME$ will despawn in $LIFETIME$ without a Territory";
	string BuildPartWarningMessage = "Sorry, you don't have permissions to build in this area.";
	string DismantleWarningMessage = "Sorry, you can't dismantle anything this close to a raised flag";
	string LowerFlagWarningMessage = "Sorry, you do not have permissions to lower the flag in this territory";
	string TerritoryRequiredWarningMessage = "Sorry, you are required to build a territory to be able to build";
	
	int FlagRefreshFrequency = 0;
	
	ref map<string, int> KitLifeTimes = new map<string, int>;
	
	[NonSerialized()]
	protected int m_BlockWarnPlayer = 0;
	[NonSerialized()]
	protected string m_BlockLastMessage = "";
	[NonSerialized()]
	protected bool m_RaidHandlerLoaded = false;
	
	[NonSerialized()]
	int m_disableBaseDamage = 0;
	
	void Load(){
		Print("[BasicTerritories] Loading Config");
		if (GetGame().IsServer()){
			if (FileExist(ConfigPATH)){ //If config exist load File
			    JsonFileLoader<BasicTerritoriesConfig>.JsonLoadFile(ConfigPATH, this);
				Save();
			}else{ //File does not exist create file
				if (!FileExist(DirPATH)){
					MakeDirectory(DirPATH);
				}
				NoBuildZones.Insert(new ref BasicTerritoriesNoBuildZones(3703.5, 5985.11, 100));
				NoBuildZones.Insert(new ref BasicTerritoriesNoBuildZones(8345.61, 5985.93, 100));
				FlagRefreshFrequency = GetCEApi().GetCEGlobalInt("FlagRefreshFrequency");
				if (FlagRefreshFrequency <= 0){
					FlagRefreshFrequency = GameConstants.REFRESHER_FREQUENCY_DEFAULT;
				}
				KitLifeTimes.Insert("fencekit", 3888000);
				KitLifeTimes.Insert("watchtowerkit", 3888000);
				KitLifeTimes.Insert("msp_", 3888000);
				Save();
			}
			m_disableBaseDamage = GetGame().ServerConfigGetInt("disableBaseDamage");
			if( m_disableBaseDamage < 1){
				Print("[BasicTerritories] BaseDamage [Enabled]");	
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
			} else {
				Print("[BasicTerritories] BaseDamage [Disabled]");	
			}
		}
	}
	
	void Save(){
		JsonFileLoader<BasicTerritoriesConfig>.JsonSaveFile(ConfigPATH, this);
	}

	int disableBaseDamage(){
		return m_disableBaseDamage;
	}
	
	void LoadRaidHandler(){
		if (!FileExist(RaidPATH)) {
			Print("[BasicTerritories] File At" + RaidPATH + " could not be found");
			return;
		}
		autoptr JsonSerializer m_Serializer = new JsonSerializer;
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
				item.ToLower();
				string wItem = WhiteList.Get(i);
				wItem.ToLower();
				if (item.Contains(wItem)){
					return true;
				}
			}
		}
		return false;
	}
	
	bool CanWarnPlayer(string message = ""){
		message.ToLower();
		int curTime = GetGame().GetTime();
		if ( curTime > m_BlockWarnPlayer || m_BlockLastMessage != message){
			m_BlockLastMessage = message;
			m_BlockWarnPlayer = curTime + 6000;
			return true;
		}
		return false;
	}
	
	
	int PublicPermission(){
		return PublicPermissions;
	}
	
	int MemberPermission(){
		return MemberPermissions;
	}
	
	void SendNotification(string text, string icon = "BasicTerritories/images/NoBuild.paa"){
		if (GetGame().IsClient() && CanWarnPlayer(text)){
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
	
	
	bool CanBuildHere(vector pos, string item = ""){
		if (IsInWhiteList(item)){
			return true;
		}
		if (NoBuildZones){
			for (int i = 0; i < NoBuildZones.Count(); i++){
				if (NoBuildZones.Get(i) && NoBuildZones.Get(i).Check(pos) ){
					return false;
				}
			}
		}
		return true;
	}
	
	float GetHealBack(float dmg, string type, int dmgType, string tool, string ammo, string part, TStringArray PartsWithHealth = NULL, float distance = 0){
		if (!m_RaidHandlerLoaded || !GetRaidHandlers() ||  m_disableBaseDamage > 0){
			return 0;
		}
		bool totalDmgSet = false;
		float totalDmg = 0;
		type.ToLower();
		for (int i = 0; i < GetRaidHandlers().Count(); i++){
			string theKey = GetRaidHandlers().Get(i).TypeName;
			theKey.ToLower();
			if ( type.Contains( theKey ) || theKey == "*"){
				totalDmg = GetRaidHandlers().Get(i).GetTrueDamage(dmg, dmgType, tool, ammo, part,PartsWithHealth, distance);
				totalDmgSet = true;
				if (theKey != "*"){
					break;
				}
			}
		}
		
		if (totalDmgSet){
			float rtnValue = dmg - totalDmg;
			return rtnValue;
		}
		return 0;
	}
	
	string NiceExpireTime(float LifeTime){
		float Hours = Math.Floor(LifeTime / 3600);
		
		Print("Hour: " + Hours + " RefreshFrequency:" + FlagRefreshFrequency);
		int rtnValue = 0;
		if (LifeTime < FlagRefreshFrequency){
			return ""; //Means that this items wouldn't get affected by refresh anyways.
		}
		int Days = Math.Floor(Hours / 24);
		int remander = Days % 7;
		if (remander >= 2){
			return Days.ToString() + " Days";
		}
		int Weeks = Math.Floor(Days / 7);
		return Weeks.ToString() + " Weeks";
	}
	
	int GetKitLifeTime(string item){
		item.ToLower();
		int lt = KitLifeTimes.Get(item);
		if (lt == 0){
			foreach (string key, int lifetime : KitLifeTimes){
				if (item.Contains(key)){
					return lifetime;
				}
			}
		} else {
			return lt;
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