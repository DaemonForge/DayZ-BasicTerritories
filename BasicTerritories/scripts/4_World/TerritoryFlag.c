modded class TerritoryFlag extends BaseBuildingBase
{
	#ifdef GAMELABS
    protected ref _Event _customEventInstance;
	#endif
	protected bool m_CanAddMember = false;
		
	protected bool m_AwaitingReset = false;
	
	protected string m_TerritoryOwner = "";
	
	protected bool m_isRequestingSync = false;
	
	protected ref BasicTerritoryMembers m_TerritoryMembers = new BasicTerritoryMembers;
	
	void TerritoryFlag(){
		//Print("[BASICTERRITORY] +TerritoryFlag");
		RegisterNetSyncVariableBool("m_CanAddMember");
	}

	void ~TerritoryFlag(){
		//Print("[BASICTERRITORY]  ~TerritoryFlag");
	#ifdef GAMELABS
		if ( _customEventInstance ){
			GetGameLabs().RemoveEvent(_customEventInstance);
		}
	#endif
		if (m_isRequestingSync){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.DoFirstSync);
		}
	}
	
	override void EEInit(){
		
		super.EEInit();
		
		
		if ( GetGame().IsClient() ){
			DayZPlayer player =  DayZPlayer.Cast(GetGame().GetPlayer());
			if (player){
				m_isRequestingSync = true;
				int Distance = vector.Distance(player.GetPosition(), GetPosition()) + 10;
				int Time = Distance * 10;
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.DoFirstSync, Time, false);
			}
			//SyncTerritory();
			AnimateFlag(1 - GetRefresherTime01());
		}
	}
	
	
	TStringArray TerritoryMembers(){
		return m_TerritoryMembers.GetMemberArray();
	}
	
	
	void DoFirstSync(){
		m_isRequestingSync = false;
		SyncTerritory();
	}
	
	
	bool IsTerritoryOwner(string guid){
		if (!m_TerritoryOwner || m_TerritoryOwner == ""){
			return false;
		}
		return (m_TerritoryOwner == guid);
	}
	
	bool CanReceiveNewOwner(){
		if (!m_TerritoryOwner || m_TerritoryOwner == ""){
			return true;
		}
		return false;
	}
	
	int GetMemberCount(){
		return TerritoryMembers().Count();
	}
	
	void ResetMembers(){
		Print("[BASICTERRITORY] ResetMembers m_TerritoryOwner: " + m_TerritoryOwner);
		m_TerritoryMembers.Debug();
		m_TerritoryMembers.Clear();
		SyncTerritory();
	}
	
	bool CanAddMember(){
		return m_CanAddMember;
	}
	
	void SetTerritoryOwner(string guid){
		m_TerritoryOwner = guid;
		SyncTerritory();
		
	#ifdef GAMELABS
		
		if ( GetGame().IsServer() && _customEventInstance ){
			GetGameLabs().RemoveEvent(_customEventInstance);
			_customEventInstance = NULL;
		}
		
		if ( GetGame().IsServer() && !_customEventInstance && m_TerritoryOwner != "") {
			if ( this._GetEventInstance() ){
				Print("[BASICTERRITORIES] Removing _registeredInstance");
				GetGameLabs().RemoveEvent( this._GetEventInstance() );
				this._SetEventInstance(NULL);
			}
			Print("[BASICTERRITORIES] Adding _customEventInstance");
			_customEventInstance = new _Event("<strong>Basic Territory</strong><br />Owner: [ <i>" + m_TerritoryOwner + "</i> ]<br />Raised: "+ Math.Round(GetRefresherTime01() * 100) + "%", "flag", this);
			GetGameLabs().RegisterEvent( _customEventInstance );
		}
	#endif
	}
	
	void AllowMemberToBeAdded(bool state = true){
		m_CanAddMember = state;
		if (m_AwaitingReset && GetGame().IsServer()){
			m_AwaitingReset = false;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ResetAllowMemberToBeAdded);
		}
		if (state && GetGame().IsServer()){
			m_AwaitingReset = true;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.ResetAllowMemberToBeAdded, 300 * 1000);
		}
		SetSynchDirty();
	}
	
	void ResetAllowMemberToBeAdded(){
		m_AwaitingReset = false;
		m_CanAddMember = false;
		SetSynchDirty();
	}
	
	void AddMember(string guid){
		if (guid == m_TerritoryOwner){
			return;
		} else {
			m_TerritoryMembers.AddMember(guid);
			AllowMemberToBeAdded(false);
			SyncTerritory();
		}
	}
	
	void RemoveMember(string guid){
		if (guid == m_TerritoryOwner){
			return;
		} else {
			m_TerritoryMembers.RemoveMember(guid);
			SyncTerritory();
		}
	}
	
	bool IsTerritoryMember(string guid){
		if (m_TerritoryOwner == ""){
			return true;
		} else if (guid == m_TerritoryOwner){
			return true;
		} else if (m_TerritoryMembers.CheckId(guid)){
			return true;
		} else {
			return false;
		}
	}
	
	void AddMemberClient(string guid){
		if (CanAddMember()){
			RPCSingleParam(BASICYNRRPCs.ADD_MEMBER, new Param2< string, BasicTerritoryMembers >(guid, m_TerritoryMembers), true, NULL);
		}
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		ctx.Write( m_TerritoryOwner );
		
		ctx.Write( m_TerritoryMembers );
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) ) {
			return false;
		}
		if ( !ctx.Read( m_TerritoryOwner ) ) {
			return false;
		}
		
		if ( !ctx.Read( m_TerritoryMembers ) ) {
			return false;
		}
		
		if (GetGame().IsServer()){
			Print("[BASICTERRITORY] m_TerritoryOwner: " + m_TerritoryOwner + " Pos: " + GetPosition());
			m_TerritoryMembers.Debug();
		}
		
		return true;
	}
	
	override void AfterStoreLoad(){
		super.AfterStoreLoad();
		
	#ifdef GAMELABS
		Print("[BASICTERRITORIES] Detected GAMELABS");
		if ( GetGame().IsServer() && !_customEventInstance && m_TerritoryOwner != "") {
			if ( this._GetEventInstance() ){
				Print("[BASICTERRITORIES] Removing _registeredInstance");
				GetGameLabs().RemoveEvent( this._GetEventInstance() );
				this._SetEventInstance(NULL);
			}
			Print("[BASICTERRITORIES] Adding _customEventInstance");
			_customEventInstance = new _Event("<b>Basic Territory</b><br/>Owner: [ " + m_TerritoryOwner + " ]<br/>Raised: "+ Math.Round(GetRefresherTime01() * 100) + "%", "flag", this);
			Print(_customEventInstance);
			GetGameLabs().RegisterEvent(_customEventInstance);
		}
	#endif
	}
	
	void SyncTerritory(PlayerIdentity identity = NULL)
	{
		if ( GetGame().IsServer() ) {
			SetSynchDirty();
			RPCSingleParam(BASICYNRRPCs.SEND_DATA, new Param2< string, BasicTerritoryMembers >(m_TerritoryOwner, m_TerritoryMembers), true, identity);
		} else if (GetGame().IsClient()){
			RPCSingleParam(BASICYNRRPCs.REQUEST_DATA, new Param1<bool>( true ), true, NULL);
		}
	}
	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, rpc_type, ctx);
		Param2< string, BasicTerritoryMembers > data;
		if (rpc_type == BASICYNRRPCs.SEND_DATA && GetGame().IsClient()) {
			if (ctx.Read(data))	{
				m_TerritoryOwner = data.param1;
				m_TerritoryMembers = BasicTerritoryMembers.Cast(data.param2);
			}
			return;
		}
		if (rpc_type == BASICYNRRPCs.REQUEST_DATA && GetGame().IsServer()) {
			if (sender){
				Print("[BASICTERRITORY] SyncTerritory Request From Client (" + sender.GetId() +") " + sender.GetName());
				SyncTerritory(sender);
			} else {
				Print("[BASICTERRITORY] SyncTerritory Request From Client With No sender");
				SyncTerritory();
			}
			return;
		}
		if (rpc_type == BASICYNRRPCs.ADD_MEMBER && GetGame().IsServer()){
			if (ctx.Read(data)){
				if (CanAddMember() && sender){
					if (TerritoryMembers().Count() == data.param2.Count() && sender.GetId() == data.param1){ //To ensure client and server where in sync
						AddMember(data.param1);
						AllowMemberToBeAdded(false);
					}
					SyncTerritory();
				}
			}
			return;
		}
	}
		
	bool HasRaisedFlag(){
		if (FindAttachmentBySlotName("Material_FPole_Flag")){
			float state = GetAnimationPhase("flag_mast");
			if (state <= TerritoryConst.FLAGDOWNSTATE){
				return true;
			}
		}
		return false;
	}
		
	bool CheckPlayerPermission(string guid, int permission){
		int publicPerms = GetBasicTerritoriesConfig().PublicPermission();
		int PermsCheck = publicPerms & permission;
		
		if ( PermsCheck == permission || guid == m_TerritoryOwner){
			//Print("[BasicTerritory] Action is in public permissions");
			return true;
		}
		if (HasRaisedFlag()){
			return m_TerritoryMembers.Check(guid, permission);
		}
		return true;
	}
	
	
	override void SetActions(){
		super.SetActions();
		
		AddAction( ActionAddTerritoryMember );
		AddAction( ActionResetTerritoryMembers );
		AddAction( ActionAcceptMembership );
	}
	
}
