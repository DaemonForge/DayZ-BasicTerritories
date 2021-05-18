modded class ActionDismantlePart : ActionContinuousBase
{
	protected int m_LastSync = 0;
	protected bool m_CanDismantleHere = true;
	protected vector m_LastCheckLocation = vector.Zero;
	protected int m_LastCheckLocationNextTime = 0;
	
	void ~ActionDismantlePart(){
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
						return CanIDismantleHere(theTarget.GetPosition(), theGUID);
					} else {
						return m_CanDismantleHere;
					}
				}
			}
			return true;
		}
		return false;
	}
	
	protected bool CanIDismantleHere(vector pos, string GUID = ""){
		m_LastCheckLocation = pos;
		int curTime = GetGame().GetTime();
		m_LastCheckLocationNextTime = curTime + 5000;
		if (pos == vector.Zero){
				m_CanDismantleHere = false;
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
						m_CanDismantleHere = theFlag.CheckPlayerPermission(GUID, TerritoryPerm.DISMANTLE);
						if (!m_CanDismantleHere ){
							GetBasicTerritoriesConfig().SendNotification(GetBasicTerritoriesConfig().DismantleWarningMessage, TerritoryIcons.DismantleWarning);
						}
						return m_CanDismantleHere;
					}
				}
			}
		}
		m_CanDismantleHere = true;
		return true;
	}
	
	void ResetLastCheckLocation(){
		m_LastCheckLocation = vector.Zero;
	}
}