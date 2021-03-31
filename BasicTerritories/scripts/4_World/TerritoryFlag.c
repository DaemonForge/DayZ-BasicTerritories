modded class TerritoryFlag extends BaseBuildingBase
{
	protected bool m_CanAddMember = false;
		
	protected bool m_AwaitingReset = false;
	
	protected string m_TerritoryOwner = "";
	
	protected ref BasicTerritoryMembers m_TerritoryMembers = new BasicTerritoryMembers;
	
	TStringArray TerritoryMembers(){
		return m_TerritoryMembers.GetMemberArray();
	}
	
	void TerritoryFlag(){
		RegisterNetSyncVariableBool("m_CanAddMember");
	}
		
	override void EEInit(){
		super.EEInit();
		if ( GetGame().IsClient() ){
			SyncTerritory();
			AnimateFlag(1 - GetRefresherTime01());
		}
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
		Print("[BASICTERRITORY] m_TerritoryOwner: " + m_TerritoryOwner);
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
		Print("[BASICTERRITORY] m_TerritoryOwner: " + m_TerritoryOwner);
		
		if ( !ctx.Read( m_TerritoryMembers ) ) {
			return false;
		}
		
		m_TerritoryMembers.Debug();
		
		return true;
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

