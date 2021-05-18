modded class ActionBuildPart: ActionContinuousBase
{
	
	protected int m_LastSync = 0;
	protected bool m_CanBuildHere = true;
	protected vector m_LastCheckLocation = vector.Zero;
	protected int m_LastCheckLocationNextTime = 0;
	
	int LastSync = 0;
	
	void ~ActionBuildPart(){
		if (m_LastCheckLocation != vector.Zero){
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.ResetLastCheckLocation);
		}
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		ItemBase theTarget;
		if (super.ActionCondition( player, target, item )){
			if (GetGame().IsServer()){
				return true;
			}
			if (Class.CastTo(theTarget, target.GetObject()) || Class.CastTo(theTarget, target.GetParent())){
				PlayerBase thePlayer = PlayerBase.Cast(player);
				if (theTarget && thePlayer){
					int curTime = GetGame().GetTime();
					if (vector.Distance(m_LastCheckLocation, theTarget.GetPosition()) > 0.1 || curTime > m_LastCheckLocationNextTime){
						string theGUID = "";
						if (thePlayer.GetIdentity()){
							theGUID = thePlayer.GetIdentity().GetId();
						}
						return CanIBuildHere(theTarget.GetPosition(), theGUID);
					} else {
						return m_CanBuildHere;
					}
				}
			}
			return true;
		}
		return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		super.OnFinishProgressServer(action_data);
		
		
		TerritoryFlag theFlag = TerritoryFlag.Cast( action_data.m_Target.GetObject() );
		if (!theFlag)
			return;
			
		
		Construction construction = theFlag.GetConstruction();
		
		string part_name = BuildPartActionData.Cast(action_data).m_PartType;
		
		if ( construction && construction.IsPartConstructed(part_name) && part_name == "base" ) {
			if (action_data.m_MainItem) {
				action_data.m_MainItem.DecreaseHealth( GetBasicTerritoriesConfig().BuildBonusSledgeDamage, false );
			}
		}
	}
	
	protected bool CanIBuildHere(vector pos, string GUID = ""){
		m_LastCheckLocation = pos;
		int curTime = GetGame().GetTime();
		m_LastCheckLocationNextTime = curTime + 5000;
		if (pos == vector.Zero){
				m_CanBuildHere = false;
				return false;
		} else {
			array<Object> objects = new array<Object>;
			array<CargoBase> proxyCargos = new array<CargoBase>;
			float theRadius = GameConstants.REFRESHER_RADIUS;
			GetGame().GetObjectsAtPosition(pos, theRadius, objects, proxyCargos);
			TerritoryFlag theFlag;
			if (objects){
				for (int i = 0; i < objects.Count(); i++ ){
					if (Class.CastTo( theFlag, objects.Get(i) ) ){
						if (m_LastSync < curTime){
							m_LastSync = curTime + 60000;
							theFlag.SyncTerritory();
						}
						m_CanBuildHere = theFlag.CheckPlayerPermission(GUID, TerritoryPerm.BUILD);
						if (!m_CanBuildHere){
							GetBasicTerritoriesConfig().SendNotification(GetBasicTerritoriesConfig().BuildPartWarningMessage, TerritoryIcons.BuildPartWarning);
						}
						return m_CanBuildHere;
					}
				}
			}
		}
		m_CanBuildHere = true;
		return true;
	}
	
	void ResetLastCheckLocation(){
		m_LastCheckLocation = vector.Zero;
	}
}